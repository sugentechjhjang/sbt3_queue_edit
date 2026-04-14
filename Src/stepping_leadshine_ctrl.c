/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include <string.h>

#ifdef Motor_LEAD  //LeadShine

enum mt_com_state mt_xstate=stanby;
enum mt_com_state mt_ystate=stanby;
enum mt_com_state mt_zstate=stanby;

#define front_packet 3
#define crc_length 2
uint8_t lead_rx_length;

struct st_motor xmt_ctrl ={
  12000,
  1000,
  100,
  1000,
  7000,
  3000,
  30000,
  4000,
  4500,
  5500,
  55000,
  5500,
  6000,
  10000,
  120,
};

struct st_motor ymt_ctrl ={
  10000,
  1000,
  50,
  1000,
  1200,
  3000,
  30000,
  4000,
  4500,
  5500,
  5500,
  55000,
  6000,
  100,
  120,
};

struct st_motor zmt_ctrl ={
  5000,
  1000,
  50,  
  1000,
  1200,
  3000,
  30000,
  4000,
  4500,
  5500,
  5500,
  55000,
  6000,
  100,
  100,
  2200,
};

void stm_memory_init()
{
  //int32_t spd, acc;
  err_tmout_set(errMemory);
  xmt_param_read();
  ymt_param_read();
  zmt_param_read();
  err_tmout_en(FALSE);
  if(xmt_ctrl.bath_pos==(~0))
  {
    xmt_ctrl.bath_pos=12000;
    xmt_ctrl.tray_pos=1000;
    xmt_ctrl.speed_rpm=100;
    xmt_ctrl.accel_rpm=1000;
    xmt_ctrl.strip_width=7000;
    xmt_ctrl.dsp_pos=49500;
    xmt_ctrl.dsp_pos_end=135500;
    xmt_ctrl.asp_pos=4000;
    xmt_ctrl.sample_pos=2500;
    xmt_ctrl.sample_pos_end=249000;
    xmt_ctrl.sample_pos2=4000;
    xmt_ctrl.bar_pos=5500;
    xmt_ctrl.cam_pos=155000;
    xmt_ctrl.sample_width=10000;
    xmt_ctrl.cl_amp=120;
    xmt_param_write();
  }  
    
  if(ymt_ctrl.bath_pos==(~0))
  {
    ymt_ctrl.bath_pos=56000;
    ymt_ctrl.tray_pos=1000;
    ymt_ctrl.speed_rpm=50;
    ymt_ctrl.accel_rpm=1000;
    ymt_ctrl.strip_width=1200;
    ymt_ctrl.dsp_pos=54000;
    ymt_ctrl.dsp_pos_end=30000;
    ymt_ctrl.asp_pos=4000;
    ymt_ctrl.sample_pos=-32000;
    ymt_ctrl.sample_pos_end=30000;
    ymt_ctrl.sample_pos2=-56000;
    ymt_ctrl.bar_pos=5500;
    ymt_ctrl.cam_pos=269500;
    ymt_ctrl.sample_width=14300;
    ymt_ctrl.cl_amp=120;
    ymt_param_write();
  }

  if(zmt_ctrl.bath_pos==(~0))
  {
    zmt_ctrl.bath_pos=1000;
    zmt_ctrl.tray_pos=1000;
    zmt_ctrl.speed_rpm=50;
    zmt_ctrl.accel_rpm=1000;
    zmt_ctrl.strip_width=1200;
    zmt_ctrl.dsp_pos=28000;
    zmt_ctrl.dsp_pos_end=27000;
    zmt_ctrl.asp_pos=4000;
    zmt_ctrl.sample_pos=4500;
    zmt_ctrl.sample_pos_end=30000;
    zmt_ctrl.sample_pos2=4000;
    zmt_ctrl.bar_pos=5500;
    zmt_ctrl.cam_pos=6000;
    zmt_ctrl.sample_width=100;
    zmt_ctrl.cl_amp=100;
    zmt_param_write();
  }

  if(zmt_ctrl.clld_z_axis_offset==(~0))
  {
    zmt_ctrl.clld_z_axis_offset=2200;
    zmt_param_write();
  }

}

void probe_Hardware_protect()
{
    if((admin_positon_flag == true) && (servo_move_flag == true))
    {
      servo_mv(asp_mt.up_pos);
      HAL_Delay(1000);
      servo_move_flag = false;
    }
    
    if((admin_positon_flag == true) && (z_homeing_success == false) && (barcode_flag == false))
    {
      z_homeing();
      HAL_Delay(3000);
    }
}

void stm_reset()
{
  stmt_servo_off();
  HAL_Delay(500);
  stmt_servo_on();
  HAL_Delay(500);
}



void all_homeing()
{
  give_event(eventAllhomeCk,0);
}

void x_homeing()
{
  x_homing_state = true;

  probe_Hardware_protect();

  motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_HOME_OFFSET_H,xmt_ctrl.bath_pos);
  motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_HOME_OFFSET_L,xmt_ctrl.bath_pos);
  motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_TRIGGER_REG,LEAD_HOMING);
  set_timer_(eventXhomeCk,100,0);
}

void y_homeing()
{
  y_homing_state = true;

  probe_Hardware_protect();
 
  motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_HOME_OFFSET_H,ymt_ctrl.bath_pos);
  motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_HOME_OFFSET_L,ymt_ctrl.bath_pos);
  motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_TRIGGER_REG,LEAD_HOMING);
  set_timer_(eventYhomeCk,100,0);
}

void z_homeing()
{
  z_homing_state = true;
  
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_HOME_OFFSET_H,Z_BATH_POS); //z���� ȣ�� ������ GUI���� ��������
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_HOME_OFFSET_L,Z_BATH_POS);
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_TRIGGER_REG,LEAD_HOMING);
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_SPEED_ABS,200);
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,1000);
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_DEC_ABS,1000);


  set_timer_(eventZhomeCk,100,0);
}


  
void stmt_speed_set(byte adress,signed long int value)
{
  if(adress == ADDR_MOTOR_X)
  {
    motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_SPEED_ABS,value);
    if(value > 35)
    {
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,(1000*1000)/value);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_DEC_ABS,1000);
    }
    else
    {
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,30000);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_DEC_ABS,1000);
    }
  }
   
  if(adress == ADDR_MOTOR_Y)
  {
    motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_SPEED_ABS,value);
    if(value > 35)
    {
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,(1000*1000)/value);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_DEC_ABS,1000);
    }
    else
    {
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,30000);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_DEC_ABS,1000);
    }
  } 
  
  if(adress == ADDR_MOTOR_Z)
  {
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_SPEED_ABS,value);
    if(value > 35)
    {
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,(1000*1000)/value);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_DEC_ABS,1000);
    }
    else
    {
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,30000);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_DEC_ABS,1000);
    }
    
  }  
}


void CLLD_OP()
{  
    cll_state = 6;

    stmt_speed_set(ADDR_MOTOR_Z,30);
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.sample_pos);
}

void SWS_OP()
{  
    cll_state = 5;
}

void CLLD_HOME()
{  
    cll_state = 5;
    
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_SPEED_ABS,10);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,30000);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_DEC_ABS,30000);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_MOVE_ABS,0x41);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_POSITION_H_ABS,-5000);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_POSITION_L_ABS,-5000);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_PATH_0_SET,PATH_0);
    
    HAL_Delay(1500);
    
    //stmt_rel_move(ADDR_MOTOR_Z,-5000); //rel_move  

    //while(!z_reach_pos); 

    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_SPEED_ABS,200);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,5000);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_DEC_ABS,5000);

    stmt_abs_move(ADDR_MOTOR_Z,Z_BATH_POS);
    
      
}


