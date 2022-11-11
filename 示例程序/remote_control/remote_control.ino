/*
 * 蓝牙遥控小车示例程序
 */


#include "moto.h"
#include "myBlueTooth.h"


static char* recBuff; 
static String strBuff;
static String substr;  //字符串解析缓冲变量



// the setup function runs once when you press reset or power the board
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  //串口初始化
  
  BT_init();//蓝牙设备初始化--蓝牙设备名称"trackbot"
  Serial.println("The device started, now you can pair it with bluetooth!");

  moto_pwm_init();
  moto_pwm_set(1, 0);
  moto_pwm_set(2, 0);
  moto_pwm_set(3, 0);
  moto_pwm_set(4, 0);
}

// the loop function runs over and over again forever
void loop() {
  static int m_turn = 0;
  static int m_speed=0;
  
  recBuff = getDataBT();  //蓝牙接收字符串

  /**摇杆控制模式，接收蓝牙数据格式:"{X10Y10}"
    *手机上位机APP见工程链接附件
    *原程序为横屏，此处使用竖屏操作摇杆，因此需要X、Y坐标转换，均取负号
   **/
   
  strBuff=recBuff;  

  substr = strBuff.substring(strBuff.indexOf("X")+1,strBuff.indexOf("Y"));  //读取速度值:取’X‘和’Y‘中间的值
  m_speed = (substr.toInt());  //调节运动速度，取相反值翻转方向，将提取出的数据转为整形
  
  substr = strBuff.substring(strBuff.indexOf("Y")+1,strBuff.indexOf("}"));  //读取转向值:取'Y'和'}'中间的值
  m_turn =  substr.toInt();  //调节转向角，同样取相反值

  Serial.print("speed: ");  //调试用
  Serial.println(m_speed);  //调试用
  Serial.print("turn: ");  //调试用
  Serial.println(m_turn);  //调试用


  moto_pwm_set(1, (-m_speed-m_turn));
  moto_pwm_set(3, (-m_speed-m_turn));
  moto_pwm_set(2, (-m_speed+m_turn));
  moto_pwm_set(4, (-m_speed+m_turn));

  delay(10);  //延时10ms
}
