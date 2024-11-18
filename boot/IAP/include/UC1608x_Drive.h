/**
  ******************************************************************************
  * @file    
  * @author  전자제어 팀
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
  *8080 제어와 6800제어
  8080 READ
  /RD = 0  /WR = 1
  8080 WRITE
  /RD = 1  /WR = 0

  6800 READ
  R/W = 1 E = Active
  6800 WRITE
  R/W = 0 E = Active
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UC1608_DRIVE_H
#define __UC1608_DRIVE_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"
#include "stm32f10x_lib.h"
#include "stdio.h"

#include "stm32_KMAC_Base.h"

/* defines */
#define RCC_APB2Periph_UC1608X_IO     RCC_APB2Periph_GPIOE
#define UC1608X_IO                    GPIOE
#define nUC1608X_CS                   GPIO_Pin_12
#define nUC1608X_RSTn                 GPIO_Pin_11
#define nUC1608X_CD                   GPIO_Pin_10 // CD = 1 Command, CD = 0 Data   
#define nUC1608X_WR                   GPIO_Pin_9
#define nUC1608X_RD                   GPIO_Pin_8
#define nUC1608X_D0_8                 GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7

#define IO_UC1608X_WR                (UC1608X_IO->ODR)
#define IO_UC1608X_RD                (UC1608X_IO->IDR)


#define nBZ                           7
#define nMX                           6
#define nDE                           5
#define nRS                           4
#define nWA                           3
#define nGN1                          2
#define nGN0                          1

// UC1608X Command 
#define nU1608X_RESET                 0xE2
#define nUC1608X_MR_TC                0x20 + 0x04 + 0x03   // 0x20 Command, 0x04 = 128(MUX ratio definition), 0x03 = -0.20%/C
#define nUC1608X_PWCTRL               0x28 + 0x04 + 0x03   // 0x28 Command, 0x04 = Internal VLCD, 0x03 = 60nF < LCD < 90nF 
#define nUC1608X_START_LINE           0x40 + 0x00          // 0x40 Command, 0x00 = image row 0 
#define nUC1608X_BIAS_RATIO           0xE8 + 0x02          // 0xE8 Command, 0x02 =  Bais ratio 12( 1/12)
#define nUC1608X_G_PM                 0x81                 // Command
#define nUC1608X_G_PM_DATA            0x40 + 0x10          // 0x40 = Gain 1(max 3) , 0x23 = PM Value 35 (max 63) 
#define nUC1608X_RAM_ADD_CTRL         0x88 + 0x01          // 0x88 Command , 0x01 = restart & PA or CA will increment by one step
#define nUC1608X_MAP_CTRL             0xC0 + 0x00 + 0x08   // 0xC0 Command, 0x04 = MX
#define nUC1608X_PIXEL_ON             0xA4 + 0x00          // 0xA4 Command, 0x00 
#define nUC1608X_INVERS_DISPLAY       0xA6 + 0x00          // 0xA6 Command, 0x00
#define nUC1608X_DISPLAY_ON           0xAE + 0x01          // 0xAE Cpmmand, 0x01 =  LCD Enable



#define IMG_OVW                       0 // over write 바탱 데이터 소실
#define IMG_OR                        1 // 기존 데이터와 OR 연산
#define IMG_Clr                       2 // 해당 이미지만 지움

/* Macro */
//#define UC1608X_CS        PERIPH_GetBit_BB((uint32_t)UC1608X_IO, nBIT12)
//#define UC1608X_RSTn      PERIPH_GetBit_BB((uint32_t)UC1608X_IO, nBIT11)
//#define UC1608X_CD        PERIPH_GetBit_BB((uint32_t)UC1608X_IO, nBIT10)
//#define UC1608X_WR        PERIPH_GetBit_BB((uint32_t)UC1608X_IO, nBIT9)
//#define UC1608X_RD        PERIPH_GetBit_BB((uint32_t)UC1608X_IO, nBIT8)

#define UC1608X_CS(x)        if(x) GPIO_SetBits(UC1608X_IO, nUC1608X_CS);else GPIO_ResetBits(UC1608X_IO, nUC1608X_CS)
#define UC1608X_RSTn(x)      if(x) GPIO_SetBits(UC1608X_IO, nUC1608X_RSTn);else GPIO_ResetBits(UC1608X_IO, nUC1608X_RSTn)
#define UC1608X_CD(x)        if(x) GPIO_SetBits(UC1608X_IO, nUC1608X_CD);else GPIO_ResetBits(UC1608X_IO, nUC1608X_CD)
#define UC1608X_WR(x)        if(x) GPIO_SetBits(UC1608X_IO, nUC1608X_WR);else GPIO_ResetBits(UC1608X_IO, nUC1608X_WR)
#define UC1608X_RD(x)        if(x) GPIO_SetBits(UC1608X_IO, nUC1608X_RD);else GPIO_ResetBits(UC1608X_IO, nUC1608X_RD)



#define nUC1608X_CMD      0
#define nUC1608X_DAT      1

#define UC1608X_READ()    UC1608X_WR(SET); UC1608X_RD(RESET)
#define UC1608X_WRITE()   UC1608X_WR(RESET); UC1608X_RD(SET)
#define UC1608X_RW_CL()   UC1608X_WR(SET); UC1608X_RD(SET)


/* functions */  
void vUC1608X_Init(void);
void vRectangular(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t put);
void vGlcdAreaClear(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void vGlcdAllClear(void);
void vGlcdPixel(uint8_t x, uint8_t y, uint8_t put);

void vGlcdLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t put);
void vGlcdCircle(uint8_t xcenter, uint8_t ycenter, uint8_t radius, uint8_t put);
void vRectangular(uint8_t x, uint8_t y, uint8_t width, uint8_t high, uint8_t put);
void vRectangular_Full(uint8_t x, uint8_t y, uint8_t width, uint8_t high, uint8_t put);

uint8_t ImageDisplay(uint8_t mode, uint8_t turn, uint8_t x, uint8_t Page, uint8_t x_IndexSize, uint8_t Page_IndexSize, const uint8_t *image);
uint8_t ImageDisplayRam(uint8_t mode, uint8_t turn, uint8_t x, uint8_t Page, uint8_t x_IndexSize, uint8_t Page_IndexSize, uint8_t *image);

uint8_t uGlcdEngString(uint8_t mode, uint8_t turn, uint8_t x, uint8_t Page, uint8_t Fontc, uint8_t *pStr);
uint8_t uGlcdEngStringFlash(uint8_t mode, uint8_t turn, uint8_t x, uint8_t Page, uint8_t Fontc,const uint8_t *pStr);

void GLCD_SRAM_ReWrite(void);

void KmacLog(void);
#endif /*__UC1608_DRIVE_H*/
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