bool x_reach_pos = true, y_reach_pos = true, z_reach_pos = true,
     prev_x_reach_pos  = true, prev_y_reach_pos  = true, prev_z_reach_pos  = true;

bool x_excute_reach_error = false , y_excute_reach_error = false , z_excute_reach_error = false;
signed long int x_prev_value = 0,y_prev_value = 0,z_prev_value = 0;

bool x_homing_state=false;
bool x_abs_move_state=false;
bool x_running_state = false;
bool y_homing_state=false;
bool y_abs_move_state=false;
bool y_running_state = false;
bool z_homing_state=false;
bool z_abs_move_state=false;
bool z_running_state = false;


bool stmt_abs_move(byte adress,signed long int value){
  switch(adress)
  {
  case ADDR_MOTOR_X:
    probe_Hardware_protect(); //admin mode
    
    if(x_reach_pos){
      x_abs_move_state = true;
      x_reach_pos=false;
      x_homeing_success = false;
      mt_xstate=stanby;
    
      motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_MOVE_ABS,0x01);
      motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_POSITION_H_ABS,value);
      motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_POSITION_L_ABS,value);
      motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_PATH_0_SET,PATH_0);
      
      set_timer_(eventXAbsPosCk,100,0);
    }
    break;
  case ADDR_MOTOR_Y:
    probe_Hardware_protect(); //admin mode

    if(y_reach_pos){
      y_abs_move_state = true;
      y_reach_pos=false;
      y_homeing_success = false;
      mt_ystate=stanby;
     
      motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_MOVE_ABS,0x01);
      motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_POSITION_H_ABS,value);
      motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_POSITION_L_ABS,value);
      motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_PATH_0_SET,PATH_0);
      
      set_timer_(eventYAbsPosCk,100,0);
    }
    break;
  case ADDR_MOTOR_Z:
    if(z_reach_pos){
      z_abs_move_state = true;
      z_reach_pos=false;
      z_homeing_success = false;
      mt_zstate=stanby;
     
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_MOVE_ABS,0x01);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_POSITION_H_ABS,value);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_POSITION_L_ABS,value);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_PATH_0_SET,PATH_0);
      
      set_timer_(eventZAbsPosCk,100,0);  
    }
    break;
  default:
    break;
  }
 
  if(adress==ADDR_MOTOR_X) return x_reach_pos;
  if(adress==ADDR_MOTOR_Y) return y_reach_pos; 
  if(adress==ADDR_MOTOR_Z) return z_reach_pos; 
  
  return 0;
}


bool stmt_rel_move(byte adress,signed long int value){
  switch(adress)
  {
  case ADDR_MOTOR_X:
    probe_Hardware_protect(); //admin mode
    
    if(x_reach_pos){
      x_abs_move_state = true;
      x_reach_pos=false;
      x_homeing_success = false;
      mt_xstate=stanby;
    
      motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_MOVE_ABS,0x41);
      motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_POSITION_H_ABS,value);
      motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_POSITION_L_ABS,value);
      motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_PATH_0_SET,PATH_0);
      
      set_timer_(eventXAbsPosCk,100,0);
    }
    break;
  case ADDR_MOTOR_Y:
    probe_Hardware_protect(); //admin mode

    if(y_reach_pos){
      y_abs_move_state = true;
      y_reach_pos=false;
      y_homeing_success = false;
      mt_ystate=stanby;
     
      motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_MOVE_ABS,0x41);
      motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_POSITION_H_ABS,value);
      motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_POSITION_L_ABS,value);
      motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_PATH_0_SET,PATH_0);
      
      set_timer_(eventYAbsPosCk,100,0);
    }
    break;
  case ADDR_MOTOR_Z:
    if(z_reach_pos){
      z_abs_move_state = true;
      z_reach_pos=false;
      z_homeing_success = false;
      mt_zstate=stanby;
     
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_MOVE_ABS,0x41);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_POSITION_H_ABS,value);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_POSITION_L_ABS,value);
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_PATH_0_SET,PATH_0);
      
      set_timer_(eventZAbsPosCk,100,0);  
    }
    break;
  default:
    break;
  }
 
  if(adress==ADDR_MOTOR_X) return x_reach_pos;
  if(adress==ADDR_MOTOR_Y) return y_reach_pos; 
  if(adress==ADDR_MOTOR_Z) return z_reach_pos; 
  
  return 0;
}

void stmt_servo_on()
{
  motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,Servo_Address,Servo_On);
  motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,Servo_Address,Servo_On);
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,Servo_Address,Servo_On);
}

void stmt_servo_off()
{
  motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,Servo_Address,Servo_Off);
  motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,Servo_Address,Servo_Off);
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,Servo_Address,Servo_Off);
}


void x_pram_set()
{
  int32_t spd, acc;

  xmt_ctrl.speed_rpm=100;
  xmt_ctrl.accel_rpm=1000;
  
  spd=xmt_ctrl.speed_rpm; 
  acc=xmt_ctrl.accel_rpm; 

  motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_TRIGGER_REG,LEAD_E_STOP);
  motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_SPEED_ABS,spd);
  motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_ACC_ABS,acc);
  motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_DEC_ABS,10000);
}

void y_pram_set()
{
  int32_t spd, acc;
  spd=ymt_ctrl.speed_rpm; //50 
  acc=ymt_ctrl.accel_rpm; //1000

  motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_TRIGGER_REG,LEAD_E_STOP);
  motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_SPEED_ABS,spd);
  motor_cmd_send(ADDR_MOTOR_Y,LEAD_WRITE,LEAD_ACC_ABS,acc);
}

void z_pram_set()
{
  int32_t spd, acc;
  spd=zmt_ctrl.speed_rpm; //50 
  acc=zmt_ctrl.accel_rpm; //1000
  
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_TRIGGER_REG,LEAD_E_STOP);
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_SPEED_ABS,spd);
  motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,acc);
}
uint x_mt_send_cnt=0;
uint x_reach_count , y_reach_count , z_reach_count;
bool x_homeing_success=false , y_homeing_success=false , z_homeing_success=false;

