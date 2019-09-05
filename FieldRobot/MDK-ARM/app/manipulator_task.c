/** 
  * @file manipulator_task. c
  * @version 1.0
  * @date July,26 2019
	*
  * @brief  ��е��
	*
  *	@author linking
  *
  */
#include "manipulator_task.h"
#include "mytype.h"
#include "cmsis_os.h"
#include "comm_task.h"
#include "string.h"
#include "modeswitch_task.h"
#include "data_processing.h"
#include "remote_msg.h"
#include "pid.h"
#include "stdlib.h"
#include "bsp_can.h"
#include "visionfire_task.h"

#define up_limit   666
#define down_limit 2730

uint8_t pit_ecd_limit_flag=0;
manipulator_t manipulator;
extern TaskHandle_t can_msg_send_task_t;
void manipulator_param_init(void)
{
  memset(&manipulator, 0, sizeof(manipulator_t));
  /* pitch axis motor pid parameter */

  PID_struct_init(&pid_pit_ecd, POSITION_PID, 700, 300,
                  0.2f, 0.00, 0.0); //
  PID_struct_init(&pid_pit_spd, POSITION_PID, 30000, 20000,
                  450.0f, 0.50f, 0.0);
	/******************************************/
	PID_struct_init(&pid_2006_spd, POSITION_PID, 15000, 1000,
                  4.0f, 0.000f, 0.0);
	
	manipulator.pit_center_offset=666;
}

/**
  * @brief manipulator_task
  * @param     
  * @attention  
	* @note  
  */
void manipulator_task(void const *argu)
{
	 switch(ctrl_mode)
	 {
	   case PROTECT_MODE:
	   {
	   			 taskENTER_CRITICAL();
	   			manipulator.current[1] = 0; //6020
	   			manipulator.current[2] = 0; //2006
	   			 taskEXIT_CRITICAL();
	   
	   }break;			
	   case NORMAL_MODE:
	   {
	   			 taskENTER_CRITICAL();
			
	   			 manipulator_control(MANIPULATOR_REMOTER);
			
	   			 taskEXIT_CRITICAL();
	   }break;					
	   case VISION_MODE:
	   {
	   			 taskENTER_CRITICAL();
			
	   		   manipulator_control(MANIPULATOR_VISION_AUTO);
					
	   			 taskEXIT_CRITICAL();
					
	   }break;
		 case TASK_1_MODE:
		 {
			     taskENTER_CRITICAL();

			     normal_calcu();
			     manipulator.current[1] = pid_pit_spd.pos_out;
			     taskEXIT_CRITICAL();
			break;
		 }
	   default:
	   {
	   }break;
	 }		

	motor_cur.manipulator_6020_cur=manipulator.current[1];
	motor_cur.manipulator_2006_cur=manipulator.current[2];
	 
  osSignalSet(can_msg_send_task_t, MANIPULATOR_MOTOR_MSG_SEND);
}


