#include "moto.h"



// the setup function runs once when you press reset or power the board
void setup() {
  // put your setup code here, to run once:
  
  moto_pwm_init();  //电机pwm引脚初始化

  moto_pwm_set(1, 400);  //设置电机1的pwm为400
  moto_pwm_set(2, 400);  //设置电机2的pwm为400
  moto_pwm_set(3, 400);  //设置电机3的pwm为400
  moto_pwm_set(4, 400);  //设置电机4的pwm为400

}

// the loop function runs over and over again forever
void loop() {
  
  //do something
}
