#ifndef KDY_DEVICE_H
#define KDY_DEVICE_H
#include "utils.h"
#include <chrono>
#include <mutex>
#include "json/json.hpp"

NAMESPACE_BEGIN{
namespace api{ class DeviceListHandler; }
namespace rtp{
    struct DeviceParams{
        int exposure_time;
        DEFINE_JSON(DeviceParams, exposure_time)
    };

    class Device{
    private:
        std::chrono::steady_clock::time_point last_heartbeat;
        mutable std::mutex mtx;
        std::string device_name;
        std::string stream_id;
        DeviceParams params;

    public:
        Device() = default;
        Device(const std::string & device_name, const std::string & stream_id);

        Device(const Device & other);

        Device & operator=(Device && other);

        inline void setExposureTime(int new_value) { std::lock_guard<std::mutex> locker(mtx); params.exposure_time = new_value;}
        
        bool timeover(std::chrono::steady_clock::time_point & now_time) const;

        DEFINE_JSON(Device, device_name, stream_id, params);

        inline void updateHeartbeat(const std::chrono::steady_clock::time_point & t) {last_heartbeat = t;}

        inline const DeviceParams * getParams() const {std::lock_guard<std::mutex> locker(mtx); return &params;}

        friend class api::DeviceListHandler;
    };
}}    

#endif