void manipulator_control(manipulator_mode_e manipulator_mode)
{

  switch(manipulator_mode)
	{
	  case MANIPULATOR_REMOTER:
		{
			//pit ecd
			if(pit_ecd_limit_flag==0)manipulator.pid.pit_relative_ecd += rc.ch2 * -0.0040f;//ң�����ı����ֵ����
			else
			{
				if(manipulator.pid.pit_ecd_fdb>=down_limit)
				{
					if(rc.ch2>0)manipulator.pid.pit_relative_ecd += rc.ch2 * -0.0050f;
				}
				if(manipulator.pid.pit_ecd_fdb<=up_limit)
				{
					if(rc.ch2<0)manipulator.pid.pit_relative_ecd += rc.ch2 * -0.0050f;
				}
			}
			
			if(rc.sw2==3)manipulator.M2006_spd_ref=3000;
			else manipulator.M2006_spd_ref=0;
			//2006 spd
			manipulator.pid.M2006_spd_ref=manipulator.M2006_spd_ref;

			normal_calcu();
			
			
	    manipulator.current[1] = pid_pit_spd.pos_out;
      manipulator.current[2] = pid_2006_spd.pos_out;
			
		}break;
		case MANIPULATOR_VISION_AUTO:
		{

//	      vision_calcu();
//			 manipulator.current[0] = pid_vision_yaw_spd.pos_out;	//�����������Զ�������ǽ��ٶ�
//	     manipulator.current[1] = pid_vision_pit_spd.pos_out;
//       manipulator.current[2] = -pid_vision_pit_spd.pos_out;
//				
//				manipulator.pid.pit_relative_ecd = manipulator.pid.pit_vision_relative_ecd;
//				manipulator.pid.yaw_angle_ref = imu_manipulator.yaw;
			
		}
		break;
		default:
		{
		}break;
	}
	

}
void normal_calcu()
{
	manipulator.pid.pit_ecd_fdb = moto_manipulator_6020.ecd;	//�������ֵ����
	manipulator.pid.pit_ecd_ref = manipulator.pid.pit_relative_ecd + manipulator.pit_center_offset;	//����+�е�=Ŀ��
	
	if(manipulator.pid.pit_ecd_fdb>=down_limit||manipulator.pid.pit_ecd_fdb<=up_limit)pit_ecd_limit_flag=1;
	else pit_ecd_limit_flag=0;
		
//	if(manipulator.pid.pit_ecd_ref<=up_limit)manipulator.pid.pit_ecd_ref=up_limit;
//	if(manipulator.pid.pit_ecd_ref>=down_limit)manipulator.pid.pit_ecd_ref=down_limit;
	
	manipulator.pid.pit_error_ecd = circle_error(&manipulator.pid.pit_ecd_ref,&manipulator.pid.pit_ecd_fdb,8191);//����������
	
	pid_calc(&pid_pit_ecd, manipulator.pid.pit_ecd_fdb, manipulator.pid.pit_ecd_fdb + manipulator.pid.pit_error_ecd);//λ�û�
	//pit spd
	manipulator.pid.pit_spd_ref = pid_pit_ecd.pos_out; 	//λ�û����Ϊ�ٶȻ��趨
	//manipulator.pid.pit_spd_ref = 0; 
	manipulator.pid.pit_spd_fdb = moto_manipulator_6020.speed_rpm;	//���ת�ٷ���
	pid_calc(&pid_pit_spd, manipulator.pid.pit_spd_fdb, manipulator.pid.pit_spd_ref);//�ٶȻ�
	
	/****************************************************************************************************************************/
	
	manipulator.pid.M2006_spd_fdb=moto_manipulator_2006.speed_rpm;
  pid_calc(&pid_2006_spd, manipulator.pid.M2006_spd_fdb, manipulator.pid.M2006_spd_ref);//�ٶȻ�
	
}
void vision_calcu()
{
//	 	KalmanFilter(imu_manipulator.yaw-((float)vision_msg.vision_yaw/100),kal_yaw.Q,kal_yaw.R);
//	 kal_predict =  1.0f * (imu_manipulator.yaw-((float)vision_msg.vision_yaw/100) - kal_yaw.x_now);
//	 kal_predict = kal_predict > 10 ? 10:kal_predict;
//	 kal_predict = kal_predict < -10 ? -10:kal_predict;
//	 
//	 if(vision_msg.vision_distance>=0)  // cm
//	 {
//    manipulator.pid.yaw_vision_angle_ref=imu_manipulator.yaw-((float)vision_msg.vision_yaw/100)-kal_predict;
//		 
//	 if(manipulator.pid.yaw_vision_angle_ref<0)manipulator.pid.yaw_vision_angle_ref += 360;	//����������������
//	 else if(manipulator.pid.yaw_vision_angle_ref>360)manipulator.pid.yaw_vision_angle_ref-=360;
//	
//	  manipulator.pid.yaw_ecd_fdb = moto_yaw.ecd;
//	  if ((manipulator.pid.yaw_ecd_fdb <= yaw_limit_left_ecd)&&(manipulator.pid.yaw_ecd_fdb >= yaw_limit_right_ecd))
//	  {
//	   	manipulator.yaw_ecd_limit_flag = 0 ;	
//	  }
//	  else
//	  {
//		  manipulator.yaw_ecd_limit_flag = 1 ;
//	  }
//		float error_yaw_angle = circle_error(&manipulator.pid.yaw_vision_angle_ref,&manipulator.pid.yaw_angle_fdb,360);//���㻷�����
//		manipulator.pid.yaw_angle_fdb = imu_manipulator.yaw;		//�Ƕȷ���Ϊyaw�Ƕ�
//		pid_calc(&pid_vision_yaw_angle, manipulator.pid.yaw_angle_fdb, manipulator.pid.yaw_angle_fdb + error_yaw_angle);//�ǶȻ�
//	
//		//yaw spd
//		manipulator.pid.yaw_vision_spd_ref = pid_vision_yaw_angle.pos_out;	//�ǶȻ����Ϊ�ٶȻ��趨
//		//manipulator.pid.yaw_spd_ref = 0;
//		manipulator.pid.yaw_spd_fdb = manipulator.sensor.yaw_palstance;	//�����ǽ��ٶȷ���
//		pid_calc(&pid_vision_yaw_spd, manipulator.pid.yaw_spd_fdb, manipulator.pid.yaw_vision_spd_ref);	//�ٶȻ�	
//		}
//	 
//  if(vision_msg.vision_distance>=0)
//	{
//	manipulator.pid.pit_vision_relative_ecd = ((float)vision_msg.vision_pitch/100)/360*8191;
//		
//  if(manipulator.pid.pit_vision_relative_ecd < -pit_up_range_ecd)manipulator.pid.pit_vision_relative_ecd = -pit_up_range_ecd;	//�Ա���ֵ���������޷�
//	else if(manipulator.pid.pit_vision_relative_ecd > pit_down_range_ecd)manipulator.pid.pit_vision_relative_ecd = pit_down_range_ecd;
//	
//	manipulator.pid.pit_vision_ecd_ref = manipulator.pid.pit_vision_relative_ecd + manipulator.pit_center_offset;	//����+�е�=Ŀ��
//	manipulator.pid.pit_vision_error_ecd = circle_error(&manipulator.pid.pit_vision_ecd_ref,&manipulator.pid.pit_ecd_fdb,8191);//����������
//	manipulator.pid.pit_ecd_fdb = moto_pit.ecd;	//�������ֵ����
//	pid_calc(&pid_vision_pit_ecd, manipulator.pid.pit_ecd_fdb, manipulator.pid.pit_ecd_fdb + manipulator.pid.pit_vision_error_ecd);//λ�û�
//	//pit spd
//	manipulator.pid.pit_vision_spd_ref = -pid_vision_pit_ecd.pos_out; 	//λ�û����Ϊ�ٶȻ��趨
//	//manipulator.pid.pit_spd_ref = 0; 
//	manipulator.pid.pit_spd_fdb = manipulator.sensor.pit_palstance;	//�����ǽ��ٶȷ���
//	pid_calc(&pid_vision_pit_spd, manipulator.pid.pit_spd_fdb, manipulator.pid.pit_vision_spd_ref);//�ٶȻ�
//	}
}

