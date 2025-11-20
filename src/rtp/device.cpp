#include "rtp/device.h"
NAMESPACE_BEGIN{ namespace rtp{
    Device::Device(const std::string & device_name, const std::string & stream_id):device_name(device_name), stream_id(stream_id), \
        last_heartbeat(std::chrono::steady_clock::now()){
    }

    void Device::setExposureTime(int new_value){
        this->params.exposure_time = new_value;
    }
    
    bool Device::timeover(std::chrono::steady_clock::time_point & now_time) const {
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now_time - last_heartbeat);
        if(duration.count() > 3) return false;
        else return true;
    }

    void Device::fresh_from_json(const json & j){
        
    }

    json Device::toJson() const {

    }

    Device Device::fromJson(const json & j){
        Device device;
        if(j.contains("id")){
            
        }
    }


}}