void (*fc)(byte , byte , byte ,byte , signed long int );
int32_t bath_ypos_temp=0,bath_zpos_temp=0;
event execute_stepping_ctrl(event event)
{
  int32_t x_jog_pram=0,y_jog_pram=0,z_jog_pram=0 ;

  byte dev_send_buf[4]={0,};
  int32_t spd, acc;
  switch(event)
  {
    
  case eventXhomeCk:
    if(mt_xstate==stanby_home){
      asp_home_init();
      x_homeing_success = true;
      break;
    }else{     
      x_running_state = true;
      motor_cmd_send(ADDR_MOTOR_X,LEAD_READ,LEAD_RUNNING_STATE,0x01);
      set_timer_(eventXhomeCk,200,0);
    }
    break;
  case eventXAbsPosCk:
    if(!x_reach_pos)
    {
      x_abs_move_state = true;
      x_running_state = true;
      motor_cmd_send(ADDR_MOTOR_X,LEAD_READ,LEAD_RUNNING_STATE,0x01);
      set_timer_(eventXAbsPosCk,200,0);
    }
    break;
  case eventYhomeCk:
    if(mt_ystate==stanby_home){
      bath_ypos_temp=ymt_ctrl.bath_pos;
      y_homeing_success = true;
      break;
    }else{
      y_running_state = true;
      motor_cmd_send(ADDR_MOTOR_Y,LEAD_READ,LEAD_RUNNING_STATE,0x01);
      set_timer_(eventYhomeCk,200,0);
    }    
    break;

  case eventYAbsPosCk:
    if(!y_reach_pos)
    {
      y_abs_move_state = true;
      y_running_state = true;
      motor_cmd_send(ADDR_MOTOR_Y,LEAD_READ,LEAD_RUNNING_STATE,0x01);
      set_timer_(eventYAbsPosCk,200,0); 
    }
    break;
  case eventZhomeCk:    
    if(mt_zstate==stanby_home){
      z_homeing_success = true;
      break;
    }else{
      z_running_state = true;
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_READ,LEAD_RUNNING_STATE,0x01);
      set_timer_(eventZhomeCk,200,0);
    }     
    break;

  case eventZAbsPosCk:
    if(!z_reach_pos)
    {
      z_abs_move_state = true;
      z_running_state = true;
      motor_cmd_send(ADDR_MOTOR_Z,LEAD_READ,LEAD_RUNNING_STATE,0x01);
      set_timer_(eventZAbsPosCk,200,0); 
    }
    break;
    
  case eventAllhomeCk:
    //z_homeing();
    set_timer_(eventAllhomeEnd,10,0); 
    break;
  case eventAllhomeEnd:
    x_homeing();
    y_homeing();
    break;
    //-----------------pc cmd-----------------------------------------------------------
    //x_motor allin
  case hseXaxiHome:
    xmt_param_write();
    xmt_param_read();
    x_homeing();
    usb_send_pack(hseXaxiHome,dev_send_buf);
    break;
    
  case hseXaxiSpeedSet:
    xmt_ctrl.speed_rpm=merge_32bit(xmt_ctrl.speed_rpm,usb_data_buf);
    motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_SPEED_ABS,xmt_ctrl.speed_rpm);
    usb_send_pack(hseXaxiSpeedSet,usb_data_buf);
    break;
  case hseXaxiSpeedRead:
    sort_8bit(xmt_ctrl.speed_rpm,dev_send_buf);
    usb_send_pack(hseXaxiSpeedRead, dev_send_buf);
    break;
  case  hseXaxiAccelSet:
    xmt_ctrl.accel_rpm=merge_32bit(xmt_ctrl.accel_rpm,usb_data_buf);
    motor_cmd_send(ADDR_MOTOR_X,LEAD_WRITE,LEAD_ACC_ABS,xmt_ctrl.accel_rpm);
    usb_send_pack(hseXaxiAccelSet,usb_data_buf);
    break;
  case  hseXaxiAccelRead:
    sort_8bit(xmt_ctrl.accel_rpm,dev_send_buf);
    usb_send_pack(hseXaxiAccelRead, dev_send_buf);
    break;
    
  case hseXaxiStripWidthSet:
    xmt_ctrl.strip_width=merge_32bit(xmt_ctrl.strip_width,usb_data_buf);
    usb_send_pack(hseXaxiStripWidthSet,usb_data_buf);
    break;
    
  case hseXaxiStripWidthRead:
    sort_8bit(xmt_ctrl.strip_width,dev_send_buf);
    usb_send_pack(hseXaxiStripWidthRead, dev_send_buf);
    break;
  case hseXaxiBathPosSet:
    xmt_ctrl.bath_pos=merge_32bit(xmt_ctrl.bath_pos,usb_data_buf);
    usb_send_pack(hseXaxiBathPosSet,usb_data_buf);
    break;
  case hseXaxiBathPosRead:
    sort_8bit(xmt_ctrl.bath_pos,dev_send_buf);
    usb_send_pack(hseXaxiBathPosRead, dev_send_buf);
    break;
  case hseProbeTrayXSet:
  case hseXaxiDspPosSet:
    xmt_ctrl.dsp_pos=merge_32bit(xmt_ctrl.dsp_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X, xmt_ctrl.dsp_pos);
    xmt_param_write();
    usb_send_pack(hseProbeTrayXSet,usb_data_buf);
    break;
  case hseProbeTrayXSave:
    xmt_ctrl.dsp_pos=merge_32bit(xmt_ctrl.dsp_pos,usb_data_buf);
    xmt_param_write();
    xmt_param_read();
    sort_8bit(xmt_ctrl.dsp_pos,dev_send_buf);
    usb_send_pack(hseProbeTrayXSave, dev_send_buf);
    break;
  case hseXaxiDspPosSave:
    xmt_ctrl.dsp_pos=merge_32bit(xmt_ctrl.dsp_pos,usb_data_buf);
    xmt_param_write();
    xmt_param_read();
    sort_8bit(xmt_ctrl.dsp_pos,dev_send_buf);
    usb_send_pack(hseXaxiDspPosSave, dev_send_buf);
    break;  
  
  case hseProbeTrayXRead:
  case hseXaxiDspPosRead:
    xmt_param_read();
    sort_8bit(xmt_ctrl.dsp_pos,dev_send_buf);
    usb_send_pack(hseProbeTrayXRead, dev_send_buf);
    break;
  case hseXaxiAspPosSet:
    xmt_ctrl.asp_pos=merge_32bit(xmt_ctrl.asp_pos,usb_data_buf);
    usb_send_pack(hseXaxiAspPosSet,usb_data_buf);
    break;
  case hseXaxiAspPosRead:
    sort_8bit(xmt_ctrl.asp_pos,dev_send_buf);
    usb_send_pack(hseXaxiAspPosRead, dev_send_buf);
    break;
  case hseProbeStripGapSet:
  case hseXaxiCamPosSet:
    xmt_ctrl.cam_pos=merge_32bit(xmt_ctrl.cam_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X, xmt_ctrl.cam_pos);
    xmt_param_write();
    usb_send_pack(hseProbeStripGapSet,usb_data_buf);
    break;
  case hseProbeStripGapSave:
    xmt_ctrl.cam_pos=merge_32bit(xmt_ctrl.cam_pos,usb_data_buf);
    xmt_param_write();
    xmt_param_read();
    sort_8bit(xmt_ctrl.cam_pos,dev_send_buf);
    usb_send_pack(hseProbeStripGapSave, dev_send_buf);
    break;  
  case hseXaxiCamPosSave:
    xmt_ctrl.cam_pos=merge_32bit(xmt_ctrl.cam_pos,usb_data_buf);
    xmt_param_write();
    xmt_param_read();
    sort_8bit(xmt_ctrl.cam_pos,dev_send_buf);
    usb_send_pack(hseXaxiCamPosSave, dev_send_buf);
    break;  

  case hseProbeStripGapRead:
  case hseXaxiCamPosRead:
    xmt_param_read();
    sort_8bit(xmt_ctrl.cam_pos,dev_send_buf);
    usb_send_pack(hseProbeStripGapRead, dev_send_buf);
    break;
  case hseXaxiBarPosSet:
    xmt_ctrl.bar_pos=merge_32bit(xmt_ctrl.bar_pos,usb_data_buf);
    usb_send_pack(hseXaxiBarPosSet,usb_data_buf);
    break;
  case hseXaxiBarPosRead:
    sort_8bit(xmt_ctrl.bar_pos,dev_send_buf);
    usb_send_pack(hseXaxiBarPosRead, dev_send_buf);
    break;
  case hseProbeSmplXSet:
  case hseXaxiSamplePosSet:
    xmt_ctrl.sample_pos=merge_32bit(xmt_ctrl.sample_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X, xmt_ctrl.sample_pos);
    xmt_param_write();
    usb_send_pack(hseProbeSmplXSet,usb_data_buf);
    break;

  case hseProbeSmplXSave:
    xmt_ctrl.sample_pos=merge_32bit(xmt_ctrl.sample_pos,usb_data_buf);
    xmt_param_write();
    xmt_param_read();
    sort_8bit(xmt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseProbeSmplXSave, dev_send_buf);
    break;

  case hseXaxiSamplePosSave:
    xmt_ctrl.sample_pos=merge_32bit(xmt_ctrl.sample_pos,usb_data_buf);
    xmt_param_write();
    xmt_param_read();
    sort_8bit(xmt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseXaxiSamplePosSave, dev_send_buf);
    break;
  case hseProbeSmplXRead:
  case hseXaxiSamplePosRead:
    xmt_param_read();
    sort_8bit(xmt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseProbeSmplXRead, dev_send_buf);
    break;
  case hseXaxiSampleWidthSet:
    xmt_ctrl.sample_width=merge_32bit(xmt_ctrl.sample_width,usb_data_buf);
    usb_send_pack(hseXaxiSampleWidthSet,usb_data_buf);
    break;
  case hseXaxiSampleWidthRead:
    sort_8bit(xmt_ctrl.sample_width,dev_send_buf);
    usb_send_pack(hseXaxiSampleWidthRead, dev_send_buf);
    break;
  case hseProbeSampleGapSet:
  case hseXaxiClCurrentSet:
   // xmt_ctrl.cl_amp=merge_32bit(xmt_ctrl.cl_amp,usb_data_buf);
    ymt_ctrl.cam_pos=merge_32bit(ymt_ctrl.cam_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X,  ymt_ctrl.cam_pos);
    ymt_param_write();
   // edbmt_cmd_send(ADDR_MOTOR_X,INST_SAP,EDB2000_CL_CURRENT_MIN_TYPE,0,xmt_ctrl.cl_amp);    
    usb_send_pack(hseProbeSampleGapSet,usb_data_buf);
    break;
  
  case hseProbeSampleGapSave:
    ymt_ctrl.cam_pos=merge_32bit(ymt_ctrl.cam_pos,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.cam_pos,dev_send_buf);
    usb_send_pack(hseProbeSampleGapSave, dev_send_buf);
    break;    
   case hseXaxiClCurrentSave:
    ymt_ctrl.cam_pos=merge_32bit(ymt_ctrl.cam_pos,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.cam_pos,dev_send_buf);
    usb_send_pack(hseXaxiClCurrentSave, dev_send_buf);
    break;     
    
  case hseProbeSampleGapRead:
  case hseXaxiClCurrentRead:
    //sort_8bit(xmt_ctrl.cl_amp,dev_send_buf);
    ymt_param_read();
    sort_8bit(ymt_ctrl.cam_pos,dev_send_buf);
    usb_send_pack(hseProbeSampleGapRead, dev_send_buf);
    break;
  case hseXAixsJogMoveSet:
  case hseXaxiJogMove:
    x_jog_pram=merge_32bit(x_jog_pram,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X,x_jog_pram);
    usb_send_pack(hseXAixsJogMoveSet,usb_data_buf);
    break;
  case hseProbeSmplXEndSet:
    xmt_ctrl.sample_pos_end=merge_32bit(xmt_ctrl.sample_pos_end,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X, xmt_ctrl.sample_pos_end);
    xmt_param_write();
    usb_send_pack(hseProbeSmplXEndSet,usb_data_buf);
    break;
  
  case hseProbeSmplXEndSave:
    xmt_ctrl.sample_pos_end=merge_32bit(xmt_ctrl.sample_pos_end,usb_data_buf);
    xmt_param_write();
    xmt_param_read();
    sort_8bit(xmt_ctrl.sample_pos_end,dev_send_buf);
    usb_send_pack(hseProbeSmplXEndSave, dev_send_buf);
    break;

  case hseProbeSmplXEndRead:
    xmt_param_read();
    sort_8bit(xmt_ctrl.sample_pos_end,dev_send_buf);
    usb_send_pack(hseProbeSmplXEndRead, dev_send_buf);
   break;
  case hseProbeTrayXEndSet:
    xmt_ctrl.dsp_pos_end=merge_32bit(xmt_ctrl.dsp_pos_end,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X, xmt_ctrl.dsp_pos_end);
    xmt_param_write();
    usb_send_pack(hseProbeTrayXEndSet,usb_data_buf);
    break;

  case hseProbeTrayXEndSave:
    xmt_ctrl.dsp_pos_end=merge_32bit(xmt_ctrl.dsp_pos_end,usb_data_buf);
    xmt_param_write();
    xmt_param_read();
    sort_8bit(xmt_ctrl.dsp_pos_end,dev_send_buf);
    usb_send_pack(hseProbeTrayXEndSave, dev_send_buf);
    break;

  case hseProbeTrayXEndRead:
    xmt_param_read();
    sort_8bit(xmt_ctrl.dsp_pos_end,dev_send_buf);
    usb_send_pack(hseProbeTrayXEndRead, dev_send_buf);
    break;
