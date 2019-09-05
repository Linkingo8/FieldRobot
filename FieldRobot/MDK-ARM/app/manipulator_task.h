/**
  * @file manipulator_task.h
  * @version 1.0
  * @date July,26,2019
  *
  * @brief
  *
  *	@author linking
  *
  */


#ifndef __MANIPULATOR_TASK_H__
#define __MANIPULATOR_TASK_H__

#include "stm32f4xx_hal.h"

#define MANIPULATOR_PERIOD	 1

#include "pid.h"
typedef enum
{
	MANIPULATOR_REMOTER         = 0,
	MANIPULATOR_VISION_AUTO     = 1,
	MANIPULATOR_PROTECT         = 2,
}
manipulator_mode_e;


typedef struct
{
  /* position loop ecd*/
  float yaw_ecd_ref;
  float pit_ecd_ref;
  float yaw_ecd_fdb;
  float pit_ecd_fdb;
  /* position loop angle*/
  float yaw_angle_ref;
  float pit_angle_ref;
  float yaw_angle_fdb;
  float pit_angle_fdb;
  /* speed loop */
  float yaw_spd_ref;
  float pit_spd_ref;
  float yaw_spd_fdb;
  float pit_spd_fdb;
	
	
	float M2006_spd_ref;
	float M2006_spd_fdb;
	
  float yaw_vision_ref;
  float pit_vision_ref;
	
  float pit_relative_ecd;	//变化的增量
  float yaw_relative_ecd;
  /* unit: degree */
  float pit_relative_angle;
  float yaw_relative_angle;
	
  float pit_error_ecd;
  float yaw_error_ecd;
  /* unit: degree */
  float pit_error_angle;
  float yaw_error_angle;


  float yaw_vision_angle_ref;
	float pit_vision_ecd_ref;
  float pit_vision_relative_ecd;	//变化的增量
	
	float pit_vision_error_ecd;

  float yaw_vision_spd_ref;
  float pit_vision_spd_ref;

} manipulator_pid_t;

typedef struct
{
//  float gyro_angle;
  /* uint: degree/s */
  float yaw_palstance;
  float pit_palstance;
} manipulator_sensor_t;

typedef struct
{
  manipulator_mode_e ctrl_mode;
  
  /* gimbal information */

  float         ecd_offset_angle;
  float         yaw_offset_angle;
  
  /* gimbal ctrl parameter */
  manipulator_pid_t     pid;
	
  float         M2006_spd_ref;
  
  /* read from flash */
  int32_t       pit_center_offset;
  int32_t       yaw_center_offset;
	int32_t       yaw_giving_ceter_offset;
  
  //gimbal_cmd_e  auto_ctrl_cmd;
	
  int16_t         current[3];  //yaw 0  pit  1  trigger  2
	
	uint8_t          yaw_ecd_limit_flag;

}manipulator_t;

extern manipulator_t manipulator;


void manipulator_task(void const *argu);
void manipulator_control(manipulator_mode_e manipulator_mode);
void normal_calcu(void);
void vision_calcu(void);
void manipulator_param_init(void);
#endif
