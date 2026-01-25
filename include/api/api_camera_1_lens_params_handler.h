#ifndef KDY_API_LENS_PARAMS_HANDLER_H
#define KDY_API_LENS_PARAMS_HANDLER_H
#include "utils.h"
#include "api_handler.h"
#include "rtp/mqtt.h"

NAMESPACE_BEGIN {
namespace api{
    struct CameraConfigResponse {
        bool success = false;
        std::string message;
        rtp::CameraConfig data;
        
        DEFINE_JSON(CameraConfigResponse,
            success, message, data
        )
    };

    struct CameraConfigPutReponse{
        bool success = false;
        std::string message;
        DEFINE_JSON(CameraConfigPutReponse, success, message)
    };

    class CameraLensParamsHandler: public APIHandler{
    public:
        CameraLensParamsHandler() = default;
        virtual ~CameraLensParamsHandler() noexcept = default;

        virtual void handle(const server::HttpRequest & req, server::HttpResponse * resq);
    };
}}
#endif