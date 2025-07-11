
#include "main.h"


TIM_HandleTypeDef htim4;
TIM_OC_InitTypeDef sConfigOC;

//#define SERVO_OVERLOAD_LIMIT            40
#define SERVO_OVERLOAD_LIMIT            4000
#define SERVO_UP_POSITION_DEFAULT       4300
#define SERVO_ACTION_DELAY              1000        
/*
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
*/
struct servo asp_mt={
  100,
  5950,
  4650,
  1001,
  1002,
  1003,
  1004,
  1005,
  1006
  
};

/* TIM4 init function */
void MX_TIM4_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 31;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 45000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_PWM_Init(&htim4);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 3700;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);

}

void servo_mem_init()
{
 // servo_param_read();
  servo_param_read2();
  if(asp_mt.speed==0||asp_mt.disp_tray_y==(~0)){
    asp_mt.speed=100; 
    asp_mt.up_pos=1200;
    asp_mt.down_pos=3000;
    asp_mt.asp_bath_x=800;
    asp_mt.asp_bath_z=1002;
    asp_mt.disp_bath_y=1003;
    asp_mt.asp_tray_x=45000;
    asp_mt.asp_tray_z=2500;
    asp_mt.disp_tray_y=1006;
   // servo_param_write();
     servo_param_write2();
  } 
}
void sevo_stop() //SERVO STOP
{
 HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
}

bool servo_move_flag = false;

void servo_mv(uint16_t pos) //SERVO MOVE(without overcurrent detecting)
{
 // timer_intrrupt_stop();
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = pos;//5%(-90��) = 2250  7.5%(0��) = 3750 10%(+90��) = 4500
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1); 
  set_timer_(eventAspTimeOut,1200,0); 
  servo_move_flag = true;
}

int32_t bath_xpos_temp=0;
void asp_home_init()
{
   bath_xpos_temp=asp_mt.asp_bath_x;
}

