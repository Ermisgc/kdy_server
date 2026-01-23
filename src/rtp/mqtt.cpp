#include "rtp/mqtt.h"
NAMESPACE_BEGIN{ namespace rtp{
    json get_fixed_control_command() {
        ImagingParams dd;
        json ret;
        to_json(ret, dd);
        return ret;
    }

    void MQTTClientCallback::connected(const std::string& cause) {
        LOG_INFO << "MQTT Connected...";
        if(!client_) return;
        try {
            auto ret = client_->subscribe(DISTANCE_TOPIC, MQTT_QOS);
            LOG_INFO << "MQTT Subscribe: " << DISTANCE_TOPIC;
            
            ret = client_->subscribe(CAMERA_MESSAGE_TOPIC, MQTT_QOS);
            LOG_INFO << "MQTT Subscribe: " << CAMERA_MESSAGE_TOPIC;        
        } catch (const mqtt::exception& e) {
            LOG_ERROR << "MQTT Subscrible Failed: " << e.what();
        }
    }

    void MQTTClientCallback::connection_lost(const std::string& cause) {
        LOG_WARN << "MQTT disconnect: " << (cause.empty() ? "Unknown" : cause);
    }

    void MQTTClientCallback::message_arrived(mqtt::const_message_ptr msg) {
        try {
            std::string topic = msg->get_topic();
            std::string payload = msg->get_payload_str();
            json data = json::parse(payload);            
            if (topic == DISTANCE_TOPIC) {
                process_distance_data(data);
            } else if(topic == CAMERA_MESSAGE_TOPIC){
                process_camera_data(data);
            }
        } catch (const std::exception& e) {
            LOG_ERROR << "MQTT Process failed: " << e.what();
        }
    }
    
    bool MQTTClientCallback::send_control_command(const json& command) {
        if(!client_) return false;
        try {
            std::string payload = command.dump();
            mqtt::message_ptr msg = mqtt::make_message(CAMERA_CONTROL_TOPIC, payload);
            msg->set_qos(MQTT_QOS);
            
            auto pub_token = client_->publish(msg);
            pub_token->wait();
            
            return true;
        } catch (const mqtt::exception& e) {
            LOG_ERROR << "MQTT send command failed: " << e.what();
            return false;
        }
    }

    void MQTTClientCallback::process_distance_data(const json& data) {  //点位数据的Data并未传送
        DistanceData dd; 
        
        try {
            from_json(data, dd);
        } catch (const std::exception& e){}
    }

    void MQTTClientCallback::process_camera_data(const json & data){
        CameraConfig cc;
        try{
            from_json(data, cc);
            MQTTClient::getInstance().setCameraConfig(cc);
        } catch (const std::exception & e){}
    }

    MQTTClient::MQTTClient(const std::string & broker, const std::string & client_id): client(broker, client_id) {
        conn_opts.set_clean_session(true);
        conn_opts.set_keep_alive_interval(20);
        conn_opts.set_automatic_reconnect(true);
        conn_opts.set_connect_timeout(10);
        callback.set_client(&client);
        client.set_callback(callback);
        this->client_id = client_id;
    }

    MQTTClient::~MQTTClient(){
        client.disconnect();
    }

    bool MQTTClient::connect(const std::string & user_name, const std::string & password) {
        conn_opts.set_user_name(user_name);
        conn_opts.set_password(password);       
        try {
            auto conntok = client.connect(conn_opts);
            
            if (conntok->wait_for(std::chrono::seconds(10))) {
                LOG_INFO << "MQTT connection established successfully";
                
                if (client.is_connected()) {
                    LOG_INFO << "Client is fully connected and ready";
                    return true;
                } else {
                    LOG_ERROR << "Client reports not connected after successful connect";
                    return false;
                }
            } else {
                LOG_ERROR << "MQTT connection timeout after 30 seconds";
                return false;
            }
        } catch (const mqtt::exception& e) {
            LOG_ERROR << "MQTT connection failed: " << e.what();
            LOG_ERROR << "Error code: " << e.get_reason_code();
            return false;
        } catch (const std::exception& e) {
            LOG_ERROR << "Connection error: " << e.what();
            return false;
        }
        return true;
    }

    void MQTTClient::sendCameraInfo(const CameraConfig & cc){
        json json_cc;
        to_json(json_cc, cc);
        callback.send_control_command(json_cc);
    }

    MQTTClient & MQTTClient::getInstance(){
        static const std::string CLIENT_ID = "host_b_simple_" + std::to_string(std::time(nullptr));
        static MQTTClient instance(MQTT_BROKER, CLIENT_ID);
        static std::once_flag init_flag;
        
        std::call_once(init_flag, [&]() {
            instance.connect(MQTT_USERNAME, MQTT_PASSWORD);
        });
        return instance;
    }
}}