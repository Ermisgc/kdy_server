#ifndef KDY_UTILS_H
#define KDY_UTILS_H

#include <muduo/net/http/HttpServer.h>
#include <muduo/net/http/HttpRequest.h>
#include <muduo/net/http/HttpResponse.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>

#define NAMESPACE_ID kdy
#define NAMESPACE_BEGIN namespace NAMESPACE_ID
#define USING_NAMESPACE using namespace kdy

NAMESPACE_BEGIN{
    using Logger = muduo::Logger; 
namespace server{
    //muduo网络库类型继承，现在它们是本项目的类了muduo::net = kdy::server
    using HttpRequest = muduo::net::HttpRequest;
    using HttpResponse = muduo::net::HttpResponse;
    using EventLoop = muduo::net::EventLoop;
    using InetAddress = muduo::net::InetAddress;    
}}

//定义有关www的基础信息：
#define HTML_BASE "./www"
#define HELLO_PAGE "./www/login/index.html"

#endif