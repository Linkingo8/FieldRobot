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

#define MAX_WHEEL_RPM   6000		//底盘电机转速限制极限
#define MAX_MOTOR_RPM   3000		//平台电机转速限制极限
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
  float           Rotate;     // positive and negative   杆正反转
	float           Lift;       // rise     and fall       杆抬升
	float           Gyrate;     // left     and right      平台左右回转
	float           Telescopic; // stretch  and draw back  平台伸缩
	

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
