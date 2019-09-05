/** 
  * @file menu_interaction_task.c
  * @version 1.0
  * @date July,23 2019
	*
  * @brief  OLED菜单交互任务文件
	*
  *	@author linking
  *
  */
	
#include "menu_interaction_task.h"
#include "chassis_task.h"
#include "cmsis_os.h"
#include "gui.h"
#include "adc.h"
#include "oled.h"
#include "Task_1.h"

uint16_t button_adc_value;
Mark Mark_Sign;
Button_t Button;

void menu_switch_task(void const *argu)
{
	Interface_switch();
  switch(Mark_Sign.Interface_Mark)
  {
        //主界面
        case Main_Interface:
				{  
					  if(Mark_Sign.last_Interface_Mark!=Main_Interface)
						{	
               Main_Interface_APP();//显示主界面
					     Mark_Sign.last_Interface_Mark=Main_Interface;
						}
				}
        break;

        //菜单界面
        case Menu_Interface:
				{
					if(Mark_Sign.last_Interface_Mark!=Menu_Interface||Mark_Sign.Menu_Mark!=Mark_Sign.last_Menu_Mark)
					{	
							switch(Mark_Sign.Menu_Mark)
							{
								case Menu_Function:
								{
									if(Mark_Sign.last_Interface_Mark!=Menu_Interface||Mark_Sign.last_Menu_Mark!=Menu_Function)
									{
										GUI_ShowString(8,0,(unsigned char*)"-----Menu-----",16,1);
										GUI_ShowString(6,15,(unsigned char*)"1.",16,0);
										GUI_ShowString(6,31,(unsigned char*)"2.",16,1);
										GUI_ShowString(6,47,(unsigned char*)"3.",16,1);
										GUI_ShowString(20,15,(unsigned char*)"Function List",16,0);
										GUI_ShowString(20,31,(unsigned char*)"Data Panel   ",16,1);
										GUI_ShowString(20,47,(unsigned char*)"System Setup ",16,1);
										GUI_Fill(6,15,123,15,1);										
										Mark_Sign.last_Menu_Mark=Menu_Function;
									}
								}
								break;
								case Menu_Data:
								{
									if(Mark_Sign.last_Interface_Mark!=Menu_Interface||Mark_Sign.last_Menu_Mark!=Menu_Data)
									{
										GUI_ShowString(8,0,(unsigned char*)"-----Menu-----",16,1);
										GUI_ShowString(6,15,(unsigned char*)"1.",16,1);
										GUI_ShowString(6,31,(unsigned char*)"2.",16,0);
										GUI_ShowString(6,47,(unsigned char*)"3.",16,1);
										GUI_ShowString(20,15,(unsigned char*)"Function List",16,1);
										GUI_ShowString(20,31,(unsigned char*)"Data Panel   ",16,0);
										GUI_ShowString(20,47,(unsigned char*)"System Setup ",16,1);
										GUI_Fill(6,31,123,31,1);										
										Mark_Sign.last_Menu_Mark=Menu_Data;
									}
								}
								break;
								case Menu_System:
								{
									if(Mark_Sign.last_Interface_Mark!=Menu_Interface||Mark_Sign.last_Menu_Mark!=Menu_System)
									{
										GUI_ShowString(8,0,(unsigned char*)"-----Menu-----",16,1);
										GUI_ShowString(6,15,(unsigned char*)"1.",16,1);
										GUI_ShowString(6,31,(unsigned char*)"2.",16,1);
										GUI_ShowString(6,47,(unsigned char*)"3.",16,0);
										GUI_ShowString(20,15,(unsigned char*)"Function List",16,1);
										GUI_ShowString(20,31,(unsigned char*)"Data Panel   ",16,1);
										GUI_ShowString(20,47,(unsigned char*)"System Setup ",16,0);
										GUI_Fill(6,47,123,47,1);
										Mark_Sign.last_Menu_Mark=Menu_System;
									}
								}
								break;
								default:
									break;
							}

						  Mark_Sign.last_Interface_Mark=Menu_Interface;
					}
				}
         break;
				
        //任务界面
        case Task_Interface:
				{
					if(Mark_Sign.last_Interface_Mark!=Task_Interface||Mark_Sign.Task_Mark!=Mark_Sign.last_Task_Mark)
					{
             switch(Mark_Sign.Task_Mark)
						 {
							 case Task_1:
							 {
								 	if(Mark_Sign.last_Interface_Mark!=Task_Interface||Mark_Sign.last_Task_Mark!=Task_1)
									{
			                GUI_ShowString(6,0,(unsigned char*)"Function List",16,1);
			                GUI_ShowString(6,15,(unsigned char*)"1.",16,0);
			                GUI_ShowString(6,31,(unsigned char*)"2.",16,1);
			                GUI_ShowString(6,47,(unsigned char*)"3.",16,1);
										  GUI_ShowString(68,15,(unsigned char*)"       ",16,0);										
										  GUI_ShowString(68,31,(unsigned char*)"       ",16,1);
										  GUI_ShowString(68,47,(unsigned char*)"       ",16,1);		              
  		                GUI_ShowCHinese(20,15,16,(unsigned char*)"任务一",0);
			                GUI_ShowCHinese(20,31,16,(unsigned char*)"任务二",1);
			                GUI_ShowCHinese(20,47,16,(unsigned char*)"任务三",1);
				              GUI_Fill(6,15,123,15,1);    									
								      Mark_Sign.last_Task_Mark=Task_1;
									}
							 }
							 break;
							 case Task_2:
							 {
								 	if(Mark_Sign.last_Interface_Mark!=Task_Interface||Mark_Sign.last_Task_Mark!=Task_2)
									{
			                GUI_ShowString(6,0,(unsigned char*)"Function List",16,1);
			                GUI_ShowString(6,15,(unsigned char*)"1.",16,1);
			                GUI_ShowString(6,31,(unsigned char*)"2.",16,0);
			                GUI_ShowString(6,47,(unsigned char*)"3.",16,1);
										  GUI_ShowString(68,15,(unsigned char*)"       ",16,1);										
										  GUI_ShowString(68,31,(unsigned char*)"       ",16,0);
										  GUI_ShowString(68,47,(unsigned char*)"       ",16,1);										                
  		                GUI_ShowCHinese(20,15,16,(unsigned char*)"任务一",1);
			                GUI_ShowCHinese(20,31,16,(unsigned char*)"任务二",0);
			                GUI_ShowCHinese(20,47,16,(unsigned char*)"任务三",1);
			                GUI_Fill(6,31,123,31,1);  										
								      Mark_Sign.last_Task_Mark=Task_2;
									}
							 }
							 break;
							 case Task_3:
							 {
								 	if(Mark_Sign.last_Interface_Mark!=Task_Interface||Mark_Sign.last_Task_Mark!=Task_3)
									{
			                GUI_ShowString(6,0,(unsigned char*)"Function List",16,1);
			                GUI_ShowString(6,15,(unsigned char*)"1.",16,1);
			                GUI_ShowString(6,31,(unsigned char*)"2.",16,1);
			                GUI_ShowString(6,47,(unsigned char*)"3.",16,0);
										  GUI_ShowString(68,15,(unsigned char*)"       ",16,1);										
										  GUI_ShowString(68,31,(unsigned char*)"       ",16,1);
										  GUI_ShowString(68,47,(unsigned char*)"       ",16,0);
										  GUI_ShowCHinese(20,15,16,(unsigned char*)"任务一",1);
			                GUI_ShowCHinese(20,31,16,(unsigned char*)"任务二",1);
			                GUI_ShowCHinese(20,47,16,(unsigned char*)"任务三",0);
			                GUI_Fill(6,47,123,47,1);                  
								      Mark_Sign.last_Task_Mark=Task_3;
									}
							 }
							 break;
               case Task_1_Running:	
               {
								 	if(Mark_Sign.last_Interface_Mark!=Task_Interface||Mark_Sign.last_Task_Mark!=Task_1_Running)
									{
								      GUI_ShowString(12,20,(unsigned char*)"Task_1 Running",16,1);    									
								      Mark_Sign.last_Task_Mark=Task_1_Running;
									}

               }	
               break;							 
               default:
               break;								 
						 }
					   Mark_Sign.last_Interface_Mark=Task_Interface;
					}
				}
        break;
				//数据界面
        case Data_Interface:
				{
					if(Mark_Sign.last_Interface_Mark!=Data_Interface)
					{
           Data_Interface_APP();//数据面板界面
					 Mark_Sign.last_Interface_Mark=Data_Interface;
					}
				}
        break;
       default:
         break;
	 }	 
}

