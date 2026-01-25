#ifndef KDY_MQTT_H
#define KDY_MQTT_H
#include "utils.h"
#include "rtp/mqtt_types.h"
#include <mqtt/async_client.h>
#include <mutex>
#include <atomic>
#include <thread>

NAMESPACE_BEGIN{ namespace rtp {
    class MQTTClientCallback : public mqtt::callback {
    public:
        MQTTClientCallback() = default;
        ~MQTTClientCallback() = default;

        /**
         * @brief 连接成功时的回调
         */
        void connected(const std::string& cause) override;

        void connection_lost(const std::string& cause) override;

        void message_arrived(mqtt::const_message_ptr msg) override;

        inline void set_client(mqtt::async_client* client) { 
            client_ = client;
        }
        
        bool send_control_command(const json& command);

    private:
        mqtt::async_client* client_ = nullptr;
        int receive_count_ = 0;
        
        void process_distance_data(const json& data);

        void process_camera_data(const json & data);
    };

    /**
     * @brief MQTT客户端
     */
    class MQTTClient {
    public:
        bool connect(const std::string & user_name, const std::string & password);

        inline bool is_connected(){
            return client.is_connected();
        }

        void sendCameraInfo(const CameraConfig & cc);
        
        static MQTTClient & getInstance();

        inline std::string cliendID() {return this->client_id;}

        inline void setCameraConfig(CameraConfig & cc) {
            std::lock_guard<std::mutex> locker(client_mtx);
            current_camera_config = cc;
        }

        inline CameraConfig getCameraConfig(){ 
            std::lock_guard<std::mutex> locker(client_mtx);
            return current_camera_config;
        }
    private:
        MQTTClient() = delete;
        MQTTClient(const std::string & broker, const std::string & client_id);
        ~MQTTClient();

        std::string client_id;
        mqtt::async_client client;
        mqtt::connect_options conn_opts;
        MQTTClientCallback callback;
        std::mutex client_mtx;

        CameraConfig current_camera_config;
    };  
}}

#endif