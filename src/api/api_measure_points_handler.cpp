#include "api/api_measure_points_handler.h"
#include "rtp/measure_point_manager.h"
NAMESPACE_BEGIN{ namespace api{
    void MeasurePointHandler::handle(const server::HttpRequest & req, server::HttpResponse * resq){
        static MeasurePointResponse nullMeasurePointResponse{false, "No mqtt config get"};
        auto pts = rtp::MeasurePointManager::getInstance().getPointList();
        if(pts.size() > 0){
            resq->setStatusCode(server::HttpResponse::k200Ok);
            resq->setStatusMessage("OK");
            resq->setContentType("text/plain");
            MeasurePointResponse get_response{true, "success", pts};
            resq->setBody(json(get_response).dump());
        } else {
            resq->setStatusCode(server::HttpResponse::k200Ok);
            resq->setStatusMessage("OK but not success");
            resq->setContentType("text/plain");
            resq->setBody(json(nullMeasurePointResponse).dump());
        }
    }
}}