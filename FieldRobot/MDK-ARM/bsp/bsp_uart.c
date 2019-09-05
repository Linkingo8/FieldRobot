/** 
  * @file bsp_uart.c
  * @version 1.0
  * @date Nov,05th 2018
	*
  * @brief  µ×²ã´®¿ÚÅäÖÃ£¬
	*
  *	@author lin kr
  *
  */
                                                                                                              
#include "string.h"
#include "stdlib.h"
#include "bsp_uart.h"
#include "usart.h"
#include "main.h"
#include "gui.h"
#include "remote_msg.h"
#include "visionfire_task.h"
#define   TOF_BUFFER_SIZE		8
uint8_t   dbus_buf[DBUS_BUFLEN];
uint8_t   vision_buf[VISION_BUFLEN];
uint8_t   forward_tof_rxbuf[TOF_BUFFER_SIZE];
uint8_t   back_tof_rxbuf[TOF_BUFFER_SIZE];
uint8_t   left_tof_rxbuf[TOF_BUFFER_SIZE];
uint8_t   right_tof_rxbuf[TOF_BUFFER_SIZE];

tof_typedef tof;

/**
  * @brief      enable global uart it and do not use DMA transfer done it
  * @param[in]  huart: uart IRQHandler id
  * @param[in]  pData: receive buff 
  * @param[in]  Size:  buff size
  * @retval     set success or fail
  */
static int uart_receive_dma_no_it(UART_HandleTypeDef* huart, uint8_t* pData, uint32_t Size)
{
  uint32_t tmp1 = 0;

  tmp1 = huart->RxState;
	
	if (tmp1 == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (Size == 0))
		{
			return HAL_ERROR;
		}

		huart->pRxBuffPtr = pData;
		huart->RxXferSize = Size;
		huart->ErrorCode  = HAL_UART_ERROR_NONE;

		/* Enable the DMA Stream */
		HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32_t)pData, Size);

		/* 
		 * Enable the DMA transfer for the receiver request by setting the DMAR bit
		 * in the UART CR3 register 
		 */
		SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

		return HAL_OK;
	}
	else
	{
		return HAL_BUSY;
	}
}

/**
  * @brief      returns the number of remaining data units in the current DMAy Streamx transfer.
  * @param[in]  dma_stream: where y can be 1 or 2 to select the DMA and x can be 0
  *             to 7 to select the DMA Stream.
  * @retval     The number of remaining data units in the current DMAy Streamx transfer.
  */
uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
  /* Return the number of remaining data units for DMAy Streamx */
  return ((uint16_t)(dma_stream->NDTR));
}


/**
  * @brief      clear idle it flag after uart receive a frame data
  * @param[in]  huart: uart IRQHandler id
  * @retval  
  */
static void uart_rx_idle_callback(UART_HandleTypeDef* huart)
{
	/* clear idle it flag avoid idle interrupt all the time */
	__HAL_UART_CLEAR_IDLEFLAG(huart);

	/* handle received data in idle interrupt */
	if (huart == &DBUS_HUART)
	{
		/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);

		uint32_t DMA_FLAGS = __HAL_DMA_GET_TC_FLAG_INDEX(huart->hdmarx);
    		__HAL_DMA_DISABLE(huart->hdmarx);
		__HAL_DMA_CLEAR_FLAG(huart->hdmarx, DMA_FLAGS);
		
		/* restart dma transmission */
		__HAL_DMA_SET_COUNTER(huart->hdmarx, DBUS_MAX_LEN);
		__HAL_DMA_ENABLE(huart->hdmarx);
	}  
	else if(huart == &VISION_HUART)
  {
				/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);

		uint32_t DMA_FLAGS = __HAL_DMA_GET_TC_FLAG_INDEX(huart->hdmarx);
    		__HAL_DMA_DISABLE(huart->hdmarx);
		__HAL_DMA_CLEAR_FLAG(huart->hdmarx, DMA_FLAGS);
		
		/* restart dma transmission */
		__HAL_DMA_SET_COUNTER(huart->hdmarx, VISION_MAX_LEN);
		__HAL_DMA_ENABLE(huart->hdmarx);
		
  }
	else 
  {
				/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);

		uint32_t DMA_FLAGS = __HAL_DMA_GET_TC_FLAG_INDEX(huart->hdmarx);
    		__HAL_DMA_DISABLE(huart->hdmarx);
		__HAL_DMA_CLEAR_FLAG(huart->hdmarx, DMA_FLAGS);
		
		/* restart dma transmission */
		__HAL_DMA_SET_COUNTER(huart->hdmarx, TOF_BUFFER_SIZE);
		__HAL_DMA_ENABLE(huart->hdmarx);
		
  }
}