//y__motor allin
  case hseYaxiHome:
    ymt_param_write();
    ymt_param_read();
    y_homeing();
    usb_send_pack(hseYaxiHome,dev_send_buf);
    break;
 /* case hseYaxiSpeedSet:
    ymt_ctrl.speed_rpm=merge_32bit(ymt_ctrl.speed_rpm,usb_data_buf);
    spd=(ymt_ctrl.speed_rpm*51200)/60;
     edbmt_cmd_send(ADDR_MOTOR_Y,INST_SAP,EDB2000_MAX_SPEED_TYPE,0,spd);
    usb_send_pack(hseYaxiSpeedSet,usb_data_buf);
    break;
  case hseYaxiSpeedRead:
    sort_8bit(ymt_ctrl.speed_rpm,dev_send_buf);
    usb_send_pack(hseYaxiSpeedRead, dev_send_buf);
    break;
  case hseYaxiAccelSet:
    ymt_ctrl.accel_rpm=merge_32bit(ymt_ctrl.accel_rpm,usb_data_buf);
    acc=(ymt_ctrl.accel_rpm*51200)/60;
    edbmt_cmd_send(ADDR_MOTOR_Y,INST_SAP,EDB2000_ACCELERATOR_TYPE,0,acc);
    usb_send_pack(hseYaxiAccelSet,usb_data_buf);
    break;
  case hseYaxiAccelRead:
    sort_8bit(ymt_ctrl.accel_rpm,dev_send_buf);
    usb_send_pack(hseYaxiAccelRead, dev_send_buf);
    break;*/
  case hseYAixsJogMoveSet:
  case hseYaxiJogMove:
    y_jog_pram=merge_32bit(y_jog_pram,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_Y,y_jog_pram);
    usb_send_pack(hseYAixsJogMoveSet,usb_data_buf);
    break;
  case hseYaxiStripWidthSet:
    ymt_ctrl.strip_width=merge_32bit(ymt_ctrl.strip_width,usb_data_buf);
    usb_send_pack(hseYaxiStripWidthSet,usb_data_buf);
    break;
  case hseYaxiStripWidthRead:
    sort_8bit(ymt_ctrl.strip_width,dev_send_buf);
    usb_send_pack(hseYaxiStripWidthRead, dev_send_buf);
    break;
  
  case hseProbeBathYSet:
  case hseYaxiBathPosSet:
    ymt_ctrl.bath_pos=merge_32bit(ymt_ctrl.bath_pos,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
    usb_send_pack(hseProbeBathYSet,usb_data_buf);
    break;
  
  case hseProbeBathYSave:
    ymt_ctrl.bath_pos=merge_32bit(ymt_ctrl.bath_pos,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.bath_pos,dev_send_buf);
    usb_send_pack(hseProbeBathYSave, dev_send_buf);
    break;

  case hseYaxiBathPosSave:
    ymt_ctrl.bath_pos=merge_32bit(ymt_ctrl.bath_pos,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.bath_pos,dev_send_buf);
    usb_send_pack(hseYaxiBathPosSave, dev_send_buf);
    break;

  case hseProbeBathYRead:
  case hseYaxiBathPosRead:
    ymt_param_read();
    sort_8bit(ymt_ctrl.bath_pos,dev_send_buf);
    usb_send_pack(hseProbeBathYRead, dev_send_buf);
    break;
    
  case hseProbeTrayYSet:
  case hseYaxiDspPosSet:
    dSPIN_Go_To(shk_pram.down_ang_pos);
    ymt_ctrl.dsp_pos=merge_32bit(ymt_ctrl.dsp_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_Y, ymt_ctrl.dsp_pos);
    ymt_param_write();
    usb_send_pack(hseProbeTrayYSet,usb_data_buf);
    break;

  case hseProbeTrayYSave:
    ymt_ctrl.dsp_pos=merge_32bit(ymt_ctrl.dsp_pos,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.dsp_pos,dev_send_buf);
    usb_send_pack(hseProbeTrayYSave, dev_send_buf);
    break;    

  case hseYaxiDspPosSave:
    ymt_ctrl.dsp_pos=merge_32bit(ymt_ctrl.dsp_pos,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.dsp_pos,dev_send_buf);
    usb_send_pack(hseYaxiDspPosSave, dev_send_buf);
    break;    

  case hseProbeTrayYRead:
  case hseYaxiDspPosRead:
    ymt_param_read();
    sort_8bit(ymt_ctrl.dsp_pos,dev_send_buf);
    usb_send_pack(hseProbeTrayYRead, dev_send_buf);
    break;
  case hseYaxiAspPosSet:
    ymt_ctrl.asp_pos=merge_32bit(ymt_ctrl.asp_pos,usb_data_buf);
    usb_send_pack(hseYaxiAspPosSet,usb_data_buf);
    break;
  case hseYaxiAspPosRead:
    sort_8bit(ymt_ctrl.asp_pos,dev_send_buf);
    usb_send_pack(hseYaxiAspPosRead, dev_send_buf);
    break;
  case hseProbeBathInitSet:
  case hseYaxiBathSamplePosSet:
    ymt_ctrl.sample_width=merge_32bit(ymt_ctrl.sample_width,usb_data_buf);
    dSPIN_Go_To( shk_pram.up_ang_pos);
    stmt_abs_move(ADDR_MOTOR_Z, ymt_ctrl.sample_width);
    ymt_param_write();
    usb_send_pack(hseProbeBathInitSet,usb_data_buf); 
    break;

  case hseProbeBathInitSave:
    ymt_ctrl.sample_width=merge_32bit(ymt_ctrl.sample_width,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.sample_width,dev_send_buf);
    usb_send_pack(hseProbeBathInitSave, dev_send_buf);
    break;

  case hseYaxiBathSamplePosSave:
    ymt_ctrl.sample_width=merge_32bit(ymt_ctrl.sample_width,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.sample_width,dev_send_buf);
    usb_send_pack(hseYaxiBathSamplePosSave, dev_send_buf);
    break;

  case hseProbeBathInitRead:
  case hseYaxiBathSamplePosRead:
    ymt_param_read();
    sort_8bit(ymt_ctrl.sample_width,dev_send_buf);
    usb_send_pack(hseProbeBathInitRead, dev_send_buf);
    break;
  case hseProbeSmplY1Set:
  case hseYaxiSamplePosSet:
    ymt_ctrl.sample_pos=merge_32bit(ymt_ctrl.sample_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_Y, ymt_ctrl.sample_pos);
    ymt_param_write();
    usb_send_pack(hseProbeSmplY1Set,usb_data_buf);
    break;
  case hseProbeSmplY1Save:
    ymt_ctrl.sample_pos=merge_32bit(ymt_ctrl.sample_pos,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseProbeSmplY1Save, dev_send_buf);
    break;
  case hseYaxiSamplePosSave:
    ymt_ctrl.sample_pos=merge_32bit(ymt_ctrl.sample_pos,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseYaxiSamplePosSave, dev_send_buf);
    break;



  case hseProbeSmplY1Read:
  case hseYaxiSamplePosRead:
    ymt_param_read();
    sort_8bit(ymt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseProbeSmplY1Read, dev_send_buf);
    break;

  case hseProbeSmplY2Set:
    ymt_ctrl.sample_pos2=merge_32bit(ymt_ctrl.sample_pos2,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_Y, ymt_ctrl.sample_pos2);
    ymt_param_write();
    usb_send_pack(hseProbeSmplY2Set,usb_data_buf);
    break;
  case hseProbeSmplY2Save:
    ymt_ctrl.sample_pos2=merge_32bit(ymt_ctrl.sample_pos2,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    sort_8bit(ymt_ctrl.sample_pos2,dev_send_buf);
    usb_send_pack(hseProbeSmplY2Save, dev_send_buf);
    break;

  case hseProbeSmplY2Read:
    ymt_param_read();
    sort_8bit(ymt_ctrl.sample_pos2,dev_send_buf);
    usb_send_pack(hseProbeSmplY2Read, dev_send_buf);
    break;
//Z__motor allin
  case hseZaxiHome:
    zmt_param_write();
    zmt_param_read();
    z_homeing();
    usb_send_pack(hseZaxiHome,dev_send_buf);
    break;
  case hseZaxiSpeedSet:
    zmt_ctrl.speed_rpm=merge_32bit(zmt_ctrl.speed_rpm,usb_data_buf);
    //spd=(zmt_ctrl.speed_rpm*51200)/60;
    spd=zmt_ctrl.speed_rpm;
    //edbmt_cmd_send(ADDR_MOTOR_Z,INST_SAP,EDB2000_MAX_SPEED_TYPE,0,spd);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_SPEED_ABS,spd);
    usb_send_pack(hseZaxiSpeedSet,usb_data_buf);
    break;
  case hseZaxiSpeedRead:
    sort_8bit(zmt_ctrl.speed_rpm,dev_send_buf);
    usb_send_pack(hseZaxiSpeedRead, dev_send_buf);
    break;
  case hseZaxiAccelSet:
    zmt_ctrl.accel_rpm=merge_32bit(zmt_ctrl.accel_rpm,usb_data_buf);
    //acc=(zmt_ctrl.accel_rpm*51200)/60;
    acc=zmt_ctrl.accel_rpm;
    //edbmt_cmd_send(ADDR_MOTOR_Z,INST_SAP,EDB2000_ACCELERATOR_TYPE,0,acc);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,LEAD_ACC_ABS,(1000*1000)/acc);
    usb_send_pack(hseZaxiAccelSet,usb_data_buf);
    break;
  case hseZaxiAccelRead:
    sort_8bit(zmt_ctrl.accel_rpm,dev_send_buf);
    usb_send_pack(hseZaxiAccelRead, dev_send_buf);
    break;
  case hseZAixsJogMoveSet:
  case hseZaxiJogMove:
    z_jog_pram=merge_32bit(z_jog_pram,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_Z,z_jog_pram);
    usb_send_pack(hseZAixsJogMoveSet,usb_data_buf);
    break;
  case hseProbeBathZSet:
  case hseZaxiBathPosSet:
    zmt_ctrl.dsp_pos_end=merge_32bit(zmt_ctrl.dsp_pos_end,usb_data_buf);
    zmt_param_write();
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.dsp_pos_end);
    usb_send_pack(hseProbeBathZSet,usb_data_buf);
    break;
  case hseProbeBathZSave:
    zmt_ctrl.dsp_pos_end=merge_32bit(zmt_ctrl.dsp_pos_end,usb_data_buf);
    zmt_param_write();
    zmt_param_read();
    sort_8bit(zmt_ctrl.dsp_pos_end,dev_send_buf);
    usb_send_pack(hseProbeBathZSave, dev_send_buf);
    break;
  case hseZaxiBathPosSave:
    zmt_ctrl.dsp_pos_end=merge_32bit(zmt_ctrl.dsp_pos_end,usb_data_buf);
    zmt_param_write();
    zmt_param_read();
    sort_8bit(zmt_ctrl.dsp_pos_end,dev_send_buf);
    usb_send_pack(hseZaxiBathPosSave, dev_send_buf);
    break;
  case hseProbeBathZRead:
  case hseZaxiBathPosRead:
    zmt_param_read();
    sort_8bit(zmt_ctrl.dsp_pos_end,dev_send_buf);
    usb_send_pack(hseProbeBathZRead, dev_send_buf);
    break;
    
  case hseProbeTrayZSet:
  case hseZaxiDspPosSet:
    dSPIN_Go_To( shk_pram.down_ang_pos);
    zmt_ctrl.dsp_pos=merge_32bit(zmt_ctrl.dsp_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_Z, zmt_ctrl.dsp_pos);
    zmt_param_write();
    usb_send_pack(hseProbeTrayZSet,usb_data_buf);
    break;
  case hseProbeTrayZSave:
    zmt_ctrl.dsp_pos=merge_32bit(zmt_ctrl.dsp_pos,usb_data_buf);
    zmt_param_write();
    zmt_param_read();
    sort_8bit(zmt_ctrl.dsp_pos,dev_send_buf);
    usb_send_pack(hseProbeTrayZSave, dev_send_buf);
    break;
  case hseZaxiDspPosSave:
    zmt_ctrl.dsp_pos=merge_32bit(zmt_ctrl.dsp_pos,usb_data_buf);
    zmt_param_write();
    zmt_param_read();
    sort_8bit(zmt_ctrl.dsp_pos,dev_send_buf);
    usb_send_pack(hseZaxiDspPosSave, dev_send_buf);
    break;


  case hseProbeTrayZRead:
  case hseZaxiDspPosRead:
    zmt_param_read();
    sort_8bit(zmt_ctrl.dsp_pos,dev_send_buf);
    usb_send_pack(hseProbeTrayZRead, dev_send_buf);
    break;
  case hseZaxiAspPosSet:
    zmt_ctrl.asp_pos=merge_32bit(zmt_ctrl.asp_pos,usb_data_buf);
    usb_send_pack(hseZaxiAspPosSet,usb_data_buf);
    break;
  case hseZaxiAspPosRead:
    sort_8bit(zmt_ctrl.asp_pos,dev_send_buf);
    usb_send_pack(hseZaxiAspPosRead, dev_send_buf);
    break;
  case hseZaxiBathSamplePosSet:
    zmt_ctrl.sample_width=merge_32bit(zmt_ctrl.sample_width,usb_data_buf);
    usb_send_pack(hseZaxiBathSamplePosSet,usb_data_buf);
    break;  
  case hseZaxiBathSamplePosRead:
    sort_8bit(zmt_ctrl.sample_width,dev_send_buf);
    usb_send_pack(hseZaxiBathSamplePosRead, dev_send_buf);
    break;
  case hseProbeSmplZSet:
  case hseZaxiSamplePosSet:
    zmt_ctrl.sample_pos=merge_32bit(zmt_ctrl.sample_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_Z, zmt_ctrl.sample_pos);
    zmt_param_write();
    //edbmt_cmd_send(ADDR_MOTOR_Z,INST_SGP,4,2, zmt_ctrl.sample_pos); ����
    usb_send_pack(hseProbeSmplZSet,usb_data_buf);
    break;
  case hseProbeSmplZSave:
    zmt_ctrl.sample_pos=merge_32bit(zmt_ctrl.sample_pos,usb_data_buf);
    zmt_param_write();
    zmt_param_read();
    sort_8bit(zmt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseProbeSmplZSave, dev_send_buf);
    break;
  case hseZaxiSamplePosSave:
    zmt_ctrl.sample_pos=merge_32bit(zmt_ctrl.sample_pos,usb_data_buf);
    zmt_param_write();
    zmt_param_read();
    sort_8bit(zmt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseZaxiSamplePosSave, dev_send_buf);
    break;
  case hseProbeSmplZRead:
  case hseZaxiSamplePosRead:
    zmt_param_read();
    sort_8bit(zmt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseProbeSmplZRead, dev_send_buf);
    break;
    
  case hseZaxiClCurrentSet:
    zmt_ctrl.cl_amp=merge_32bit(zmt_ctrl.cl_amp,usb_data_buf);
    //edbmt_cmd_send(ADDR_MOTOR_Z,INST_SAP,EDB2000_CL_CURRENT_MIN_TYPE,0,zmt_ctrl.cl_amp); 
    usb_send_pack(hseZaxiClCurrentSet,usb_data_buf);
    break;
  case hseZaxiClCurrentRead:
    sort_8bit(zmt_ctrl.cl_amp,dev_send_buf);
    usb_send_pack(hseZaxiClCurrentRead, dev_send_buf);
    break;

  case hseZaxiClldMoveOffsetSet:
    zmt_ctrl.clld_z_axis_offset=merge_32bit(zmt_ctrl.clld_z_axis_offset,usb_data_buf);
    zmt_param_write();
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,CLLD_Z_AXIS_OFFSET_H,zmt_ctrl.clld_z_axis_offset);
    motor_cmd_send(ADDR_MOTOR_Z,LEAD_WRITE,CLLD_Z_AXIS_OFFSET_L,zmt_ctrl.clld_z_axis_offset);
    usb_send_pack(hseZaxiClldMoveOffsetSet,usb_data_buf);
    break;

  case hseZaxiClldMoveOffsetRead:
    zmt_param_read();
    sort_8bit(zmt_ctrl.clld_z_axis_offset,dev_send_buf);
    usb_send_pack(hseZaxiClldMoveOffsetRead,dev_send_buf);
    break;

  case hseProbePage:
    state=stStEng;
    usb_send_pack(hseProbePage, 0);
    break;
  default:
    break;
  }
  return event;
}



