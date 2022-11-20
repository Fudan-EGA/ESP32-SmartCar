/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/


#include "moto.h"
#include "myBlueTooth.h"
#include <HCSR04.h>

#define OUT1 4
#define OUT2 5
#define OUT3 33
#define OUT4 34
#define OUT5 35


#define TRACK     1  //循迹模式
#define REMOTE    0  //遥控模式
#define AUTOCROSS 2  //避障模式

#define MODE TRACK

#define TRIG 26
#define ECHO 27

#define Kp 50
#define SET_DIST 20

static char* recBuff; 
static String strBuff;
static String substr;  //字符串解析

void track_init(void)
{
  pinMode(OUT1, INPUT);
  pinMode(OUT2, INPUT);
  pinMode(OUT3, INPUT);
  pinMode(OUT4, INPUT);
  pinMode(OUT5, INPUT);
}

uint8_t track_get(void)
{
  uint8_t ret = 0;
  ret |= (digitalRead(OUT5))<<0;
  ret |= (digitalRead(OUT4))<<1;
  ret |= (digitalRead(OUT3))<<2;
  ret |= (digitalRead(OUT2))<<3;
  ret |= (digitalRead(OUT1))<<4;
  return ret;
}


// the setup function runs once when you press reset or power the board
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  BT_init();//蓝牙设备初始化--"trackbot"
  Serial.println("The device started, now you can pair it with bluetooth!");

  moto_pwm_init();
  HCSR04.begin(TRIG, ECHO);
  track_init();
}

// the loop function runs over and over again forever
void loop() {
  static uint8_t run_mode = 0;
  static uint8_t track_num  = 0;
  static int spd_turn = 0;
  static int temp_spd=0;
  static double *distances;
  static int spd=0;
  
  recBuff = getDataBT();

  if(strlen(recBuff) == 1){
    switch(recBuff[0]){
      case 'S':
        run_mode = 0;
        break;
      case 'A':
        run_mode = 1;
        break;
      case 'B':
        run_mode =2;
      break;
      default: break;
    }
  }

  Serial.print("Mode: ");
  Serial.println(run_mode);

  switch(run_mode){
    case 0:  //停止
      moto_pwm_set(3, 0);
      moto_pwm_set(4, 0);
      moto_pwm_set(1, 0);
      moto_pwm_set(2, 0);
      break;
      
    case 1:  //循迹或避障模式，根据宏定义选择

#if(MODE==TRACK) //循迹模式
    
    track_num = track_get();
    Serial.println(track_num);
      switch(track_num){
        case 0:
          moto_pwm_set(3, 0);
          moto_pwm_set(4, 0);
          moto_pwm_set(1, 0);
          moto_pwm_set(2, 0);
          break;
        case 0b11000:
        case 0b11100:
        case 0b11010:
        case 0b11110:
          moto_pwm_set(1, 600);
          moto_pwm_set(3, 600);
          moto_pwm_set(2, -1000);
          moto_pwm_set(4, -1000);
          break;
    
        case 0b11001:
        case 0b11101:
          moto_pwm_set(1, 400);
          moto_pwm_set(3, 400);
          moto_pwm_set(2, -400);
          moto_pwm_set(4, -400);
          break;
        
        case 0b00011:
        case 0b00111:
        case 0b01011:
        case 0b01111:
          moto_pwm_set(2, 600);
          moto_pwm_set(4, 600);
          moto_pwm_set(1, -1000);
          moto_pwm_set(3, -1000);
          break;
        case 0b10011:
        case 0b10111:
          moto_pwm_set(2, 400);
          moto_pwm_set(4, 400);
          moto_pwm_set(1, -400);
          moto_pwm_set(3, -400);
          break;
        default:
          moto_pwm_set(2, 400);
          moto_pwm_set(4, 400);
          moto_pwm_set(1, 400);
          moto_pwm_set(3, 400);
          break;
      }
#endif
          
      

#if(MODE==AUTOCROSS) //避障模式
        distances = HCSR04.measureDistanceCm();
        Serial.println(distances[0]);

        spd = Kp*(distances[0]-SET_DIST);

        constrain(spd, -800,800);
        
        moto_pwm_set(1, spd);
        moto_pwm_set(2, spd);
        moto_pwm_set(3, spd);
        moto_pwm_set(4, spd);
        
#endif

      break;

    case 2:  //摇杆模式
      /**摇杆控制模式，接收蓝牙数据格式:"{X10Y10}"
        *手机上位机APP见工程链接附件
        *原程序为横屏，此处使用竖屏操作摇杆，因此需要X、Y坐标转换，均取负号
       **/
      strBuff=recBuff;  
    
      substr = strBuff.substring(strBuff.indexOf("X")+1,strBuff.indexOf("Y"));  //读取速度值:X和Y中间的值
      
      temp_spd = (substr.toInt());  //调节运动速度，取相反值翻转方向
      
      //Serial.print(substr); //调试输出
      //Serial.print(",");
      
      substr = strBuff.substring(strBuff.indexOf("Y")+1,strBuff.indexOf("}"));  //读取转向值
      spd_turn =  substr.toInt();  //调节转向角，同样取相反值
    
      Serial.print("speed: ");  //调试用
      Serial.println(temp_spd);  //调试用
      Serial.print("turn: ");  //调试用
      Serial.println(spd_turn);  //调试用
    
      Serial.print("RRRRR: ");  
      Serial.println(-temp_spd+spd_turn);
      Serial.print("RRRRR: ");    
      Serial.println(-temp_spd-spd_turn);

      distances = HCSR04.measureDistanceCm();
      if(distances[0]<15)
        temp_spd=0;
    
      moto_pwm_set(1, (-temp_spd-spd_turn));
      moto_pwm_set(3, (-temp_spd-spd_turn));
      moto_pwm_set(2, (-temp_spd+spd_turn));
      moto_pwm_set(4, (-temp_spd+spd_turn));
      break;
    default: break;
  }
  delay(10);
}
