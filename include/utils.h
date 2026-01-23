#ifndef KDY_UTILS_H
#define KDY_UTILS_H

#include <string_view>
#include <vector>
#include <sstream>
#include <muduo/net/TcpServer.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include "json/json.hpp"

#define NAMESPACE_ID kdy
#define NAMESPACE_BEGIN namespace NAMESPACE_ID
#define USING_NAMESPACE using namespace kdy

NAMESPACE_BEGIN{
    using Logger = muduo::Logger; 
    using json = nlohmann::json;
namespace server{
    using TcpServer = muduo::net::TcpServer;
    //muduo网络库类型继承，现在它们是本项目的类了muduo::net = kdy::server
    using EventLoop = muduo::net::EventLoop;
    using InetAddress = muduo::net::InetAddress;    
    using TcpConnectionPtr = muduo::net::TcpConnectionPtr;
    using Buffer = muduo::net::Buffer;
    using TimeStamp = muduo::Timestamp;
}

    //字符串分割函数
    static inline std::vector<std::string> split(const std::string & str, char delimiter = ' '){
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while(std::getline(ss, token, delimiter)){
            tokens.push_back(token);
        }
        return tokens;
    }
}

//定义有关www的基础信息：
#define HTML_BASE "./www"
#define HELLO_PAGE "./www/login/index.html"

#define MySQL_DATABASE_NAME "kdy"
#define MySQL_USER_NAME "root"
#define MySQL_USER_PASSWORD "Mm165597636!"
#define MySQL_DEFAULT_HOST "localhost"
#define MySQL_DEFAULT_PORT 3306
#define MySQL_CONNPOOL_SIZE 10

//有关json处理的宏
#ifdef NLOHMANN_DEFINE_TYPE_INTRUSIVE
//定义to_json、from_json的序列化和反序列化方法
#define DEFINE_JSON(type, ...) NLOHMANN_DEFINE_TYPE_INTRUSIVE(type, __VA_ARGS__)
#else
#endif

//有关mqtt的宏定义
#define MQTT_HOST "120.27.205.147:1883"
#define MQTT_BROKER "tcp://120.27.205.147:1883"
#define DISTANCE_TOPIC "/distance"
#define CAMERA_CONTROL_TOPIC "/camera_params"
#define CAMERA_MESSAGE_TOPIC "/CCC"
#define MQTT_QOS 0
#define MQTT_USERNAME "MQTT3"
#define MQTT_PASSWORD "123456"
#define MQTT_CLIENTID "test"

#define DEVICE_TIMEOUT_SECOND 30
#endif