//-----------------------------------------------------------------------------
//2024 02 29 code 

//EDB Queue ==============================================================
#define LEAD_QUE_SZ	20

#define LEAD_QUE_OK	0
#define LEAD_QUE_FULL	1
#define LEAD_QUE_EMPTY	2

typedef struct
{
  uint32_t dwTail;
  uint32_t dwHead;
  uint8_t uQueBuf[LEAD_QUE_SZ];
}LEAD_Que_t;

LEAD_Que_t g_tLEAD_Que_Handle;

int32_t LEAD_ReceivePutQue(uint8_t uPutData)
{
  int32_t dwTail;

  dwTail = (g_tLEAD_Que_Handle.dwTail + 1)%LEAD_QUE_SZ;

  if( dwTail == g_tLEAD_Que_Handle.dwHead){
    return LEAD_QUE_FULL;
  }

  g_tLEAD_Que_Handle.uQueBuf[g_tLEAD_Que_Handle.dwTail] = uPutData;

  g_tLEAD_Que_Handle.dwTail = dwTail;
          
  return LEAD_QUE_OK;
}

int32_t LEAD_ReceiveGetQue(uint8_t *p_uGetData)
{
  if( g_tLEAD_Que_Handle.dwTail == g_tLEAD_Que_Handle.dwHead ){ 
    return LEAD_QUE_EMPTY;
  }

  *p_uGetData = g_tLEAD_Que_Handle.uQueBuf[g_tLEAD_Que_Handle.dwHead];

  g_tLEAD_Que_Handle.dwHead = (g_tLEAD_Que_Handle.dwHead + 1)%LEAD_QUE_SZ;

  return LEAD_QUE_OK;
}

