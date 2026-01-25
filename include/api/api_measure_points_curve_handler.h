#ifndef KDY_API_MEASURE_POINTS_CURVE_HANDLER_H
#define KDY_API_MEASURE_POINTS_CURVE_HANDLER_H
#include "utils.h"
#include "api_handler.h"
#include "rtp/mqtt_types.h"

NAMESPACE_BEGIN {
namespace api{
    struct MeasurePointsCurveResponse {
        bool success = false;
        std::vector<db::Displacement> data;

        DEFINE_JSON(MeasurePointsCurveResponse, success, data)
    };

    class MeasurePointsCurveHandler: public APIHandler{
    public:
        MeasurePointsCurveHandler() = default;
        virtual ~MeasurePointsCurveHandler() noexcept = default;

        virtual void handle(const server::HttpRequest & req, server::HttpResponse * resq);
    };
}}
#endif //KDY_API_MEASURE_POINTS_CURVE_HANDLER_H