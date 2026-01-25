#include "api/api_measure_points_curve_handler.h"
#include "db/kdy_db_api.h"

NAMESPACE_BEGIN{ namespace api{
    void MeasurePointsCurveHandler::handle(const server::HttpRequest & req, server::HttpResponse * resq){
        static MeasurePointsCurveResponse ErrorResponse{false};
        auto method = req.method();

        if(method == server::HttpMethod::kGet){
            resq->setStatusCode(server::HttpResponse::k200Ok);
            resq->setStatusMessage("OK");
            resq->setContentType("text/plain");
            //请求数据库
            MeasurePointsCurveResponse cr;
            cr.success = true;
            cr.data = db::Displacement::queryDuration("123", 60);
            resq->setBody(json(cr).dump());
        } else {
            resq->setStatusCode(server::HttpResponse::k404NotFound);
            resq->setStatusMessage("OK");
            resq->setContentType("text/plain");
            resq->setBody(json(ErrorResponse).dump());
        }
    }
}}