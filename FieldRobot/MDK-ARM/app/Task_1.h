/**
  * @file Task_1.h
  * @version 1.0
  * @date July,27,2019
  *
  * @brief
  *
  *	@author linking
  *
  */


#ifndef __TASK_1_H__
#define __TASK_1_H__

#include "stm32f4xx_hal.h"

#define TASK_1_PERIOD	10

typedef enum
{
	TASK_READY                  = 0,
	
	TASK_RUNNING_1              = 1,
	TASK_RUNNING_2              = 2,
	TASK_RUNNING_3              = 3,
	TASK_RUNNING_4              = 4,
	TASK_RUNNING_5              = 5,
	TASK_RUNNING_6              = 6,
	TASK_RUNNING_7              = 7,
	TASK_RUNNING_8              = 8,
	TASK_RUNNING_9              = 9,
	TASK_RUNNING_10             = 10,
	TASK_RUNNING_11             = 11,
	TASK_RUNNING_12             = 12,
	TASK_RUNNING_13             = 13,
	TASK_RUNNING_14             = 14,
	TASK_RUNNING_15             = 15,
	TASK_RUNNING_16             = 16,
	TASK_RUNNING_17             = 17,
	TASK_RUNNING_18             = 18,
	TASK_RUNNING_19             = 19,
	TASK_RUNNING_20             = 20,
	TASK_RUNNING_21             = 21,
	TASK_RUNNING_22             = 22,
	TASK_RUNNING_23             = 23,	
	TASK_RUNNING_24             = 24,		
	
	TASK_END                    = 88,
}
task_process_e;

typedef struct
{
	task_process_e  task_process;
	
	float           position_ref;
	uint16_t        foward_limit;
	uint16_t        back_limit;
	uint16_t        left_limit;
	uint16_t        right_limit;
	
}task_1_t;

extern task_1_t task_1;

void Task_No_1(void const *argu);
void Task_1_param_init(void);
void Foward_go(int16_t speed);
void Along_the_left_side(int16_t speed,int16_t left_limit);


#endif
