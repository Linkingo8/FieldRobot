/** 
  * @file imu_task.c
  * @version 1.0
  * @date 7,14 2019
	*
  * @brief  RM内测陀螺仪
	*
  *	@author linking
  * @note   CAN接口，陀螺仪选择输出原始四元数，根据yaw-pitch-roll数据 解算出欧拉角
  */
	
#include "imu_task.h"
#include "cmsis_os.h"
#include "comm_task.h"
#include "bsp_can.h"
#include "chassis_task.h"
#include "string.h"
#include "math.h"
#include "gui.h"

imu_typedef imu;
rm_imu_data_t rm_imu_data;
uint8_t imu_can_data[8];

void imu_datahandler(uint32_t dlc,uint32_t can_id)
{
   switch(can_id)
   {
		 case IMU_PARAM_ID:
     {
         rm_imu_data.accel_rangle = imu_can_data[0] &0x0F;
         rm_imu_data.gyro_rangle = (imu_can_data[0] &0xF0) >> 4;
         rm_imu_data.sensor_control_temperature = imu_can_data[2];
         rm_imu_data.imu_sensor_rotation = imu_can_data[3] & 0x1F;
         rm_imu_data.ahrs_rotation_sequence = (imu_can_data[3] & 0xE0) >> 5;
         rm_imu_data.quat_euler = imu_can_data[4] & 0x01;
			 
         switch(rm_imu_data.gyro_rangle)
         {
             case 0: rm_imu_data.gyro_sen = GYRO_2000_SEN; break;
             case 1: rm_imu_data.gyro_sen = GYRO_1000_SEN; break;
             case 2: rm_imu_data.gyro_sen = GYRO_500_SEN;  break;
             case 3: rm_imu_data.gyro_sen = GYRO_250_SEN;  break;
             case 4: rm_imu_data.gyro_sen = GYRO_125_SEN;  break;
         }
			   switch(rm_imu_data.accel_rangle)
         {
             case 0: rm_imu_data.accel_sen = ACCEL_3G_SEN;  break;
             case 1: rm_imu_data.accel_sen = ACCEL_6G_SEN;  break;
             case 2: rm_imu_data.accel_sen = ACCEL_12G_SEN; break;
             case 3: rm_imu_data.accel_sen = ACCEL_24G_SEN; break;
         }
         break;
	   }
		 
		 case IMU_QUAT_ID:
     {
         if(rm_imu_data.quat_euler && dlc == 6)
         {
            memcpy(rm_imu_data.euler_angle, &imu_can_data[0], dlc);
            rm_imu_data.euler_angle_fp32[0] = rm_imu_data.euler_angle[0] * 0.0001f;
            rm_imu_data.euler_angle_fp32[1] = rm_imu_data.euler_angle[1] * 0.0001f;
            rm_imu_data.euler_angle_fp32[2] = rm_imu_data.euler_angle[2] * 0.0001f;
         }
         else if(rm_imu_data.quat_euler == 0 && dlc == 8)
         {
            memcpy(rm_imu_data.quat, &imu_can_data[0], dlc);
            rm_imu_data.quat_fp32[0] = rm_imu_data.quat[0] * 0.0001f;
            rm_imu_data.quat_fp32[1] = rm_imu_data.quat[1] * 0.0001f;
            rm_imu_data.quat_fp32[2] = rm_imu_data.quat[2] * 0.0001f;
            rm_imu_data.quat_fp32[3] = rm_imu_data.quat[3] * 0.0001f;
         }
         break;
      }
		  
			case IMU_GYRO_ID:
      {
          memcpy(rm_imu_data.gyro_int16, &imu_can_data[0],6);
          rm_imu_data.gyro_fp32[0] = rm_imu_data.gyro_int16[0] * rm_imu_data.gyro_sen;
          rm_imu_data.gyro_fp32[1] = rm_imu_data.gyro_int16[1] * rm_imu_data.gyro_sen;
          rm_imu_data.gyro_fp32[2] = rm_imu_data.gyro_int16[2] * rm_imu_data.gyro_sen;
          rm_imu_data.sensor_temperature = (int16_t)((imu_can_data[6] << 3) | (imu_can_data[7] >> 5));
          if (rm_imu_data.sensor_temperature > 1023)
          {
          rm_imu_data.sensor_temperature -= 2048;
          }
          break;
      }
			
			case IMU_ACCEL_ID:
      {
          memcpy(rm_imu_data.accel_int16, &imu_can_data[0],6);
          rm_imu_data.accel_fp32[0] = rm_imu_data.accel_int16[0] * rm_imu_data.accel_sen;
          rm_imu_data.accel_fp32[1] = rm_imu_data.accel_int16[1] * rm_imu_data.accel_sen;
          rm_imu_data.accel_fp32[2] = rm_imu_data.accel_int16[2] * rm_imu_data.accel_sen;
          memcpy(&rm_imu_data.sensor_time, (&imu_can_data[0] + 6), 2);
          break;
      }
			
			case IMU_MAG_ID:
      {
         memcpy(rm_imu_data.mag_int16, &imu_can_data[0],6);
         break;
      }
	 }
	 
	 	imu.yaw = (atan2f(rm_imu_data.quat_fp32[0]*rm_imu_data.quat_fp32[3]+rm_imu_data.quat_fp32[1]*rm_imu_data.quat_fp32[2],
	                          rm_imu_data.quat_fp32[0]*rm_imu_data.quat_fp32[0]+rm_imu_data.quat_fp32[1]*rm_imu_data.quat_fp32[1]-0.5f)/PI)*180+180;
		imu.roll = (asinf(2*(rm_imu_data.quat_fp32[0]*rm_imu_data.quat_fp32[2]-rm_imu_data.quat_fp32[1]*rm_imu_data.quat_fp32[3]))/PI)*180;
		imu.pitch = (atan2f(rm_imu_data.quat_fp32[0]*rm_imu_data.quat_fp32[1]+rm_imu_data.quat_fp32[2]*rm_imu_data.quat_fp32[3],
	                          rm_imu_data.quat_fp32[0]*rm_imu_data.quat_fp32[0]+rm_imu_data.quat_fp32[3]*rm_imu_data.quat_fp32[3]-0.5f)/PI)*180;
		
//    imu_gimbal.yaw=(rm_imu_data.euler_angle_fp32[0]/PI)*180+180;
//	  imu_gimbal.pitch=(rm_imu_data.euler_angle_fp32[2]/PI)*180;
//	  imu_gimbal.roll=(rm_imu_data.euler_angle_fp32[1]/PI)*180;
	 
		imu.wx = rm_imu_data.gyro_int16[0];
		imu.wy = rm_imu_data.gyro_int16[1];
		imu.wz = rm_imu_data.gyro_int16[2];
		
		imu.ax = rm_imu_data.accel_fp32[0];
		imu.ay = rm_imu_data.accel_fp32[1];
		imu.az = rm_imu_data.accel_fp32[2];
	 
}


