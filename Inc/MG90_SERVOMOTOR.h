#ifndef __MG90S_H
#define __MG90S_H

#include "stm32f1xx_hal.h"


#define ASP_UP   3750
#define ASP_DOWN 2250

#pragma pack(push,1)
 __packed  struct servo
 {
  int32_t speed;
  int32_t up_pos;
  int32_t down_pos;
  int32_t asp_bath_x;
  int32_t asp_bath_z;
  int32_t disp_bath_y;
  int32_t asp_tray_x;
  int32_t asp_tray_z;
  int32_t disp_tray_y;
 };
#pragma pack(pop)

extern struct servo asp_mt;

void MX_TIM4_Init(void);
void sevo_stop();
void servo_mv( uint16_t pos);
void servo_mem_init();
event execute_servo_ctrl(event event);

void asp_home_init();

#endif //__MG90S_H