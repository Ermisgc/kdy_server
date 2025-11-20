#ifndef KDY_DEVICE_H
#define KDY_DEVICE_H
#include "utils.h"
#include <chrono>
#include <mutex>
#include "json/json.hpp"

NAMESPACE_BEGIN{ namespace rtp{
    class Device{
    private:
        std::chrono::steady_clock::time_point last_heartbeat;
        mutable std::mutex mtx;
        std::string device_name;
        std::string stream_id;

        struct Params{
            int exposure_time;
        } params;

    public:
        Device() = default;
        Device(const std::string & device_name, const std::string & stream_id);

        void setExposureTime(int new_value);
        
        bool timeover(std::chrono::steady_clock::time_point & now_time) const;

        void fresh_from_json(const json & j);

        json toJson() const;

        static Device fromJson(const json & j);
    };

}}    

#endif