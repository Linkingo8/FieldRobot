/** 
  * @file imu_task.h
  * @version 1.0
  * @date 7,14 2019
	*
  * @note  
	*
  *	
  */
#ifndef __IMU_TASK_H__
#define __IMU_TASK_H__

#include "stm32f4xx_hal.h"	
#include "main.h"

#ifndef PI
#define PI 3.14159265358979f
#endif

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

#define IMU_QUAT_ID  0x401
#define IMU_GYRO_ID  0x402
#define IMU_ACCEL_ID 0x403
#define IMU_MAG_ID   0x404
#define IMU_PARAM_ID 0x405

//转换成 m/s^2
#define ACCEL_3G_SEN 0.0008974358974f
#define ACCEL_6G_SEN 0.00179443359375f
#define ACCEL_12G_SEN 0.0035888671875f
#define ACCEL_24G_SEN 0.007177734375f
//转换成 rad/s
#define GYRO_2000_SEN 0.00106526443603169529841533860381f
#define GYRO_1000_SEN 0.00053263221801584764920766930190693f
#define GYRO_500_SEN 0.00026631610900792382460383465095346f
#define GYRO_250_SEN 0.00013315805450396191230191732547673f
#define GYRO_125_SEN 0.000066579027251980956150958662738366f

typedef enum
{
	DATA_NOT_READY         = 0x00,
	DATA_READY             = 0x01,
	DATA_STABLE            = 0x02,
	DATA_ERROR             = 0x03,
}
imu_data_status_e;

typedef struct
{
    uint8_t quat_euler:1;
    uint8_t gyro_rangle:3;
    uint8_t accel_rangle:2;
    uint8_t imu_sensor_rotation:5;
    uint8_t ahrs_rotation_sequence:3;
    int16_t quat[4];
    fp32 quat_fp32[4];
    int16_t euler_angle[3];
    fp32 euler_angle_fp32[3];
    int16_t gyro_int16[3];
    int16_t accel_int16[3];
    int16_t mag_int16[3];
    fp32 gyro_fp32[3];
    fp32 accel_fp32[3];
    uint16_t sensor_time;
    uint16_t sensor_temperature;
    int16_t sensor_control_temperature;
    fp32 gyro_sen;
    fp32 accel_sen;
	  uint8_t data_ready_flag;
}rm_imu_data_t;


typedef struct imu_typedef1
{
	float pitch;
	float roll;
	float yaw;
	float wx;
	float wy;
	float wz;
	float ax;
	float ay;
	float az;
}imu_typedef;

extern imu_typedef imu;
extern rm_imu_data_t rm_imu_data;
extern uint8_t imu_can_data[8];
void imu_datahandler(uint32_t dlc,uint32_t can_id);

#endif

