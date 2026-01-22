#ifndef KDY_RTP_MQTT_TYPES_H
#define KDY_RTP_MQTT_TYPES_H
#include "utils.h"

NAMESPACE_BEGIN{ namespace rtp{
    struct ImagingParams {
        double brightness = 80.0;
        DEFINE_JSON(ImagingParams, brightness)
    };

    struct Focus {
        int mode = 1;
        int initialize = 1;
        int sensitivity = 2;
        int minFocusLength = 10;
        int ratioLimit = 33;
        int ratioShow = 0;
        int alg = 0;
        DEFINE_JSON(Focus, 
            mode, initialize, sensitivity, minFocusLength, 
            ratioLimit, ratioShow, alg)
    };

    struct CameraConfig{
        ImagingParams imaging_params;
        Focus focus;
        DEFINE_JSON(CameraConfig, imaging_params, focus)
    };

    //<-------------------->//

    struct Displacement {
        double value = 0.0;
        std::string unit = "mm";
        DEFINE_JSON(Displacement, value, unit)
    };

    struct Params {
        Displacement displacement;
        DEFINE_JSON(Params, displacement)
    };

    struct DistanceData {
        std::string id = "123";
        std::string version = "1.0";
        int64_t timestamp = 0;
        Params params;
        DEFINE_JSON(DistanceData, id, version, timestamp, params)
    };


    //<----------------------------->//
    struct MQTTConfig {
        bool enabled = true;
        bool useDefaultPlatform = false;
        std::string protocol = "tcp://";
        std::string serverAddress = MQTT_HOST;
        std::string username = MQTT_USERNAME;
        std::string password = MQTT_PASSWORD;
        std::string dataTopic = DISTANCE_TOPIC;
        std::string clientId;
        std::string imagePushUrl = "stream123";
        std::string offlineDataPushUrl;
        bool eventPushEnabled = false;
        
        DEFINE_JSON(MQTTConfig,
            enabled,
            useDefaultPlatform,
            protocol,
            serverAddress,
            username,
            password,
            dataTopic,
            clientId,
            imagePushUrl,
            offlineDataPushUrl,
            eventPushEnabled
        )
    };

}}


#endif