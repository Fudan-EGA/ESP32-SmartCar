/*
 * 避障演示程序，检测到距离过近时则停止前进
 * 
 */

#include "moto.h"
#include <HCSR04.h>

#define TRIG 26
#define ECHO 27


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  HCSR04.begin(TRIG, ECHO);
  
  moto_pwm_init();

}


void loop() {
    double* distances = HCSR04.measureDistanceCm();

    if(distances[0] <10){ //如果距离小于10则后退
        moto_pwm_set(1, -60*(10-distances[0]));
        moto_pwm_set(2, -60*(10-distances[0]));
        moto_pwm_set(3, -60*(10-distances[0]));
        moto_pwm_set(4, -60*(10-distances[0]));
    }

    else if((distances[0] > 10)&&(distances[0] <15)){ //如果距离在(10,15)则停止
        moto_pwm_set(1, 0);
        moto_pwm_set(2, 0);
        moto_pwm_set(3, 0);
        moto_pwm_set(4, 0);
    }

    else if((distances[0] >15)&&(distances[0] <30)) //如果距离在(15,30)则跟随
    {  //否则跟随
        moto_pwm_set(1, 70*(distances[0]-15));
        moto_pwm_set(2, 70*(distances[0]-15));
        moto_pwm_set(3, 70*(distances[0]-15));
        moto_pwm_set(4, 70*(distances[0]-15));
    }
    else{  //默认向前运动
        moto_pwm_set(1, 200);
        moto_pwm_set(2, 200);
        moto_pwm_set(3, 200);
        moto_pwm_set(4, 200);
      
    }
    delay(50);
}
