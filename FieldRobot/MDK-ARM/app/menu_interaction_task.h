/** 
  * @file menu_interaction_task.h
  * @version 1.0
  * @date July,23 2018
	*
  * @brief  
	*
  *	@author linking
  *
  */
	
#ifndef __MENU_INTERACTION_TASK_H__
#define __MENU_INTERACTION_TASK_H__

#include "stdint.h"
#define MENU_PERIOD 10   

typedef struct
{
    uint8_t  Interface_Mark;          //界面状态
	  uint8_t  last_Interface_Mark;     //上次界面状态
	  uint8_t  Menu_Mark;
	  uint8_t  last_Menu_Mark;
    uint8_t  Task_Mark; 
    uint8_t  last_Task_Mark;	
	  uint8_t  Task1_Status;               //任务状态
		uint8_t  Task2_Status;               //任务状态
		uint8_t  Task3_Status;               //任务状态
	  uint32_t cnt;
	  uint32_t up_cnt;
	  uint32_t down_cnt;
	  uint32_t left_cnt;
	  uint32_t right_cnt;
	  uint32_t mid_cnt;
} Mark;


enum
{
    Main_Interface = 0x10, /****主界面*****/
    Menu_Interface = 0x20, /****菜单界面***/
    Task_Interface = 0x30, /****任务界面***/
	  Data_Interface = 0x40, /****数据界面***/
};

enum
{
    Menu_Function  = 0x10, 
    Menu_Data      = 0x20, 
	  Menu_System    = 0x30, 
};

enum
{
    Task_1                 = 0x10, 
    Task_2                 = 0x20, 
	  Task_3                 = 0x30, 
    Task_1_Running         = 0x40, 
    Task_2_Running         = 0x50, 
    Task_3_Running         = 0x60, 	
};
enum
{
    Task_Ready = 0x10, 
    Task_Running = 0x20, 
    Task_End   = 0x30, 
};

typedef struct
{
	 __packed union
  {
    uint8_t key_code;
    __packed struct 
    {
     uint8_t Mid:1;
     uint8_t Left:1;
     uint8_t Right:1;
     uint8_t Up:1;  
	   uint8_t Down:1;
    } bit;
  } kb;
} Button_t;

extern Mark Mark_Sign;
extern Button_t Button; 
void menu_switch_task(void const *argu);
void Interface_switch(void);
void Main_Interface_APP(void);
void Menu_Interface_APP(void);
void Function_Menu_APP(void);
void Data_Interface_APP(void);
void Button_ADC_Data_Handler(void);

#endif
