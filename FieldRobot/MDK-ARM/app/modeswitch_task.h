/** 
  * @file modeswitch_task.h
  * @version 1.0
  * @date Nov,05th 2018
	*
  * @brief  µ×²ã´®¿ÚÅäÖÃ£¬
	*
  *	@author lin kr
  *
  */
	
#ifndef __MODESWITCH_TASK_H__
#define __MODESWITCH_TASK_H__

#define INFO_GET_PERIOD 20

typedef enum
{
  PROTECT_MODE           = 0,
  NORMAL_MODE            = 1,
	VISION_MODE            = 2,
	TASK_1_MODE            = 3,
	TASK_2_MODE            = 4,
	
} ctrl_mode_e;

extern ctrl_mode_e ctrl_mode;

void mode_switch_task(void const *argu);
void get_sw_mode(void);

#endif
