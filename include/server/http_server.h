#ifndef KDY_HTTP_SERVER_H
#define KDY_HTTP_SERVER_H

#include "utils.h"
#include <unordered_map>
#include "server/mime_types.h"

NAMESPACE_BEGIN{ namespace api {
    class APIHandler;  //针对单个API执行某个handle行为
}
    
namespace server {
    class HTTPServer{
        std::unordered_map<std::string, api::APIHandler *> api_maps;
        server::EventLoop loop;
        server::InetAddress listenAddr;
        muduo::net::HttpServer server;  //注意这里的loop listenAddr和server必须按照这个顺序来，不能重排
        MimeTypeDetector mime_detector;

    public:
        HTTPServer(const uint16_t port, const std::string & name);
        ~HTTPServer();

        void start();

        void addApi(const std::string & api_name, api::APIHandler * api_handler);

    private:
        void onRequest(const HttpRequest & req, HttpResponse * resp);

        void handleNotFound(const HttpRequest & req, HttpResponse * resp);

        void handleMethodNotAllowed(const HttpRequest & req, HttpResponse * resp);
    };
}}

#endif