void Interface_switch(void)
{
	Button_ADC_Data_Handler();
	
	if(Button.kb.key_code==0&&(Mark_Sign.Task_Mark==Task_1_Running|Mark_Sign.Task_Mark==Task_2_Running|Mark_Sign.Task_Mark==Task_3_Running)) //任务运行也不返回
	{
		Mark_Sign.cnt++;
		if(Mark_Sign.cnt>2000&&Mark_Sign.cnt<3000&&Mark_Sign.Interface_Mark!=Main_Interface)
		{
			OLED_Clear(0);
			Mark_Sign.Interface_Mark=Main_Interface;       //一分钟按键无动作 自动返回主界面
			Mark_Sign.cnt=4000;
		}
	}
	else
	{
		Mark_Sign.cnt=0;
	}
	
	if(Button.kb.bit.Up) 
	{
		Mark_Sign.up_cnt++;
		if(Mark_Sign.up_cnt>4&&Mark_Sign.Interface_Mark==Menu_Interface&&Mark_Sign.Menu_Mark>0x10&&Mark_Sign.up_cnt<30)
		{
			Mark_Sign.Menu_Mark-=0x10;
			Mark_Sign.up_cnt=40;
		}
		if(Mark_Sign.up_cnt>4&&Mark_Sign.Interface_Mark==Task_Interface&&Mark_Sign.Task_Mark>0x10&&Mark_Sign.up_cnt<30)
		{
			Mark_Sign.Task_Mark-=0x10;
			Mark_Sign.up_cnt=40;
		}
	}
	else
	{
		Mark_Sign.up_cnt=0;
	}
	
	if(Button.kb.bit.Down) 
	{
		Mark_Sign.down_cnt++;
		if(Mark_Sign.down_cnt>4&&Mark_Sign.Interface_Mark==Menu_Interface&&Mark_Sign.Menu_Mark<0x30&&Mark_Sign.down_cnt<30)
		{
			Mark_Sign.Menu_Mark+=0x10;
			Mark_Sign.down_cnt=40;
		}
		if(Mark_Sign.down_cnt>4&&Mark_Sign.Interface_Mark==Task_Interface&&Mark_Sign.Task_Mark<0x30&&Mark_Sign.down_cnt<30)
		{
			Mark_Sign.Task_Mark+=0x10;
			Mark_Sign.down_cnt=40;
		}
	}
	else
	{
		Mark_Sign.down_cnt=0;
	}
	
	if(Button.kb.bit.Mid) 
	{
		Mark_Sign.mid_cnt++;
		if(Mark_Sign.mid_cnt>4&&Mark_Sign.Interface_Mark==Menu_Interface&&Mark_Sign.mid_cnt<80)
		{
      if(Mark_Sign.Menu_Mark==Menu_Function)
			{
				OLED_Clear(0);
				Mark_Sign.Interface_Mark=Task_Interface;
				Mark_Sign.Task_Mark=Task_1;
				Mark_Sign.mid_cnt=40;
			}
			if(Mark_Sign.Menu_Mark==Menu_Data)
			{
				OLED_Clear(0);
				Mark_Sign.Interface_Mark=Data_Interface;
				Mark_Sign.mid_cnt=40;
			}
//			if(Mark_Sign.Menu_Mark==Menu_System)Mark_Sign.Interface_Mark=Task_Interface;
		}
		if(Mark_Sign.mid_cnt>100&&Mark_Sign.Interface_Mark==Main_Interface)
		{
			OLED_Clear(0);
			Mark_Sign.Interface_Mark=Menu_Interface; //长按菜单键
			Mark_Sign.Menu_Mark=Menu_Function;
		}
		if(Mark_Sign.mid_cnt>4&& Mark_Sign.Task_Mark==Task_1&&Mark_Sign.mid_cnt<40)
		{
				OLED_Clear(0);
        Mark_Sign.Task_Mark=Task_1_Running;
			  task_1.task_process=TASK_RUNNING_1;
			  Mark_Sign.mid_cnt=40;
	  }
	}
	else
	{
		Mark_Sign.mid_cnt=0;
	}
	
 if(Button.kb.bit.Left) 
	{
		Mark_Sign.left_cnt++;
		if(Mark_Sign.left_cnt>4&&Mark_Sign.Interface_Mark==Menu_Interface&&Mark_Sign.left_cnt<60)
		{
			OLED_Clear(0);
			Mark_Sign.Interface_Mark=Main_Interface;
			Mark_Sign.left_cnt=80;
		}
		if(Mark_Sign.left_cnt>4&&Mark_Sign.Interface_Mark==Task_Interface&&Mark_Sign.left_cnt<60)
		{
			OLED_Clear(0);
			Mark_Sign.Interface_Mark=Menu_Interface;
			Mark_Sign.Menu_Mark=Menu_Function;
			Mark_Sign.left_cnt=80;
		}
		if(Mark_Sign.left_cnt>4&&Mark_Sign.Interface_Mark==Data_Interface&&Mark_Sign.left_cnt<60)
		{
			OLED_Clear(0);
			Mark_Sign.Interface_Mark=Menu_Interface;
			Mark_Sign.Menu_Mark=Menu_Data;
			Mark_Sign.left_cnt=80;
		}
	}
	else
	{
		Mark_Sign.left_cnt=0;
	}
}


