

#ifndef _C3000_SYRINGE_
#define _C3000_SYRINGE_

#include "stm32f1xx_hal.h"
#pragma pack(push,1)
struct syringe_param 
{
  uint vol;
  uint air_gap;
  uint speed;
};
#pragma pack(pop)
extern struct syringe_param syrg_pram;
//typedef enum 
//{
//  No_ERROR        = 0x00,
//  TX_HAL_ERROR    = 0x01,
//  TX_HAL_BUSY     = 0x02,
//  TX_HAL_TIMEOUT  = 0x03,
//  RX_HAL_ERROR    = 0X04,
//  RX_HAL_BUSY     = 0x05,
//  RX_HAL_TIMEOUT  = 0x06,
//  CHECKSUM_ERROR  = 0x07,
//  DEVICE_ERROR    = 0x08
//} COM_StatusTypeDef;


typedef enum 
{
  NO_VALVE = 0,
  PORT_3_VALVE,
  PORT_4_VALVE,
  PORT_3_DISP_VALVE_F,
  T_VALVE = 5,
  PORT_6_DISP_VALVE = 7,
  PORT_9_DISP_VALVE,
  DUAL_LOOP_VALVE,
  PORT_3_DISP_VALVE_P = 11
} ValveTypeDef;

typedef enum 
{
  Syringe_idle = 0,
  Syringe_init = 1,
  Syringe_sq1,
  Syringe_sq2,
  Syringe_sq3,
  Syringe_stop = 5,
} SyringeTypeDef;



typedef enum 
{
  SYRINGE_NONE          = 0x00,
  SYRINGE_INIT          = 0x01,
  SYRINGE_STATUS        = 0x02,
  SYRINGE_SPEED_SET     = 0x03,
  SYRINGE_ACCEL_SET     = 0x04,
  SYRINGE_MOVE_DOWN     = 0x05,
  SYRINGE_MOVE_UP       = 0X06,
  SYRINGE_VALVE_LEFT    = 0x07,
  SYRINGE_VALVE_RIGHT   = 0x08,
  SYRINGE_VALVE_BYPASS  = 0x09,
  SYRINGE_STATE_CHECK   = 0x10,
  SYRINGE_STOP          = 0x11,
} SYRINGE_COMMAND_LIST;

enum sy_pump_state
{
  pmNone,
  sypm_idle,
  sypm_busy,
  sypm_error,
  sypm_stanby_home,
};

extern enum sy_pump_state sy_state;


void MX_USART3_UART_Init(void);
void C3000_send_command(uint8_t addr, char *pdata);
void C3000_srige_oper(byte cmd , uint data_value);
void UART3_Syringe_RxCpltCallback(void);
void UART_Syringe_RxCpltCallback(UART_HandleTypeDef *huart);
void syge_mem_init();
int32_t syringe_rxpack_aly(uint8_t *p_sy_GetPacketBuf);
void syringe_rs232();
void syge_init();


extern bool sy_flag;
event execute_syringe_ctrl(event event);
//unsigned char C3000_Syringe_Control(unsigned char syringe_control_temp, unsigned short int data_value, unsigned char * syringe_rx_data_temp);

#endif// _C3000_SYRINGE_