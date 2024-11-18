/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  *
  ******************************************************************************
  */  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HS_COMMON_UTILL_H
#define __HS_COMMON_UTILL_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#define UART5_QUE_SZ      50

#define UART5_QUE_OK	    0
#define UART5_QUE_FULL	  1
#define UART5_QUE_EMPTY	2



   
int32_t dwHsSW_Delay_us(uint32_t dwDelay_us);
int32_t dwHsSW_Delay_ms(uint32_t dwDelay_ms);

void vSwap_u32bit(uint32_t *p_A, uint32_t *p_B);
void vSwap_u8bit(uint8_t *p_A, uint8_t *p_B);
void vHsSwapUpSort_u32bit(uint32_t *p_Data, uint32_t Len);

int32_t UART5_ReceivePutQue(uint8_t uPutData);
int32_t UART5_ReceiveGetQue(uint8_t *p_uGetData);
void UART5_QueReset(void);
int32_t UART5_QueHaveDataSize(void);

extern int32_t hsErr_PutQue(uint8_t);
extern int32_t hsErr_GetQue(uint8_t *);

typedef struct
{
  uint32_t dwTail;
  uint32_t dwHead;
  uint8_t uQueBuf[UART5_QUE_SZ];
}hsUART5_Que_t;

extern hsUART5_Que_t g_tUART5_Que_Handle;

#ifdef __cplusplus
}
#endif


#endif /* __HS_COMMON_UTILL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