void Button_ADC_Data_Handler(void)
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&button_adc_value,1);
	if(button_adc_value<500)Button.kb.key_code=0x01;
	else if(button_adc_value<1200)Button.kb.key_code=0x02;	
	else if(button_adc_value<2000)Button.kb.key_code=0x04;	
	else if(button_adc_value<2800)Button.kb.key_code=0x08;
	else if(button_adc_value<3300)Button.kb.key_code=0x10;
	else Button.kb.key_code=0x00;
}

void Main_Interface_APP(void)
{
	GUI_ShowCHinese(32,12,24,(unsigned char*)"东方红",1);
	GUI_ShowString(45,37,(unsigned char*)"Taurus",16,1);
}
void Menu_Interface_APP(void)
{
	GUI_ShowString(8,0,(unsigned char*)"------Menu------",16,1);
	GUI_ShowString(6,15,(unsigned char*)"1.",16,0);
	GUI_ShowString(6,31,(unsigned char*)"2.",16,1);
	GUI_ShowString(6,47,(unsigned char*)"3.",16,1);
	GUI_Fill(6,15,123,15,1);
	GUI_ShowString(20,15,(unsigned char*)"Function List",16,0);
	GUI_ShowString(20,31,(unsigned char*)"Data Panel   ",16,1);
	GUI_ShowString(20,47,(unsigned char*)"System Setup",16,1);
}
void Function_Menu_APP(void)
{
	GUI_ShowString(6,0,(unsigned char*)"Function List",16,1);
	GUI_ShowString(6,15,(unsigned char*)"1.",16,0);
	GUI_ShowString(6,31,(unsigned char*)"2.",16,1);
	GUI_ShowString(6,47,(unsigned char*)"3.",16,1);
	GUI_Fill(6,15,123,15,1);                  
  GUI_ShowCHinese(20,15,16,(unsigned char*)"任务一",0);
	GUI_ShowString(68,15,(unsigned char*)"       ",16,0);
	GUI_ShowCHinese(20,31,16,(unsigned char*)"任务二",1);
	GUI_ShowCHinese(20,47,16,(unsigned char*)"任务三",1);
}
void Data_Interface_APP(void)
{
	GUI_ShowString(12,20,(unsigned char*)"Data_Interface",16,1);
}
