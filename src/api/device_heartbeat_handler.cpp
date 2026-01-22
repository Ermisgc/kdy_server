#include "api/device_heartbeat_handler.h"
NAMESPACE_BEGIN { namespace api{
    void DeviceHeartbeatHandler::handle(const server::HttpRequest & req, server::HttpResponse * resp){
        resp->setStatusCode(server::HttpResponse::k200Ok);
        resp->setContentType("text/plain");

        static const DeviceHeartbeatResponse notApplyResp{false, "Json must contain device_name and stream_id"};
        auto & body= req.body();
        auto j = json(body);

        //Step1. 先获取更新的信息，从而让相机知道要更改哪些参数
        if(!j.contains("device_name") || !j.contains("stream_id")){
            resp->setStatusMessage("Ok but not success");
            resp->setBody(json(notApplyResp).dump());
            return;
        }

        auto & device_name = j["device_name"];
        auto & dm = rtp::DeviceManager::instance();
        if(auto ret = dm.getDeviceParams(device_name)){
            if(ret.has_value()){
                DeviceHeartbeatResponse response{true, "Check Params", ret.value()};
                resp->setStatusMessage("Ok");
                resp->setBody(json(response).dump());                
            }
        }

        //Step2. 再修改相机的信息
        auto success = rtp::DeviceManager::instance().freshDevice(j);
        if(success){
            DeviceHeartbeatResponse response{true, "First Connect", dm.getDeviceParams(device_name).value()};
            resp->setStatusMessage("Ok");
            resp->setBody(json(response).dump());   
        } else {
            resp->setStatusCode(server::HttpResponse::k500InternalError);
            resp->setStatusMessage("Error happens in server");
        }
    }
}}