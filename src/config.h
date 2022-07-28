#ifndef CONFIG_H
#define CONFIG_H
/**
 * 下列为个人配置文件
 * 
 */
//Wifi信息配置
const char* ssid      = "your-ssid";        //连接的WiFi的名称
const char* password  = "your-password";        //WiFi密码
//MQTT服务器配置
const char* mqttServer  = "192.168.0.1";  //要连接到的MQTT服务器IP
const int   mqttPort    = 1883;              //要连接到的服务器端口号
const char* mqttUser    = "admin";           //MQTT服务器账号
const char* mqttPassword = "public";         //MQTT服务器密码
const char* client_id = "bletracker";        //MQTT服务器标识当前设备的客户端编号
const char* blepresence_UID = "blepresence";
const char* blerssi_UID = "blerssi";

//mqtt主题已配置自动发现，可不进行修改
//presence配置主题
const char* Presence_Config = "homeassistant/binary_sensor/bletracker/config";
const char* RSSI_Config = "homeassistant/sensor/blerssi/config";
//设备在线主题
const char* Availability_topic = "homeassistant/bletracker/availability";
//蓝牙状态追踪状态主题
const char* Presence_State =   "ha/bletracker/state";
const char* RSSI_State = "ha/blerssi/state";
#endif