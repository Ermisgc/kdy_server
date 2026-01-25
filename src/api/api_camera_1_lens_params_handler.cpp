#include "api/api_camera_1_lens_params_handler.h"

NAMESPACE_BEGIN{ namespace api{
    void CameraLensParamsHandler::handle(const server::HttpRequest & req, server::HttpResponse * resq){
        static CameraConfigResponse ErrorResponse{false, "Error Happens when trying to get camera config"};
        static CameraConfigPutReponse SuccessPutResponse{true, "Ok"};
        auto method = req.method();

        if(method == server::HttpMethod::kGet){
            resq->setStatusCode(server::HttpResponse::k200Ok);
            resq->setStatusMessage("OK");
            resq->setContentType("text/plain");
            rtp::CameraConfig ret = rtp::MQTTClient::getInstance().getCameraConfig();
            CameraConfigResponse ccr{true, "Ok", ret};
            resq->setBody(json(ccr).dump());
        } else if(method == server::HttpMethod::kPost){
            json body_json = json::parse(req.body());
            rtp::CameraConfig cc;
            from_json(body_json, cc);
            rtp::MQTTClient::getInstance().sendCameraInfo(cc);

            resq->setStatusCode(server::HttpResponse::k200Ok);
            resq->setStatusMessage("OK");
            resq->setContentType("text/plain");
            resq->setBody(json(SuccessPutResponse).dump());
        }
    }
}}