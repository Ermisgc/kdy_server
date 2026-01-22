#include "api/api_streams_status_handler.h"
#include "rtp/device_manager.h"

NAMESPACE_BEGIN{ namespace api{
    void DeviceListHandler::handle(const server::HttpRequest & req, server::HttpResponse * resq){
        auto res = rtp::DeviceManager::instance().getActiveDeviceList();
        std::vector<RtmpInfo> rif;
        for(auto & de: res){
            rif.emplace_back(Device2RtmpInfo(de));
        }
        
        static const DeviceListApiResponse noDeviceFoundJson(false, "No device found.");
        int count = res.size();
        if(count == 0){
            resq->setStatusCode(server::HttpResponse::k200Ok);
            resq->setStatusMessage("OK but not success");
            resq->setContentType("text/plain");
            resq->setBody(json(noDeviceFoundJson).dump());
        } else {
            DeviceListApiResponse deviceList(true, std::to_string(count) + " devices is active.", rif);
            resq->setStatusCode(server::HttpResponse::k200Ok);
            resq->setStatusMessage("OK");
            resq->setContentType("text/plain");
            resq->setBody(json(deviceList).dump());            
        }
    }

    RtmpInfo DeviceListHandler::Device2RtmpInfo(rtp::Device & de){
        RtmpInfo ret;
        ret.name = std::move(de.device_name);
        ret.stream_id = std::move(de.stream_id);
        return ret;
    }
}}