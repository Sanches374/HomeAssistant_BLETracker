# HomeAssistant_BLETracker<br>
基于HomeAssistant MQTT Discovery 的 蓝牙RSSI判定在家状态项目

## 开发基础
> ### 硬件配置
> * 合宙ESP32C3开发板经典版（非简约版，两个按键间有元器件）
> ![image](img/1.png)
> ### 软件配置
> * MQTT服务器
> * HomeAssistant系统（配置好MQTT）

## 开发环境
> * VScode+platformio
> * 开发板配置参考B站视频：[使用PlatformIo 把玩 合宙ESP32-C2](https://www.bilibili.com/read/cv16215201)

## 文件配置
> * 修改`config.h`
##
* 将软件程烧录到ESP32C3中
测试使用小米手环7打开蓝牙广播
