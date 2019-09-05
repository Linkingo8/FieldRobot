/** 
  * @file modeswitch_task.c
  * @version 1.0
  * @date Nov,23th 2018
	*
  * @brief  模式选择任务文件
	*
  *	@author lin kr
  *
  */
	
#include "modeswitch_task.h"
#include "manipulator_task.h"
#include "chassis_task.h"
#include "menu_interaction_task.h"
#include "cmsis_os.h"
#include "remote_msg.h"
#include "Task_1.h"

extern osTimerId chassis_timer_id;
extern osTimerId manipulator_timer_id;              					
extern osTimerId menu_timer_id;
extern osTimerId task_1_timer_id;
ctrl_mode_e ctrl_mode;

void mode_switch_task(void const *argu)
{
	//开启软件定时器任务
	osTimerStart(chassis_timer_id, CHASSIS_PERIOD);
	osTimerStart(menu_timer_id, MENU_PERIOD);
	osTimerStart(manipulator_timer_id,MANIPULATOR_PERIOD);
	osTimerStart(task_1_timer_id, TASK_1_PERIOD);
   uint32_t mode_wake_time = osKernelSysTick();
	for(;;)
	{
		taskENTER_CRITICAL();

		get_sw_mode();
		
	  taskEXIT_CRITICAL();
		
		osDelayUntil(&mode_wake_time, INFO_GET_PERIOD);
	}
}

//static void sw1_mode_handler(void)
//{
//	switch (rc.sw1)
//  {
//		case RC_UP:
//		{
//			ctrl_mode = PROTECT_MODE;
//		}
//		break;
//		case RC_MI:
//		{
//			ctrl_mode = NORMAL_MODE;
//		}
//		break;
//		case RC_DN:
//		{
//      ctrl_mode = VISION_MODE;
//		}
//		break;
//		default:
//		break;
//  }
//}

static void sw2_mode_handler(void)
{
	switch (rc.sw2)
	{
		case RC_UP:
		{
			
		}
		break;
		case RC_MI:
		{
		
		}
		break;
		case RC_DN:
		{
		
		}
		break;
		default:
		break;
	}
}

void get_sw_mode(void)
{
//   sw1_mode_handler();
	 sw2_mode_handler();
}
