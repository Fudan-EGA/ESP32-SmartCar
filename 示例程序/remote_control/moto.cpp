/**moto.cpp + moto.h
 * 功能：直流电机控制函数，包括基于ledc库的pwm调速功能和编码器计数值获取功能
 * 说明：接口部分通过宏定义配置，可结合实际使用管脚进行配置
 */


#include "moto.h"


/*使用ESP32LEDC库函数配置为pwm输出模式，16个通道：0-7为高速模式，8-15为低速模式 */
void moto_pwm_init()
{
  ledcSetup(8, 1000, 10);  //设置LEDC通道8频率为1000，分辨率为10位，即占空比可选0~1023
  ledcAttachPin(PWM1P, 8); //绑定通道8输出IO口为PWMR1P
  ledcSetup(9, 1000, 10);  //通道9，右边电机反向通道
  ledcAttachPin(PWM1N, 9); 

  ledcSetup(10, 1000, 10);  //通道10,左边电机正向通道
  ledcAttachPin(PWM2P, 10); 
  ledcSetup(11, 1000, 10);  //通道11,左边电机反向通道
  ledcAttachPin(PWM2N, 11); 

  ledcSetup(12, 1000, 10);  
  ledcAttachPin(PWM3P, 12); 
  ledcSetup(13, 1000, 10);  
  ledcAttachPin(PWM3N, 13); 

  ledcSetup(14, 1000, 10);  
  ledcAttachPin(PWM4P, 14); 
  ledcSetup(15, 1000, 10);  
  ledcAttachPin(PWM4N, 15); 
}


/* 根据pwm输出电机转速，适用于驱动芯片DRV8833，正反方向根据pwm正负选择*/
void moto_pwm_set(uint8_t moto, float pwm)
{ 
  pwm = (int)pwm;  //转换为整形数据
  switch(moto){
    case 1:
      if(pwm<0){
        ledcWrite(9, -pwm); //设置输出PWM占空比
        ledcWrite(8, 0);
      }
      else{
        ledcWrite(8, pwm); //设置输出PWM占空比
        ledcWrite(9, 0);
      }
      break;

    case 2:
      if(pwm<0){
        ledcWrite(11, -pwm); //设置输出PWM占空比
        ledcWrite(10, 0);
      }
      else{
        ledcWrite(10, pwm); //设置输出PWM占空比
        ledcWrite(11, 0);
      }
      break;

    case 3:
      if(pwm<0){
        ledcWrite(13, -pwm); //设置输出PWM占空比
        ledcWrite(12, 0);
      }
      else{
        ledcWrite(12, pwm); //设置输出PWM占空比
        ledcWrite(13, 0);
      }
      break;
    case 4:
      if(pwm<0){
        ledcWrite(15, -pwm); //设置输出PWM占空比
        ledcWrite(14, 0);
      }
      else{
        ledcWrite(14, pwm); //设置输出PWM占空比
        ledcWrite(15, 0);
      }
      break;

    default:break;
  }
}
