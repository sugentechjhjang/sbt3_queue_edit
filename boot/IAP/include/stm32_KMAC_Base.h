/**
  ******************************************************************************
  * @file    stm32_KMAC_Base.h
  * @author  전자제어 팀
  * @version V1.0.0
  * @date    08/24/2011
  * @brief   main함수 및 stm32_Device.c에서 사용 할 해더 파일 
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_KMAC_BASE_H
#define __STM32_KMAC_BASE_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
   
/*Std Lib include*/
   
/* defines */
#define RAM_BASE_K        0x20000000
#define RAM_BB_BASE_K     0x22000000

#define VECT_OFFSET       0x0000

  
#define nBIT0            0 //1
#define nBIT1            1 //2
#define nBIT2            2 //3
#define nBIT3            3 //4
#define nBIT4            4 //5
#define nBIT5            5 //6
#define nBIT6            6
#define nBIT7            7
#define nBIT8            8
#define nBIT9            9
#define nBIT10           10
#define nBIT11           11
#define nBIT12           12
#define nBIT13           13
#define nBIT14           14
#define nBIT15           15
   
#define uint8_t   unsigned char   
#define uint16_t  unsigned short  
#define uint32_t  unsigned int 

#define int8_t  char    
#define int16_t short
#define int32_t int    
   
/* Macro */
#define Var_ResetBit_BB(VarAddr, BitNumber)                 (*(__IO uint32_t *)(RAM_BB_BASE_K|((VarAddr - RAM_BASE_K)<<5)|((BitNumber)<<2)))=0

#define Var_SetBit_BB(VarAddr, BitNumber)                   (*(__IO uint32_t *)(RAM_BB_BASE_K|((VarAddr - RAM_BASE_K)<<5)|((BitNumber)<<2))) = 1 

#define Var_GetBit_BB(VarAddr, BitNumber)                   (*(__IO uint32_t *)(RAM_BB_BASE_K|((VarAddr - RAM_BASE_K)<<5)|((BitNumber)<<2)))

#define PERIPH_GetBit_BB(PERIPH_Addr, BitNumber)            (*(__IO uint32_t *)(PERIPH_BB_BASE|((PERIPH_Addr - PERIPH_BASE)<<5)|((BitNumber)<<2)))

#define PERIPH_ResetBit_BB(PERIPH_Addr, BitNumber)          PERIPH_GetBit_BB(PERIPH_Addr, BitNumber)=0

#define PERIPH_SetBit_BB(PERIPH_Addr, BitNumber)            PERIPH_GetBit_BB(PERIPH_Addr, BitNumber)=1

#define SYS_DELAY(x)                                         for(uint32_t tempCnt = 0; tempCnt < x ; tempCnt++);

   
#endif /* __STM32_KMAC_BASE_H*/
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
