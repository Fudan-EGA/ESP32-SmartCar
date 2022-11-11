/**myBlueTooth.cpp + myBlueTooth.h
 * 功能：对ESP32经典蓝牙的简单封装，工作在从机模式，基于单字节蓝牙发送定义了浮点数、字符串等发送函数
 * 说明：接收蓝牙数据时通过中断接收，直接调用相应接口就能读取
 * 
 */

#include "myBlueTooth.h"

BluetoothSerial SerialBT;

static char recBuffBT[100];

/*************蓝牙参数配置**********************/
void Bluetooth_Event(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);  //蓝牙事件回调函数

/*蓝牙参数配置*/
void BT_init()
{
  SerialBT.register_callback(Bluetooth_Event); //设置事件回调函数 连接 断开 发送 接收
  SerialBT.begin("trackbot"); //Bluetooth device name    
}

/*蓝牙发送浮点数据，转为字符串发送*/
void sendFloatBT(float data)
{
  int i;
  char dstr[20];
  sprintf(dstr, "%.2f", data);  //浮点数转字符串
  for(i=0;i<strlen(dstr);i++)
  {  
    SerialBT.write(dstr[i]);
  }
}

/*蓝牙发送字符串*/
void sendStringBT(char *sdata) 
{
  int i;
  for(i=0;i<strlen(sdata);i++)
  {  
    SerialBT.write(sdata[i]);
  }
}

/*获取蓝牙发送内容*/
char* getDataBT()
{
  return recBuffBT;
}

/*******************    蓝牙事件回调函数   ******************/
void Bluetooth_Event(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)  //蓝牙事件回调函数
{
    uint16_t idx = 0;
    if(event == ESP_SPP_OPEN_EVT || event == ESP_SPP_SRV_OPEN_EVT) //蓝牙连接成功标志 
    {                                                              //蓝牙主机和从机模式对应的标志不同，前面的是主机模式的，后面是从机模式
        sendStringBT("connection successful!\r\n");
    }
    else if(event == ESP_SPP_CLOSE_EVT)     //蓝牙断开连接标志
    {
        sendStringBT("disconnect successful!\r\n");
    }
    else if(event == ESP_SPP_DATA_IND_EVT)  //数据接收标志
    {
        memset(recBuffBT, 0 , sizeof(recBuffBT));
        while(SerialBT.available())  //等待接收完成
        {
            recBuffBT[idx++]=SerialBT.read(); //按字节存储
        }
    }
    else if(event == ESP_SPP_WRITE_EVT)     //数据发送标志
    {
        Serial.write("  send complete! \r\n");
    }
}
