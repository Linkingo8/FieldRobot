#ifndef __VISION_FIRE_TASK_H__
#define __VISION_FIRE_TASK_H__

#include "stm32f4xx_hal.h"
typedef struct
{
  int8_t vision_head;
  int16_t vision_pitch;
  int16_t vision_yaw;
  int16_t vision_distance;
	int8_t vision_check_last;
	int8_t vision_check;
	uint8_t data_ready_flag;
} vision_message_t;

typedef struct
{
  float x;
  float y;
	float z;
	int time;
	int num;
} vis_data_typedef;	


typedef struct
{
    float R;
    float Q;
    float x_last;
    float x_mid;
    float x_now;
	  float p_last;
    float p_mid ;
    float p_now;
    float kg;
} kalman_typedef;

#define VISION_BUFLEN 8
#define VISION_MAX_LEN 8

extern kalman_typedef kal_yaw;
extern vision_message_t vision_msg;
extern vis_data_typedef kalman_filter;
extern float kal_predict;
void vision_fire_task(void const *argu);
void vision_message(uint8_t *pbuff);
float KalmanFilter(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R);
void kal_init(void);

#endif
