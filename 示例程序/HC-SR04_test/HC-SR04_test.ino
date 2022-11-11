#include <HCSR04.h>  //导入模块驱动库，需要自行下载：项目-加载库-管理库中搜索“HC-SR04”下载

//定义超声波传感器接口
#define TRIG 26  //TRIG接26号IO口
#define ECHO 27  //ECHO接27号IO口

void setup () {
  Serial.begin(115200);  //串口初始化，即USB线连接的端口
  HCSR04.begin(TRIG, ECHO);  //超声波传感器接口初始化
}

void loop () {
  double* distances = HCSR04.measureDistanceCm();  //调用函数直接获取距离（单位厘米cm），返回的数据为double类型的数组，数组第一个元素为距离
  
  Serial.print("1: ");
  Serial.print(distances[0]);  //通过串口显示测得的距离
  Serial.println(" cm");
  
  Serial.println("---");
  delay(250);  //延时250毫秒
}
