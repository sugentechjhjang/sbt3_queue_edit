/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 05/21/2007
* Description        : The main program
********************************************************************************
* History:
* 05/21/2007: V0.1 
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "common.h"


#include "stm32_KMAC_Base.h"
//#include "UC1608x_Drive.h"

#define BLCOK_SOURCE_OLD                0

#define SysTick_CLKSource_HCLK         ((u32)0x00000004)
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define nOn                   1
#define nOff                  0

#define nLED_MCU              0
#define nLED_USART1           1
#define nLED_USART2           2
#define nLED_USART3           3
#define nLED_USART4           4

#define nMOTOR_STOP           0
#define nMOTOR_CW             1
#define nMOTOR_CCW            2


#define GPIO_DRY_IND_Pin               GPIO_Pin_1//B
#define GPIO_BUZZER_Pin                GPIO_Pin_8 //A
#define GPIO_Blower_FAN_Pin            GPIO_Pin_14//C

#define Fan_B_ON()              GPIO_ResetBits(GPIOC, GPIO_Blower_FAN_Pin)
#define Fan_B_OFF()             GPIO_SetBits(GPIOC, GPIO_Blower_FAN_Pin)

//                             buzzer
#define Buzzer_ON_PIN()         GPIO_SetBits(GPIOA, GPIO_BUZZER_Pin)
#define Buzzer_OFF_PIN()        GPIO_ResetBits(GPIOA, GPIO_BUZZER_Pin)

#define USB5V_OnOff_Pin GPIO_PIN_8
#
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern pFunction Jump_To_Application;
extern u32 JumpAddress;
extern void __MSR_MSP(u32);
/* Private function prototypes -----------------------------------------------*/

#if BLCOK_SOURCE_OLD
//static u8 Push_Button_Read (void);
//static void Push_Button_Config(void);
#endif //BLCOK_SOURCE_OLD



static void IAP_Init(void);

/*YD-Main v2.0 HW Set*/
void vClock_Configuration(void);
void vUART1_Set(void);
void USART3_Init(void);
void GPIO_Configuration(void);
void Allo_S_GPIO_Init(void);

/*YD-Main v2.0 Util*/
void userDelay(void);

void vUserDelay_HS(uint32_t dwDelay);

uint32_t dwIAP_InputMode_Check(void);

void TIMER_Wait_us(volatile unsigned int);
void delay_int_count(volatile unsigned int);
void Delay_ms(volatile unsigned int);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIMER_Wait_us(volatile unsigned int nCount)
{
  for(;nCount != 0;nCount--);
}

void delay_int_count(volatile unsigned int nTime)
{
  for(;nTime > 0;nTime--);  
}

void Delay_ms(volatile unsigned int time)
{
  for(;time > 0;time--)
    delay_int_count(7200);
}

