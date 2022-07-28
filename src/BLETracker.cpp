#include "BLETracker.h"
/**
 * @brief 主函数中设定语句：生成数组指针
int* bletracker_p = new int[2];
在setup()中对初始状态进行定义
bletracker_p[0] = -2;
bletracker_p[1] = 0; 
 * 
 */

BLE_User BLE_List[BLE_Buff];  //获取到的蓝牙列表存储(全局)
int BLE_i = 0;                //蓝牙列表遍历int
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice) 
  {
    BLE_List[BLE_i].BLEName = advertisedDevice.getName().c_str();
    BLE_List[BLE_i].BLEAddress = advertisedDevice.getAddress().toString().c_str();
    BLE_List[BLE_i].BLERSSI = advertisedDevice.getRSSI();
    BLE_i++;
  }
};

//蓝牙列表串口打印
void Serialprint_BLEList(BLE_User BLE_List[],int BLE_Count)
{
  Serial.println("BLEList");
  for(int i = 0; i < BLE_Count;i++)
  {
    Serial.print("No:");
    Serial.print(i+1);
    Serial.print("  Name:");
    Serial.print(BLE_List[i].BLEName);
    Serial.print("  Address:");
    Serial.print(BLE_List[i].BLEAddress);
    Serial.print("  RSSI:");
    Serial.println(BLE_List[i].BLERSSI);
  }
}

/**
 * 蓝牙追踪器初始化
 */
BLEScan* pBLEScan;
void BLETracker_Init()
{
  //蓝牙扫描初始化
  BLEDevice::init(""); // 设备初始化
  pBLEScan = BLEDevice::getScan(); // 创建一个新扫描入口
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());// 注册扫描结果回调
  pBLEScan->setActiveScan(true); // 配置主动扫描
  pBLEScan->setInterval(100); // 配置扫描PDU间隔
  pBLEScan->setWindow(99);   // 设置扫描窗口大小，需要小于扫描间隔
}

/**
 * @brief 
 * 蓝牙追踪器循环扫描
 * @param state_int 
 *        输入参数代表当前设备状态：
 *        -2 ：离家，且未发现设备；
 *        -1 ：离家，已扫描到蓝牙设备，但信号强度不足设定；
 *         0 ：在家，且已扫描到蓝牙设备；
 *        >0 : 在家，暂未扫描的设备，未扫描到的次数/强度不足次数为 state_int 
 * @return int* 数组指针，[0]代表设备状态，[1]代表信号强度
 */
int* BLETracker_Loop(int* ble_state)
{
  //int* ble_state = new int[2]; //返回值
  bool BLEfind_flag;//能否搜索到BLE设备标志
  //进行蓝牙扫描
  BLE_i = 0;    //蓝牙设备列表重制
  BLEScanResults foundBLEDevices = pBLEScan->start(scanTime, false);// 开始扫描 等待扫描结果；
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory，释放扫描缓存消耗
  Serialprint_BLEList(BLE_List,foundBLEDevices.getCount()); //串口输出BLE列表
  
  //BLE追踪,是否找到相应设备 
  for (int i = 0; i < foundBLEDevices.getCount(); i++)
  {
    if (BLE_List[i].BLEAddress == BLE_Users.BLEAddress)
    {
      BLEfind_flag = 1;
      ble_state[1] = BLE_List[i].BLERSSI;
      break;
    }
    else
    {
      BLEfind_flag = 0;
    }
  }
  //状态信息更新
  if (BLEfind_flag)
  {
    //扫描到设备
    if (ble_state[1] > BLE_Users.BLERSSI)
    {
      //强度足够
      ble_state[0] = 0;
    }
    else if ((ble_state[0] < 0) && (ble_state[1] > BLE_Users.BLERSSI))
    {
      //强度不够且前一状态为离家
      ble_state[0] = -1;
    }
    else
    {
      //强度不够且前一状态为在家
      if (ble_state[0] < BLE_OffLine_N)
      {
        //不足离家判定
        ble_state[0]++;
      }
      else
      {
        ble_state[0] = -1;
      }
    }
  }
  //未扫描到设备
  else
  {
    ble_state[1] = 0;
    if (ble_state[0] < 0)
    {
      //前一状态为离家
      ble_state[0] = -2;
    }
    else
    {
      //前一状态为在家
      if (ble_state[0] < BLE_OffLine_N)
      {
        //不足离家判定
        ble_state[0]++;
      }
      else
      {
        ble_state[0] = -2;
      }
    }
  }
  return ble_state;
}
/**
 * @brief 
 * 对应状态MQTT消息生成
 * @param state_int BLETracker_Loop返回数组
 * @return String 返回的MQTT发送数组
 */
String BLETracker_State(int* state_int)
{
  String ble_state;
  if (state_int[0] >= 0)
  {
    ble_state = "{\"state\":\"ON\",\"rssi\":\"";
    ble_state += *(state_int + 1);
    ble_state += "\"}";
  }
  else
  {
    ble_state = "{\"state\":\"OFF\",\"rssi\":\"";
    ble_state += *(state_int + 1);
    ble_state += "\"}";
  }
  return ble_state;
}