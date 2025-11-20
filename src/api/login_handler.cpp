#include "api/login_handler.h"
#include "server/http_base.h"
#include "db/db_connection_pool.h"
#include "db/kdy_db_api.h"
#include "utils.h"
#include <fstream>

NAMESPACE_BEGIN{ namespace api{
    LoginHandler::LoginHandler(){
        auto results = db::DBConnectionPool::instance().getConnection()->selectAll<db::User>();
        for(auto & result : results){
            user_table.insert({result.user_name, result});
        }
    }

    void LoginHandler::handle(const server::HttpRequest & req, server::HttpResponse * resp){
        static const LoginReturnJson userNotFound{false, "用户名不存在", ""};
        static const LoginReturnJson wrongPassword{false, "密码错误", ""};
        static const LoginReturnJson loginSuccess{true, "登录成功", "/main/index.html"};
        static const LoginReturnJson noParam{false, "未填写用户名和密码", ""};

        const std::string & body = req.body();

        resp->setStatusCode(server::HttpResponse::k200Ok);
        resp->setStatusMessage("OK but not success");
        resp->setContentType("text/plain");

        if(!body.empty()){
            json j = json::parse(body);
            std::string user_name = j.value("username", "unknown");
            std::string password = j.value("password", "unknown");
            if(auto itr = user_table.find(user_name); itr != user_table.end()){
                if(password == itr->second.user_password){
                    resp->setStatusMessage("OK");
                    resp->setBody(json(loginSuccess).dump());
                } else resp->setBody(json(wrongPassword).dump());
            } else resp->setBody(json(userNotFound).dump()); //未找到用户
        } else {
            resp->setBody(json(noParam).dump());
        }
    }
}}