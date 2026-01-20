#ifndef KDY_MQTT_H
#define KDY_MQTT_H
#include "utils.h"
#include <mosquittopp.h>
#include <mutex>
#include <atomic>
#include <thread>

NAMESPACE_BEGIN{ namespace rtp {
    using mosquittopp = mosqpp::mosquittopp;

    class MqttClient: public mosquittopp{
        static int lib_ref_count;
        static std::mutex lib_mtx;
    public:
        MqttClient(const char * id);
        ~MqttClient();
        void on_connect(int rc) override;
        void on_disconnect(int rc) override;
        void on_message(const struct mosquitto_message * msg) override;
        void on_subscribe(int mid, int qos_count, const int* granted_qos) override;
        void stop();
    private:
    };

}}

#endif