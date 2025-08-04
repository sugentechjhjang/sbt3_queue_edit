#ifndef SQ_H__
#define SQ_H__



enum step
{
  St_None,
  Wetting ,
  Sample,
  Washing_WS,
  Conj_ANA,
  Anti_ALG,
  Enzy_ALG,
  Stop_ANA,
  Substrate,
  Washing_DW,
  Drying,
  Auto_Clean,
  Analysis,
};

enum process
{
  Pr_None,
  Prime,
  Prime1,
  Prime2,
  DispAsp,
  DispAsp2,
  DispAsp3,
  Disp,
  Disp2,
  Asp,
  Incubation,
  Sample_LLD,
  Dry,
  RollBack,
  RollBack1,
  RollBack2,
  Analy,
  Incubation_Ws,
  Pause,
  Incubation2,
  Incubation_Ws2,
};

enum panel
{
  Pannel_None,
  Inhalant,
  Food,
  Inhalant_CON,
  Food_CON

};
#pragma pack(push,1)
__packed  struct strip_err_bit
{
  uint32_t err[(STRIP_NUM_END/32)+1];
};
#pragma pack(pop)
extern struct strip_err_bit str_errbit;

#pragma pack(push,1)
__packed  struct sample_strip
{
  uint8_t  strip;
  uint8_t  smpl; 
  uint8_t  penel;
  uint8_t  manual;
};
#pragma pack(pop)

extern struct sample_strip smp_strip[STRIP_NUM_END+1];

#pragma pack(push,1)
__packed  struct sq
{
  uint8_t  stNum; //step number
  uint8_t  prNum[20]; //process number
  uint32_t dword[20];
};
#pragma pack(pop)

extern struct sq sq[12];
extern byte sq_strp_mach;
extern byte full_step, full_pr;
extern int pr_time_sec;

event sq_ctrl(event event);
void sq_mem_init();

extern byte penel_sel;
extern int pr_time_min;
extern int pr_time_sec;
extern int base_time;

extern byte sq_smp_strp_mach_cnt;
extern byte full_step_cnt, full_pr_cnt;
bool get_pasue_event(uint16_t eve);
void step_time_cal(byte step);
void pr_time_cal(byte step, byte pro);
void state_led_init();
void strip_num_shake_time();
void pr_incubation_time();

extern uint dw_prime_vol;
extern uint rbk_time;
extern bool sq_start_dw_washing_flg;
extern bool aging_mode;

#endif