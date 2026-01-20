#include "server/http_server.h"
#include "api/api_handler.h"
#include "api/api_login_handler.h"
#include "api/api_streams_status_handler.h"
#include "api/device_heartbeat_handler.h"
#include <iostream>
// #include "rtp/mqtt.h"

USING_NAMESPACE;

int main() {
    // 设置日志级别
    Logger::setLogLevel(Logger::INFO);
    
    LOG_INFO << "Starting Server...";
    
    uint16_t listenPort = 2250;
    server::HTTPServer server(listenPort, "KDYServer");

    api::HelloPageHandler hello_api;
    api::LoginHandler login_api;
    api::DeviceListHandler stream_status_api;
    api::DeviceHeartbeatHandler heartbeat_api;

    server.addApi("/", &hello_api);
    server.addApi("/api/login", &login_api);
    server.addApi("/api/streams/status", &stream_status_api);
    server.addApi("/device/heartbeat", &heartbeat_api);

    // client.loop_start();

    server.start();
    return 0;
}