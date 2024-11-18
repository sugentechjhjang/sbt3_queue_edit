/**
  ******************************************************************************
  * @file    
  * @author  전자제어팀 
  * @version V1.0.0
  * @date    08/24/2011
  * @brief   Main program body
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


/* Includes ------------------------------------------------------------------*/
#include "KMAC_HW_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Uart_GPIO_Set(void);
void USART1_Set(void);

/* Private functions ---------------------------------------------------------*/
/* ****************************************************************************
 * Name   : USART1_Init
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void USART1_Init(void)
{
    /* Enable GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_USART, ENABLE);

    /* Enable USART1 clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    /* Periphal 설정 */
    USART1_Set();
    
    /* GPIO 상태 설정 */
    Uart_GPIO_Set();    
}

/* ****************************************************************************
 * Name   : USART1_Init
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void USART1_Set(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USARTx configuration -------------------------------------------------*/
    /* USARTx configured as follow:
     - BaudRate = 115200 baud  
     - Word Length = 8 Bits
     - One Stop Bit
     - No parity
     - Hardware flow control disabled (RTS and CTS signals)
     - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate   = BAURATE_SET;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;
    USART_InitStructure.USART_Parity     = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl
                                         = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;

    /* Configure the USARTx */ 
    USART_Init(USART1, &USART_InitStructure);

    /* Enable the USART1 */
    USART_Cmd(USART1, ENABLE);
}

/*****************************************************************************/
/* ****************************************************************************
 * Name   : fputc // printf 출력 함수 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void Uart_GPIO_Set(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // UART configuration ...

    /* Configure USARTx_Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_USART_Tx_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIO_USART, &GPIO_InitStructure);

    /* Configure USARTx_Rx as input floating */
    GPIO_InitStructure.GPIO_Pin  = GPIO_USART_Rx_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_USART, &GPIO_InitStructure);
}

/*****************************************************************************/

/* ****************************************************************************
 * Name   : fputc // printf 출력 함수 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
#if 0
int fputc(int ch, FILE *f)
{
    /* Write a character to the USART */  
    if( ch == '\n') {
        USART_SendData(USART1, '\r');
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, '\n');
    }else {
        USART_SendData(USART1, (u8) ch);
    }

    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return ch;
}
#endif
/*****************************************************************************/

/* ****************************************************************************
 * Name   : USART_GetCharacter
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
uint8_t USART_GetCharacter(USART_TypeDef * usart_p)
{
  uint8_t data;
  
  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(usart_p, USART_FLAG_RXNE) == RESET);
  
  /* Write a character to the USART */
  data = USART_ReceiveData(usart_p);
#if 1  
  USART_SendData(usart_p, data);
  while(USART_GetFlagStatus(usart_p, USART_FLAG_TXE) == RESET);
  USART_SendData(usart_p, '\r');
  while(USART_GetFlagStatus(usart_p, USART_FLAG_TXE) == RESET);
  USART_SendData(usart_p, '\n');
  while(USART_GetFlagStatus(usart_p, USART_FLAG_TXE) == RESET);
#endif  
  if( data == '\r' )  return (int)('\n');
  else                return(data);
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