int main(void)
{
//  u16 cnt = 0;

  uint32_t uIAP_Check  = 0;

  
 
#ifdef DEBUG
  debug();
#endif
  
  /* Flash unlock */  
  FLASH_Unlock();
  
#if BLCOK_SOURCE_OLD
  Push_Button_Config();
#endif //BLCOK_SOURCE_OLD
  
  IAP_Init();
  //SerialPutString("\r\nBoot Space Key: ");
  
//  vUserDelay_HS(10000);
  
#if BLCOK_SOURCE_OLD
  
  
  /* Test if PB.09 level is low (Key push-button on Eval Board pressed) */
  SerialPutString("\r\nBoot Space Key: ");

  uatdData = USART1->DR;
  for(cnt = 0; cnt < 5 ; cnt++)
  {
    SerialPutChar('.');
//    Delay_ms(500);
    
    uatdData = USART1->DR;
    if( uatdData == ' ')
    {
      SerialPutString("\r\nEnter Boot\r\n");
      break;
    }
    else if(uatdData > 0)
    {
      break;
    }
    else;
  }
#endif //BLCOK_SOURCE_OLD
  /* ���� ������� IAP Mode�� ������ �ʴ� ��Ȳ�� ������ �� �ش� ����� ���� */  
  Delay_ms(300);
//  SerialPutString("\r\nPassword: ");
//  SerialPutString("\r\nPW ");
  SerialPutString("PW");
  uIAP_Check = dwIAP_InputMode_Check();
  
  if (uIAP_Check == 1)       
  { /* If Key is pressed */
    /* Execute the IAP driver in order to re-program the Flash */
#if 0
    //SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n=              (C) Copyright 2012 KMAC Boot System                   =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n=     In-Application Programming Application  (Version 0.1)          =");
    SerialPutString("\r\n=                                                                    =");
   // SerialPutString("\r\n=                                   By MCD Application Team          =");
    //SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n\r\n");
#endif
    Main_Menu ();
    
  }
  /* Keep the user application running */
  else 
  {
    //SerialPutString("\r\n App Jump OK\n");
    
    /* Test if user code is programmed starting from address "ApplicationAddress" */
    if (((*(vu32*)ApplicationAddress) & 0x2FFF0000 ) == 0x20000000)
    { /* Jump to user application */
       __MSR_MSP(*(int*)ApplicationAddress);
      JumpAddress = *(vu32*) (ApplicationAddress + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      Jump_To_Application();
    }
  }

  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : IAP_Init
* Description    : Initialize the IAP:Configure RCC, USART and GPIOs.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IAP_Init(void)
{
  /* Clock HW Set */
  vClock_Configuration();
   
  /* PORT SET */
  GPIO_Configuration();
//  Allo_S_GPIO_Init();
  /* UART1 HW Set */
  vUART1_Set();
//  USART3_Init();  // ���� �����ָ��� x axis �̻� ���� ��. ������ �м���....
}



/*==============================================================================
  YD-Mainboard HW Set
==============================================================================*/

/*******************************************************************************
* Function Name  : Clock_Configuration
* Description    : IAP�� Ŭ�� �� ���� �ϴ� �Լ� 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void vClock_Configuration(void)
{
  /* Clock configuration -------------------------------------------------------*/
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
  }

  /* Flash 2 wait state and Prefetch enable*/
  FLASH_SetLatency(FLASH_Latency_2);  
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

  /* HCLK = SYSCLK */
  RCC_HCLKConfig(RCC_SYSCLK_Div1); 

  /* PCLK2 = HCLK */
  RCC_PCLK2Config(RCC_HCLK_Div1); 

  /* PCLK1 = HCLK/2 */
  RCC_PCLK1Config(RCC_HCLK_Div2);

  /* PLLCLK = 8MHz * 9 = 72 MHz */
  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

  /* Enable PLL */ 
  RCC_PLLCmd(ENABLE);

  /* Wait till PLL is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {
  }

  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

  /* Wait till PLL is used as system clock source */
  while(RCC_GetSYSCLKSource() != 0x08)
  {
  }
  
}

/*******************************************************************************
* Function Name  : UART1_Set
* Description    : USART1�� GPIO Pin ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void vUART1_Set(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
  /* Enable GPIOA, GPIOC and USART1 clock  */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);


  /* Configure the GPIO ports( USART1 Transmit and Receive Lines) */
  /* Configure the USART1_Tx as Alternate function Push-Pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure the USART1_Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  /* --------------------------------- */
  

  /*-- USART1 configured as follow:------------------------
  - Word Length = 8 Bits
  - 1 Stop Bit
  - No parity
  - BaudRate = 115200 baud
  - Receive and transmit enabled
  -------------------------------------------------------*/

  USART_InitStructure.USART_BaudRate = 57600;  //115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_Clock = USART_Clock_Disable;
  USART_InitStructure.USART_CPOL = USART_CPOL_Low;
  USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
  USART_InitStructure.USART_LastBit = USART_LastBit_Disable;

  USART_Init(USART1, &USART_InitStructure);
  
  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
}

void USART3_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  RCC->APB2ENR |= RCC_APB2Periph_GPIOB;
  RCC->APB1ENR |= RCC_APB1Periph_USART3;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    //tx
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    //tx
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  USART_InitStructure.USART_BaudRate = 38400; //57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART3, &USART_InitStructure);
  
  /* Enable the USART1 */
  USART_Cmd(USART3, ENABLE);

}



