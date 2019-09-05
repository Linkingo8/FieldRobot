/**
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 * @file       bsp_uart.h
 * @brief      this file contains the common defines and functions prototypes for 
 *             the bsp_uart.c driver
 * @note         
 * @Version    V1.0.0
 * @Date       Jan-30-2018      
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 */

#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "usart.h"
#include "visionfire_task.h"

#define UART_RX_DMA_SIZE (1024)
#define DBUS_HUART          huart1 /* for dji remote controler reciever */
#define VISION_HUART        huart3 
#define FOWARD_TOF_HUART    huart6
#define BACK_TOF_HUART      huart2
#define LEFT_TOF_HUART      huart8
#define RIGHT_TOF_HUART     huart7

typedef struct tof_typedef1
{
	uint16_t  forward_val;
	uint16_t  back_val;
	uint16_t  left_val;
	uint16_t  right_val;
  uint16_t  forward_cnt;
	uint16_t  back_cnt;
	uint16_t  left_cnt;
	uint16_t  right_cnt;
}tof_typedef;

extern tof_typedef tof;
extern uint8_t     vision_buf[VISION_BUFLEN];

void uart_receive_handler(UART_HandleTypeDef *huart);
void uart_init(void);
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
void tof_data_decoding(UART_HandleTypeDef* huart);
#endif

