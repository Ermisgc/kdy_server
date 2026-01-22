#include "rtp/device_manager.h"
#include <chrono>

NAMESPACE_BEGIN { namespace rtp{
    DeviceManager & DeviceManager::instance(){
        static DeviceManager dm;
        static Device test("test_camera", "stream123");  //每次获取的自动刷新这个
        json jso_dvc;
        to_json(jso_dvc, test);
        dm.freshDevice(jso_dvc);
        return dm;
    }
    
    std::vector<Device> DeviceManager::getActiveDeviceList(){
        std::vector<Device> ret;
        auto now = std::chrono::steady_clock::now();
        for(auto & [device_name, device] : this->device_map){
            if(!device.timeover(now)) ret.push_back(device);
        }
        return ret;
    }

    bool DeviceManager::freshDevice(const json & j){
        std::string device_name;
        if(!j.contains("device_name")){
            device_name = "test_camera";
        } else device_name = j["device_name"];

        auto now = std::chrono::steady_clock::now();
        auto itr = this->device_map.find(device_name);
        Device temp;
        from_json(j, temp);
        temp.updateHeartbeat(now);
        if(itr == device_map.end()){
            device_map.emplace(device_name, temp);
        } else {
            device_map[device_name] = std::move(temp);
        }
        return true;
    }

    std::optional<CameraConfig> DeviceManager::getDeviceParams(const std::string & device_name) const {
        auto itr = device_map.find(device_name);
        if(itr == device_map.end()){
            return std::nullopt;
        } else {
            return itr->second.getParams();
        }
    }
}}