#ifndef KDY_DEVICE_HEARTBEAT_HANDLER_H
#define KDY_DEVICE_HEARTBEAT_HANDLER_H
#include "utils.h"
#include "api/api_handler.h"
#include "rtp/device_manager.h"

NAMESPACE_BEGIN{ namespace api{
    struct DeviceHeartbeatResponse{
        bool success;
        std::string message;
        rtp::DeviceParams params;

        DEFINE_JSON(DeviceHeartbeatResponse,success,message,params)
    };

    /**
     * @brief 更新设备心跳，对应的调用API为POST /device/heartbeat
     */
    class DeviceHeartbeatHandler: public APIHandler {
    public:
        virtual void handle(const server::HttpRequest & req, server::HttpResponse * resq);
    };
}}
#endif