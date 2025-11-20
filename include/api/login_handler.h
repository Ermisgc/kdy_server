#ifndef KDY_LOGIN_HANDLER_H
#define KDY_LOGIN_HANDLER_H
#include "utils.h"
#include "api_handler.h"
#include <string>
#include <unordered_map>
#include "db/kdy_db_api.h"

NAMESPACE_BEGIN {
namespace api{
    struct LoginReturnJson{
        bool success;  //这里没办法内存对齐??
        std::string message;
        std::string redirectUrl;
        DEFINE_JSON(LoginReturnJson, success, message, redirectUrl)
    };

    /**
     * @brief 用于注册的类，对应的api为：/api/login
     */
    class LoginHandler: public APIHandler{
        //本地缓存的
        std::unordered_map<std::string, db::User> user_table;
    public:
        LoginHandler();
        virtual ~LoginHandler() noexcept = default;

        virtual void handle(const server::HttpRequest & req, server::HttpResponse * resq);
    };
}}
#endif