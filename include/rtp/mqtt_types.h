#ifndef KDY_RTP_MQTT_TYPES_H
#define KDY_RTP_MQTT_TYPES_H
#include "utils.h"
#include "db/kdy_db_api.h"

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

    using Displacement = db::Displacement;


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

    //<-------------------------->//
    enum class MeasurePointType {
        MEASURE,            // 测量点
        MAIN_REFERENCE,     // 主参考点
        BACKUP_REFERENCE    // 备用参考点
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(MeasurePointType, {
        {MeasurePointType::MEASURE, "measure"},
        {MeasurePointType::MAIN_REFERENCE, "main_reference"},
        {MeasurePointType::BACKUP_REFERENCE, "backup_reference"}
    })

    struct MeasurePoint {
        int id;                     // 测点ID
        std::string pointId;        // 测点标识符
        MeasurePointType type;      // 测点类型
        double targetDiameter;      // 目标直径
        double xDisplacement;       // X轴位移
        double yDisplacement;       // Y轴位移
        double distance;            // 距离
        double coordinateX;         // X坐标
        double coordinateY;         // Y坐标
        std::string pixelInfo;      // 像素信息（可能是JSON字符串或像素坐标）
        bool enabled;               // 是否启用
        
        // 默认构造函数
        MeasurePoint() 
            : id(0), 
            type(MeasurePointType::MEASURE), 
            targetDiameter(0.0),
            xDisplacement(0.0),
            yDisplacement(0.0),
            distance(0.0),
            coordinateX(0.0),
            coordinateY(0.0),
            enabled(false) {}
        
        MeasurePoint(int id, const std::string& pointId, MeasurePointType type = MeasurePointType::MEASURE)
            : id(id), pointId(pointId), type(type), targetDiameter(0.0),
            xDisplacement(0.0), yDisplacement(0.0), distance(0.0),
            coordinateX(0.0), coordinateY(0.0), enabled(true) {}
        
        double getActualDiameter() const {
            return targetDiameter + xDisplacement + yDisplacement;
        }
        
        double getDistanceToOrigin() const {
            return std::sqrt(coordinateX * coordinateX + coordinateY * coordinateY);
        }
        
        bool isInRange(double minDistance, double maxDistance) const {
            return distance >= minDistance && distance <= maxDistance;
        }
        
        std::string getTypeString() const {
            switch (type) {
                case MeasurePointType::MEASURE: return "measure";
                case MeasurePointType::MAIN_REFERENCE: return "main_reference";
                case MeasurePointType::BACKUP_REFERENCE: return "backup_reference";
                default: return "unknown";
            }
        }
        
        void setTypeFromString(const std::string& typeStr) {
            if (typeStr == "measure") {
                type = MeasurePointType::MEASURE;
            } else if (typeStr == "main_reference") {
                type = MeasurePointType::MAIN_REFERENCE;
            } else if (typeStr == "backup_reference") {
                type = MeasurePointType::BACKUP_REFERENCE;
            } else {
                type = MeasurePointType::MEASURE; // 默认值
            }
        }
        
        bool isValid() const {
            return id >= 0 && !pointId.empty() && 
                (coordinateX != 0.0 || coordinateY != 0.0 || distance != 0.0);
        }
        
        bool isReferencePoint() const {
            return type == MeasurePointType::MAIN_REFERENCE || 
                type == MeasurePointType::BACKUP_REFERENCE;
        }
        
        DEFINE_JSON(MeasurePoint,
            id, pointId, type, targetDiameter, xDisplacement, yDisplacement,
            distance, coordinateX, coordinateY, pixelInfo, enabled
        )
    };

}}


#endif