/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
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

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define CW_CCW_M_Pin GPIO_PIN_2
#define CW_CCW_M_GPIO_Port GPIOE
#define RESET_M_Pin GPIO_PIN_3
#define RESET_M_GPIO_Port GPIOE
#define ENABLE_M_Pin GPIO_PIN_4
#define ENABLE_M_GPIO_Port GPIOE
#define STBY_M_Pin GPIO_PIN_5
#define STBY_M_GPIO_Port GPIOE
#define M1_M_Pin GPIO_PIN_6
#define M1_M_GPIO_Port GPIOE
#define HOME1_Pin GPIO_PIN_13
#define HOME1_GPIO_Port GPIOC
#define HOME2_Pin GPIO_PIN_14
#define HOME2_GPIO_Port GPIOC
#define HOME3_Pin GPIO_PIN_15
#define HOME3_GPIO_Port GPIOC
#define L6470_SW_Pin GPIO_PIN_0
#define L6470_SW_GPIO_Port GPIOF
#define L6470_STCK_Pin GPIO_PIN_1
#define L6470_STCK_GPIO_Port GPIOF
#define L6470_STB_RST_Pin GPIO_PIN_2
#define L6470_STB_RST_GPIO_Port GPIOF
#define L6470_FLAG_Pin GPIO_PIN_3
#define L6470_FLAG_GPIO_Port GPIOF
#define L6470_BUSY_SYNC_Pin GPIO_PIN_4
#define L6470_BUSY_SYNC_GPIO_Port GPIOF
#define L6470_CS_Pin GPIO_PIN_5
#define L6470_CS_GPIO_Port GPIOF
#define ADG733_A0_Pin GPIO_PIN_6
#define ADG733_A0_GPIO_Port GPIOF
#define ADG733_A1_Pin GPIO_PIN_7
#define ADG733_A1_GPIO_Port GPIOF
#define ADG733_A2_Pin GPIO_PIN_8
#define ADG733_A2_GPIO_Port GPIOF
#define ADG733_EN_Pin GPIO_PIN_9
#define ADG733_EN_GPIO_Port GPIOF
#define ADG1607_EN_Pin GPIO_PIN_10
#define ADG1607_EN_GPIO_Port GPIOF
#define X_IN0_Pin GPIO_PIN_0
#define X_IN0_GPIO_Port GPIOC
#define X_IN1_Pin GPIO_PIN_1
#define X_IN1_GPIO_Port GPIOC
#define X_IN2_Pin GPIO_PIN_2
#define X_IN2_GPIO_Port GPIOC
#define X_OUT2_Pin GPIO_PIN_3
#define X_OUT2_GPIO_Port GPIOC
#define SERVO_ADC_Pin GPIO_PIN_0
#define SERVO_ADC_GPIO_Port GPIOA
#define LOAD_CELL_Pin GPIO_PIN_1
#define LOAD_CELL_GPIO_Port GPIOA
#define Thermistor_Pin GPIO_PIN_2
#define Thermistor_GPIO_Port GPIOA
#define M_Buzzer_Pin GPIO_PIN_3
#define M_Buzzer_GPIO_Port GPIOA
#define M_FLASH_CSn_Pin GPIO_PIN_4
#define M_FLASH_CSn_GPIO_Port GPIOA
#define X_OUT1_Pin GPIO_PIN_4
#define X_OUT1_GPIO_Port GPIOC
#define X_OUT0_Pin GPIO_PIN_5
#define X_OUT0_GPIO_Port GPIOC
#define LED_PDL_Pin GPIO_PIN_0
#define LED_PDL_GPIO_Port GPIOB
#define LED_UPO_Pin GPIO_PIN_1
#define LED_UPO_GPIO_Port GPIOB
#define ADG1607_A0_Pin GPIO_PIN_11
#define ADG1607_A0_GPIO_Port GPIOF
#define ADG1607_A1_Pin GPIO_PIN_12
#define ADG1607_A1_GPIO_Port GPIOF
#define ADG1607_A2_Pin GPIO_PIN_13
#define ADG1607_A2_GPIO_Port GPIOF
#define LEVEL_1_Pin GPIO_PIN_14
#define LEVEL_1_GPIO_Port GPIOF
#define LEVEL_2_Pin GPIO_PIN_15
#define LEVEL_2_GPIO_Port GPIOF
#define FAN_L_Pin GPIO_PIN_0
#define FAN_L_GPIO_Port GPIOG
#define FAN_R_Pin GPIO_PIN_1
#define FAN_R_GPIO_Port GPIOG
#define M2_M_Pin GPIO_PIN_7
#define M2_M_GPIO_Port GPIOE
#define DCY_M_Pin GPIO_PIN_8
#define DCY_M_GPIO_Port GPIOE
#define TQ_M_Pin GPIO_PIN_9
#define TQ_M_GPIO_Port GPIOE
#define M_Power_Pin GPIO_PIN_10
#define M_Power_GPIO_Port GPIOE
#define M_ASP_DSP_Pin GPIO_PIN_11
#define M_ASP_DSP_GPIO_Port GPIOE
#define M_INCUBATION_Pin GPIO_PIN_12
#define M_INCUBATION_GPIO_Port GPIOE
#define M_DRY_Pin GPIO_PIN_13
#define M_DRY_GPIO_Port GPIOE
#define M_ANALYSIS_Pin GPIO_PIN_14
#define M_ANALYSIS_GPIO_Port GPIOE
#define M_MCU_LED_Pin GPIO_PIN_15
#define M_MCU_LED_GPIO_Port GPIOE
#define ACC_VALVE_PROBE_Pin GPIO_PIN_10
#define ACC_VALVE_PROBE_GPIO_Port GPIOB
#define ACC_PUMP_WASTE_Pin GPIO_PIN_11
#define ACC_PUMP_WASTE_GPIO_Port GPIOB
#define ACC_PUMP_PRIME_Pin GPIO_PIN_12
#define ACC_PUMP_PRIME_GPIO_Port GPIOB
#define ACC_PUMP_VACCUM_Pin GPIO_PIN_13
#define ACC_PUMP_VACCUM_GPIO_Port GPIOB
#define ACC_READY_Pin GPIO_PIN_14
#define ACC_READY_GPIO_Port GPIOB
#define ACC_LEVEL_Pin GPIO_PIN_15
#define ACC_LEVEL_GPIO_Port GPIOB
#define ASP_HOME_S_Pin GPIO_PIN_10
#define ASP_HOME_S_GPIO_Port GPIOD
#define SHAKE_HOME_S_Pin GPIO_PIN_11
#define SHAKE_HOME_S_GPIO_Port GPIOD
#define SERVO_PWM_Pin GPIO_PIN_12
#define SERVO_PWM_GPIO_Port GPIOD
#define FAN_ARR_Pin GPIO_PIN_13
#define FAN_ARR_GPIO_Port GPIOD
#define BD_ADDR0_Pin GPIO_PIN_14
#define BD_ADDR0_GPIO_Port GPIOD
#define AC_POWER_Pin GPIO_PIN_15
#define AC_POWER_GPIO_Port GPIOD
#define FAN_EX1_Pin GPIO_PIN_2
#define FAN_EX1_GPIO_Port GPIOG
#define FAN_EX2_Pin GPIO_PIN_3
#define FAN_EX2_GPIO_Port GPIOG
#define FAN_EX3_Pin GPIO_PIN_4
#define FAN_EX3_GPIO_Port GPIOG
#define FAN_EX4_Pin GPIO_PIN_5
#define FAN_EX4_GPIO_Port GPIOG
#define DOOR_1_Pin GPIO_PIN_6
#define DOOR_1_GPIO_Port GPIOG
#define DOOR_2_Pin GPIO_PIN_7
#define DOOR_2_GPIO_Port GPIOG
#define DOOR_3_Pin GPIO_PIN_8
#define DOOR_3_GPIO_Port GPIOG
#define Y_LIMIT_R_Pin GPIO_PIN_6
#define Y_LIMIT_R_GPIO_Port GPIOC
#define Y_LIMIT_L_Pin GPIO_PIN_7
#define Y_LIMIT_L_GPIO_Port GPIOC
#define Y_OUT_Pin GPIO_PIN_8
#define Y_OUT_GPIO_Port GPIOC
#define Z_OUT_Pin GPIO_PIN_9
#define Z_OUT_GPIO_Port GPIOC
#define USB5V_OnOff_Pin GPIO_PIN_8
#define USB5V_OnOff_GPIO_Port GPIOA
#define LED_CS_Pin GPIO_PIN_11
#define LED_CS_GPIO_Port GPIOA
#define LED_CL_Pin GPIO_PIN_12
#define LED_CL_GPIO_Port GPIOA
#define HOME4_Pin GPIO_PIN_0
#define HOME4_GPIO_Port GPIOD
#define HOME5_Pin GPIO_PIN_1
#define HOME5_GPIO_Port GPIOD
#define UART4_DIR_Pin GPIO_PIN_3
#define UART4_DIR_GPIO_Port GPIOD
#define UART5_DIR_Pin GPIO_PIN_4
#define UART5_DIR_GPIO_Port GPIOD
#define UART3_DIR_Pin GPIO_PIN_7
#define UART3_DIR_GPIO_Port GPIOD
#define DOOR_4_Pin GPIO_PIN_9
#define DOOR_4_GPIO_Port GPIOG
#define BAR_PWR_Pin GPIO_PIN_10
#define BAR_PWR_GPIO_Port GPIOG
#define BAR_WAKE_Pin GPIO_PIN_11
#define BAR_WAKE_GPIO_Port GPIOG
#define BAR_DECODE_Pin GPIO_PIN_12
#define BAR_DECODE_GPIO_Port GPIOG
#define BAR_TRIG_Pin GPIO_PIN_13
#define BAR_TRIG_GPIO_Port GPIOG
#define LEVEL_3_Pin GPIO_PIN_14
#define LEVEL_3_GPIO_Port GPIOG
#define LEVEL_4_Pin GPIO_PIN_15
#define LEVEL_4_GPIO_Port GPIOG
#define BD_ADDR1_Pin GPIO_PIN_5
#define BD_ADDR1_GPIO_Port GPIOB
#define ACC_VALVE_WASTE_Pin GPIO_PIN_8
#define ACC_VALVE_WASTE_GPIO_Port GPIOB
#define ACC_VALVE_ASP_Pin GPIO_PIN_9
#define ACC_VALVE_ASP_GPIO_Port GPIOB
#define nMO_M_Pin GPIO_PIN_0
#define nMO_M_GPIO_Port GPIOE
#define CK_M_Pin GPIO_PIN_1
#define CK_M_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
#define dSPIN_STBY_RESET_Pin GPIO_PIN_2
#define dSPIN_STBY_RESET_Port GPIOF
#define dSPIN_FLAG_Pin GPIO_PIN_3
#define dSPIN_FLAG_Port GPIOF
#define dSPIN_BUSY_Pin GPIO_PIN_4
#define dSPIN_BUSY_Port GPIOF
#define dSPIN_nSS_Pin GPIO_PIN_5
#define dSPIN_nSS_Port GPIOF

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
