#ifndef __DATA_PROCESSING_H
#define __DATA_PROCESSING_H

#ifdef  __DATA_PROCESSING_GLOBALS
#define __DATA_PROCESSING_EXT
#else
#define __DATA_PROCESSING_EXT extern
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#define ABS(x)		((x>0)? (x): (-x)) 



typedef struct
{
    float input;        //输入数据
    float out;          //输出数据
    float min_value;    //限幅最小值
    float max_value;    //限幅最大值
    float frame_period; //时间间隔
} ramp_function_source_t;

extern ramp_function_source_t chassis_x_ramp;
extern ramp_function_source_t chassis_y_ramp;
extern ramp_function_source_t chassis_w_ramp;  

float circle_error(float *set ,float *get ,float circle_para);
void ramp_calc(ramp_function_source_t *ramp_source_type, float frame_period, float input, float max, float min);
#endif

