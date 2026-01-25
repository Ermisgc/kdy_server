#include "server/http_server.h"
#include "utils.h"
#include "api/api_handler.h"
#include <fstream>
#include <iostream>

NAMESPACE_BEGIN{ namespace server{
    HTTPServer::HTTPServer(const uint16_t port, const std::string& name) :listenAddr(port), server(&loop, listenAddr, name){
        server.setConnectionCallback(std::bind(&HTTPServer::onConnection, this, std::placeholders::_1));
        server.setMessageCallback(std::bind(&HTTPServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    HTTPServer::~HTTPServer(){
        //不负责任何APIHandler的析构，析构需要由外部负责
    }

    void HTTPServer::start() {
        server.start();
        loop.loop();
    }

    void HTTPServer::onConnection(const TcpConnectionPtr& conn){
        // 记录连接状态
        if (conn->connected()) {
            LOG_INFO << "New connection established from " 
                    << conn->peerAddress().toIpPort() << " to "
                    << conn->localAddress().toIpPort();
        } else {
            LOG_INFO << "Connection closed from " 
                    << conn->peerAddress().toIpPort();
        }        
    }

    void HTTPServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, server::TimeStamp receiveTime) {
        HttpRequest req;
        HttpResponse resp(true); // 默认短连接，解析keep-alive再后续判断是否要变为长连接
        
        // 解析请求行
        const char* crlf = buf->findCRLF();
        if (!crlf) {
            // 数据不完整，等待更多数据
            return;
        }
        
        // 解析请求行: METHOD PATH VERSION
        std::string requestLine(buf->peek(), crlf);
        std::istringstream iss(requestLine);
        std::string methodStr, path, versionStr;
        
        if (!(iss >> methodStr >> path >> versionStr)) {
            // 请求行格式错误
            resp.setStatusCode(HttpResponse::k400BadRequest);
            resp.setStatusMessage("Bad Request");
            resp.setCloseConnection(true);
            
            Buffer responseBuf;
            resp.appendToBuffer(&responseBuf);
            conn->send(&responseBuf);
            conn->shutdown();
            return;
        }
        
        // 解析HTTP方法
        if (methodStr == "GET") {
            req.setMethod(HttpMethod::kGet);
        } else if (methodStr == "POST") {
            req.setMethod(HttpMethod::kPost);
        } else if (methodStr == "HEAD") {
            req.setMethod(HttpMethod::kHead);
        } else if (methodStr == "PUT") {
            req.setMethod(HttpMethod::kPut);
        } else if (methodStr == "DELETE") {
            req.setMethod(HttpMethod::kDelete);
        } else {
            req.setMethod(HttpMethod::kInvalid);
        }
        
        req.setPath(path);
        
        // 解析HTTP版本
        if (versionStr == "HTTP/1.0") {
            req.setVersion(HttpVersion::kHttp10);
            resp.setCloseConnection(true); // HTTP/1.0 默认短连接
        } else if (versionStr == "HTTP/1.1") {
            req.setVersion(HttpVersion::kHttp11);
            // HTTP/1.1 默认长连接，但可以通过Connection头控制
        } else {
            req.setVersion(HttpVersion::kInvalid);
        }
        
        buf->retrieveUntil(crlf + 2); // 跳过请求行和CRLF
        
        // 解析请求头
        while (true) {
            crlf = buf->findCRLF();
            if (!crlf) break;
            
            std::string headerLine(buf->peek(), crlf);
            if (headerLine.empty()) {
                // 空行表示头部结束
                buf->retrieveUntil(crlf + 2);
                break;
            }
            
            // 解析头部字段: Key: Value
            size_t colon = headerLine.find(':');
            if (colon != std::string::npos) {
                std::string key = headerLine.substr(0, colon);
                std::string value = headerLine.substr(colon + 1);
                
                // 去除value的前导空格
                size_t start = value.find_first_not_of(" \t");
                if (start != std::string::npos) {
                    value = value.substr(start);
                }
                
                req.addHeader(key, value);
            }
            
            buf->retrieveUntil(crlf + 2);
        }
        
        // 检查是否需要关闭连接
        std::string connectionHeader = req.getHeader("Connection");
        if ((req.version() == HttpVersion::kHttp10 && connectionHeader != "keep-alive") ||
            (req.version() == HttpVersion::kHttp11 && connectionHeader == "close")) {
            resp.setCloseConnection(true);
        } else if (req.version() == HttpVersion::kHttp11 && connectionHeader != "close") {
            resp.setCloseConnection(false);
        }
        
        std::string contentLengthStr = req.getHeader("Content-Length");
        if (!contentLengthStr.empty()) {
            try {
                size_t contentLength = std::stoul(contentLengthStr);
                if (buf->readableBytes() >= contentLength) {
                    req.setBody(std::string(buf->peek(), contentLength));
                    buf->retrieve(contentLength);
                } else {
                    // 数据不完整，等待更多数据
                    return;
                }
            } catch (const std::exception& e) {
                LOG_ERROR << "Invalid Content-Length: " << contentLengthStr;
                resp.setStatusCode(HttpResponse::k400BadRequest);
                resp.setStatusMessage("Bad Request");
                
                Buffer responseBuf;
                resp.appendToBuffer(&responseBuf);
                conn->send(&responseBuf);
                conn->shutdown();
                return;
            }
        }
        
        // 处理请求
        onRequest(req, &resp);
        
        // 发送响应
        Buffer responseBuf;
        resp.appendToBuffer(&responseBuf);
        conn->send(&responseBuf);
        
        // 如果需要关闭连接
        if (resp.closeConnection()) {
            conn->shutdown();
        }
    }

    void HTTPServer::onRequest(const HttpRequest& req, HttpResponse* resp) {
        // 解析请求
        std::string path = req.path();
        HttpMethod method = req.method();
        // UrlParser::splitPathAndQuery(path, req.path_, )
        LOG_INFO << "Request: " << req.methodString() << " " << path;

        if(method != HttpMethod::kGet && method != HttpMethod::kPost){
            handleMethodNotAllowed(req, resp);
            return;
        }

        if(auto itr = api_maps.find(path); itr != api_maps.end()){  //case1. 精确匹配
            itr->second->handle(req, resp);
        } else if(0) {  //case2. TODO:前缀路由匹配

        } else {  //case3. 静态文件获取
            path = HTML_BASE + path;
            std::ifstream file(path, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                handleNotFound(req, resp);
                return;
            }

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            // 读取文件内容
            std::string content;
            content.resize(size);
            if (file.read(&content[0], size)) {
                resp->setStatusCode(server::HttpResponse::k200Ok);
                resp->setStatusMessage("OK");
                resp->setContentType(mime_detector.getMimeType(path));
                resp->setBody(content);
            } else {
                handleNotFound(req, resp);
            }
            file.close();
        }
    }

    void HTTPServer::handleNotFound(const HttpRequest& req, HttpResponse* resp) {
        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setStatusMessage("Not Found");
        resp->setContentType("text/html");
        resp->setBody("<html><body><h1>404 Not Found</h1></body></html>");
    }

    void HTTPServer::handleMethodNotAllowed(const HttpRequest & req, HttpResponse * resp){
        resp->setStatusCode(HttpResponse::k400BadRequest);
        resp->setStatusMessage("Not Found");
        resp->setContentType("text/html");
        resp->setBody("<html><body><h1>405 Method Not Allowed</h1></body></html>");        
    }

    void HTTPServer::addApi(const std::string & api_name, api::APIHandler * api_handler){
        size_t len = api_name.length();
        if(len > 2 && api_name.substr(len - 2) == "/*"){
            //通配符匹配，放到路由表
            std::string prefix = api_name.substr(0, len - 1);
            wildcard_routes.emplace_back(prefix, api_handler);
            std::sort(wildcard_routes.begin(), wildcard_routes.end(), [](const RouteEntry & a, const RouteEntry &b){
                return a.prefix.length() > b.prefix.length();
            });
        } else api_maps[api_name] = api_handler;
    }
}}