// FAN_OnOff(nOff);
/*******************************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
#define GPIO_PW_INDICATE_Pin           GPIO_Pin_5
#define GPIO_ASP_DSP_IND_Pin           GPIO_Pin_6
#define GPIO_INCUBATION_IND_Pin        GPIO_Pin_7
#define GPIO_DRY_IND_Pin               GPIO_Pin_8
#define GPIO_ANALYSIS_IND_Pin          GPIO_Pin_9
#define GPIO_BUZZER_Pin                GPIO_Pin_8
#define GPIO_PTC_Pin                   GPIO_Pin_11
#define GPIO_Blower_FAN_Pin            GPIO_Pin_12
#define GPIO_FAN_L_Pin                 GPIO_Pin_14
#define GPIO_FAN_R_Pin                 GPIO_Pin_15
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC->APB2ENR |= RCC_APB2Periph_GPIOA;
  RCC->APB2ENR |= RCC_APB2Periph_GPIOC;
  RCC->APB2ENR |= RCC_APB2Periph_GPIOD;
  RCC->APB2ENR |= RCC_APB2Periph_GPIOE;  

  
  //INDICATE LED
  GPIO_InitStructure.GPIO_Pin =  GPIO_Blower_FAN_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  Fan_B_OFF() ;
  

  /*GPIO_InitStructure.GPIO_Pin = GPIO_PW_INDICATE_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);*/
  
  //PORT A
  GPIO_InitStructure.GPIO_Pin = GPIO_BUZZER_Pin ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 Buzzer_ON_PIN();
  
//PORT E  
  /*
  GPIO_InitStructure.GPIO_Pin = GPIO_FAN_L_Pin | GPIO_FAN_R_Pin;
  GPIO_InitStructure.GPIO_Speed = GP0IO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  */
}


void Allo_S_GPIO_Init(void)
{



}

/*******************************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


/*==============================================================================
  YD-Mainboard IAP Util Code
==============================================================================*/

/*******************************************************************************
* Function Name  : userDelay
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void vUserDelay_HS(uint32_t dwDelay)
{
  uint32_t dwCnt = 0;
  for(dwCnt = 0; dwCnt < dwDelay; dwCnt++)
  {
    asm("NOP");
  }
}

/*******************************************************************************
* Function Name  : dwIAP_InputMode_Check
* Description    : None
* Input          : None
* Output         : None
* Return         : dwReturnData = 1: IAP ����, 0 �̸� APP ���� 
*******************************************************************************/
uint32_t dwIAP_InputMode_Check(void)
{
  uint8_t uGetKey[10] ={0};
  uint32_t dwGetCnt=0, dwCnt=0,timeCnt=0;
  uint32_t dwReturnData = 0;
//  const uint8_t uCkData[9] = { '2', '0', '1', '2', '0', '2', '5', '7', '8'};
  const uint8_t uCkData[9] = { 'K', 'M', 'A', 'C', '1', '7', '1', '7', '0'};
  
  for(dwGetCnt = 0; dwGetCnt < 10 ; dwGetCnt++)
  {
    uGetKey[0] = (uint8_t)(USART1->DR);
    vUserDelay_HS(30000);
  }
  
  dwGetCnt = 0;
  
//  vBuzzer_OnOff(nOn);
//  vUserDelay_HS(1000000);
  Delay_ms(500);
//  vBuzzer_OnOff(nOff);
  
  while(1)
  {
    if(SerialKeyPressed(&uGetKey[dwGetCnt]))
    {
      dwGetCnt++;
      if(dwGetCnt == 9 ) break; //'K', 'M', 'A', 'C', '1', '7', '1', '7', '0'// '2', '0', '1', '2', '0', '2', '5', '7', '8'  --> 9�� �� Key 
    }
    else
    {
      timeCnt++;
  //   if(dwCnt > 6000000 )break; // 0xCDFE600 == 216000000 --> 4-5��
      if(timeCnt > 1000000 )break; // 0xCDFE600 == 216000000 --> 4-5��
    }
  }
  
  if(dwGetCnt == 9)
  {
    for(dwCnt = 0; dwCnt < 9; dwCnt++)
    {
      if(uGetKey[dwCnt] != uCkData[dwCnt] ) break;
    }
    
    if(dwCnt == 9) dwReturnData = 1;
    else dwReturnData = 0;
  }
  else
  {
    dwReturnData = 0;
  }
  
  return dwReturnData;
}

/*******************************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*******************************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*******************************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while(1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
