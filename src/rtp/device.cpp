#include "rtp/device.h"
NAMESPACE_BEGIN{ namespace rtp{
    Device::Device(const std::string & device_name, const std::string & stream_id):device_name(device_name), stream_id(stream_id), \
        last_heartbeat(std::chrono::steady_clock::now()){
    }

    Device::Device(const Device & other):device_name(other.device_name), stream_id(other.stream_id), last_heartbeat(other.last_heartbeat){
    }

    Device & Device::operator=(Device && other){
        std::lock_guard<std::mutex> locker(mtx);
        this->params = std::move(other.params);
        this->device_name = std::move(other.device_name);
        this->stream_id = std::move(other.stream_id);
        this->last_heartbeat = std::move(other.last_heartbeat);
        return *this;
    }
    
    bool Device::timeover(std::chrono::steady_clock::time_point & now_time) const {
        std::lock_guard<std::mutex> locker(mtx);
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now_time - last_heartbeat);
        if(duration.count() > DEVICE_TIMEOUT_SECOND) return true;
        else return false;
    }
}}