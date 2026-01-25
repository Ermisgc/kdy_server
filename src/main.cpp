#include "server/http_server.h"
#include "api/api_handler.h"
#include "api/api_login_handler.h"
#include "api/api_streams_status_handler.h"
#include "api/device_heartbeat_handler.h"
#include "api/api_config_mqtt_handler.h"
#include "api/api_measure_points_handler.h"
#include "api/api_camera_1_lens_params_handler.h"
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
    api::ConfigMQTTHandler config_mqtt_api(false);
    api::ConfigMQTTHandler config_mqtt2_api(true);
    api::MeasurePointHandler measure_points_api;
    api::CameraLensParamsHandler camera_lens_params_api;

    server.addApi("/", &hello_api);
    server.addApi("/api/login", &login_api);
    server.addApi("/api/streams/status", &stream_status_api);
    server.addApi("/device/heartbeat", &heartbeat_api);
    server.addApi("/api/config/mqtt", &config_mqtt_api);
    server.addApi("/api/config/mqtt2", &config_mqtt2_api);
    server.addApi("/api/measure-points/*", &measure_points_api);
    server.addApi("/api/camera/1/lens/params", &camera_lens_params_api);

    server.start();
    return 0;
}