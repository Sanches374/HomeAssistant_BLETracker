#ifndef BLETRACKER_H
#define BLETRACKER_H

#include <Arduino.h>
#include <BLEDevice.h> // 蓝牙Ble设备库
#include <BLEUtils.h> 
#include <BLEScan.h> // 蓝牙ble设备的扫描功能库
#include <BLEAdvertisedDevice.h> // 扫描到的蓝牙设备（广播状态）

//蓝牙用户结构定义
typedef struct BLE_User{
  String BLEName;
  String BLEAddress;
  int BLERSSI;
}BLE_User;

//定义需要追踪的蓝牙MAC地址(小写)，名称可以自定义，RSSI强度设置为在线状态的最低阈值
const BLE_User BLE_Users = {/*名称*/"MI Band 7 NFC",/*设备蓝牙MAC地址*/"c2:04:0b:02:03:42",/*设备在线时RSSI下限*/-80};

#define BLE_OffLine_N 3//BLE设备确认处于离线状态的扫描次数
#define BLE_Buff 50 //蓝牙列表大小，缓冲设备数量
#define scanTime 3  //蓝牙每次扫描时间

void Serialprint_BLEList(BLE_User BLE_List[],int BLE_Count);
void BLETracker_Init();
int* BLETracker_Loop(int*);
String BLETracker_State(int*);

#endif