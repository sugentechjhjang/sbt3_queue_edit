#ifndef __SHAKE_MOTOR_H
#define __SHAKE_MOTOR_H

#include "stm32f1xx_hal.h"
#include "17HS3410_Dspin.h"

#define SAMPLE_NUM 24//44
#define STRIP_NUM  12//47
#define STRIP_NUM_END  48//47
enum shake_state
  {
    stShkrStby,
    stShkrPrepare,
    stShkrOper,
    stShkrOperEnd,
    stShkrShutdown,
    stShkrReady,
    stShkrEmit,
    stShkrErr,
    stShkrErrSafe,
    stShkrPowerUp,
    stShkrBoot,
  };
extern enum shake_state sk_state;

#pragma pack(push,1)
 __packed  struct shake_pram
 {
  int32_t hoiz_pos;
  int32_t dasp_pos;
  int32_t aly_pos;
  int32_t dry_pos;
  int32_t speed_rpm;
  int32_t accel_rpm;
  int32_t run_time_s;
  int32_t up_ang_pos;
  int32_t down_ang_pos;

 };
#pragma pack(pop)
extern struct shake_pram shk_pram;

void shaker_spi_handle(SPI_HandleTypeDef *);
void shaker_Init(void);
void MX_SPI1_Init(void);
byte shake_home();
byte shake_run();


//void Shake_homing(int32_t abs_start_pos);
unsigned char Shake_homing(void);
void shake_speed_set(uint32_t Nowspeed, uint32_t Maxspeed, uint32_t acc);
void shake_mem_init();

event execute_shake_ctrl(event event);

#endif // __SHAKE_MOTOR_H

