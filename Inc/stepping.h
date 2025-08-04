

#ifndef __STEPPING_H
#define __STEPPING_H

#include "stm32f1xx_hal.h"
#include "string.h"

#pragma pack(push,1)
struct erea_rs_pack 
{
  byte host;
  byte target;
  byte state;
  byte inst;
  byte val[4];  
} ;
#pragma pack(pop)

#pragma pack(push,1)
struct leadshine_rs_pack 
{
  byte address;
  byte fc;
  byte rg_address[2]; 
  byte val[2];  
} ;
#pragma pack(pop)



extern struct erea_rs_pack er_rs_pack;
extern struct leadshine_rs_pack lead_rs_pack;

void MX_USART4_UART_Init(void);
void UART4_Motor_RxCpltCallback(void); 
void HAL_UART4_Transmit(byte *data);
void UART_STEP_MT_RxCpltCallback(UART_HandleTypeDef *huart);
void calculateModbusCRC_16(uint8_t *data, uint16_t length, uint8_t *crcArray);
void mt_pack_aly();

extern HAL_StatusTypeDef UART4_ReInit(void);
extern UART_HandleTypeDef huart4;

#ifdef Motor_LEAD
  int32_t handleXmotorState(uint8_t val);
  int32_t handleYmotorState(uint8_t val);
  int32_t handleZmotorState(uint8_t val);
#endif

void stepping_rs();



#endif   //__K_BLOT_UART