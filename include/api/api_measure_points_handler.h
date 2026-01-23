#ifndef KDY_API_MEASURE_POINTS_H
#define KDY_API_MEASURE_POINTS_H
#include "utils.h"
#include "api_handler.h"
#include <string>
#include <unordered_map>
#include "db/kdy_db_api.h"
#include "rtp/mqtt_types.h"

NAMESPACE_BEGIN {
namespace api{
    struct MeasurePointResponse {
        bool success = false;
        std::string message;
        std::vector<rtp::MeasurePoint> data;
        
        DEFINE_JSON(MeasurePointResponse,
            success, message, data
        )
    };

    class MeasurePointHandler: public APIHandler{
    public:
        MeasurePointHandler() = default;
        virtual ~MeasurePointHandler() noexcept = default;

        virtual void handle(const server::HttpRequest & req, server::HttpResponse * resq);
    };
}}
#endif