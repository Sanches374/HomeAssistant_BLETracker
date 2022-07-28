#include <Arduino.h>
#include <string.h>
#include <WiFi.h>
#include "PubSubClient.h"
#include "BLETracker.h"
#include "config.h"

//蓝牙追踪初始状态设置
int* bletracker_p = new int[2];
WiFiClient espClient;                     // 定义wifiClient实例
PubSubClient client(espClient);          // 定义PubSubClient的实例






void mqtt_presence_config()
{
  //发布设备配置主题
  String Config = "{\"name\": \"BLEPresence\", \"unique_id\": \"";
  Config += blepresence_UID;
  Config += "\",\"stat_t\": \"";
  Config += Presence_State;
  Config += "\", \"avty_t\": \"";
  Config += Availability_topic;
  Config += "\", \"device_class\": \"presence\",\"dev\": {\"connections\": [[\"mac\", \"";
  Config += WiFi.macAddress();
  Config += "\"],[\"ip\",\"";
  Config += WiFi.localIP().toString();
  Config += "\"]],\"name\": \"bletracker\",\"model\": \"ESP32C3\",\"manufacturer\": \"Sanches\"}}";

  Serial.println(Config.c_str());
    
  int msgLen = Config.length();
  client.beginPublish(Presence_Config,msgLen,false);
  client.print(Config.c_str());
  client.endPublish();
  client.publish(Presence_Config,Config.c_str());
}

void mqtt_rssi_config()
{
  //发布设备配置主题
  String Config = "{\"name\": \"BLERSSI\", \"unique_id\": \"";
  Config += blerssi_UID;
  Config += "\",\"stat_t\": \"";
  Config += RSSI_State;
  Config += "\", \"avty_t\": \"";
  Config += Availability_topic;
  Config += "\",\"unit_of_measurement\": \"dBm\",\"value_template\": \"{{ value_json.rssi }}\",";
  Config += "\"dev\": {\"connections\": [[\"mac\", \"";
  Config += WiFi.macAddress();
  Config += "\"],[\"ip\",\"";
  Config += WiFi.localIP().toString();
  Config += "\"]],\"name\": \"bletracker\",\"model\": \"ESP32C3\",\"manufacturer\": \"Sanches\"}}";


  Serial.println(Config.c_str());
    
  int msgLen = Config.length();
  client.beginPublish(RSSI_Config,msgLen,false);
  client.print(Config.c_str());
  client.endPublish();
  client.publish(RSSI_Config,Config.c_str());
}

// 扫描广播设备结果回调，每一次扫描到执行
//设置MQTT回调
void callback(char* topic, byte* payload, unsigned int length) 
{
    Serial.print("来自订阅的主题：");      //串口打印：来自订阅的主题:
    Serial.println(topic);                //串口打印订阅的主题
    Serial.print("信息：");               //串口打印：信息：
    for (int i = 0; i< length; i++)       //使用循环打印接收到的信息
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}
/**
 * Wifi和MQT连接初始化，需要先完成串口初始化
 */
void connect_init()
{
  //WiFi初始化
  WiFi.begin(ssid,password);   //Wifi初始化，接入WiFi函数（WiFi名称，密码）
  Serial.println();
  Serial.print("正在连接Wifi至: ");
  Serial.println(ssid);
  Serial.print("连接中");
  while (WiFi.status() != WL_CONNECTED)   //若WiFi接入成功WiFi.status()会返回 WL_CONNECTED
  { 
    Serial.print(".");
    delay(500); 
  }
  Serial.println("WiFi连接成功");           //连接wifi成功输出
  Serial.print("IP:");
  Serial.println(WiFi.localIP());          //输出IP地址
  Serial.print("MAC:");
  Serial.println(WiFi.macAddress());       //输出Mac地址

  //MQTT_Server初始化
  client.setServer(mqttServer,mqttPort);  //MQTT服务器连接函数（服务器IP，端口号）
  client.setCallback(callback);           //设定回调方式，当ESP32收到订阅消息时会调用此方法
  while (!client.connected())             //是否连接上MQTT服务器
  {
    Serial.println("MQTT服务器连接中...");         //串口打印：连接服务器中
    if (client.connect(client_id,mqttUser, mqttPassword,Availability_topic,0,0,"offline"))  //连接服务器，并设置遗嘱消息
    {
      Serial.println("MQTT服务器连接成功!");          //串口打印：服务器连接成功
    }
    else 
    {
      Serial.println("MQTT服务器连接失败, 正在尝试重新连接...");            //串口打印：连接服务器失败
      Serial.println(client.state());       //重新连接函数                  
    }
  }

  //发布设备在线主题消息
  client.publish(Availability_topic,"online");
}
/**
 * Wifi和MQTT断线检测，断线自动重连
 */
void reconnect()
{
  //Wifi断线重连
  
  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid,password);  
    Serial.println("WiFi已离线, 正在尝试重新连接");
    for (int i = 0;/*重连时长*/ i < 10; i++)
    {
      Serial.print(".");
      delay(1000); 
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("WiFi重新连接成功");
      Serial.print("IP:");
      Serial.println(WiFi.localIP());
      Serial.print("MAC:");
      Serial.println(WiFi.macAddress());
    }
  }
  
  //MQTT断线重连
  if (!client.connected())
  {
    Serial.println("MQTT服务器已离线, 正在重新连接...");
    while (!client.connected())
    {
      
      if (client.connect(client_id,mqttUser, mqttPassword,Availability_topic,0,0,"offline"))  //如果服务器连接成功
      {
        Serial.println("MQTT服务器连接成功!");
      }
      else 
      {
        Serial.println("MQTT服务器连接失败, 正在尝试重新连接...");
        Serial.println(client.state());       //重新连接函数                  
      }
    }
  }
  //发布设备在线消息
  client.publish(Availability_topic,"online");
}


void setup() {
  //串口初始化

  Serial.begin(115200);
  connect_init();
  BLETracker_Init();
  //状态初始化
  bletracker_p[0] = -2;
  bletracker_p[1] = 0;
  //client.subscribe("ESP32");                 //连接MQTT服务器后订阅主题
  //Serial.print("已订阅主题，等待主题消息");                  //串口打印：已订阅主题，等待主题消息
  //client.publish("/World","Hello from ESP32");//向服务器发送的信息(主题，内容)
  mqtt_presence_config();
  mqtt_rssi_config();
}

void loop() 
{
  //Wifi和MQTT断线重连
  reconnect();
  bletracker_p = BLETracker_Loop(bletracker_p);
  if (bletracker_p[0] >= 0)
  {
    //在家
    client.publish(Presence_State,"ON");
  }
  else
  {
    //离家
    client.publish(Presence_State,"OFF");
  }
  String rssi_buff = "{\"rssi\":\"";
  rssi_buff += bletracker_p[1];
  rssi_buff += "\"}";
  client.publish(RSSI_State,rssi_buff.c_str());
  
  Serial.println(rssi_buff);
  Serial.println(bletracker_p[0]);
  Serial.println(bletracker_p[1]);
  //MQTT处理消息以及保持心跳
  client.loop();
  //MQTT发布信息
  //client.publish(Presence_State,state.c_str());
}