void LEAD_QueReset(void)
{
  NVIC_DisableIRQ(UART4_IRQn); 
  g_tLEAD_Que_Handle.dwHead = 0;
  g_tLEAD_Que_Handle.dwTail = 0;
  memset(g_tLEAD_Que_Handle.uQueBuf,0,LEAD_QUE_SZ);
  NVIC_EnableIRQ(UART4_IRQn);
}

uint32_t LEAD_QueHaveDataSize(void)
{
  
  uint32_t dwReturnData;
  NVIC_DisableIRQ(UART4_IRQn); 

  if (g_tLEAD_Que_Handle.dwHead <= g_tLEAD_Que_Handle.dwTail)
      dwReturnData = g_tLEAD_Que_Handle.dwTail - g_tLEAD_Que_Handle.dwHead;
  else
  {
      dwReturnData = LEAD_QUE_SZ - g_tLEAD_Que_Handle.dwHead;
      dwReturnData = dwReturnData + g_tLEAD_Que_Handle.dwTail;
  }
  NVIC_EnableIRQ(UART4_IRQn);
  
  return dwReturnData;

}



uint32_t g_dwLEAD_Start = 0;
extern UART_HandleTypeDef huart4;
extern byte mt_chr;


void UART_StepMotor_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart4)
  {
    int32_t dwQueCheck;

    dwQueCheck = LEAD_ReceivePutQue(mt_chr);
    if(dwQueCheck == LEAD_QUE_FULL)
    {
      //Error code
    }
   
    dwQueCheck=HAL_UART_Receive_IT(&huart4, &mt_chr, 1);    
    if(dwQueCheck)
    {
      while(1);
    }
  }
}


