#include "server/http_server.h"
#include "api/api_handler.h"
#include "api/login_handler.h"
#include <iostream>

USING_NAMESPACE;

int main() {
    // 设置日志级别
    Logger::setLogLevel(Logger::INFO);
    
    LOG_INFO << "Starting Server...";
    
    uint16_t listenPort = 2250;
    server::HTTPServer server(listenPort, "KDYServer");

    api::HelloPageHandler hello_api;
    api::LoginHandler login_api;

    server.addApi("/", &hello_api);
    server.addApi("/api/login", &login_api);

    server.start();
    return 0;
}

// #define FEILD_NAME(...) \
//     std::string fieldsName() { \
//         std::string names = #__VA_ARGS__;  \
//         return names; \
//     }

// FEILD_NAME(user_name, user_password, id)

// int main(){
//     std::cout << fieldsName() << std::endl;
//     return 0;
// }