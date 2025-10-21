#include "server/mime_types.h"

NAMESPACE_BEGIN{ namespace server{
    MimeTypeDetector::MimeTypeDetector(){
        // 常见文件类型的 MIME 类型映射
        mime_map = {
            // 文本类型
            {".txt", "text/plain"},
            {".html", "text/html"},
            {".htm", "text/html"},
            {".css", "text/css"},
            {".js", "application/javascript"},
            {".json", "application/json"},
            {".xml", "application/xml"},
            
            // 图像类型
            {".jpg", "image/jpeg"},
            {".jpeg", "image/jpeg"},
            {".png", "image/png"},
            {".gif", "image/gif"},
            {".bmp", "image/bmp"},
            {".svg", "image/svg+xml"},
            {".ico", "image/x-icon"},
            {".webp", "image/webp"},
            
            // 音频类型
            {".mp3", "audio/mpeg"},
            {".wav", "audio/wav"},
            {".ogg", "audio/ogg"},
            {".flac", "audio/flac"},
            
            // 视频类型
            {".mp4", "video/mp4"},
            {".avi", "video/x-msvideo"},
            {".mov", "video/quicktime"},
            {".webm", "video/webm"},
            
            // 应用程序类型
            {".pdf", "application/pdf"},
            {".zip", "application/zip"},
            {".tar", "application/x-tar"},
            {".gz", "application/gzip"},
            {".7z", "application/x-7z-compressed"},
            {".exe", "application/x-msdownload"},
            {".doc", "application/msword"},
            {".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
            {".xls", "application/vnd.ms-excel"},
            {".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
            {".ppt", "application/vnd.ms-powerpoint"},
            {".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
            
            // 字体类型
            {".ttf", "font/ttf"},
            {".otf", "font/otf"},
            {".woff", "font/woff"},
            {".woff2", "font/woff2"}
        };        
    }
}
}
