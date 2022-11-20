#include <HCSR04.h>

#include "moto.h"


//定义超声波传感器接口
#define TRIG 26  //TRIG接26号IO口
#define ECHO 27  //ECHO接27号IO口


// the setup function runs once when you press reset or power the board
void setup() {
  // put your setup code here, to run once:
  
  moto_pwm_init();  //电机pwm引脚初始化
  HCSR04.begin(TRIG, ECHO);  //超声波传感器接口初始化

  moto_pwm_set(1, 0);  //设置电机1的pwm为400
  moto_pwm_set(2, 0);  //设置电机2的pwm为400
  moto_pwm_set(3, 0);  //设置电机3的pwm为400
  moto_pwm_set(4, 0);  //设置电机4的pwm为400

}

// the loop function runs over and over again forever
void loop() {
  double* distances = HCSR04.measureDistanceCm();  //调用函数直接获取距离（单位厘米cm），返回的数据为double类型的数组，数组第一个元素为距离
  if(distances[0] < 20){
    moto_pwm_set(1, 0);  //设置电机1的pwm为400
    moto_pwm_set(2, 0);  //设置电机2的pwm为400
    moto_pwm_set(3, 0);  //设置电机3的pwm为400
    moto_pwm_set(4, 0);  //设置电机4的pwm为400
  }
  else{
    moto_pwm_set(1, 200);  //设置电机1的pwm为400
    moto_pwm_set(2, 200);  //设置电机2的pwm为400
    moto_pwm_set(3, 200);  //设置电机3的pwm为400
    moto_pwm_set(4, 200);  //设置电机4的pwm为400
  }

  delay(100);
  //do something
}
