

#ifndef __STEPPING_CTRL_H
#define __STEPPING_CTRL_H

#define HOMEING     10
#define CLL_EN      11
#define SWS_OP      12
#define CLLD_HOME   13   

#define HEATING_PAD_ON           HAL_GPIO_WritePin(ACC_HEATING_PAD_GPIO_Port,ACC_HEATING_PAD_Pin,GPIO_PIN_RESET)
#define HEATING_PAD_OFF          HAL_GPIO_WritePin(ACC_HEATING_PAD_GPIO_Port,ACC_HEATING_PAD_Pin,GPIO_PIN_SET)


enum mt_com_state
{
  mtNone,
  stanby,
  busy,
  stanby_home,
};


#pragma pack(push,1)
 __packed  struct st_motor
 {
  int32_t bath_pos;
  int32_t tray_pos;
  int32_t speed_rpm;
  int32_t accel_rpm;
  int32_t strip_width;
  int32_t dsp_pos;
  int32_t dsp_pos_end;
  int32_t asp_pos;
  int32_t sample_pos;
  int32_t sample_pos_end;
  int32_t sample_pos2;
  int32_t bar_pos;
  int32_t cam_pos;
  int32_t sample_width;
  int32_t cl_amp;

 };
#pragma pack(pop)
 extern struct st_motor xmt_ctrl;
 extern struct st_motor ymt_ctrl;
 extern struct st_motor zmt_ctrl;
 
extern enum mt_com_state mt_xstate, mt_ystate, mt_zstate;
event execute_stepping_ctrl(event event);

void stm_memory_init();
void all_homeing();
void x_homeing();
void z_homeing();
void y_homeing();
extern byte all_home;
void stmt_speed_set(byte adress,signed long int value);
void stmt_accel_set(byte adress,signed long int value);
void stmt_decel_set(byte adress,signed long int value);
bool stmt_abs_move(byte adress,signed long int value);


extern bool x_reach_pos, y_reach_pos, z_reach_pos;
extern signed long int x_prev_value, y_prev_value, z_prev_value;

void x_pram_set();
void y_pram_set();
void z_pram_set();

#endif   //__K_BLOT_UART
