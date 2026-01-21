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

I2C_HandleTypeDef hi2c1;


/* Private variables ---------------------------------------------------------*/

const uint32_t dwFpAT24xx_delay_ms = (uint32_t)dwHsSW_Delay_ms;

//
int32_t (*I2C_Mem_Write)(uint16_t wDevAddr, uint16_t wMemAddr, uint16_t wMemAddrSize, uint8_t *p_uData, uint16_t wSize, uint32_t dwTimeout) = BSP_AT24_I2C_Write;
int32_t (*I2C_Mem_Read)(uint16_t wDevAddr, uint16_t wMemAddr, uint16_t wMemAddrSize, uint8_t *p_uData, uint16_t wSize, uint32_t dwTimeout) = BSP_AT24_I2C_Read;



/* Private function prototypes -----------------------------------------------*/



/* Private function  ---------------------------------------------------------*/

void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
  HAL_I2C_Init(&hi2c1);
}

void I2C1_Error_Recovery(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 1. I2C 주변장치 비활성화
    HAL_I2C_DeInit(&hi2c1);

    // 2. SCL, SDA 핀을 GPIO 출력(오픈 드레인)으로 재설정
    //    외부 풀업 저항이 없는 경우를 대비해 내부 풀업을 활성화합니다.
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7; // PB6: SCL, PB7: SDA
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 3. SDA 라인이 LOW로 고착된 경우를 대비해 SCL 클럭을 9번 토글
    for(int i = 0; i < 9; i++)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
        HAL_Delay(1);
    }

    // 4. I2C 버스에 STOP 신호 생성하여 슬레이브 상태를 리셋
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
    dwHsSW_Delay_ms(1);

    // 5. I2C 주변장치 재초기화
    MX_I2C1_Init();
}
/**

typedef struct
{
  uint16_t wPageSize;
  uint16_t wDev_Address;
  uint16_t wMemStart_Adrress;
  uint16_t wMemAddress_Size;
  uint16_t wMemMaxSize;
}hsAT24xx_ConfigType;

* @brief :
* @param :
* @retval:
*/



/**
* @brief :
* @param :
* @retval:
*/


int32_t BSP_AT24_I2C_Write(uint16_t wDevAddr, uint16_t wMemAddr, uint16_t wMemAddrSize, uint8_t *p_uData, uint16_t wSize, uint32_t dwTimeout)
{
  HAL_StatusTypeDef tCheck;
  uint32_t dwCnt = 0;
  
  while(dwCnt < 5) // 5번 재시도
  {
    tCheck = HAL_I2C_Mem_Write( &hi2c1, wDevAddr, wMemAddr, wMemAddrSize, p_uData, wSize, dwTimeout);

    if( tCheck == HAL_OK )
    {
      return 0; // 성공
    }
    else if( tCheck == HAL_BUSY)
    {
      // HAL_BUSY 상태는 잠시 기다린 후 재시도.
      HAL_Delay(10);
    }
    else // HAL_ERROR, HAL_TIMEOUT 등 더 심각한 에러
    {
      // I2C 버스 고착(Stuck) 등의 문제일 수 있으므로 버스 복구 절차를 수행.
      dbg_serial("BSP_AT24_I2C_Write_Recovery");
      I2C1_Error_Recovery();
      HAL_Delay(10); // 복구 후 안정화 시간
    }
    dwCnt++;
  }
  return 1; // 5번 시도 후에도 실패
}

/**
* @brief :
* @param :
* @retval:
*/
int32_t BSP_AT24_I2C_Read(uint16_t wDevAddr, uint16_t wMemAddr, uint16_t wMemAddrSize, uint8_t *p_uData, uint16_t wSize, uint32_t dwTimeout)
{
  HAL_StatusTypeDef tCheck;
  uint32_t dwCnt = 0;
  
  while(dwCnt < 5) // 5번 재시도
  {
    tCheck = HAL_I2C_Mem_Read( &hi2c1, wDevAddr, wMemAddr, wMemAddrSize, p_uData, wSize, dwTimeout);

    if( tCheck == HAL_OK )
    {
      return 0; // 성공
    }
    else if( tCheck == HAL_BUSY)
    {
      // HAL_BUSY 상태는 잠시 기다린 후 재시도합니다.
      HAL_Delay(10);
    }
    else // HAL_ERROR, HAL_TIMEOUT 등 더 심각한 에러
    {
      // I2C 버스 고착(Stuck) 등의 문제일 수 있으므로 버스 복구 절차를 수행합니다.
      dbg_serial("BSP_AT24_I2C_Read_Recovery");
      I2C1_Error_Recovery();
      HAL_Delay(10); // 복구 후 안정화 시간
    }
    dwCnt++;
  }
  return 1; // 5번 시도 후에도 실패
}