/**
  * @brief      callback this function when uart interrupt 
  * @param[in]  huart: uart IRQHandler id
  * @retval  
  */
void uart_receive_handler(UART_HandleTypeDef *huart)
{  
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) && 
			__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
	{
		
		if (huart == &DBUS_HUART)
		{
		  rc_callback_handler(&rc, dbus_buf);
		  uart_rx_idle_callback(huart);
		}
		if (huart == &VISION_HUART)
		{		
			vision_message(vision_buf);		
			uart_rx_idle_callback(huart);
		}
		if (huart == &FOWARD_TOF_HUART|huart == &BACK_TOF_HUART|huart == &LEFT_TOF_HUART|huart == &RIGHT_TOF_HUART)
		{
			tof_data_decoding(huart);
      uart_rx_idle_callback(huart);
		}
		
	}

}

/**
  * @brief   initialize dbus uart device 
  * @param   
  * @retval  
  */
void uart_init(void)
{
	/* open uart idle it */
	__HAL_UART_CLEAR_IDLEFLAG(&DBUS_HUART);
	__HAL_UART_ENABLE_IT(&DBUS_HUART, UART_IT_IDLE);   

	uart_receive_dma_no_it(&DBUS_HUART, dbus_buf, DBUS_MAX_LEN);
	
	__HAL_UART_CLEAR_IDLEFLAG(&VISION_HUART);
	__HAL_UART_ENABLE_IT(&VISION_HUART, UART_IT_IDLE);

	uart_receive_dma_no_it(&VISION_HUART, vision_buf, VISION_MAX_LEN);
	
	__HAL_UART_CLEAR_IDLEFLAG(&FOWARD_TOF_HUART);
	__HAL_UART_ENABLE_IT(&FOWARD_TOF_HUART, UART_IT_IDLE);   

	uart_receive_dma_no_it(&FOWARD_TOF_HUART, forward_tof_rxbuf, TOF_BUFFER_SIZE);
	
	__HAL_UART_CLEAR_IDLEFLAG(&BACK_TOF_HUART);
	__HAL_UART_ENABLE_IT(&BACK_TOF_HUART, UART_IT_IDLE);   

	uart_receive_dma_no_it(&BACK_TOF_HUART, back_tof_rxbuf, TOF_BUFFER_SIZE);
	
	__HAL_UART_CLEAR_IDLEFLAG(&LEFT_TOF_HUART);
	__HAL_UART_ENABLE_IT(&LEFT_TOF_HUART, UART_IT_IDLE);  

	uart_receive_dma_no_it(&LEFT_TOF_HUART, left_tof_rxbuf, TOF_BUFFER_SIZE);
	
	__HAL_UART_CLEAR_IDLEFLAG(&RIGHT_TOF_HUART);
	__HAL_UART_ENABLE_IT(&RIGHT_TOF_HUART, UART_IT_IDLE);  

	uart_receive_dma_no_it(&RIGHT_TOF_HUART, right_tof_rxbuf, TOF_BUFFER_SIZE);
	
	
}

