

#ifndef __UART_PC_H
#define __UART_PC_H

#include "stm32f1xx_hal.h"
#include "string.h"

#define ECHO_BUF_SIZE 1034
#define FW_Data_buf 8

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

extern uint8_t echoBuf[ECHO_BUF_SIZE];

extern UART_HandleTypeDef huart1;

extern bool pause_flag;

extern bool isEchoEnabled;
extern bool download_start_flag;
extern bool PCtoSub;

extern uint16_t expectedTotalLength;

typedef enum {
    PACKET_START0 = 0,
    PACKET_START1,
    PACKET_START2,
    PACKET_START3,
    PACKET_LENGTH_H,
    PACKET_LENGTH_L,
    PACKET_CMD,
    PACKET_CHECKSUM,
    PACKET_FW_DATA_START
} PacketFieldIndex;

#define FIND_ADDRESS_CMD 0x01
#define BODY_CMD 0x02
#define END_CMD 0x03

extern uint32_t hsDevToPC_QueUseCheck();
extern void hsDevToPC_ErrHandle();
extern void hsDevToPC_PacketSendHandle(); 
extern void hsPC_To_DevRxPacket_Handle(void);

void dbg_serial(char *s);
void dbg_serial_fw_date(void);
void save_puque();
void send_pw_message(char *s);
void send_ack_buf_as_message(uint8_t *buf, uint16_t len);
int parse_version_string(const char *versionStr, uint8_t out[4]);
#endif   //__K_BLOT_UART