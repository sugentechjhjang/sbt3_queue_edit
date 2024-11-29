

#ifndef __UART_PC_H
#define __UART_PC_H

#include "stm32f1xx_hal.h"
#include "string.h"

extern uint8_t usb_data_buf[4];

void MX_USART1_UART_Init(void);
void UART1_Main_RxCpltCallback(void);

void send_test(uint8_t *data, unsigned char length);
void pc_pack_aly();
void usb_rs232();
void usb_send_pack(enum cntrl_event cmd, uint8_t *data);
void pc_send_pack(uint16_t cmd, uint8_t *data);
event dev_cfg_ctrl(event event);
uint8_t ascii2hex(uint8_t code);



int32_t merge_32bit(int32_t out_data, byte* in_data );
void sort_8bit(int32_t in_data, byte* out_data );
uint8_t math_uint(uint8_t lsb, uint8_t hsb );

extern bool pause_flag;
extern bool debug_serial_mode;

extern uint32_t hsDevToPC_QueUseCheck();
extern void hsDevToPC_ErrHandle();
extern void hsDevToPC_PacketSendHandle(); 
extern void hsPC_To_DevRxPacket_Handle(void);

void dbg_serial(char *s);
void feedback_motor_packet(uint8_t *p_uGetPacketBuf, uint32_t dwCount);
void save_puque();
#endif   //__K_BLOT_UART