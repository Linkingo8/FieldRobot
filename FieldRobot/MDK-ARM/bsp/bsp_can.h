/** 
  * @file bap_can.h
  * @version 1.0
  * @date Oct,15th 2018
  *
  * @brief  
  *
  * @author lin kr
  *
  */
 
#ifndef _BSP_CAN_H_
#define _BSP_CAN_H_

#include "can.h"
#define CHASSIS_CAN   hcan1

/* CAN send and receive ID */
typedef enum
{
  CAN_3508_M1_ID       = 0x201,
  CAN_3508_M2_ID       = 0x202,
  CAN_3508_M3_ID       = 0x203,
  CAN_3508_M4_ID       = 0x204,
	
	CAN_CHASSIS_ALL_ID   = 0x200,
	
  CAN_6020_ID = 0x205,
	CAN_2006_ID = 0x206,


	CAN_MANIPULATOR_ID = 0x1FF,
	
} can_msg_id_e;

typedef struct
{
  uint8_t Tx1Data[8];
} CAN_RecvMsg;

/* can receive motor parameter structure */
#define FILTER_BUF 5

typedef struct
{
  uint16_t ecd;
  uint16_t last_ecd;
  
  int16_t  speed_rpm;
  int16_t  given_current;

  int32_t  round_cnt;
  int32_t  total_ecd;
  int32_t  total_angle;
  
  uint16_t offset_ecd;
  uint32_t msg_cnt;
  
  int32_t  ecd_raw_rate;
  int32_t  rate_buf[FILTER_BUF];
  uint8_t  buf_cut;
  int32_t  filter_rate;
} moto_measure_t;

extern moto_measure_t moto_chassis[4];
extern moto_measure_t moto_manipulator_2006;
extern moto_measure_t moto_manipulator_6020;
extern CAN_RecvMsg can_tx_data;
extern uint8_t RxData[8];

void send_chassis_cur(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
void send_manipulator_cur(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
void my_can_filter_init_recv_all(void);
void get_moto_offset(moto_measure_t* ptr, CAN_HandleTypeDef* hcan);
void encoder_data_handler(moto_measure_t* ptr, CAN_HandleTypeDef* hcan);
void can_device_init(void);

#endif