uint32_t qc_etc_asp=0;
event execute_servo_ctrl(event event)
{
  byte dev_send_buf[4]={0,}; 
  switch(event)
  {
  case eventAspTimeOut:
  //  timer_intrrupt_start();
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
    break;
  case hseAspHome:
    servo_mv(asp_mt.up_pos);
    usb_send_pack(hseAspHome,usb_data_buf);
    break;
  case hseAspHomeSet:
  case hseAspUpPosSet:
    asp_mt.up_pos=merge_32bit(asp_mt.up_pos,usb_data_buf);
    servo_mv(asp_mt.up_pos);
    //servo_param_write();
    servo_param_write2();
    usb_send_pack(hseAspHomeSet,usb_data_buf);
    break;
  case hseAspHomeSave:
    asp_mt.up_pos=merge_32bit(asp_mt.up_pos,usb_data_buf);
    servo_param_write2();
    servo_param_read2();
    sort_8bit(asp_mt.up_pos,dev_send_buf);
    usb_send_pack(hseAspHomeSave, dev_send_buf);
    break;    
  case hseAspUpPosSave:
    asp_mt.up_pos=merge_32bit(asp_mt.up_pos,usb_data_buf);
    servo_param_write2();
    servo_param_read2();
    sort_8bit(asp_mt.up_pos,dev_send_buf);
    usb_send_pack(hseAspUpPosSave, dev_send_buf);
    break;
  case hseAspHomeRead:
  case hseAspUpPosRead:
   // servo_param_read();
    servo_param_read2();
    sort_8bit(asp_mt.up_pos,dev_send_buf);
    usb_send_pack(hseAspHomeRead, dev_send_buf);
    break;
  case hseAspBathZSet:
  case hseAspDownPosSet:
    asp_mt.down_pos=merge_32bit(asp_mt.down_pos,usb_data_buf);
    servo_mv(asp_mt.down_pos);
    usb_send_pack(hseAspBathZSet,usb_data_buf);
    break;
  case hseAspBathZSave:
    asp_mt.down_pos=merge_32bit(asp_mt.down_pos,usb_data_buf);
    usb_send_pack(hseAspDownPosSave,usb_data_buf);
    break;
  case hseAspDownPosSave:
    asp_mt.down_pos=merge_32bit(asp_mt.down_pos,usb_data_buf);
    usb_send_pack(hseAspDownPosSave,usb_data_buf);
    break;

  case hseAspBathZRead:
  case hseAspDownPosRead:
    sort_8bit(asp_mt.down_pos,dev_send_buf);
    usb_send_pack(hseAspBathZRead, dev_send_buf);
    break;
  case hseAspSpeedSet:
    usb_send_pack(hseAspSpeedSet,usb_data_buf);
    break;
  case hseAspSpeedRead:
    usb_send_pack(hseAspSpeedRead,usb_data_buf);
    break;
  case hseAspBathXSet:
    asp_mt.asp_bath_x=merge_32bit(asp_mt.asp_bath_x,usb_data_buf);
    xmt_ctrl.bath_pos= asp_mt.asp_bath_x;
    //servo_param_write();
    servo_param_write2();
    xmt_param_write();
    stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.bath_pos-bath_xpos_temp); //jjh
    usb_send_pack(hseAspBathXSet,usb_data_buf);
    break;
  case hseAspBathXSave:
    asp_mt.asp_bath_x=merge_32bit(asp_mt.asp_bath_x,usb_data_buf);
    xmt_ctrl.bath_pos= asp_mt.asp_bath_x;
    servo_param_write2();
    xmt_param_write();
    servo_param_read2();
    sort_8bit(asp_mt.asp_bath_x,dev_send_buf);
    usb_send_pack(hseAspBathXSave, dev_send_buf);
    break;
  case hseAspBathXRead:
   // servo_param_read();
     servo_param_read2();
    sort_8bit(asp_mt.asp_bath_x,dev_send_buf);
    usb_send_pack(hseAspBathXRead, dev_send_buf);
    break;
  case hseDispBathYSet:
    asp_mt.disp_bath_y=merge_32bit(asp_mt.disp_bath_y,usb_data_buf);
    servo_param_write2();
    stmt_abs_move(ADDR_MOTOR_Y,asp_mt.disp_bath_y);
    usb_send_pack(hseDispBathYSet,usb_data_buf);
    break;
  case hseDispBathYSave:
    asp_mt.disp_bath_y=merge_32bit(asp_mt.disp_bath_y,usb_data_buf);
    servo_param_write2();
    servo_param_read2();
    sort_8bit(asp_mt.disp_bath_y,dev_send_buf);
    usb_send_pack(hseDispBathYSave, dev_send_buf);
    break;         
  case hseDispBathYRead:
    //servo_param_read();
    servo_param_read2();
    sort_8bit(asp_mt.disp_bath_y,dev_send_buf);
    usb_send_pack(hseDispBathYRead, dev_send_buf);
    break;
  case hseAspTrayXSet:
    asp_mt.asp_tray_x=merge_32bit(asp_mt.asp_tray_x,usb_data_buf);
    //servo_param_write();
    servo_param_write2();
    stmt_abs_move(ADDR_MOTOR_X,asp_mt.asp_tray_x);
    usb_send_pack(hseAspTrayXSet,usb_data_buf);
    break;
  case hseAspTrayXSave:
    asp_mt.asp_tray_x=merge_32bit(asp_mt.asp_tray_x,usb_data_buf);
    //servo_param_write();
    servo_param_write2();
    servo_param_read2();
    sort_8bit(asp_mt.asp_tray_x,dev_send_buf);
    usb_send_pack(hseAspTrayXSave, dev_send_buf);
    break;   
  case hseAspTrayXRead:
    //servo_param_read();
     servo_param_read2();
    sort_8bit(asp_mt.asp_tray_x,dev_send_buf);
    usb_send_pack(hseAspTrayXRead, dev_send_buf);
    break;
  case  hseAspTrayZSet:
    asp_mt.asp_tray_z=merge_32bit(asp_mt.asp_tray_z,usb_data_buf);
    //servo_param_write();
    servo_param_write2();
    dSPIN_Go_To(shk_pram.dasp_pos);
    set_timer_(eventAspQcDelay,1000,0);
    usb_send_pack(hseAspTrayZSet,usb_data_buf);
    break;
  case hseAspTrayZSave:
    asp_mt.asp_tray_z=merge_32bit(asp_mt.asp_tray_z,usb_data_buf);
    //servo_param_write();
    servo_param_write2();
    servo_param_read2();
    sort_8bit(asp_mt.asp_tray_z,dev_send_buf);
    usb_send_pack(hseAspTrayZSave, dev_send_buf);
    break;
  
  case hseAspTrayZRead:
   // servo_param_read();
     servo_param_read2();
    sort_8bit(asp_mt.asp_tray_z,dev_send_buf);
    usb_send_pack(hseAspTrayZRead, dev_send_buf);
    break;
  case hseDispTrayYSet:
    asp_mt.disp_tray_y=merge_32bit(asp_mt.disp_tray_y,usb_data_buf);
   // servo_param_write();
    servo_param_write2();
    stmt_abs_move(ADDR_MOTOR_Y,asp_mt.disp_tray_y);
    usb_send_pack(hseDispTrayYSet,usb_data_buf);
    break;
  case hseDispTrayYSave:
    asp_mt.disp_tray_y=merge_32bit(asp_mt.disp_tray_y,usb_data_buf);
   // servo_param_write();
    servo_param_write2();
    servo_param_read2();
    sort_8bit(asp_mt.disp_tray_y,dev_send_buf);
    usb_send_pack(hseDispTrayYSave, dev_send_buf);
    break;
  case hseDispTrayYRead:
    //servo_param_read();
    servo_param_read2();
    sort_8bit(asp_mt.disp_tray_y,dev_send_buf);
    usb_send_pack(hseDispTrayYRead, dev_send_buf);
    break;
  case hseAspDispPage:
        state=stStEng;
    usb_send_pack(hseAspDispPage, 0);
    break;
  case hseAspJogMoveSet:
  case hseAspSet:
    qc_etc_asp=merge_32bit(qc_etc_asp,usb_data_buf);
    servo_mv(qc_etc_asp);
    usb_send_pack(hseAspJogMoveSet, usb_data_buf);
    break;
  case eventAspQcDelay:
    servo_mv(asp_mt.asp_tray_z);
    break;
  default:
    break;
  }
  return event;
}


















