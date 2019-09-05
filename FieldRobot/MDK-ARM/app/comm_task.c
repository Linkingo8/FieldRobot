/** 
  * @file comm_task.c
  * @version 1.1
  * @date Oct,23th 2018
	*
  * @brief  交互任务文件
	*
  *	@author lin kr
  *
  */
#include "comm_task.h"
#include "bsp_can.h"
#include "cmsis_os.h"
#include "pid.h"

motor_current_t motor_cur;

/**
  * @brief can_msg_send_task
  * @param 
  * @attention  
  * @note  
  */
void can_msg_send_task(void const *argu)
{
	osEvent event;
	for(;;)
  {
		event = osSignalWait(CHASSIS_MOTOR_MSG_SEND|MANIPULATOR_MOTOR_MSG_SEND, osWaitForever);
		
		if (event.status == osEventSignal)
    {
			if (event.value.signals & CHASSIS_MOTOR_MSG_SEND)
      {
        send_chassis_motor_ctrl_message(motor_cur.chassis_cur);
			}
      if (event.value.signals & MANIPULATOR_MOTOR_MSG_SEND)
      {
				send_manipulator_motor_ctrl_message(motor_cur.manipulator_6020_cur,motor_cur.manipulator_2006_cur);
			}
		}
	}
}

/**
  * @brief send_chassis_motor_ctrl_message
  * @param 
  * @attention  
  * @note  
  */
void send_chassis_motor_ctrl_message(int16_t chassis_cur[])
{
  send_chassis_cur(chassis_cur[0], chassis_cur[1], 
                   chassis_cur[2], chassis_cur[3]);
}

/**
  * @brief send_chassis_motor_ctrl_message
  * @param 
  * @attention  
  * @note  
  */
void send_manipulator_motor_ctrl_message(int16_t moto_6020_cur,int16_t moto_2006_cur)
{
  send_manipulator_cur(moto_6020_cur, moto_2006_cur, 0, 0);
}
