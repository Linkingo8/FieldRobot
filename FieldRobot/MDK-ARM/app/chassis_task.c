/** 
  * @file chassis_task.c
  * @version 1.0
  * @date Oct,19th 2018
	*
  * @brief  底盘姿态任务文件
	*
  *	@author lin kr
  *
  */
#include "chassis_task.h"
#include "cmsis_os.h"
#include "comm_task.h"
#include "string.h"
#include "modeswitch_task.h"
#include "remote_msg.h"
#include "pid.h"
#include "stdlib.h"
#include "bsp_can.h"
#include "gui.h"
#include "imu_task.h"
#include "data_processing.h"
#include "Task_1.h"
chassis_t  chassis;
platform_t platform;

extern TaskHandle_t can_msg_send_task_t;

/**
  * @brief chassis_task
  * @param     
  * @attention  
	* @note  
  */
void chassis_task(void const *argu)
{
	switch(ctrl_mode)
	{
		case PROTECT_MODE:
		{
			taskENTER_CRITICAL();
				
			chassis.vx = 0;
			chassis.vy = 0;
			chassis.vw = 0;
				
			taskEXIT_CRITICAL();
			break;
		}
		case NORMAL_MODE:
		{
			taskENTER_CRITICAL();
				
			chassis.vx = (float)rc.ch4*10;
			chassis.vy = (float)rc.ch3*10;
			chassis.vw = (float)rc.ch1*(-10);
				
			taskEXIT_CRITICAL();
			break;
		}
		case VISION_MODE:
		{
			taskENTER_CRITICAL();
			
			
			taskEXIT_CRITICAL();
			break;
		}
		case TASK_1_MODE:
		{
			taskENTER_CRITICAL();
			chassis.position_ref = (float)task_1.position_ref;
			chassis.position_fdb = (float)imu.yaw;
			chassis.position_error = circle_error(&chassis.position_ref,&chassis.position_fdb,360);
			chassis.vw = pid_calc(&pid_chassis_angle,chassis.position_fdb,chassis.position_fdb+chassis.position_error); 
			taskEXIT_CRITICAL();
			break;
		}
		case TASK_2_MODE:
		{
			taskENTER_CRITICAL();
//			chassis.position_ref = (float)task_1.position_ref;
//			chassis.position_fdb = (float)imu.yaw;
//			chassis.position_error = circle_error(&chassis.position_ref,&chassis.position_fdb,360);
//			chassis.vw = -1.0f*pid_calc(&pid_chassis_angle,chassis.position_fdb+chassis.position_error,chassis.position_ref); 
			taskEXIT_CRITICAL();
			break;
		}
		default:
			break;
	}		
	
	mecanum_calc(chassis.vx, chassis.vy, chassis.vw, chassis.wheel_spd_ref);       
	
	for (uint8_t i = 0; i < 4; i++)
  {
    chassis.wheel_spd_fdb[i]  = moto_chassis[i].speed_rpm;
  }
	
	for (int i = 0; i < 4; i++)
	{
		chassis.current[i]  = pid_calc(&pid_spd[i],chassis.wheel_spd_fdb[i],chassis.wheel_spd_ref[i]);
	}
		
	memcpy(motor_cur.chassis_cur, chassis.current, sizeof(chassis.current));
		 
	osSignalSet(can_msg_send_task_t, CHASSIS_MOTOR_MSG_SEND);
}

 /**
  * @brief 麦轮解算函数
  * @param input : ?=+vx(mm/s)  ?=+vy(mm/s)  ccw=+vw(deg/s)
  *        output: every wheel speed(rpm)
  * @note  1=FL 2=FR 3=BL 4=BR
  */
void mecanum_calc(float vx, float vy, float vw, int16_t speed[])
{
  int16_t wheel_rpm[4];
  float   max = 0;
  
  wheel_rpm[0] =     vx + vy - vw;
  wheel_rpm[1] = -1*(vx - vy + vw);
  wheel_rpm[2] =     vx - vy - vw;
  wheel_rpm[3] = -1*(vx + vy + vw);

	
	//find max item
  for (uint8_t i = 0; i < 4; i++)
  {
    if (abs(wheel_rpm[i]) > max)
      max = abs(wheel_rpm[i]);
  }
  //equal proportion
  if (max > 10000)
  {
    float rate = 10000 / max;
    for (uint8_t i = 0; i < 4; i++)
      wheel_rpm[i] *= rate;
  }
	memcpy(speed, wheel_rpm, 4*sizeof(int16_t));
}


void chassis_init(void)
{
	for(int i=0; i<4; i++)
	{
		PID_struct_init(&pid_spd[i], POSITION_PID, 15000, 15000,
									3.5f,	0.008f,	0.0f	);  
	}
	PID_struct_init(&pid_chassis_angle, POSITION_PID, 1000, 300,
									100.0f,	0.12f,0.0f	); 
}


