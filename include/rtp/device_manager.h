#ifndef KDY_DEVICE_MANAGER_H
#define KDY_DEVICE_MANAGER_H
#include "utils.h"
#include "device.h"
#include <unordered_map>
#include <vector>

NAMESPACE_BEGIN{ namespace rtp{
    class DeviceManager{
        std::unordered_map<std::string, Device> device_map;
        
    public:
        static DeviceManager & instance();
        
        std::vector<Device> getActiveDeviceList();

        bool freshDevice(const json & j);

        std::optional<const DeviceParams *> getDeviceParams(const std::string & device_name) const;
    };
}}    

#endif