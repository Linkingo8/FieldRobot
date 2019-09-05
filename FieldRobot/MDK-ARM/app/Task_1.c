/** 
  * @file Task_1. c
  * @version 1.0
  * @date July,27 2019
	*
  * @brief  东方红，任务一
	*
  *	@author linking
  *
  */
#include "Task_1.h"
#include "mytype.h"
#include "cmsis_os.h"
#include "comm_task.h"
#include "string.h"
#include "modeswitch_task.h"
#include "chassis_task.h"
#include "data_processing.h"
#include "remote_msg.h"
#include "pid.h"
#include "stdlib.h"
#include "bsp_can.h"
#include "imu_task.h"
#include "bsp_uart.h"

task_1_t task_1;

void Task_1_param_init(void)
{
   task_1.task_process=TASK_READY;
	 task_1.foward_limit=500;
	
	 PID_struct_init(&pid_tof_left, POSITION_PID, 3000, 500,
									4.0f,	0.005f,0.0f	); 
}

/**
  * @brief Task_1
  * @param     
  * @attention  
	* @note  
  */
void Task_No_1(void const *argu)
{
	 switch(task_1.task_process)
	 {
	   case TASK_READY:
	   {
       ctrl_mode=PROTECT_MODE;
			 task_1.position_ref=imu.yaw;
	   }break;			
	   case TASK_RUNNING_1:
	   {
       ctrl_mode=TASK_1_MODE;
       Foward_go(3000);
			 Along_the_left_side(60);
			 
	   }break;					
	   case TASK_RUNNING_2:
	   {
       task_1.position_ref+=90;
       if(ABS(task_1.position_ref-imu.yaw)<3)task_1.task_process++;
	   }break;
	   case TASK_RUNNING_3:
	   {
       Foward_go(3000);
			 Along_the_left_side(60);
	   }break;		 
	   case TASK_RUNNING_4:
	   {
       task_1.position_ref+=90;
       if(ABS(task_1.position_ref-imu.yaw)<3)task_1.task_process++;
	   }break;   
	   default:
	   {
	   }break;
	 }		


}

void Foward_go(int16_t speed)
{
	 if(tof.forward_val>task_1.foward_limit)chassis.vx=speed;
	 else
	 {
      chassis.vx=speed/2;
		  task_1.task_process++;
	 }
}

void Along_the_left_side(int16_t left_limit)
{
	 float left_limit_ref = left_limit;
	 float left_tof_fdb = (float)tof.left_val;
	 chassis.vy=pid_calc(&pid_tof_left,left_tof_fdb,left_limit_ref); 
}
