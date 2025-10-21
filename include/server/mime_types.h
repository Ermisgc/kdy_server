#ifndef KDY_MIME_TYPES_H
#define KDY_MIME_TYPES_H

#include "utils.h"
#include <string>
#include <unordered_map>
#include <filesystem>

NAMESPACE_BEGIN{ namespace server{
    /**
     * @brief 用于获得文件的mime_type，对应于HTTP格式的ContentType字段
     */
    class MimeTypeDetector {
    private:
        std::unordered_map<std::string, std::string> mime_map;
    public:
        MimeTypeDetector();
        
        inline std::string getContentType(const std::string & filename){
            auto itr = mime_map.find(getExtension(filename));
            if(itr != mime_map.end()) return itr->second;
            else return "";
        }

        inline std::string getMimeType(const std::string & filename){
            return getContentType(filename);
        }

    private:
        inline std::string getExtension(const std::string & filename){
            std::filesystem::path path(filename);
            std::string ext = path.extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            return ext;
        }
    };

}}

#endif