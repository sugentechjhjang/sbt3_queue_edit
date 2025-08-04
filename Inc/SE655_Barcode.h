#ifndef __SE655_H
#define __SE655_H

#include "stm32f1xx_hal.h"
enum barcode_state
{
  BarNone,
  BarInit,
  BarInitOper,
  BarStnby,
  BarReady,
  BarOper,
  BarReadComp
};
extern enum barcode_state br_state;
void MX_USART2_UART_Init(void);
event execute_barcode_ctrl(event event);
void UART_Barcode_RxCpltCallback(UART_HandleTypeDef *huart);
void barcode_triger();
void br_read_reflash();
void br_read_time();
void br_rs232_read(byte br_chr);
void br_init_rs232_read(byte br_chr);
HAL_StatusTypeDef barcode_data_send(unsigned char *data, unsigned char data_length);
bool br_init_pram_ack_chek();
void tset_barcode(void);
void barcode_off();
void bar_mem_init();

void Se655_Barcode_Init(void);
unsigned char Se655_Barcode_Trigger_Set(void);
unsigned char Barcode_Reading_Data(unsigned char *barcode_data_temp);

#pragma pack(push,1)
struct bar_pram 
{
  int32_t start_pos;
  int32_t end_pos;
  int32_t spl_num;
  int32_t start_pos2;
  int32_t qr_tray_pos;
} ;
#pragma pack(pop)
extern struct bar_pram bar_pm;
extern UART_HandleTypeDef huart2;
extern HAL_StatusTypeDef UART2_ReInit(void);

extern bool bar_indx_oper_flg;
extern bool barcode_flag;
#endif //__SE655_H