/**
 * 主机B - 简单数据收发器 (C++版本)
 * 功能：
 * 1. 每隔30秒发送固定的控制命令到主机A
 * 2. 接收主机A的位移数据并实时打印
 * 
 * 编译: g++ -std=c++17 host_b_simple.cpp -o host_b_simple -I/usr/local/include -L/usr/local/lib -lpaho-mqttpp3 -lpaho-mqtt3a -lpthread -lssl -lcrypto -lnlohmann_json
 * 运行: ./host_b_simple
 */

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <atomic>
#include <mqtt/async_client.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// -------------------------- 配置项 --------------------------
const std::string MQTT_BROKER = "tcp://120.27.205.147:1883";
const std::string CLIENT_ID = "host_b_simple_" + std::to_string(std::time(nullptr));

// 订阅主题（从主机A接收）
const std::string DISTANCE_TOPIC = "/distance";           // 接收位移数据

// 发布主题（向主机A发送）
const std::string CAMERA_CONTROL_TOPIC = "/camera_params"; // 发送控制指令

const int QOS = 1;

// MQTT认证
const std::string MQTT_USERNAME = "MQTT3";
const std::string MQTT_PASSWORD = "123456";

// 控制周期（秒）
const int CONTROL_INTERVAL = 30;



// -------------------------- 固定控制命令 --------------------------
json get_fixed_control_command() {
    json command;
    
    // 基础信息
    command["command_id"] = "fixed_cmd_" + std::to_string(std::time(nullptr));
    command["device_id"] = "camera_001";
    command["timestamp"] = std::time(nullptr);
    command["timeout"] = 10;
    
    // 相机参数
    json params;
    params["exposure"] = 500;      // 固定曝光值
    params["gain"] = 1.2;          // 固定增益值
    params["frame_rate"] = 30;     // 固定帧率
    
    command["params"] = params;
    
    return command;
}

// -------------------------- MQTT回调类 --------------------------
class SimpleCallback : public mqtt::callback {
public:
    // 连接成功回调
    void connected(const std::string& cause) override {
        std::cout << "[主机B] 连接MQTT服务器成功！" << std::endl;
        
        try {
            // 只订阅位移数据主题
            client_->subscribe(DISTANCE_TOPIC, QOS);
            std::cout << "[主机B] 已订阅位移主题: " << DISTANCE_TOPIC << std::endl;
            
        } catch (const mqtt::exception& e) {
            std::cerr << "[主机B] 订阅失败: " << e.what() << std::endl;
        }
    }

    void connection_lost(const std::string& cause) override {
        std::cout << "[主机B] 连接断开: " 
                  << (cause.empty() ? "未知原因" : cause) << std::endl;
    }

    // 接收消息
    void message_arrived(mqtt::const_message_ptr msg) override {
        try {
            std::string topic = msg->get_topic();
            
            // 只处理位移数据主题
            if (topic == DISTANCE_TOPIC) {
                std::string payload = msg->get_payload_str();
                json data = json::parse(payload);
                process_distance_data(data);
            }
            
        } catch (const json::exception& e) {
            std::cerr << "[主机B] JSON解析失败: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[主机B] 处理消息失败: " << e.what() << std::endl;
        }
    }

    // 设置客户端
    void set_client(mqtt::async_client* client) {
        client_ = client;
    }
    
    // 发送控制命令到主机A
    bool send_control_command(const json& command) {
        try {
            std::string payload = command.dump();
            mqtt::message_ptr msg = mqtt::make_message(CAMERA_CONTROL_TOPIC, payload);
            msg->set_qos(QOS);
            
            auto pub_token = client_->publish(msg);
            pub_token->wait();
            
            return true;
            
        } catch (const mqtt::exception& e) {
            std::cerr << "[主机B] 发送控制命令失败: " << e.what() << std::endl;
            return false;
        }
    }

private:
    mqtt::async_client* client_ = nullptr;
    int receive_count_ = 0;
    
