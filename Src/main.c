/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
//ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
//ADC_HandleTypeDef hadc3;

//I2C_HandleTypeDef hi2c1;

//SPI_HandleTypeDef hspi1;


//UART_HandleTypeDef huart4;
//UART_HandleTypeDef huart5;
//UART_HandleTypeDef huart1;
//UART_HandleTypeDef huart2;
//UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);

static void MX_ADC2_Init(void);
//static void MX_ADC3_Init(void);
//static void MX_I2C1_Init(void);
//static void MX_SPI1_Init(void);
//static void MX_TIM4_Init(void);
//static void MX_UART4_Init(void);
//static void MX_USART5_UART_Init(void);
//static void MX_USART1_UART_Init(void);
//static void MX_USART2_UART_Init(void);
//static void MX_USART3_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
   uint16_t tx_state = 0;
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
  state=stBoot;
  beep_en=1;
  /* Initialize all configured peripherals */
  MX_DMA_Init();

  MX_GPIO_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  
  MX_TIM2_Init();
  MX_TIM4_Init();
  
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART4_UART_Init();
  MX_USART5_UART_Init();

  state_led_init();
  HAL_GPIO_WritePin(FAN_R_GPIO_Port,FAN_R_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(FAN_L_GPIO_Port,FAN_L_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(USB5V_OnOff_GPIO_Port,USB5V_OnOff_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(ACC_VALVE_PROBE_GPIO_Port,ACC_VALVE_PROBE_Pin,GPIO_PIN_SET);
  
  // state=stPrepare;
  ASP_PUMP_OFF;
  PRIME_DW_OFF;
  BATH_DW_OFF;
  HAL_Delay(500);
  #ifdef Motor_LEAD 
    stmt_servo_on(); 
  #endif  
  eeprom_init();
  shaker_Init();
  HAL_Delay(500);
  beep(80, 1);
  cam_led_on();
  set_timer_(eventSpuOn,2000,0);

   

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */
    event event=get_event();
  
    if(state!=stStopPrepare)
    event=execute_main_ctrl(event);
    event=execute_stepping_ctrl(event);
    event=execute_barcode_ctrl(event);
    event=execute_shake_ctrl(event);
    event=dev_cfg_ctrl(event);
    event=execute_servo_ctrl(event);
    event=execute_pump_ctrl(event);
    event=execute_syringe_ctrl(event);
    event=execute_sys_ctrl(event);
    event=execute_hlld_ctrl(event);
    event=execute_cam_ctrl(event);
    event=sq_ctrl(event);  

    tm_execute();






 /////////////////LLD_FW_DOWNLOAD/////////////////////////////////////   
    
    while(SUB_FW_DOWN_MODE)
    {
      if(download_start_flag == true)
      {
        dSPIN_Hard_Stop();
        hlld_send_pack(HLLD_ADD, HLLD_SYSTEM_RESET,0, 0);
        HAL_Delay(500);
        send_pw_message("PW");
        download_start_flag = false;
      }  

      if (PCtoSub) 
      {
          if((echoBuf[PACKET_CMD] == FIND_ADDRESS_CMD) || (echoBuf[PACKET_CMD] == END_CMD) )
          {
            HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, GPIO_PIN_SET);   
            tx_state = HAL_UART_Transmit(&huart5, echoBuf, expectedTotalLength, 1000);
            
            if(tx_state) 
            {
              UART5_ReInit();
            }

            HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, GPIO_PIN_RESET);
          }
          
          if((echoBuf[PACKET_CMD] == BODY_CMD))
          { 
            HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, GPIO_PIN_SET); 
            tx_state = HAL_UART_Transmit(&huart5, echoBuf, sizeof(echoBuf), 1000);

            if(tx_state) 
            {
              UART5_ReInit();
            }

            HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, GPIO_PIN_RESET);
          }
          memset(echoBuf,0,sizeof(echoBuf));
          PCtoSub = false;
      }
      
      if(SubToPC)
      {
        send_ack_buf_as_message(ACKBuf, ACK_BUF_SIZE);
        
        if(ACKBuf[PACKET_CMD] == END_CMD)
        {
          NVIC_SystemReset();
        }
        
        memset(ACKBuf,0,sizeof(ACKBuf));
        SubToPC = false;
      }
    }  
  }
}
//////////////////////////////////////////////////////////////////////



/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void MX_DMA_Init(void)
{

  /* DMA controller clock enable */

  __HAL_RCC_DMA1_CLK_ENABLE();
  /* DMA1_Channel4_IRQn interrupt configuration */

  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

/* ADC2 init function */
void MX_ADC2_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc2);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);

}

/* ADC3 init function */
/*
void MX_ADC3_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

 
  hadc3.Instance = ADC3;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc3);

  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADC_ConfigChannel(&hadc3, &sConfig);

}
*/
/* I2C1 init function */
/*
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
*/
/* SPI1 init function */
/*
void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  hspi1.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi1);

}
*/



/* UART5 init function */
/*
void MX_USART5_UART_Init(void)
{

  huart5.Instance = UART5;
  huart5.Init.BaudRate = 38400;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart5);

}
*/



/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
 
