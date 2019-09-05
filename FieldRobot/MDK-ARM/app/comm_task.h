/** 
  * @file comm_task.h
  * @version 1.0
  * @date Oct,19th 2018
	*
  * @brief  
	*
  *	@author lin kr
  *
  */
#ifndef __COMM_TASK_H__
#define __COMM_TASK_H__

#include "stm32f4xx_hal.h"

#define MANIPULATOR_MOTOR_MSG_SEND  ( 1 << 6 )
#define CHASSIS_MOTOR_MSG_SEND      ( 1 << 7 )


/* motor current parameter structure */
typedef struct
{
  /* 4 chassis motor current */
  int16_t chassis_cur[4];
	int16_t manipulator_6020_cur;
	int16_t manipulator_2006_cur;
  /* yaw/pitch/trigger motor current */
 // int16_t gimbal_cur[2];
} motor_current_t;

void can_msg_send_task(void const *argu);
void send_chassis_motor_ctrl_message  (int16_t chassis_cur[]);
void send_manipulator_motor_ctrl_message(int16_t moto_6020_cur,int16_t moto_2006_cur);
extern motor_current_t motor_cur;

#endif
