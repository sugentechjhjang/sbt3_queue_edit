/**
  ******************************************************************************
  * @file    
  * @author  ÀüÀÚÁ¦¾î ÆÀ
  * @version V1.0.0
  * @date    08/24/2011
  * @brief   
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
#ifndef __KMAC_HW_CONFIG_H
#define __KMAC_HW_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"
#include "stm32f10x_lib.h"
#include "stdio.h"

#include "stm32_KMAC_Base.h"
   
/* defines */
#define RCC_APB2Periph_GPIO_USART           RCC_APB2Periph_GPIOA

#define GPIO_USART                          GPIOA

#define GPIO_USART_Rx_Pin                   GPIO_Pin_10
#define GPIO_USART_Tx_Pin                   GPIO_Pin_9

#define BAURATE_SET                         115200
   
/* Macro */


/* functions */  
void USART1_Init(void);
uint8_t USART_GetCharacter(USART_TypeDef * usart_p);

#endif /*__KMAC_HW_CONFIG_H*/
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