void MX_GPIO_Init(void)
{

 GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOE_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOF_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOG_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : CW_CCW_M_Pin RESET_M_Pin ENABLE_M_Pin STBY_M_Pin 
                           M1_M_Pin M2_M_Pin DCY_M_Pin TQ_M_Pin 
                           M_Power_Pin M_ASP_DSP_Pin M_INCUBATION_Pin M_DRY_Pin 
                           M_ANALYSIS_Pin M_MCU_LED_Pin CK_M_Pin */
  GPIO_InitStruct.Pin = CW_CCW_M_Pin|RESET_M_Pin|ENABLE_M_Pin|STBY_M_Pin 
                          |M1_M_Pin|M2_M_Pin|DCY_M_Pin|TQ_M_Pin 
                          |M_Power_Pin|M_ASP_DSP_Pin|M_INCUBATION_Pin|M_DRY_Pin 
                          |M_ANALYSIS_Pin|M_MCU_LED_Pin|CK_M_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : HOME1_Pin HOME2_Pin HOME3_Pin X_OUT2_Pin 
                           X_OUT1_Pin X_OUT0_Pin Y_LIMIT_R_Pin Y_LIMIT_L_Pin 
                           Y_OUT_Pin Z_OUT_Pin */
  GPIO_InitStruct.Pin = HOME1_Pin|HOME2_Pin|HOME3_Pin|X_OUT2_Pin 
                          |X_OUT1_Pin|X_OUT0_Pin|Y_LIMIT_R_Pin|Y_LIMIT_L_Pin 
                          |Y_OUT_Pin|Z_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : L6470_SW_Pin L6470_STCK_Pin L6470_STB_RST_Pin L6470_CS_Pin 
                           ADG733_A0_Pin ADG733_A1_Pin ADG733_A2_Pin ADG733_EN_Pin 
                           ADG1607_EN_Pin ADG1607_A0_Pin ADG1607_A1_Pin ADG1607_A2_Pin */
  GPIO_InitStruct.Pin = L6470_SW_Pin|L6470_STCK_Pin|L6470_STB_RST_Pin|L6470_CS_Pin 
                          |ADG733_A0_Pin|ADG733_A1_Pin|ADG733_A2_Pin|ADG733_EN_Pin 
                          |ADG1607_EN_Pin|ADG1607_A0_Pin|ADG1607_A1_Pin|ADG1607_A2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : L6470_FLAG_Pin L6470_BUSY_SYNC_Pin LEVEL_1_Pin LEVEL_2_Pin */
  GPIO_InitStruct.Pin = L6470_FLAG_Pin|L6470_BUSY_SYNC_Pin|LEVEL_1_Pin|LEVEL_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : X_IN0_Pin X_IN1_Pin X_IN2_Pin */
  GPIO_InitStruct.Pin = X_IN0_Pin|X_IN1_Pin|X_IN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : M_Buzzer_Pin M_FLASH_CSn_Pin USB5V_OnOff_Pin LED_CS_Pin 
                           LED_CL_Pin */
  GPIO_InitStruct.Pin = M_Buzzer_Pin|M_FLASH_CSn_Pin|USB5V_OnOff_Pin|LED_CS_Pin 
                          |LED_CL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_PDL_Pin ACC_VALVE_PROBE_Pin ACC_PUMP_WASTE_Pin ACC_PUMP_PRIME_Pin 
                           ACC_PUMP_VACCUM_Pin ACC_VALVE_WASTE_Pin ACC_VALVE_ASP_Pin */
  GPIO_InitStruct.Pin = LED_PDL_Pin|ACC_VALVE_PROBE_Pin|ACC_PUMP_WASTE_Pin|ACC_PUMP_PRIME_Pin 
                          |ACC_PUMP_VACCUM_Pin|ACC_VALVE_WASTE_Pin|ACC_VALVE_ASP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_UPO_Pin ACC_READY_Pin ACC_LEVEL_Pin BD_ADDR1_Pin */
  GPIO_InitStruct.Pin = LED_UPO_Pin|ACC_READY_Pin|ACC_LEVEL_Pin|BD_ADDR1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : FAN_L_Pin FAN_R_Pin FAN_EX1_Pin FAN_EX2_Pin 
                           FAN_EX3_Pin FAN_EX4_Pin BAR_WAKE_Pin BAR_TRIG_Pin */
  GPIO_InitStruct.Pin = FAN_L_Pin|FAN_R_Pin|FAN_EX1_Pin|FAN_EX2_Pin 
                          |FAN_EX3_Pin|FAN_EX4_Pin|BAR_WAKE_Pin|BAR_TRIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(BAR_WAKE_GPIO_Port, BAR_WAKE_Pin, GPIO_PIN_SET);
  /*Configure GPIO pins : ASP_HOME_S_Pin SHAKE_HOME_S_Pin BD_ADDR0_Pin HOME4_Pin 
                           HOME5_Pin */
  GPIO_InitStruct.Pin = ASP_HOME_S_Pin|SHAKE_HOME_S_Pin|BD_ADDR0_Pin|HOME4_Pin 
                          |HOME5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : FAN_ARR_Pin AC_POWER_Pin UART4_DIR_Pin UART5_DIR_Pin 
                           UART3_DIR_Pin */
  GPIO_InitStruct.Pin = FAN_ARR_Pin|AC_POWER_Pin|UART4_DIR_Pin|UART5_DIR_Pin 
                          |UART3_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : DOOR_1_Pin DOOR_2_Pin DOOR_3_Pin DOOR_4_Pin 
                           BAR_PWR_Pin BAR_DECODE_Pin LEVEL_3_Pin LEVEL_4_Pin */
  GPIO_InitStruct.Pin = DOOR_1_Pin|DOOR_2_Pin|DOOR_3_Pin|DOOR_4_Pin 
                          |BAR_PWR_Pin|BAR_DECODE_Pin|LEVEL_3_Pin|LEVEL_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : nMO_M_Pin */
  GPIO_InitStruct.Pin = nMO_M_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(nMO_M_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
