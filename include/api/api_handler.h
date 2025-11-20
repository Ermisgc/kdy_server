#ifndef KDY_API_HANDLER_H
#define KDY_API_HANDLER_H

#include "utils.h"
#include "server/http_base.h"
#include <string>
NAMESPACE_BEGIN {
namespace api{
    /**
     * @brief 一个普通的API基类，在只使用基类的情况下，它会返回base页面，即login/index.html页面
     */
    class APIHandler{
    private:
        std::string _hello_page_content; 
    public:
        APIHandler(const std::string & hello_page = HELLO_PAGE);
        virtual ~APIHandler() noexcept;

        virtual void handle(const server::HttpRequest & req, server::HttpResponse * resq);
    };

    typedef APIHandler HelloPageHandler;
}}

#endif