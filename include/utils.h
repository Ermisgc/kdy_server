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
#define DEFINE_JSON(type, ...) NLOHMANN_DEFINE_TYPE_INTRUSIVE(type, __VA_ARGS__)
#else
    
#endif
#endif