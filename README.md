# HomeAssistant_BLETracker
基于HomeAssistant MQTT Discovery 的 蓝牙RSSI判定在家状态项目。判断是否在家状态后，通过HA自动化进行智能联动。
__本项目目前为Demo版，暂无后续更新计划...__
第一次在Github开源项目，有问题可以在Issues进行反馈，代码写的不好请轻喷...

## 开发基础
> ### 硬件配置
> * 合宙ESP32C3开发板经典版（非简约版，两个按键间有元器件）
> ![image](img/1.png)  
> 简约版（无按键版也可通过Arduino IDE进行开发）
> 参考B站专栏：[ESP32-C3，不用USB转串口芯片如何开发](https://www.bilibili.com/read/cv13107494)
> ### 软件配置
> * MQTT服务器
> * HomeAssistant系统（已配置好MQTT）

## 开发环境
> * VScode+platformio
> * 开发板配置参考B站专栏：[使用PlatformIo 把玩 合宙ESP32-C2](https://www.bilibili.com/read/cv16215201)

## 文件配置
对两个`.h`文件进行相应修改
### `config.h`
> * 修改`ssid`、`password`为需要连接的WIFI名和密码
> * 修改`mqttServer`、`mqttPort`、`mqttUser`、`mqttPassword `为需要连接的MQTT服务器IP、端口、登录名和密码  
> * 
##
* 将软件程烧录到ESP32C3中
测试使用小米手环7打开蓝牙广播
