/**
******************************************************************************
* @file    
* @author  
* @version 
* @date    
* @brief   
******************************************************************************
*
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/



/* Private function  ---------------------------------------------------------*/


/**
* @brief :
* @param :
* @retval:
*/



/**
* @brief :
* @param :
* @retval:
*/




/* Exported functions ------------------------------------------------------- */
/**
* @brief :
* @param :
* @retval:
*/
int32_t dwHsSW_Delay_us(uint32_t dwDelay_us)
{
  uint32_t dwCnt_0;
  
  for(dwCnt_0 = 0; dwCnt_0 < dwDelay_us; dwCnt_0++)
  {
//    for(dwCnt_1 = 0; dwCnt_1 < 2; dwCnt_1++)
//    {
       //sub_root();
      asm("NOP");
     
//   }
    
  }
 
/*  
for(dwCnt_0 = 0; dwCnt_0 < dwDelay_us; dwCnt_0++)
  {
    for(dwCnt_1 = 0; dwCnt_1 < 12; dwCnt_1++)
    {
      asm("NOP");
    }
      main_root();
  }*/
  return 0;
}

/**
* @brief :
* @param :
* @retval:
*/
int32_t dwHsSW_Delay_ms(uint32_t dwDelay_ms)
{
  uint32_t dwCnt_0, dwCnt_1;
  
  for(dwCnt_0 = 0; dwCnt_0 < dwDelay_ms; dwCnt_0++)
  {
    for(dwCnt_1 = 0; dwCnt_1 < 5000; dwCnt_1++)
    {
      asm("NOP");
    }
  }
  
  return 0;
}


/**
* @brief :
* @param :
* @retval:
*/
void vHsSwapUpSort_u32bit(uint32_t *p_Data, uint32_t Len)
{
  int dwCnt1, dwCnt2;
  
  for(dwCnt1 = 0; dwCnt1 < Len ; dwCnt1++)
  {
    for(dwCnt2 = 0; dwCnt2 < Len ; dwCnt2++)
    {
      if(p_Data[dwCnt1] < p_Data[dwCnt2])
      {
        vSwap_u32bit(&p_Data[dwCnt1], &p_Data[dwCnt2]);
      }
      else 
        continue;
    }
  }
  
}
/**
* @brief :
* @param :
* @retval:
*/
void vSwap_u32bit(uint32_t *p_A, uint32_t *p_B)
{
  uint32_t dwSwap;
  
  dwSwap = *p_A;
  *p_A = *p_B;
  *p_B = dwSwap; 
}

/**
* @brief :
* @param :
* @retval:
*/
void vSwap_u8bit(uint8_t *p_A, uint8_t *p_B)
{
   uint8_t dwSwap;
  
  dwSwap = *p_A;
  *p_A = *p_B;
  *p_B = dwSwap; 
}

/**
* @brief :
* @param :
* @retval:
*/

// 2024 03 05 추�??
/**
* @brief :
* @param :
* @retval:
#define UART5_QUE_SZ      50

#define UART5_QUE_OK	    0
#define UART5_QUE_FULL	  1
#define EUART5_QUE_EMPTY	2

*/


hsUART5_Que_t g_tUART5_Que_Handle;

int32_t UART5_ReceivePutQue(uint8_t uPutData)
{
  int32_t dwTail;

  dwTail = (g_tUART5_Que_Handle.dwTail + 1)%UART5_QUE_SZ;

  if( dwTail == g_tUART5_Que_Handle.dwHead) return UART5_QUE_FULL;

  g_tUART5_Que_Handle.uQueBuf[g_tUART5_Que_Handle.dwTail] = uPutData;

  g_tUART5_Que_Handle.dwTail = dwTail;
          
  return UART5_QUE_OK;
}

int32_t UART5_ReceiveGetQue(uint8_t *p_uGetData)
{
  if( g_tUART5_Que_Handle.dwTail == g_tUART5_Que_Handle.dwHead ) return UART5_QUE_EMPTY;

  *p_uGetData = g_tUART5_Que_Handle.uQueBuf[g_tUART5_Que_Handle.dwHead];

  g_tUART5_Que_Handle.dwHead = (g_tUART5_Que_Handle.dwHead + 1)%UART5_QUE_SZ;

  return UART5_QUE_OK;
}

void UART5_QueReset(void)
{
  __disable_irq();
  g_tUART5_Que_Handle.dwHead = 0;
  g_tUART5_Que_Handle.dwTail = 0;
  memset(g_tUART5_Que_Handle.uQueBuf,0,UART5_QUE_SZ);
  __enable_irq();
}

int32_t UART5_QueHaveDataSize(void)
{
  int32_t dwReturnData;
  
  __disable_irq();
  if(g_tUART5_Que_Handle.dwHead <= g_tUART5_Que_Handle.dwTail){
    dwReturnData = g_tUART5_Que_Handle.dwTail - g_tUART5_Que_Handle.dwHead;
  }
  else
  {
    dwReturnData = UART5_QUE_SZ - g_tUART5_Que_Handle.dwHead;
    dwReturnData = dwReturnData + g_tUART5_Que_Handle.dwTail;
  }  
  __enable_irq();

  return dwReturnData;
}


/**
* @brief :
* @param :
* @retval:
*/

/**
* @brief :
* @param :
* @retval:
*/

/**
* @brief :
* @param :
* @retval:
*/




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
