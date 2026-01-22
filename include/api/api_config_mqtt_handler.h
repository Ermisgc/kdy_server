#ifndef KDY_API_CONFIG_MQTT_HANDLER_H
#define KDY_API_CONFIG_MQTT_HANDLER_H

#include "utils.h"
#include "server/http_base.h"
#include "api/api_handler.h"
#include "rtp/mqtt_types.h"
#include <string>
NAMESPACE_BEGIN {
namespace api{
    struct MQTTGetResponse{
        bool success = false;
        std::string message;
        rtp::MQTTConfig data;
        DEFINE_JSON(MQTTGetResponse, success, message, data)
    };

    struct MQTTPutResponse{
        bool success = false;
        std::string message;
        DEFINE_JSON(MQTTPutResponse, success, message)
    };

    class ConfigMQTTHandler: public APIHandler{
    private:
        rtp::MQTTConfig config; 
    public:
        ConfigMQTTHandler(bool is_cmd);
        virtual ~ConfigMQTTHandler() noexcept = default;

        virtual void handle(const server::HttpRequest & req, server::HttpResponse * resq);
    }; 
}}

#endif