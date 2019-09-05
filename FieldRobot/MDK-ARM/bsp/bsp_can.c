/**
	* @file bap_can.c
	* @version 1.1
	* @date Oct,19th 2018
  *
  * @brief  底层can通讯的配置，包括发送数据帧，CAN滤波组，接受中断回调函数
  *
  *	@author lin kr
  *
  */
 
#include "bsp_can.h"
#include "pid.h"
#include "imu_task.h"
#include "string.h"
moto_measure_t moto_chassis[4] = {0};
moto_measure_t moto_manipulator_2006;
moto_measure_t moto_manipulator_6020;
CAN_RecvMsg can_tx_data;
HAL_StatusTypeDef HAL_tx_Status;
CAN_TxHeaderTypeDef Tx1Message;
CAN_RxHeaderTypeDef Rx1Message;
CAN_TxHeaderTypeDef Tx2Message;
CAN_RxHeaderTypeDef Rx2Message;
uint8_t RxData[8];
uint8_t CAN2_Rx_Data[8];



/**
  * @brief     CAN接受中断回调函数
  * @param     Rx1Data ：CAN节点反馈的数据帧
  * @attention 
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan == &hcan1)
	{
		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&Rx1Message, RxData);
	  switch (Rx1Message.StdId)
	  {
			case CAN_3508_M1_ID:
	    case CAN_3508_M2_ID:
	    case CAN_3508_M3_ID:
	    case CAN_3508_M4_ID:
	    {
 	     static uint8_t i;
  	    i = Rx1Message.StdId - CAN_3508_M1_ID;
				moto_chassis[i].msg_cnt++ <= 50 ? get_moto_offset(&moto_chassis[i], &hcan1) : encoder_data_handler(&moto_chassis[i], &hcan1);
				encoder_data_handler(&moto_chassis[i], &hcan1);
  	  }
  	  break;
			case CAN_2006_ID:
   		{
				moto_manipulator_2006.msg_cnt++ <= 50 ? get_moto_offset(&moto_manipulator_2006, &hcan1) : encoder_data_handler(&moto_manipulator_2006, &hcan1);			
  	  }
			break;
			case CAN_6020_ID:
			{
				encoder_data_handler(&moto_manipulator_6020, &hcan1);
			}
			break;
			default:
      {
      }
      break;
	  };
		
	}
  else if(hcan==&hcan2)	
	{
		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&Rx2Message, CAN2_Rx_Data);
		switch (Rx2Message.StdId)
		{
			case IMU_QUAT_ID:
		  case IMU_GYRO_ID:
		  case IMU_ACCEL_ID:
		  case IMU_MAG_ID:
		  case IMU_PARAM_ID:
		  {
					 
					if( rm_imu_data.data_ready_flag == DATA_NOT_READY ) rm_imu_data.data_ready_flag = DATA_READY;				  
			    memcpy(&imu_can_data,&CAN2_Rx_Data[0],8);
			    imu_datahandler(Rx2Message.DLC,Rx2Message.StdId);
					 
		  }
      break;	
			default:
			{
				break;
			}
		}	
	}
	  __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	  __HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/**
  * @brief     get motor initialize offset value
  * @param     ptr: Pointer to a moto_measure_t structure
  * @retval    None
  * @attention this function should be called after system can init
  */
void get_moto_offset(moto_measure_t* ptr, CAN_HandleTypeDef* hcan)
{
  ptr->ecd        = (uint16_t)(RxData[0] << 8 | RxData[1]);
  ptr->offset_ecd = ptr->ecd;
}

/**
  * @brief     get motor rpm and calculate motor round_count/total_encoder/total_angle
  * @param     ptr: Pointer to a moto_measure_t structure
  * @attention this function should be called after get_moto_offset() function
  */
void encoder_data_handler(moto_measure_t* ptr, CAN_HandleTypeDef* hcan)
{
	//机械角度
  ptr->last_ecd = ptr->ecd;
  ptr->ecd      = (uint16_t)(RxData[0] << 8 | RxData[1]);
  //转子转速
	ptr->speed_rpm     = (int16_t)(RxData[2] << 8 | RxData[3]);
  ptr->given_current = (int16_t)(RxData[4] << 8 | RxData[5]);
  //相对开机后的角度
  if (ptr->ecd - ptr->last_ecd > 4096)
  {
    ptr->round_cnt--;
    ptr->ecd_raw_rate = ptr->ecd - ptr->last_ecd - 8192;
  }
  else if (ptr->ecd - ptr->last_ecd < -4096)
  {
    ptr->round_cnt++;
    ptr->ecd_raw_rate = ptr->ecd - ptr->last_ecd + 8192;
  }
  else
  {
    ptr->ecd_raw_rate = ptr->ecd - ptr->last_ecd;
  }
  ptr->total_angle = ptr->round_cnt * 8192 + ptr->ecd - ptr->offset_ecd;
  
}

