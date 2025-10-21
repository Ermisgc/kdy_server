#include "server/http_server.h"
#include "api/api_handler.h"
#include <fstream>

NAMESPACE_BEGIN{ namespace server{
    HTTPServer::HTTPServer(const uint16_t port, const std::string& name) :listenAddr(port), server(&loop, listenAddr, name){
        // 设置HTTP回调
        server.setHttpCallback(
            std::bind(&HTTPServer::onRequest, this, 
                     std::placeholders::_1, std::placeholders::_2));
    }

    HTTPServer::~HTTPServer(){
        //不负责任何APIHandler的析构，析构需要由外部负责
    }

    void HTTPServer::start() {
        server.start();
        loop.loop();
    }

    void HTTPServer::onRequest(const HttpRequest& req, HttpResponse* resp) {
        // 解析请求
        std::string path = req.path();
        HttpRequest::Method method = req.method();
        LOG_INFO << "Request: " << req.methodString() << " " << req.path();

        if(method != HttpRequest::kGet){
            handleMethodNotAllowed(req, resp);
            return;
        }

        
        if(auto itr = api_maps.find(path); itr != api_maps.end()){  //case1.handle api calling
            itr->second->handle(req, resp);
        } else {  //case2. handle file get
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
        api_maps[api_name] = api_handler;
    }
}}