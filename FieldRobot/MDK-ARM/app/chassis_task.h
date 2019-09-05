/**
  * @file chassis_task.h
  * @version 1.0
  * @date Oct,19th 2018
  *
  * @brief  
  *
  *	@author lin kr
  *
  */
#ifndef __CLASSIS_TASK_H__
#define __CLASSIS_TASK_H__

#include "stm32f4xx_hal.h"

#define CHASSIS_PERIOD 5

#define MAX_WHEEL_RPM   6000		//���̵��ת�����Ƽ���
#define MAX_MOTOR_RPM   3000		//ƽ̨���ת�����Ƽ���
/* chassis parameter structure */

typedef struct
{
  float           vx; // forward/back
  float           vy; // left/right
  float           vw; // rotate
  
//  chassis_mode_e  ctrl_mode;

  int16_t         wheel_spd_fdb[4];
  int16_t         wheel_spd_ref[4];
  int16_t         current[4];
  
  float         position_ref;
	float         position_fdb;
	float         position_error;
} chassis_t;

typedef struct
{
  float           Rotate;     // positive and negative   ������ת
	float           Lift;       // rise     and fall       ��̧��
	float           Gyrate;     // left     and right      ƽ̨���һ�ת
	float           Telescopic; // stretch  and draw back  ƽ̨����
	

  int16_t         motor_spd_fdb[4];
  int16_t         motro_spd_ref[4];
	
  int16_t         current[4];
  
  int16_t         position_ref;
	
} platform_t;

extern chassis_t  chassis;
extern platform_t platform;

void chassis_init(void);
void chassis_task(void const *argu);
void mecanum_calc(float vx, float vy, float vw, int16_t speed[]);

#endif