/**
  * @brief   can filter initialization
  * @param   CAN_HandleTypeDef
  * @retval  None
  */
void my_can_filter_init_recv_all(void)
{
  //can1 filter config
  CAN_FilterTypeDef  can_filter;

	can_filter.FilterBank           = 0;
  can_filter.FilterMode           = CAN_FILTERMODE_IDMASK;
  can_filter.FilterScale          = CAN_FILTERSCALE_32BIT;
  can_filter.FilterIdHigh         = 0x0000;
  can_filter.FilterIdLow          = 0x0000;
  can_filter.FilterMaskIdHigh     = 0x0000;
  can_filter.FilterMaskIdLow      = 0x0000;
  can_filter.FilterFIFOAssignment = CAN_FilterFIFO0;
	can_filter.SlaveStartFilterBank = 0;  
  can_filter.FilterActivation     = ENABLE;

  HAL_CAN_ConfigFilter(&hcan1, &can_filter);
  while (HAL_CAN_ConfigFilter(&hcan1, &can_filter) != HAL_OK);
	
	can_filter.FilterBank           = 14;
  HAL_CAN_ConfigFilter(&hcan1, &can_filter);  
	while (HAL_CAN_ConfigFilter(&hcan1, &can_filter) != HAL_OK);
}

/**
  * @brief  send calculated current to motor
  * @param  3508 motor current
  */
void send_chassis_cur(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)
{
	uint8_t FreeTxNum = 0;  
	
	Tx1Message.StdId = 0x200;
	Tx1Message.IDE 	 = CAN_ID_STD;
	Tx1Message.RTR   = CAN_RTR_DATA;
  Tx1Message.DLC   = 0x08;
	
	can_tx_data.Tx1Data[0] = iq1 >> 8;
	can_tx_data.Tx1Data[1] = iq1;
	can_tx_data.Tx1Data[2] = iq2 >> 8;
	can_tx_data.Tx1Data[3] = iq2;
	can_tx_data.Tx1Data[4] = iq3 >> 8;
	can_tx_data.Tx1Data[5] = iq3;
	can_tx_data.Tx1Data[6] = iq4 >> 8;
	can_tx_data.Tx1Data[7] = iq4;
	
	//查询发送邮箱是否为空
	FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  
	while(FreeTxNum == 0) 
	{  
    FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  
  }
	
	HAL_CAN_AddTxMessage(&CHASSIS_CAN, &Tx1Message,can_tx_data.Tx1Data,(uint32_t*)CAN_TX_MAILBOX0);
}
/**
  * @brief  send calculated current to motor
  * @param   motor current
  */
void send_manipulator_cur(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)
{
	uint8_t FreeTxNum = 0;  
	
	Tx1Message.StdId = 0x1FF;
	Tx1Message.IDE 	 = CAN_ID_STD;
	Tx1Message.RTR   = CAN_RTR_DATA;
  Tx1Message.DLC   = 0x08;
	
	can_tx_data.Tx1Data[0] = iq1 >> 8;
	can_tx_data.Tx1Data[1] = iq1;
	can_tx_data.Tx1Data[2] = iq2 >> 8;
	can_tx_data.Tx1Data[3] = iq2;
	can_tx_data.Tx1Data[4] = iq3 >> 8;
	can_tx_data.Tx1Data[5] = iq3;
	can_tx_data.Tx1Data[6] = iq4 >> 8;
	can_tx_data.Tx1Data[7] = iq4;
	
	//查询发送邮箱是否为空
	FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  
	while(FreeTxNum == 0) 
	{  
    FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  
  }
	
	HAL_CAN_AddTxMessage(&hcan1, &Tx1Message,can_tx_data.Tx1Data,(uint32_t*)CAN_TX_MAILBOX0);
}

/**
  * @brief  init the can transmit and receive
  * @param  None
  */
void can_device_init(void)
{
  my_can_filter_init_recv_all();
	HAL_Delay(100);
	HAL_CAN_Start(&hcan1);
 	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Start(&hcan2);
 	HAL_CAN_ActivateNotification(&hcan2,CAN_IT_RX_FIFO0_MSG_PENDING);
}
