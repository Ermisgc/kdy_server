#include "rtp/mqtt.h"
NAMESPACE_BEGIN{ namespace rtp{
    int MqttClient::lib_ref_count{0};
    std::mutex MqttClient::lib_mtx;

    MqttClient::MqttClient(const char * id): mosquittopp(id){
        std::lock_guard<std::mutex> locker(lib_mtx);
        if(lib_ref_count == 0){
            
            auto rc = mosqpp::lib_init();
            if(rc != MOSQ_ERR_SUCCESS){
                LOG_ERROR << "Failed to initialize Mosquitto library";
            }
        }
        lib_ref_count ++;
    }

    MqttClient::~MqttClient(){
        this->loop_stop();
        
        std::lock_guard<std::mutex> locker(lib_mtx);
        lib_ref_count --;
        if(lib_ref_count == 0) auto rc = mosqpp::lib_cleanup();
    }

    void MqttClient::on_connect(int rc) {
        if (rc == 0) LOG_INFO << "MQTT Connect Successful";
        else LOG_WARN << "MQTT Connection Fail: " << rc;

        static constexpr std::string PID = "B6WOt3UsAW";
        static constexpr std::string DEVICE_NAME = "dht11";

        std::string propertyset = "$sys/";
        propertyset += PID;
        propertyset += "/";
        propertyset += DEVICE_NAME;
        propertyset += "/thing/property/set";
        subscribe(nullptr, propertyset.c_str() , 1);  //TODO:需要了解subscribe的各项参数
    }

    void MqttClient::on_subscribe(int mid, int qos_count, const int* granted_qos) {
        LOG_INFO << "Subscribe Done, id: " << mid;
    }

    void MqttClient::on_disconnect(int rc) {
        if (rc == 0) LOG_INFO << "MQTT DisConnect Successful";
    }

    void MqttClient::on_message(const struct mosquitto_message * msg) {
        // struct mosquitto_message{
        //     int mid;
        //     char *topic;
        //     void *payload;
        //     int payloadlen;
        //     int qos;
        //     bool retain;
        // };
        LOG_INFO << "Message received on topic: " << msg->topic;
        LOG_INFO << "Message: " << msg->payload;
    }
}}