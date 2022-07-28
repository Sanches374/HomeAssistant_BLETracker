# HomeAssistant_BLETracker<br>
基于HomeAssistant MQTT Discovery 的 蓝牙RSSI判定在家状态项目

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
> * 修改`config.h`文件中的`ssid`、`password`为需要连接的WIFI名和密码
> `ssid`
##
* 将软件程烧录到ESP32C3中
测试使用小米手环7打开蓝牙广播
