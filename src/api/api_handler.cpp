#include "api/api_handler.h"
#include "server/http_server.h"
#include <fstream>
#include <muduo/net/http/HttpServer.h>
#include <muduo/net/http/HttpRequest.h>
#include <muduo/net/http/HttpResponse.h>

NAMESPACE_BEGIN{ namespace api{
    APIHandler::APIHandler(const std::string & hello_page){
        std::ifstream file(hello_page, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            LOG_ERROR << "Cannot open hello_page: " << hello_page;
            throw std::runtime_error("hello page is not found");
        }
        
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        // 读取文件内容
        _hello_page_content.resize(size);
        if (file.read(&_hello_page_content[0], size)) {
            LOG_INFO << "Served file: " << hello_page << " (" << size << " bytes)";
        } else {
            LOG_ERROR << "Cannot read hello_page: " << hello_page;
            throw std::runtime_error("hello page is not readable");            
        }
        file.close();
    }

    APIHandler::~APIHandler(){
        //不作任何处理，主要是提供virtual这个虚析构的方法
    }

    void APIHandler::handle(const server::HttpRequest & req, server::HttpResponse * resp){
        resp->setStatusCode(server::HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->setBody(_hello_page_content);
    }
}}