/**myBlueTooth.cpp + myBlueTooth.h
 * 功能：对ESP32经典蓝牙的简单封装，工作在从机模式，基于单字节蓝牙发送定义了浮点数、字符串等发送函数
 * 说明：接收蓝牙数据时通过中断接收，直接调用相应接口就能读取
 * 
 */


#ifndef MYBLUETOOTH_H_
#define MYBLUETOOTH_H_

#include <Arduino.h>
#include <String.h>
#include <BluetoothSerial.h>


/*蓝牙参数配置*/
void BT_init();

/*蓝牙发送浮点数据，转为字符串发送*/
void sendFloatBT(float data);

/*蓝牙发送字符串*/
void sendStringBT(char *sdata);

/*获取蓝牙发送内容*/
char* getDataBT();

#endif /* MYBLUETOOTH_H_ */
