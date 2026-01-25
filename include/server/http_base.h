#ifndef KDY_HTTP_BASE_H
#define KDY_HTTP_BASE_H
#include "utils.h"

NAMESPACE_BEGIN { namespace server{
    enum class HttpMethod {
        kInvalid, kGet, kPost, kHead, kPut, kDelete
    };

    enum class HttpVersion {
        kInvalid, kHttp10, kHttp11
    };

    class UrlParser {
    public:
        /**
         * @brief 解析查询参数: duration=60&type=1 -> map
         */
        static std::map<std::string, std::string> parseQuery(const std::string& query_str);

        /**
         * @brief 分离路径和查询串: /abc?q=1 -> {"/abc", "q=1"}
         */
        static void splitPathAndQuery(const std::string& raw_url, std::string& path_out, std::string& query_out);
    };
    
    /**
     * @brief Http请求类的封装，仿照muduo网络库封装，但支持body
     */
    class HttpRequest {
        HttpMethod method_;        
        HttpVersion version_;     
        std::map<std::string, std::string> headers_;
        std::string body_;
    public:
        std::string path_;      ///< 真实路径
        std::string sub_path;   ///< 后缀路径
        std::map<std::string, std::string> params_;
        HttpRequest() : method_(HttpMethod::kInvalid), version_(HttpVersion::kInvalid) {}

        void setMethod(HttpMethod method) { method_ = method; }
        void setPath(const std::string& path) { path_ = path; }
        void setVersion(HttpVersion version) { version_ = version; }
        void addHeader(const std::string& key, const std::string& value) { headers_[key] = value; }
        void setBody(const std::string& body) { body_ = body; }
        std::string methodString() const;

        HttpMethod method() const { return method_; }
        const std::string& path() const { return path_; }
        HttpVersion version() const { return version_; }
        const std::string& getHeader(const std::string& key) const {
            static const std::string empty;
            auto it = headers_.find(key);
            return it != headers_.end() ? it->second : empty;
        }
        const std::map<std::string, std::string> & headers() const {return headers_;}
        const std::string& body() const { return body_; }
    };

    /**
     * @brief Http响应类的封装，仿照muduo网络库设计
     */
    class HttpResponse {
    public:
        enum StatusCode {
            k200Ok = 200,
            k400BadRequest = 400,
            k404NotFound = 404,
            k500InternalError = 500
        };

        HttpResponse(bool close) : statusCode_(k200Ok), closeConnection_(close) {}

        void setStatusCode(StatusCode code) { statusCode_ = code; }
        void setStatusMessage(const std::string& msg) { statusMessage_ = msg; }
        void addHeader(const std::string& key, const std::string& value) { headers_[key] = value; }
        void setBody(const std::string& body) { body_ = body; }
        void setCloseConnection(bool on) { closeConnection_ = on; }
        void setContentType(const std::string & content_type) { addHeader("Content-Type", content_type);}
        bool closeConnection() const { return closeConnection_; }

        /**
         * @brief 将响应转化为HTTP格式的字节流，写入Buffer
         */
        void appendToBuffer(Buffer* buf) const;

    private:
        StatusCode statusCode_;                  // 状态码
        std::string statusMessage_;              // 状态描述
        std::map<std::string, std::string> headers_;  // 响应头部
        std::string body_;                       // 响应体
        bool closeConnection_;                   // 是否关闭连接（短连接/长连接）
    };    


}}

#endif