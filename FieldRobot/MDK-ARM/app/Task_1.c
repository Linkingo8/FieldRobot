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
	 task_1.foward_limit=400;
	
	 PID_struct_init(&pid_tof_left, POSITION_PID, 2000, 500,
									13.0f,	0.1f,0.0f	); 
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
      if(tof.left_val>300)
			{
				ramp_calc(&chassis_x_ramp,1.0f,60.0f, 800.0f, 0.0f);
				chassis.vx=chassis_x_ramp.out;
			}
	    else
	    {
		    task_1.task_process++;
	    }
		 
	   }break;					
	   case TASK_RUNNING_2:
	   {
       Foward_go(4000);
	   }break;
	   case TASK_RUNNING_3:
	   {
			 chassis.vx=0;
			 if(ABS(task_1.position_ref-imu.yaw)<2)chassis.vw=0;
	   }break;		 
	   case TASK_RUNNING_4:
	   {
//       if(ABS(task_1.position_ref-imu.yaw)<3)task_1.task_process++;
	   }break;   
	   default:
	   {
	   }break;
	 }		


}

void Foward_go(int16_t speed)
{
  if(tof.forward_val>700)
	{
		ramp_calc(&chassis_x_ramp,1.0f,60.0f, speed, 600.0f);
		chassis.vx=chassis_x_ramp.out;
	}
	else if(tof.forward_val<700&&tof.forward_val>200)
	{
    if(chassis_x_ramp.out > 0)
		{
			ramp_calc(&chassis_x_ramp,1.0f,-150.0f, speed, 300.0f);	
      chassis.vx=chassis_x_ramp.out;			
		}
	}
	else if(tof.forward_val<180)
	{
		chassis.vx=0;
		task_1.task_process++;
		task_1.position_ref-=90;
	}
}

void Along_the_left_side(int16_t speed,int16_t left_limit)
{
	if(tof.forward_val>task_1.foward_limit)
	 {
		 tof.forward_cnt=0;
	 }
	 else
	 {
		  tof.forward_cnt++;
		  if(tof.forward_val<task_1.foward_limit&&tof.forward_cnt>6)
			{
      chassis.vx=0;
//		  task_1.task_process++;
//		  task_1.position_ref-=90;
			}
	 }
	 float left_limit_ref = left_limit;
	 float left_tof_fdb = (float)tof.left_val;
	 if(ABS(left_limit_ref-left_tof_fdb)>10)
	 {
		 if(chassis.vx<1200)chassis.vy=pid_calc(&pid_tof_left,left_tof_fdb,left_limit_ref); 
		 else		 chassis.vx-=40;
	 }
	 else if(tof.forward_val>task_1.foward_limit)
	 {
		  chassis.vx+=60;
		  if(chassis.vx>=speed)chassis.vx=speed;
	 }
}