    // 处理位移数据
    void process_distance_data(const json& data) {
        receive_count_++;
        
        std::cout << "\n[主机B] 收到位移数据 #" << receive_count_ << std::endl;
        
        // 检查数据格式并提取信息
        bool has_displacement = false;
        double distance_value = 0.0;
        std::string distance_unit = "unknown";
        long long timestamp = 0;
        
        try {
            // 尝试解析位移数据
            if (data.contains("params") && data["params"].contains("displacement")) {
                auto& displacement = data["params"]["displacement"];
                distance_value = displacement.value("value", 0.0);
                distance_unit = displacement.value("unit", "unknown");
                has_displacement = true;
            }
            
            // 获取时间戳
            timestamp = data.value("timestamp", 0LL);
            if (timestamp == 0) {
                timestamp = std::time(nullptr);
            }
            
            // 显示位移信息
            if (has_displacement) {
                std::cout << "  位移值: " << distance_value << " " << distance_unit << std::endl;
            } else {
                std::cout << "  警告: 未找到位移数据字段" << std::endl;
                std::cout << "  原始数据: " << data.dump() << std::endl;
            }
            
            // 显示时间戳
            std::cout << "  时间戳: " << timestamp;
            std::cout << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "[主机B] 解析位移数据失败: " << e.what() << std::endl;
        }
    }
};

// -------------------------- 主函数 --------------------------
int main(int argc, char* argv[]) {
    try {
        // 创建MQTT客户端
        mqtt::async_client client(MQTT_BROKER, CLIENT_ID);
        SimpleCallback callback;
        callback.set_client(&client);
        client.set_callback(callback);
        
        // 连接选项
        mqtt::connect_options conn_opts;
        conn_opts.set_clean_session(true);
        conn_opts.set_keep_alive_interval(60);
        conn_opts.set_automatic_reconnect(true);
        conn_opts.set_connect_timeout(10);
        
        // 设置认证
        conn_opts.set_user_name(MQTT_USERNAME);
        conn_opts.set_password(MQTT_PASSWORD);
        
        std::cout << "[主机B] 正在连接MQTT服务器..." << std::endl;
        std::cout << "  服务器: " << MQTT_BROKER << std::endl;
        std::cout << "  客户端ID: " << CLIENT_ID << std::endl;
        
        // 连接
        auto conntok = client.connect(conn_opts);
        if (conntok->wait_for(std::chrono::seconds(10))) {
            std::cout << "[主机B] 连接成功！" << std::endl;
        } else {
            std::cerr << "[主机B] 连接超时" << std::endl;
            return 1;
        }
        
        // 等待连接稳定
        std::this_thread::sleep_for(std::chrono::seconds(2));
        int command_count = 0;
        auto last_control_time = std::chrono::steady_clock::now();
        
        while (true) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_control_time).count();
            
            // 检查是否需要发送控制命令
            if (elapsed >= CONTROL_INTERVAL) {
                command_count++;
                last_control_time = now;
                
                // 生成并发送固定控制命令
                json command = get_fixed_control_command();
                
                std::cout << "\n[主机B] 发送固定控制命令 #" << command_count << std::endl;
                std::cout << "  命令ID: " << command["command_id"] << std::endl;
                std::cout << "  目标设备: " << command["device_id"] << std::endl;
                std::cout << "  参数:" << std::endl;
                std::cout << "    exposure: " << command["params"]["exposure"] << std::endl;
                std::cout << "    gain: " << command["params"]["gain"] << std::endl;
                std::cout << "    frame_rate: " << command["params"]["frame_rate"] << std::endl;
                
                // 发送命令
                if (callback.send_control_command(command)) {
                    std::cout << "  [成功] 命令已发送" << std::endl;
                }
                
                std::cout << "----------------------------------------" << std::endl;
            }
            
            // 短暂休眠，避免CPU占用过高
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // 断开连接
        std::cout << "\n[主机B] 正在断开MQTT连接..." << std::endl;
        auto discon_token = client.disconnect();
        discon_token->wait();
        std::cout << "[主机B] 已断开连接，程序结束" << std::endl;
        
    } catch (const mqtt::exception& e) {
        std::cerr << "[主机B] MQTT异常 [" << e.get_reason_code() 
                  << "]: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "[主机B] 系统异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}