HAL_StatusTypeDef UART4_ReInit(void)
{
    HAL_StatusTypeDef status;

    // UART5 비활성화 (DeInit)
    status = HAL_UART_DeInit(&huart4);
    if (status != HAL_OK)
    {
        // UART 비활성화 실패 처리
        return status;
    }

    // UART5 다시 초기화 (Init)
    status = HAL_UART_Init(&huart4);
    if (status != HAL_OK)
    {
        // UART 초기화 실패 처리
        return status;
    }

    // 초기화 성공
    HAL_UART_Receive_IT(&huart4, &mt_chr, 1);
    dbg_serial("StepMotorX_Y_Z_UART_ReInit");
    return HAL_OK;
}





enum
{
  RCV_LEAD_ADDRESS = 0,
  RCV_LEAD_FC,
};

#define LEAD_RE_TRY_MAX		       5
#define LEAD_RX_WAIT_DELAY	     5
#define LEAD_COM_WAIT_TIM        50

#define LEAD_RECV_OK             0
#define LEAD_RECV_FAIL           1

int32_t Get_LEAD_ReceivPacketHandle(uint8_t uAddress, uint8_t *p_uGetPacketBuf);
int32_t LEAD_ReceivPacketExec(uint8_t *p_LeadReceivPacket);

void calculateModbusCRC_16(uint8_t *data, uint16_t length, uint8_t *crcArray) {
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }

    crcArray[0] = (uint8_t)(crc & 0xFF);
    crcArray[1] = (uint8_t)((crc >> 8) & 0xFF);
}

enum mt_com_state *gp_tStatus;

void motor_cmd_send(byte motor_address, byte fc, uint16_t rg_address ,signed long int data)
{
  
  uint32_t dwCnt = 0, dwCheck = 0,Lead_queue_size = 0 ,dwReTryCnt = 0, HAL_Fail_Cnt = 0;
    
  uint8_t LeadSendPacket[8] = {0};
  uint8_t LeadReceivPacket[50] = {0};
  uint8_t modbus_send_crc16_data[2] = {0};
  
  unsigned char highByte_H = (data >> 24) & 0xFF;
  unsigned char lowByte_H = (data >> 16) & 0xFF;
  unsigned char highByte_L = (data >> 8) & 0xFF;
  unsigned char lowByte_L = data & 0xFF;

  switch(motor_address)
  {
    case ADDR_MOTOR_X:
      err_tmout_cnt_set(errXaxi,20);
      gp_tStatus = &mt_xstate;
    break;
    case ADDR_MOTOR_Y:
      err_tmout_cnt_set(errYaxi,20);
      gp_tStatus = &mt_ystate;
    break;
    case ADDR_MOTOR_Z:
      err_tmout_cnt_set(errZaxi,20);
      gp_tStatus = &mt_zstate; 
    break;
    default:
    //Error Code
      while(1);
      break;
  }

  if((rg_address == LEAD_POSITION_H_ABS)||(rg_address == LEAD_HOME_OFFSET_H)||(rg_address == CLLD_Z_AXIS_OFFSET_H)){
    LeadSendPacket[0] = motor_address;
    LeadSendPacket[1] = fc;
    LeadSendPacket[2] = (rg_address >> 8) & 0xFF;
    LeadSendPacket[3] = rg_address & 0xFF; 
    LeadSendPacket[4] = highByte_H;
    LeadSendPacket[5] = lowByte_H;
    LeadSendPacket[6] = 0;
    LeadSendPacket[7] = 0;
  }

  else if((rg_address == LEAD_POSITION_L_ABS)||(rg_address == LEAD_HOME_OFFSET_L)||(rg_address == CLLD_Z_AXIS_OFFSET_L)){
    LeadSendPacket[0] = motor_address;
    LeadSendPacket[1] = fc;
    LeadSendPacket[2] = (rg_address >> 8) & 0xFF;
    LeadSendPacket[3] = rg_address & 0xFF; 
    LeadSendPacket[4] = highByte_L; 
    LeadSendPacket[5] = lowByte_L;  
    LeadSendPacket[6] = 0;
    LeadSendPacket[7] = 0;
  }                    
                  
  else{
    LeadSendPacket[0] = motor_address;
    LeadSendPacket[1] = fc;  
    LeadSendPacket[2] = (rg_address >> 8) & 0xFF ;
    LeadSendPacket[3] = rg_address & 0xFF;      
    LeadSendPacket[4] = highByte_L; 
    LeadSendPacket[5] = lowByte_L; 
    LeadSendPacket[6] = 0;
    LeadSendPacket[7] = 0;
  }
  
  calculateModbusCRC_16(LeadSendPacket, 6, modbus_send_crc16_data);
  LeadSendPacket[6] = modbus_send_crc16_data[0];
  LeadSendPacket[7] = modbus_send_crc16_data[1];

  //*gp_tStatus = busy;
  

  while(1)
  { 
    HAL_Delay(5);
    LEAD_QueReset();
    HAL_GPIO_WritePin(UART4_DIR_GPIO_Port, UART4_DIR_Pin, (GPIO_PinState)SET);
    dwCheck = HAL_UART_Transmit(&huart4, LeadSendPacket, 8, LEAD_COM_WAIT_TIM);
    //while (__HAL_UART_GET_FLAG(&huart4, UART_FLAG_TC) == RESET);
    

    if(dwCheck)
    {
      HAL_Fail_Cnt++;
      UART4_ReInit();
      if(HAL_Fail_Cnt > 5) 
      {
        while(1);
      }
    }
    
    HAL_GPIO_WritePin(UART4_DIR_GPIO_Port, UART4_DIR_Pin, (GPIO_PinState)RESET);

    for(dwCnt = 0; dwCnt < 500; dwCnt++)
    {
      HAL_Delay(LEAD_RX_WAIT_DELAY);
      Lead_queue_size=LEAD_QueHaveDataSize();
      
      if(rg_address == LEAD_RUNNING_STATE)
      {
        if(Lead_queue_size == 7) break;
      }  
      else 
        if(Lead_queue_size == 8) break;
    }
    
    dwCheck = Get_LEAD_ReceivPacketHandle(motor_address, LeadReceivPacket);
    if(dwCheck == LEAD_RECV_OK)
    {
      break;
    }
    
    dbg_serial("StepMotorX_Y_Z_Receive_Retry");
    dwReTryCnt++;
    HAL_Delay(100);
    
    if(dwReTryCnt > LEAD_RE_TRY_MAX)
    {
      //Error code
      while(1);
    }
  }  
  dwCheck = LEAD_ReceivPacketExec(LeadReceivPacket);
  if(dwCheck == LEAD_RECV_FAIL)
  {
    dbg_serial("StepMotorX_Y_Z_Hardware_fault");
    while(1);
  }
  //HAL_Delay(100);
  //Error Count Stop
  err_tmout_en(FALSE);

  if(state == stErr)
  {
    while(1);//Error code  
  }
}