/* Exported functions ------------------------------------------------------- */
/**
* @brief :
* @param :
* @retval:
*/
void hsAT24xx_Config(hsAT24xx_ConfigType *p_tMemConfig, uint16_t wPageSize, uint16_t wDev_Address, uint16_t wMemStart_Adrress, uint16_t wMemAddress_Size, uint16_t wMemMaxSize)
{
  p_tMemConfig->wPageSize               = wPageSize;
  p_tMemConfig->wDev_Address            = wDev_Address;
  p_tMemConfig->wMemStart_Adrress       = wMemStart_Adrress;
  p_tMemConfig->wMemAddress_Size        = wMemAddress_Size;
  p_tMemConfig->wMemMaxSize             = wMemMaxSize;  
}

/**
* @brief :
* @param :
* @retval:
*/
int32_t hsAT24xx_Write(hsAT24xx_ConfigType *p_tMemConfig, uint8_t *p_uData, uint16_t wStartAddress, uint32_t dwWriteDataSize)
{
  uint32_t dwMaxMeCnt = 0;
  int32_t dwCheck = 0;
  uint16_t wMemAddress = 0;
  uint16_t page_cnt=0;
    
  wMemAddress = p_tMemConfig->wMemStart_Adrress + wStartAddress;
  dwMaxMeCnt = dwWriteDataSize;
  
  if( (wMemAddress + dwWriteDataSize) >= (p_tMemConfig->wMemStart_Adrress + p_tMemConfig->wMemMaxSize) ) return 1;
   
  if(dwWriteDataSize <= p_tMemConfig->wPageSize)
  {
    dwCheck = I2C_Mem_Write(p_tMemConfig->wDev_Address, wMemAddress, p_tMemConfig->wMemAddress_Size, p_uData, dwWriteDataSize, 1000);
    if(dwCheck > 0 ) return 1;

    ((int32_t(*)(uint32_t))dwFpAT24xx_delay_ms)(10);
  }
  else
  {
    while(1)
    {
      if(dwMaxMeCnt >= p_tMemConfig->wPageSize)
      {
        dwCheck = I2C_Mem_Write(p_tMemConfig->wDev_Address, wMemAddress, p_tMemConfig->wMemAddress_Size, p_uData+(p_tMemConfig->wPageSize*page_cnt), p_tMemConfig->wPageSize, 1000);
        if(dwCheck > 0 ) return 1;
        
        ((int32_t(*)(uint32_t))dwFpAT24xx_delay_ms)(10);
        
        wMemAddress = wMemAddress + p_tMemConfig->wPageSize;
        dwMaxMeCnt = dwMaxMeCnt - p_tMemConfig->wPageSize;
        page_cnt++;
      }
      else
      {
        dwCheck = I2C_Mem_Write(p_tMemConfig->wDev_Address, wMemAddress, p_tMemConfig->wMemAddress_Size, p_uData+(p_tMemConfig->wPageSize*page_cnt), dwMaxMeCnt, 1000);
        if(dwCheck > 0 ) return 1;
        
        ((int32_t(*)(uint32_t))dwFpAT24xx_delay_ms)(10);
        
        break;
      }
    }
  }
  
  return dwCheck;
}

/**
* @brief :
* @param :
* @retval:
*/
int32_t hsAT24xx_Read(hsAT24xx_ConfigType *p_tMemConfig, uint8_t *p_uData, uint16_t wStartAddress, uint32_t dwReadDataSize)
{
  int32_t dwCheck = 0;
  uint16_t wMemAddress = 0;
  
  wMemAddress = p_tMemConfig->wMemStart_Adrress + wStartAddress;
  if( (wMemAddress + dwReadDataSize) >= (p_tMemConfig->wMemStart_Adrress + p_tMemConfig->wMemMaxSize) ) return 1;
  
  dwCheck = I2C_Mem_Read(p_tMemConfig->wDev_Address, wMemAddress, p_tMemConfig->wMemAddress_Size, p_uData, dwReadDataSize, 1000);
  
  if(dwCheck > 0) return 1;
  
  return 0;
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
