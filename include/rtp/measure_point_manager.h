#ifndef KDY_MEASURE_POINT_MANAGER_H
#define KDY_MEASURE_POINT_MANAGER_H

#include "rtp/measure_point_manager.h"
#include "rtp/mqtt_types.h"
NAMESPACE_BEGIN{ namespace rtp{
    class MeasurePointManager{
    public:
        static inline MeasurePointManager & getInstance(){
            static MeasurePointManager instance;
            return instance;
        }

        std::vector<MeasurePoint> & getPointList(){
            return points;
        } 
    private:
        MeasurePointManager(){
            MeasurePoint mp;
            mp.id = points.size();
            mp.pointId = "123";
            mp.type = MeasurePointType::MAIN_REFERENCE;
            mp.xDisplacement = 600;
            mp.yDisplacement = 400;
            mp.coordinateX = 600;
            mp.coordinateY = 400;
            mp.enabled = true;
            points.push_back(mp);
        }
        ~MeasurePointManager() = default;

        std::vector<MeasurePoint> points;
    };

}}

#endif