int32_t Get_LEAD_ReceivPacketHandle(uint8_t uAddress, uint8_t *p_uGetPacketBuf)
{
  
  
  uint32_t dwCount;
  uint8_t modbus_rev_crc16_data[2]={0,};
  int32_t dwQueCheck;
  uint8_t uDumy;
  

  for(dwCount = 0; ; dwCount++)
  {
    if(dwCount <= 50)
    {
      dwQueCheck = LEAD_ReceiveGetQue(&p_uGetPacketBuf[dwCount]);
    }  
    else 
      dwQueCheck = LEAD_ReceiveGetQue(&uDumy);

    if(dwQueCheck == LEAD_QUE_EMPTY)
    {
      break;
    }  
  }

  if(p_uGetPacketBuf[RCV_LEAD_FC] == LEAD_READ)
  { 
    lead_rx_length = p_uGetPacketBuf[2];
  }
  if(dwCount >= (front_packet + lead_rx_length + crc_length) && (p_uGetPacketBuf[RCV_LEAD_ADDRESS] == uAddress)&&(p_uGetPacketBuf[RCV_LEAD_FC] == LEAD_READ))
  {
    calculateModbusCRC_16(p_uGetPacketBuf,front_packet + lead_rx_length , modbus_rev_crc16_data);

    if(p_uGetPacketBuf[front_packet + lead_rx_length] == (modbus_rev_crc16_data[0])){
      if(p_uGetPacketBuf[front_packet + lead_rx_length+1] == (modbus_rev_crc16_data[1]))
      {
        //feedback_motor_packet(p_uGetPacketBuf, dwCount);
        return LEAD_RECV_OK;
      }
    }
  }
  
  else if((dwCount >= 8) && (p_uGetPacketBuf[RCV_LEAD_ADDRESS] == uAddress) && (p_uGetPacketBuf[RCV_LEAD_FC] == LEAD_WRITE))
  {
    
    calculateModbusCRC_16(p_uGetPacketBuf,6, modbus_rev_crc16_data);

    if((p_uGetPacketBuf[6] == (modbus_rev_crc16_data[0])) && (p_uGetPacketBuf[7] == (modbus_rev_crc16_data[1])))
    {
      //feedback_motor_packet(p_uGetPacketBuf, dwCount);
      return LEAD_RECV_OK;
    }
  }    
  return LEAD_RECV_FAIL;
}






uint16_t lead_stepping_cmd=0;
uint16_t lead_stepping_state=0;
byte lead_stepping_cmd_H=0,lead_stepping_cmd_L=0;
byte* lead_step_receive_buf;

int32_t LEAD_ReceivPacketExec(uint8_t *p_LeadReceivPacket)
{

  int32_t dwCheck = 0;
  lead_stepping_cmd_H=p_LeadReceivPacket[2];
  lead_stepping_cmd_L=p_LeadReceivPacket[3];
  
  lead_stepping_cmd = (lead_stepping_cmd_H << 8) | lead_stepping_cmd_L;

  switch(lead_stepping_cmd)
  { 
      case LEAD_MOVE_ABS:   
      case LEAD_POSITION_H_ABS:        
      case LEAD_POSITION_L_ABS:                
      case LEAD_SPEED_ABS:
      case LEAD_ACC_ABS:       
      case LEAD_DEC_ABS:
      case LEAD_HOME_OFFSET_H:
      case LEAD_HOME_OFFSET_L:  
      case LEAD_PATH_0_SET:  // 저장된 명령 값을 실행시킨다.
      case LEAD_TRIGGER_REG: // 호밍 명령 내릴 때 사용
        *gp_tStatus = stanby;  
      break;
  
    default:
      if(x_running_state){  //LEAD_RUNNING_STATE
        lead_stepping_state=p_LeadReceivPacket[4];
        dwCheck = handleXmotorState(lead_stepping_state);
      }
      if(y_running_state){  //LEAD_RUNNING_STATE
        lead_stepping_state=p_LeadReceivPacket[4];
        dwCheck = handleYmotorState(lead_stepping_state);  
      }
      if(z_running_state){  //LEAD_RUNNING_STATE
        lead_stepping_state=p_LeadReceivPacket[4];
        dwCheck = handleZmotorState(lead_stepping_state); 
      }
    break;
  } 
   return dwCheck;
}


int32_t handleXmotorState(uint8_t val) 
{
  if (x_abs_move_state == true && ((val >> 4) & 1)) 
  { // command state
      mt_xstate = stanby;
      x_abs_move_state = false;
      x_reach_pos = true;
      move_err_tmout_en(FALSE);
  } 
  else if ((val >> 2) & 1) 
  { // running state
      mt_xstate = busy;
      x_reach_pos = false;

      if(!x_reach_pos && prev_x_reach_pos)
      {
        move_err_tmout_cnt_set(errMoveTimeoutXaxi,300);
      } 
  }
  else if (x_homing_state == true && ((val >> 6) & 1)) 
  { // homing state
      mt_xstate = stanby_home;
      x_homing_state = false;
      x_reach_pos = true;
      move_err_tmout_en(FALSE);
  }
  x_running_state = false;
  prev_x_reach_pos = x_reach_pos;
  
  if (val >> 0 & 1)
  {
    return LEAD_RECV_FAIL;
  } 
  else
    return LEAD_RECV_OK;
}

int32_t handleYmotorState(uint8_t val) 
{
  if (y_abs_move_state == true && ((val >> 4) & 1)) 
  { // command state
      mt_ystate = stanby;
      y_abs_move_state = false;
      y_reach_pos = true;
      move_err_tmout_en(FALSE);
  } 
  else if ((val >> 2) & 1) 
  { // running state
      mt_ystate = busy;
      y_reach_pos = false;
      if(!y_reach_pos && prev_y_reach_pos)
      {
        move_err_tmout_cnt_set(errMoveTimeoutYaxi,300);
      } 
  } 
  else if (y_homing_state == true && ((val >> 6) & 1)) 
  { // homing state
      mt_ystate = stanby_home;
      y_homing_state = false;
      y_reach_pos = true;
      move_err_tmout_en(FALSE);
  }
  y_running_state = false;
  prev_y_reach_pos = y_reach_pos;

  if (val >> 0 & 1)
  {
    return LEAD_RECV_FAIL;
  } 
  else
    return LEAD_RECV_OK;
}

int32_t handleZmotorState(uint8_t val) 
{
  if (z_abs_move_state == true && ((val >> 4) & 1))
  { // command state
      mt_zstate = stanby;
      z_abs_move_state = false;
      z_reach_pos = true;
      move_err_tmout_en(FALSE);
  } 
  else if ((val >> 2) & 1) 
  { // running state
      mt_zstate = busy;
      z_reach_pos = false;
      if(!z_reach_pos && prev_z_reach_pos)
      {
        move_err_tmout_cnt_set(errMoveTimeoutZaxi,300);
      } 
  } 
  else if (z_homing_state == true && ((val >> 6) & 1)) 
  { // homing state
    mt_zstate = stanby_home;
    z_homing_state = false;
    z_reach_pos = true;
    cll_state = 0;
    move_err_tmout_en(FALSE);
  }
  z_running_state = false; 
  prev_z_reach_pos = z_reach_pos;

  if (val >> 0 & 1)
  {
    return LEAD_RECV_FAIL;
  } 
  else
    return LEAD_RECV_OK;

}


#endif