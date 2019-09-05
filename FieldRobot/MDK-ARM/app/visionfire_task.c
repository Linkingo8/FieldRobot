#include "visionfire_task.h"
#include "cmsis_os.h"
#include "bsp_uart.h"

vision_message_t vision_msg;
kalman_typedef kal_yaw;
vis_data_typedef kalman_filter;
float kal_predict;
uint16_t error_count;
/**
  * @brief vision_fire_task
  * @param 
  * @attention  
  * @note  
  */
void vision_fire_task(void const *argu)
{
	for(;;)
  {
		vision_message(vision_buf);
		vision_msg.data_ready_flag = 1 ;
		osDelay(5);
	}
	
}

void vision_message(uint8_t *pbuff)
{
	if(vision_msg.vision_check_last != pbuff[7])
	{
		if(pbuff[0] == 0xCC)
		{
			vision_msg.vision_head = pbuff[0];
			if((pbuff[1]&0x80)!=0)
			{
				vision_msg.vision_pitch =-((pbuff[1]&0x7f)*256+pbuff[2]);
			}
			else
			{
				vision_msg.vision_pitch =pbuff[1]*256+pbuff[2];
			}
			if((pbuff[3]&0x80)!=0)
			{
				vision_msg.vision_yaw=-((pbuff[3]&0x7f)*256+pbuff[4]);
			}
			else
			{
				vision_msg.vision_yaw=pbuff[3]*256+pbuff[4];
			}
			if((pbuff[5]&0x80)!=0)
			{
				vision_msg.vision_distance=-((pbuff[5]&0x7f)*256+pbuff[6]);
			}
			else
			{
				vision_msg.vision_distance=pbuff[5]*256+pbuff[6];
			}	
			
		  vision_msg.vision_check_last = vision_msg.vision_check;
		  vision_msg.vision_check = pbuff[7];

	  }
		else
		{
			
//			vision_msg.vision_pitch = 0;
//			vision_msg.vision_yaw = 0;
		}
	}
	else
	{
		error_count++;
		if(error_count>10)
		{
      vision_msg.vision_pitch = 0;
     	vision_msg.vision_yaw = 0;
			vision_msg.vision_distance = 0;
		}

	}
}

/*-------------------------------------------------------------------------------------------------------------*/
/*       
        Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
        R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��       
*/

/* �������˲����� */

float KalmanFilter(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R)
{
    kal_yaw.x_mid=kal_yaw.x_last;                       //x_last=x(k-1|k-1),x_mid=x(k|k-1)
    kal_yaw.p_mid=kal_yaw.p_last+kal_yaw.Q;                     //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
    /*
     *  �������˲��������Ҫ��ʽ
     */
    kal_yaw.kg=kal_yaw.p_mid/(kal_yaw.p_mid+kal_yaw.R);                 //kgΪkalman filter��R Ϊ����
    kal_yaw.x_now=kal_yaw.x_mid+kal_yaw.kg*(ResrcData-kal_yaw.x_mid);   //���Ƴ�������ֵ
    kal_yaw.p_now=(1-kal_yaw.kg)*kal_yaw.p_mid;                 //����ֵ��Ӧ��covariance
    kal_yaw.p_last = kal_yaw.p_now;                     //����covariance ֵ
    kal_yaw.x_last = kal_yaw.x_now;                 		//����ϵͳ״ֵ̬
    return kal_yaw.x_now;
}


void kal_init(void)
{
 kal_yaw.kg=1.0f;
 kal_yaw.p_last =0.0f;
 kal_yaw.p_mid = 0.0f;
 kal_yaw.p_now = 0.0f;
 kal_yaw.x_last = 0.0f;
 kal_yaw.x_mid = 0.0f;
 kal_yaw.x_now = 0.0f;
 kal_yaw.x_last = 0.0f;
 kal_yaw.Q = 1.0f;
 kal_yaw.R = 2000.0f;
}