void tof_data_decoding(UART_HandleTypeDef* huart)
{
	if(huart ==&FOWARD_TOF_HUART)
	{
		uint8_t i=0;
	  for(i=0;i<6;i++)
		{
			if(forward_tof_rxbuf[i]=='m')
			{
				if(forward_tof_rxbuf[i+1]=='m')
				{
						if((i>0)&&(forward_tof_rxbuf[i-1]>='0')&&(forward_tof_rxbuf[i-1]<='9'))
							tof.forward_val=forward_tof_rxbuf[i-1]-'0';
						if((i>1)&&(forward_tof_rxbuf[i-2]>='0')&&(forward_tof_rxbuf[i-2]<='9'))
							tof.forward_val+=(forward_tof_rxbuf[i-2]-'0')*10;
						if((i>2)&&(forward_tof_rxbuf[i-3]>='0')&&(forward_tof_rxbuf[i-3]<='9'))
							tof.forward_val+=(forward_tof_rxbuf[i-3]-'0')*100;
						if((i>3)&&(forward_tof_rxbuf[i-4]>='0')&&(forward_tof_rxbuf[i-4]<='9'))
							tof.forward_val+=(forward_tof_rxbuf[i-4]-'0')*1000;
//						GUI_ShowNum(30,16,tof.forward_val,10,8,1);
						break;
				}
			}
		}
	}
	if(huart ==&BACK_TOF_HUART)
	{
		uint8_t i=0;
	  for(i=0;i<6;i++)
		{
			if(back_tof_rxbuf[i]=='m')
			{
				if(back_tof_rxbuf[i+1]=='m')
				{
						if((i>0)&&(back_tof_rxbuf[i-1]>='0')&&(back_tof_rxbuf[i-1]<='9'))
							tof.back_val=back_tof_rxbuf[i-1]-'0';
						if((i>1)&&(back_tof_rxbuf[i-2]>='0')&&(back_tof_rxbuf[i-2]<='9'))
							tof.back_val+=(back_tof_rxbuf[i-2]-'0')*10;
						if((i>2)&&(back_tof_rxbuf[i-3]>='0')&&(back_tof_rxbuf[i-3]<='9'))
							tof.back_val+=(back_tof_rxbuf[i-3]-'0')*100;
						if((i>3)&&(back_tof_rxbuf[i-4]>='0')&&(back_tof_rxbuf[i-4]<='9'))
							tof.back_val+=(back_tof_rxbuf[i-4]-'0')*1000;
						break;
				}
			}
		}
	}
	if(huart ==&LEFT_TOF_HUART)
	{
		uint8_t i=0;
	  for(i=0;i<6;i++)
		{
			if(left_tof_rxbuf[i]=='m')
			{
				if(left_tof_rxbuf[i+1]=='m')
				{
						if((i>0)&&(left_tof_rxbuf[i-1]>='0')&&(left_tof_rxbuf[i-1]<='9'))
							tof.left_val=left_tof_rxbuf[i-1]-'0';
						if((i>1)&&(left_tof_rxbuf[i-2]>='0')&&(left_tof_rxbuf[i-2]<='9'))
							tof.left_val+=(left_tof_rxbuf[i-2]-'0')*10;
						if((i>2)&&(left_tof_rxbuf[i-3]>='0')&&(left_tof_rxbuf[i-3]<='9'))
							tof.left_val+=(left_tof_rxbuf[i-3]-'0')*100;
						if((i>3)&&(left_tof_rxbuf[i-4]>='0')&&(left_tof_rxbuf[i-4]<='9'))
							tof.left_val+=(left_tof_rxbuf[i-4]-'0')*1000;
						break;
				}
			}
		}
	}
	if(huart ==&RIGHT_TOF_HUART)
	{
		uint8_t i=0;
	  for(i=0;i<6;i++)
		{
			if(right_tof_rxbuf[i]=='m')
			{
				if(right_tof_rxbuf[i+1]=='m')
				{
						if((i>0)&&(right_tof_rxbuf[i-1]>='0')&&(right_tof_rxbuf[i-1]<='9'))
							tof.right_val=right_tof_rxbuf[i-1]-'0';
						if((i>1)&&(right_tof_rxbuf[i-2]>='0')&&(right_tof_rxbuf[i-2]<='9'))
							tof.right_val+=(right_tof_rxbuf[i-2]-'0')*10;
						if((i>2)&&(right_tof_rxbuf[i-3]>='0')&&(right_tof_rxbuf[i-3]<='9'))
							tof.right_val+=(right_tof_rxbuf[i-3]-'0')*100;
						if((i>3)&&(right_tof_rxbuf[i-4]>='0')&&(right_tof_rxbuf[i-4]<='9'))
							tof.right_val+=(right_tof_rxbuf[i-4]-'0')*1000;
						break;
				}
			}
		}
	}
	
}



