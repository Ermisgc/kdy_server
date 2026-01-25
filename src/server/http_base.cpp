#include "server/http_base.h"
NAMESPACE_BEGIN { namespace server{
    std::map<std::string, std::string> UrlParser::parseQuery(const std::string& query_str) {
        std::map<std::string, std::string> params;
        std::stringstream ss(query_str);
        std::string segment;
        while (std::getline(ss, segment, '&')) {
            auto pos = segment.find('=');
            if (pos != std::string::npos) {
                params[segment.substr(0, pos)] = segment.substr(pos + 1);
            }
        }
        return params;
    }

    void UrlParser::splitPathAndQuery(const std::string& raw_url, std::string& path_out, std::string& query_out) {
        auto pos = raw_url.find('?');
        if (pos != std::string::npos) {
            path_out = raw_url.substr(0, pos);
            query_out = raw_url.substr(pos + 1);
        } else {
            path_out = raw_url;
            query_out = "";
        }
    }

    void HttpResponse::appendToBuffer(Buffer* buf) const {
        char tmpBuf[32];
        // 状态行,如 HTTP/1.1 200 OK
        snprintf(tmpBuf, sizeof(tmpBuf), "HTTP/1.1 %d ", statusCode_);
        buf->append(tmpBuf);
        buf->append(statusMessage_);
        buf->append("\r\n");

        // 响应头部
        if (closeConnection_) {
            buf->append("Connection: close\r\n");  // 短连接
        } else {
            snprintf(tmpBuf, sizeof(tmpBuf), "Content-Length: %zd\r\n", body_.size());
            buf->append(tmpBuf);
            buf->append("Connection: keep-alive\r\n");  // 长连接
        }

        for (const auto& header : headers_) {
            buf->append(header.first);
            buf->append(": ");
            buf->append(header.second);
            buf->append("\r\n");
        }

        // 空行（头部与体的分隔）
        buf->append("\r\n");
        // 响应体
        buf->append(body_);        
    }

    std::string HttpRequest::methodString() const {
        switch (this->method_)
        {
        case HttpMethod::kGet:
            return "GET";

        case HttpMethod::kPost:
            return "POST";

        case HttpMethod::kHead:
            return "HEAD";

        case HttpMethod::kPut:
            return "PUT";

        case HttpMethod::kDelete:
            return "DELETE";

        default:
            break;
        }
        return "INVALID";
    }

}}