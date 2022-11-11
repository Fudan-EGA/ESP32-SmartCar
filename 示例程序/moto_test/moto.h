/**moto.cpp + moto.h
 * 功能：直流电机控制函数，包括基于ledc库的pwm调速功能和编码器计数值获取功能
 * 说明：接口部分通过宏定义配置，可结合实际使用管脚进行配置
 */


#ifndef MOTO_H_
#define MOTO_H_

#include<Arduino.h>

/************电机PWM控制接口定义***************/
#define PWM1P 16
#define PWM1N 25
#define PWM2P 18
#define PWM2N 17
#define PWM3P 19
#define PWM3N 21
#define PWM4P 23
#define PWM4N 22
/************电机PWM控制接口定义***************/


/*使用ESP32LEDC库函数配置为pwm输出模式，共使用8个通道 */
void moto_pwm_init();



/* 根据pwm控制电机转速，适用于驱动芯片DRV8833，dian正反方向根据pwm正负选择
   moto: 电机序号
   pwm: 速度范围（0--1023）
*/
void moto_pwm_set(uint8_t moto, float pwm);


#endif/* MOTO_H_ */
