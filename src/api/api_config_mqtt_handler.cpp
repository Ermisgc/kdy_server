#include "api/api_config_mqtt_handler.h"
#include "rtp/mqtt.h"
NAMESPACE_BEGIN{ namespace api{
    ConfigMQTTHandler::ConfigMQTTHandler(bool is_cmd){
        if(is_cmd) config.dataTopic = CAMERA_CONTROL_TOPIC;
        //如果是cmd，说明它订阅的是/camera_params
        else config.dataTopic = DISTANCE_TOPIC;
        //否则说明它订阅的是/distance
        config.clientId = rtp::MQTTClient::getInstance().cliendID();
    }

    void ConfigMQTTHandler::handle(const server::HttpRequest & req, server::HttpResponse * resq){
        static MQTTGetResponse errorGetResponse{false, "No mqtt config get"};
        static MQTTPutResponse defaultPutResponse{true, "Done"};
        auto method = req.method();
        if(method == server::HttpMethod::kGet) {
            resq->setStatusCode(server::HttpResponse::k200Ok);
            resq->setStatusMessage("OK");
            resq->setContentType("text/plain");
            MQTTGetResponse get_response{true, "success", config};
            resq->setBody(json(get_response).dump());
        } else if(method == server::HttpMethod::kPut){
            resq->setStatusCode(server::HttpResponse::k200Ok);
            resq->setStatusMessage("OK");
            resq->setContentType("text/plain");
            resq->setBody(json(defaultPutResponse).dump());
        }
    }
}}