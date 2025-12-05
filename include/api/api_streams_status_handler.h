#ifndef KDY_API_DEVICE_HANDLER_H
#define KDY_API_DEVICE_HANDLER_H
#include "utils.h"
#include "api_handler.h"

NAMESPACE_BEGIN{
namespace rtp{ class Device;}
    
namespace api{
    struct RtmpInfo {
        std::string name;        // 设备名称
        std::string stream_id;   // 流ID
        
        RtmpInfo() = default;
        RtmpInfo(const std::string& n, const std::string& s) 
            : name(n), stream_id(s) {}
        
        DEFINE_JSON(RtmpInfo, name, stream_id)
    };

    struct DeviceListApiResponse {
        bool success;
        std::string message;
        std::vector<RtmpInfo> rtmp_list;
        
        DeviceListApiResponse() : success(false) {}
        DeviceListApiResponse(bool s, const std::string& msg) 
            : success(s), message(msg) {}
        
        DeviceListApiResponse(bool s, const std::string& msg, std::vector<RtmpInfo>& list)
            : success(s), message(msg), rtmp_list(std::move(list)) {}
        
        // 序列化支持
        DEFINE_JSON(DeviceListApiResponse, success, message, rtmp_list)
    };

    /**
     * @brief 获取活跃设备列表，对应的调用API为GET /api/streams/status
     */
    class DeviceListHandler : public APIHandler{
    public:
        virtual void handle(const server::HttpRequest & req, server::HttpResponse * resq);
    
    private:
        RtmpInfo Device2RtmpInfo(rtp::Device & de);
    };
}}


#endif