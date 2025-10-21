#include "server/http_server.h"
#include "api/api_handler.h"

USING_NAMESPACE;

int main() {
    // 设置日志级别
    Logger::setLogLevel(Logger::INFO);
    
    LOG_INFO << "Starting Server...";
    
    uint16_t listenPort = 2250;
    server::HTTPServer server(listenPort, "KDYServer");

    api::HelloPageHandler hello_api;

    server.addApi("/", &hello_api);

    server.start();
    return 0;
}