#ifndef CNTRL_H
#define CNTRL_H

#define SMP_PRME_PP 0x01<<PRIME_DW

#define XMT_PWR_OFF HAL_GPIO_WritePin(FAN_EX1_GPIO_Port,FAN_EX1_Pin,GPIO_PIN_SET);
#define YZMT_PWR_OFF HAL_GPIO_WritePin(FAN_EX2_GPIO_Port,FAN_EX2_Pin,GPIO_PIN_SET);

#define XMT_PWR_ON HAL_GPIO_WritePin(FAN_EX1_GPIO_Port,FAN_EX1_Pin,GPIO_PIN_RESET);
#define YZMT_PWR_ON HAL_GPIO_WritePin(FAN_EX2_GPIO_Port,FAN_EX2_Pin,GPIO_PIN_RESET);
enum blot_state
  {
    stNone,
    stPrepare,
    stStby,
    stStEng,
    stShutdown,
    stReady,
    stEmit,
    stErr,
    stErrSafe,
    stPowerUp,
    stPause,
    stPauseSq,
    stBoot,
    stStop,
    stStopPrepare,
  };


extern enum blot_state state;
event execute_main_ctrl(event);

extern byte smple_rack_cnt;
extern byte jude_cnt;

extern byte cll_state;
extern int16_t slope_vol;

extern int32_t clld_vol;
extern int16_t p_min , p_max;
extern bool home_flag;


extern byte asp_re_cunt;
extern bool lld_retry;
extern bool smp_prime_press_chek_flg;
extern bool smp_prime_dw_check_flg;
extern bool smp_prime_oper_flg;
extern bool press_sens_flg;
extern byte smp_prime_dw_erorr_cnt;
#endif
