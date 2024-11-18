/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include <string.h>


enum mt_com_state mt_xstate=stanby;
enum mt_com_state mt_ystate=stanby;
enum mt_com_state mt_zstate=stanby;



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
};

void stm_memory_init()
{
  //int32_t spd, acc;
  err_tmout_set(errMemory);
  xmt_param_read();
  ymt_param_read();
  zmt_param_read();
  err_tmout_en(FALSE);
  if(xmt_ctrl.bath_pos==0||xmt_ctrl.sample_width==(~0)){
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
    
  if(ymt_ctrl.bath_pos==0||ymt_ctrl.sample_width==(~0)){
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
  if(zmt_ctrl.bath_pos==0||zmt_ctrl.sample_width==(~0)){
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
}

void stm_reset()
{

  edbmt_cmd_send(ADDR_MOTOR_X,INST_RESET,0,0,0);//Homeing valiable
  
  HAL_Delay(500);
  edbmt_cmd_send(ADDR_MOTOR_X,INST_RUNAPP,EDB2000_FROME_SPECIL_ADD,0,ADDR_MOTOR_X);//Homeing valiable
  HAL_Delay(500);
  edbmt_cmd_send(ADDR_MOTOR_Y,INST_RESET,0,0,0);//Homeing valiable
  HAL_Delay(500);
  edbmt_cmd_send(ADDR_MOTOR_Y,INST_RUNAPP,EDB2000_FROME_SPECIL_ADD,0,ADDR_MOTOR_Y);//Homeing valiable
  HAL_Delay(500);
  edbmt_cmd_send(ADDR_MOTOR_Z,INST_RESET,0,0,0);//Homeing valiable
  HAL_Delay(500);

  edbmt_cmd_send(ADDR_MOTOR_Z,INST_RUNAPP,EDB2000_FROME_SPECIL_ADD,0,ADDR_MOTOR_Z);//Homeing valiable

}

void all_homeing()
{
  give_event(eventAllhomeCk,0);
}

void x_homeing()
{

    edbmt_cmd_send(ADDR_MOTOR_X,INST_SGP,0,2,HOMEING);//Homeing valiable
    set_timer_(eventXhomeCk,100,0);
  
}


void y_homeing()
{
    edbmt_cmd_send(ADDR_MOTOR_Y,INST_SGP,0,2,HOMEING);//Homeing valiable
    set_timer_(eventYhomeCk,100,0);
}


void z_homeing()
{

    edbmt_cmd_send(ADDR_MOTOR_Z,INST_SGP,0,2,HOMEING);//Homeing valiable
    set_timer_(eventZhomeCk,100,0);

}

void stmt_speed_set(byte address,signed long int value)
{
  int32_t spd, acc;
  
  spd=(value*51200)/60;
  acc=(value*51200)/60;
  //edbmt_cmd_send(ADDR_MOTOR_X,INST_SAP,EDB2000_CL_CURRENT_MIN_TYPE,0,xmt_ctrl.cl_amp);
  //dwHsSW_Delay_ms(100);
  edbmt_cmd_send(address,INST_SAP,EDB2000_MAX_SPEED_TYPE,0,spd);
  dwHsSW_Delay_ms(400);
  edbmt_cmd_send(address,INST_SAP,EDB2000_ACCELERATOR_TYPE,0,acc);
  dwHsSW_Delay_ms(400);
}

void stmt_accel_set(byte address,signed long int value)
{
   mt_xstate=stanby;
   edbmt_cmd_send(address,INST_SAP,EDB2000_ACCELERATOR_TYPE,0,value);
   while(mt_xstate==busy);
   
}

void stmt_decel_set(byte address,signed long int value)
{
  mt_xstate=stanby;
  edbmt_cmd_send(address,INST_SAP,EDB2000_DECELERATE_TYPE,0,value);
  while(mt_xstate==busy);
   
}


bool x_reach_pos = true, y_reach_pos = true, z_reach_pos = true;
signed long int x_prev_value = 0,y_prev_value = 0,z_prev_value = 0;


bool stmt_abs_move(byte address,signed long int value){
  switch(address)
  {
  case ADDR_MOTOR_X:
    if(x_reach_pos){
      x_prev_value = value;
      x_reach_pos=false;
      mt_xstate=stanby;
      edbmt_cmd_send(address,INST_MVP,EDB2000_MOVE_ABS_TYPE,0,value);
      
      set_timer_(eventXAbsPosCk,100,0);
    }

    break;
  case ADDR_MOTOR_Y:
    if(y_reach_pos){
      y_prev_value = value;
      y_reach_pos=false;
      mt_ystate=stanby;
      edbmt_cmd_send(address,INST_MVP,EDB2000_MOVE_ABS_TYPE,0,value);

      set_timer_(eventYAbsPosCk,100,0);
    }

    break;
  case ADDR_MOTOR_Z:
    if(z_reach_pos){
      z_prev_value = value;
      z_reach_pos=false;
      edbmt_cmd_send(address,INST_MVP,EDB2000_MOVE_ABS_TYPE,0,value);
      mt_zstate=stanby;
      
      set_timer_(eventZAbsPosCk,100,0);
    }
    break;
  default:
    break;
  }
 
  if(address==ADDR_MOTOR_X) return x_reach_pos;
  if(address==ADDR_MOTOR_Y) return y_reach_pos; 
  if(address==ADDR_MOTOR_Z) return z_reach_pos; 
  
  return 0;
}


bool stmt_rel_move(byte address,signed long int value){
  switch(address)
  {
  case ADDR_MOTOR_X:
    if(x_reach_pos){
      x_reach_pos=false;
      mt_xstate=stanby;
      edbmt_cmd_send(address,INST_MVP,EDB2000_MOVE_REL_TYPE,0,value);
      set_timer_(eventXAbsPosCk,100,0);
    }
    break;
  case ADDR_MOTOR_Y:
    if(y_reach_pos){
      y_reach_pos=false;
      mt_ystate=stanby;
      edbmt_cmd_send(address,INST_MVP,EDB2000_MOVE_REL_TYPE,0,value);
      set_timer_(eventYAbsPosCk,100,0);
    }
    break;
  case ADDR_MOTOR_Z:
    if(z_reach_pos){
      z_reach_pos=false;
      edbmt_cmd_send(address,INST_MVP,EDB2000_MOVE_REL_TYPE,0,value);
      mt_zstate=stanby;
      set_timer_(eventZAbsPosCk,100,0);
    }
    break;
  default:
    break;
  }
 
  if(address==ADDR_MOTOR_X) return x_reach_pos;
  if(address==ADDR_MOTOR_Y) return y_reach_pos; 
  if(address==ADDR_MOTOR_Z) return z_reach_pos; 
  
  return 0;
}


void stmt_abs_home_move(byte address,signed long int value){
  switch(address)
  {
  case ADDR_MOTOR_X:
    x_reach_pos=false;
    mt_xstate=stanby;
    edbmt_cmd_send(address,INST_MVP,EDB2000_MOVE_ABS_TYPE,0,value);
  //  while(mt_xstate==busy);
    set_timer_(eventHomeXAbsPosCk,100,0);
    break;
  case ADDR_MOTOR_Y:
    y_reach_pos=false;
    mt_ystate=stanby;
    edbmt_cmd_send(address,INST_MVP,EDB2000_MOVE_ABS_TYPE,0,value);
    //while(mt_ystate==busy);
    set_timer_(eventHomeYAbsPosCk,100,0);
    break;
  case ADDR_MOTOR_Z:
    z_reach_pos=false;
    mt_zstate=stanby;
    edbmt_cmd_send(address,INST_MVP,EDB2000_MOVE_ABS_TYPE,0,value);
    //while(mt_zstate==busy);
    set_timer_(eventHomeZAbsPosCk,100,0);
    break;
  default:
    break;
  }  
}

byte mt_stat_chek()
{
  return mt_xstate;
}

void stm_speed_set(byte address, byte inst, byte type ,byte bank, signed long int value)
{
   edbmt_cmd_send(address,INST_SAP,EDB2000_MAX_SPEED_TYPE,0,value);
}

byte trans_timout_cnt=3;

  void mt_pram_set(byte address, int32_t spd_in, int32_t acc_in, int32_t dcc_in)
{
  
  int32_t spd, acc, dcc;
  
  spd=(spd_in*51200)/60;
  acc=(acc_in*51200)/60*10;
  dcc=(dcc_in*51200)/60*10;

  edbmt_cmd_send(address,INST_SAP,EDB2000_MAX_SPEED_TYPE,0,spd);
  dwHsSW_Delay_ms(100);
  edbmt_cmd_send(address,INST_SAP,EDB2000_ACCELERATOR_TYPE,0,acc);
  dwHsSW_Delay_ms(100);
  edbmt_cmd_send(address,INST_SAP,EDB2000_DECELERATE_TYPE,0,dcc);
  dwHsSW_Delay_ms(100);
}


void x_pram_set()
{
  int32_t spd, acc;
  
  xmt_ctrl.speed_rpm=100;
  xmt_ctrl.accel_rpm=1000;
  spd=(xmt_ctrl.speed_rpm*51200)/60;
  acc=(xmt_ctrl.accel_rpm*51200)/60;
  //edbmt_cmd_send(ADDR_MOTOR_X,INST_SAP,EDB2000_CL_CURRENT_MIN_TYPE,0,xmt_ctrl.cl_amp);
  //dwHsSW_Delay_ms(100);
  edbmt_cmd_send(ADDR_MOTOR_X,INST_SAP,EDB2000_MAX_SPEED_TYPE,0,spd);
  dwHsSW_Delay_ms(100);
  edbmt_cmd_send(ADDR_MOTOR_X,INST_SAP,EDB2000_ACCELERATOR_TYPE,0,acc);
  dwHsSW_Delay_ms(100);
}

void y_pram_set()
{
  int32_t spd, acc;
  spd=(ymt_ctrl.speed_rpm*51200)/60;
  acc=(ymt_ctrl.accel_rpm*51200)/60;
 // edbmt_cmd_send(ADDR_MOTOR_Y,INST_SAP,EDB2000_CL_CURRENT_MIN_TYPE,0,ymt_ctrl.cl_amp);
 // dwHsSW_Delay_ms(100);
  edbmt_cmd_send(ADDR_MOTOR_Y,INST_SAP,EDB2000_MAX_SPEED_TYPE,0,spd);
  dwHsSW_Delay_ms(100);
  edbmt_cmd_send(ADDR_MOTOR_Y,INST_SAP,EDB2000_ACCELERATOR_TYPE,0,acc);
  dwHsSW_Delay_ms(100);
}

void z_pram_set()
{
  int32_t spd, acc;
  spd=(zmt_ctrl.speed_rpm*51200)/60;
  acc=(zmt_ctrl.accel_rpm*51200)/60;
  //edbmt_cmd_send(ADDR_MOTOR_Z,INST_SAP,EDB2000_CL_CURRENT_MIN_TYPE,0,zmt_ctrl.cl_amp);
  //dwHsSW_Delay_ms(100);
  edbmt_cmd_send(ADDR_MOTOR_Z,INST_SAP,EDB2000_MAX_SPEED_TYPE,0,spd);
  dwHsSW_Delay_ms(100);
  edbmt_cmd_send(ADDR_MOTOR_Z,INST_SAP,EDB2000_ACCELERATOR_TYPE,0,acc);
  dwHsSW_Delay_ms(100);
}

uint x_mt_send_cnt=0;

uint x_reach_count , y_reach_count , z_reach_count;


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
      stmt_abs_home_move(ADDR_MOTOR_X,xmt_ctrl.bath_pos);
      asp_home_init();
      break;
    }else{
      edbmt_cmd_send(ADDR_MOTOR_X, INST_GGP, 1, 2, 0);
      set_timer_(eventXhomeCk,200,0);
    }
    
    break;
  case eventHomeXAbsPosCk:
    if(!x_reach_pos){
      edbmt_cmd_send(ADDR_MOTOR_X, INST_GAP, EDB2000_POSITION_REACHED_TYPE,0, 0);
      set_timer_(eventHomeXAbsPosCk,100,0);  
    }else{
      edbmt_cmd_send(ADDR_MOTOR_X, INST_SAP, EDB2000_ACTUAL_POSITION_TYPE,0,0);//position 0
    }  
    break;
  case eventXAbsPosCk:
    if(!x_reach_pos){
      edbmt_cmd_send(ADDR_MOTOR_X, INST_GAP, EDB2000_POSITION_REACHED_TYPE,0, 0);
      set_timer_(eventXAbsPosCk,150,0);  
    }else
      x_reach_pos=true;
    break;
  case eventYhomeCk:
    if(mt_ystate==stanby_home){
      stmt_abs_home_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
      bath_ypos_temp=ymt_ctrl.bath_pos;
      //y_pram_set();
      break;
    }else{
      edbmt_cmd_send(ADDR_MOTOR_Y, INST_GGP, 1, 2, 0);
      set_timer_(eventYhomeCk,200,0);
    }    
    break;
  case eventHomeYAbsPosCk:
    if(!y_reach_pos){
      edbmt_cmd_send(ADDR_MOTOR_Y, INST_GAP, EDB2000_POSITION_REACHED_TYPE,0, 0);
      set_timer_(eventHomeYAbsPosCk,100,0);  
    }else{
      edbmt_cmd_send(ADDR_MOTOR_Y, INST_SAP, EDB2000_ACTUAL_POSITION_TYPE,0,0);//position 0
    }  
    break;
  case eventYAbsPosCk:
    if(!y_reach_pos){
      edbmt_cmd_send(ADDR_MOTOR_Y, INST_GAP, EDB2000_POSITION_REACHED_TYPE,0, 0);
      set_timer_(eventYAbsPosCk,150,0);  
    }else
      y_reach_pos=true;
    break;
    
  case eventZhomeCk:    
    if(mt_zstate==stanby_home){
      stmt_abs_home_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);
       bath_zpos_temp=zmt_ctrl.bath_pos;
     // z_pram_set();
      break;
    }else{
      edbmt_cmd_send(ADDR_MOTOR_Z, INST_GGP, 1, 2, 0);
      set_timer_(eventZhomeCk,200,0);
    }     
    break;
  case eventHomeZAbsPosCk:
    if(!z_reach_pos){
      edbmt_cmd_send(ADDR_MOTOR_Z, INST_GAP, EDB2000_POSITION_REACHED_TYPE,0, 0);
      set_timer_(eventHomeZAbsPosCk,100,0);  
    }else{
      edbmt_cmd_send(ADDR_MOTOR_Z, INST_SAP, EDB2000_ACTUAL_POSITION_TYPE,0,0);//position 0
    }  
    break;
  case eventZAbsPosCk:
    if(!z_reach_pos){
      edbmt_cmd_send(ADDR_MOTOR_Z, INST_GAP, EDB2000_POSITION_REACHED_TYPE,0, 0);
      set_timer_(eventZAbsPosCk,100,0);  
    }else
      z_reach_pos=true;
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
    spd=(xmt_ctrl.speed_rpm*51200)/60;
    edbmt_cmd_send(ADDR_MOTOR_X,INST_SAP,EDB2000_MAX_SPEED_TYPE,0,spd);
    usb_send_pack(hseXaxiSpeedSet,usb_data_buf);
    break;
  case hseXaxiSpeedRead:
    sort_8bit(xmt_ctrl.speed_rpm,dev_send_buf);
    usb_send_pack(hseXaxiSpeedRead, dev_send_buf);
    break;
  case hseXaxiAccelSet:
    xmt_ctrl.accel_rpm=merge_32bit(xmt_ctrl.accel_rpm,usb_data_buf);
    acc=(xmt_ctrl.accel_rpm*51200)/60;
    edbmt_cmd_send(ADDR_MOTOR_X,INST_SAP,EDB2000_ACCELERATOR_TYPE,0,acc);
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
    usb_send_pack(hseProbeTrayXSave,usb_data_buf);
    break;
  case hseXaxiDspPosSave:
    xmt_ctrl.dsp_pos=merge_32bit(xmt_ctrl.dsp_pos,usb_data_buf);
    xmt_param_write();
    usb_send_pack(hseXaxiDspPosSave,usb_data_buf);
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
    usb_send_pack(hseProbeStripGapSave,usb_data_buf);
    break;  
  case hseXaxiCamPosSave:
    xmt_ctrl.cam_pos=merge_32bit(xmt_ctrl.cam_pos,usb_data_buf);
    xmt_param_write();
    usb_send_pack(hseXaxiCamPosSave,usb_data_buf);
    break;  

  case hseProbeStripGapRead:
  case hseXaxiCamPosRead:
    sort_8bit(xmt_ctrl.cam_pos,dev_send_buf);
    xmt_param_read();
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
    usb_send_pack(hseProbeSmplXSave,usb_data_buf);
    break;

  case hseXaxiSamplePosSave:
    xmt_ctrl.sample_pos=merge_32bit(xmt_ctrl.sample_pos,usb_data_buf);
    xmt_param_write();
    usb_send_pack(hseXaxiSamplePosSave,usb_data_buf);
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
    ymt_ctrl.cam_pos=merge_32bit(ymt_ctrl.cam_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X,  ymt_ctrl.cam_pos);
    ymt_param_write();   
    usb_send_pack(hseProbeSampleGapSet,usb_data_buf);
    break;
  
  case hseProbeSampleGapSave:
    usb_send_pack(hseProbeSampleGapSave,usb_data_buf);
    break;    

   case hseXaxiClCurrentSave:
    ymt_ctrl.cam_pos=merge_32bit(ymt_ctrl.cam_pos,usb_data_buf);
    ymt_param_write();
    usb_send_pack(hseXaxiClCurrentSave,usb_data_buf);
    break;     
  case hseProbeSampleGapRead:
  case hseXaxiClCurrentRead:
    //sort_8bit(xmt_ctrl.cl_amp,dev_send_buf);
    sort_8bit(ymt_ctrl.cam_pos,dev_send_buf);
    ymt_param_read();
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
    usb_send_pack(hseProbeSmplXEndSave,usb_data_buf);
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
    usb_send_pack(hseProbeTrayXEndSave,usb_data_buf);
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
  case hseYaxiSpeedSet:
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
    break;
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
    //stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos-bath_ypos_temp);
    stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
    usb_send_pack(hseProbeBathYSet,usb_data_buf);
    break;
  
  case hseProbeBathYSave:
    usb_send_pack(hseProbeBathYSave,usb_data_buf);
    break;

  case hseYaxiBathPosSave:
    ymt_ctrl.bath_pos=merge_32bit(ymt_ctrl.bath_pos,usb_data_buf);
    ymt_param_write();
    ymt_param_read();
    usb_send_pack(hseYaxiBathPosSave,usb_data_buf);
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
    usb_send_pack(hseProbeTrayYSave,usb_data_buf);
    break;    

  case hseYaxiDspPosSave:
    ymt_ctrl.dsp_pos=merge_32bit(ymt_ctrl.dsp_pos,usb_data_buf);
    ymt_param_write();
    usb_send_pack(hseYaxiDspPosSave,usb_data_buf);
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
    usb_send_pack(hseProbeBathInitSave,usb_data_buf); 
    break;

  case hseYaxiBathSamplePosSave:
    ymt_ctrl.sample_width=merge_32bit(ymt_ctrl.sample_width,usb_data_buf);
    ymt_param_write();
    usb_send_pack(hseYaxiBathSamplePosSave,usb_data_buf); 
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
    usb_send_pack(hseProbeSmplY1Save,usb_data_buf);
    break;
  case hseYaxiSamplePosSave:
    ymt_ctrl.sample_pos=merge_32bit(ymt_ctrl.sample_pos,usb_data_buf);
    ymt_param_write();
    usb_send_pack(hseYaxiSamplePosSave,usb_data_buf);
    break;



  case hseProbeSmplY1Read:
  case hseYaxiSamplePosRead:
    ymt_param_read();
    sort_8bit(ymt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseProbeSmplY1Read, dev_send_buf);
    break;
  case hseYaxiClCurrentSet:
    ymt_ctrl.cl_amp=merge_32bit(ymt_ctrl.cl_amp,usb_data_buf);
     edbmt_cmd_send(ADDR_MOTOR_Y,INST_SAP,EDB2000_CL_CURRENT_MIN_TYPE,0,ymt_ctrl.cl_amp);
    usb_send_pack(hseYaxiClCurrentSet,usb_data_buf);
    break;
  case hseYaxiClCurrentRead:
    sort_8bit(ymt_ctrl.cl_amp,dev_send_buf);
    usb_send_pack(hseYaxiClCurrentRead, dev_send_buf);
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
    usb_send_pack(hseProbeSmplY2Save,usb_data_buf);
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
    spd=(zmt_ctrl.speed_rpm*51200)/60;
    edbmt_cmd_send(ADDR_MOTOR_Z,INST_SAP,EDB2000_MAX_SPEED_TYPE,0,spd);
    usb_send_pack(hseZaxiSpeedSet,usb_data_buf);
    break;
  case hseZaxiSpeedRead:
    sort_8bit(zmt_ctrl.speed_rpm,dev_send_buf);
    usb_send_pack(hseZaxiSpeedRead, dev_send_buf);
    break;
  case hseZaxiAccelSet:
    zmt_ctrl.accel_rpm=merge_32bit(zmt_ctrl.accel_rpm,usb_data_buf);
    acc=(zmt_ctrl.accel_rpm*51200)/60;
    edbmt_cmd_send(ADDR_MOTOR_Z,INST_SAP,EDB2000_ACCELERATOR_TYPE,0,acc);
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
    //zmt_ctrl.bath_pos=merge_32bit(zmt_ctrl.bath_pos,usb_data_buf);
    zmt_param_write();
   // stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos-bath_zpos_temp);
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.dsp_pos_end);
    usb_send_pack(hseProbeBathZSet,usb_data_buf);
    break;
  case hseProbeBathZSave:
    usb_send_pack(hseProbeBathZSave,usb_data_buf); 
    break;
  case hseZaxiBathPosSave:
    zmt_ctrl.dsp_pos_end=merge_32bit(zmt_ctrl.dsp_pos_end,usb_data_buf);
    zmt_param_write();
    usb_send_pack(hseZaxiBathPosSave,usb_data_buf); 
    break;
  case hseProbeBathZRead:
  case hseZaxiBathPosRead:
    zmt_param_read();
    sort_8bit(zmt_ctrl.dsp_pos_end,dev_send_buf);
    //sort_8bit(zmt_ctrl.bath_pos,dev_send_buf);
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
    usb_send_pack(hseProbeTrayZSave,usb_data_buf);
    break;
  case hseZaxiDspPosSave:
    zmt_ctrl.dsp_pos=merge_32bit(zmt_ctrl.dsp_pos,usb_data_buf);
    zmt_param_write();
    usb_send_pack(hseZaxiDspPosSave,usb_data_buf);
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
    edbmt_cmd_send(ADDR_MOTOR_Z,INST_SGP,4,2, zmt_ctrl.sample_pos);
    usb_send_pack(hseProbeSmplZSet,usb_data_buf);
    break;
  case hseProbeSmplZSave:
    usb_send_pack(hseProbeSmplZSave,usb_data_buf);
    break;
  case hseZaxiSamplePosSave:
    zmt_ctrl.sample_pos=merge_32bit(zmt_ctrl.sample_pos,usb_data_buf);
    zmt_param_write();
    usb_send_pack(hseZaxiSamplePosSave,usb_data_buf);
    break;
  case hseProbeSmplZRead:
  case hseZaxiSamplePosRead:
    zmt_param_read();
    sort_8bit(zmt_ctrl.sample_pos,dev_send_buf);
    usb_send_pack(hseProbeSmplZRead, dev_send_buf);
    break;
    
  case hseZaxiClCurrentSet:
    zmt_ctrl.cl_amp=merge_32bit(zmt_ctrl.cl_amp,usb_data_buf);
    edbmt_cmd_send(ADDR_MOTOR_Z,INST_SAP,EDB2000_CL_CURRENT_MIN_TYPE,0,zmt_ctrl.cl_amp); 
    usb_send_pack(hseZaxiClCurrentSet,usb_data_buf);
    break;
  case hseZaxiClCurrentRead:
    sort_8bit(zmt_ctrl.cl_amp,dev_send_buf);
    usb_send_pack(hseZaxiClCurrentRead, dev_send_buf);
    break;
  case hseProbePage:
        state=stStEng;
  //  bath_ypos_temp=ymt_ctrl.bath_pos;
   // bath_zpos_temp=zmt_ctrl.bath_pos;
    usb_send_pack(hseProbePage, 0);
    break;
  default:
    break;
  }
  return event;
}




//-----------------------------------------------------------------------------
//2024 02 29 code �߰�

//EDB Queue ==============================================================
#define EDB_QUE_SZ	100

#define EDB_QUE_OK	0
#define EDB_QUE_FULL	1
#define EDB_QUE_EMPTY	2

typedef struct
{
  uint32_t dwTail;
  uint32_t dwHead;
  uint8_t uQueBuf[EDB_QUE_SZ];
}hsEDB_Que_t;

hsEDB_Que_t g_tEDB_Que_Handle;

int32_t EDB_ReceivePutQue(uint8_t uPutData)
{
  int32_t dwTail;

  dwTail = (g_tEDB_Que_Handle.dwTail + 1)%EDB_QUE_SZ;

  if( dwTail == g_tEDB_Que_Handle.dwHead) return EDB_QUE_FULL;

  g_tEDB_Que_Handle.uQueBuf[g_tEDB_Que_Handle.dwTail] = uPutData;

  g_tEDB_Que_Handle.dwTail = dwTail;
          
  return EDB_QUE_OK;
}

int32_t EDB_ReceiveGetQue(uint8_t *p_uGetData)
{
  if( g_tEDB_Que_Handle.dwTail == g_tEDB_Que_Handle.dwHead ) return EDB_QUE_EMPTY;

  *p_uGetData = g_tEDB_Que_Handle.uQueBuf[g_tEDB_Que_Handle.dwHead];

  g_tEDB_Que_Handle.dwHead = (g_tEDB_Que_Handle.dwHead + 1)%EDB_QUE_SZ;

  return EDB_QUE_OK;
}

void EDB_QueReset(void)
{
  __disable_irq();
  g_tEDB_Que_Handle.dwHead = 0;
  g_tEDB_Que_Handle.dwTail = 0;
  __enable_irq();
  memset(g_tEDB_Que_Handle.uQueBuf,0,sizeof(g_tEDB_Que_Handle.uQueBuf));
}

uint32_t EDB_QueueHaveSize(void)
{
  
  uint32_t dwReturnData;
  __disable_irq(); 

  if (g_tEDB_Que_Handle.dwHead <= g_tEDB_Que_Handle.dwTail)
      dwReturnData = g_tEDB_Que_Handle.dwTail - g_tEDB_Que_Handle.dwHead;
  else
  {
      dwReturnData = EDB_QUE_SZ - g_tEDB_Que_Handle.dwHead;
      dwReturnData = dwReturnData + g_tEDB_Que_Handle.dwTail;
  }
  __enable_irq(); 
  
  return dwReturnData;

}


//IRQ ==================================================================
uint32_t g_dwEDB_Start = 0;
extern UART_HandleTypeDef huart4;
extern byte mt_chr;

void UART_EDB_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart4)
  {
    int32_t dwQueCheck;
  
    dwQueCheck = EDB_ReceivePutQue(mt_chr);
    if(dwQueCheck == EDB_QUE_FULL)
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
    return HAL_OK;
}

//==========================================================================

//EDB Motor code ===========================================================
enum
{
  SND_EDB_ADD = 0,
  SND_EDB_INST,
  SND_EDB_TYPE,
  SND_EDB_BANK,
  SND_EDB_DAT1,
  SND_EDB_DAT2,
  SND_EDB_DAT3,
  SND_EDB_DAT4,
  SND_EDB_CKSUM,
          
  SND_EDB_MAX
};

enum
{
  RCV_EDB_HOST = 0,
  RCV_EDB_DEV,
  RCV_EDB_STATUS,
  RCV_EDB_INST,
  RCV_EDB_DAT1,
  RCV_EDB_DAT2,
  RCV_EDB_DAT3,
  RCV_EDB_DAT4,
  RCV_EDB_CKSUM,
  RCV_EDB_MAX = 50
};

#define EDB_HOST_ID             1
#define EDB_LENGTH        9
#define EDB_RE_TRY_MAX		5
#define EDB_RX_WAIT_DELAY	5
#define EDB_COM_WAIT_TIM        50

#define EDB_RECV_OK             0
#define EDB_RECV_FAIL           1

#define EDB_STATUS_OK           100

bool *gp_bReach_Pos;
enum mt_com_state *gp_tStatus;

int32_t Get_EDB_ReceivPacketHandle(uint8_t uAddress, uint8_t *p_uGetPacketBuf);
int32_t EDB_ReceivPacketExec(uint8_t *p_uReceivPacket);


void edbmt_cmd_send(byte address, byte inst, byte type ,byte bank, signed long int value)
{
  uint32_t dwCnt = 0,dwCount = 0,dwCheck = 0, dwReTryCnt = 0,EDB_queue_size = 0, HAL_Fail_Cnt = 0;
  uint8_t EDBSendPacket[SND_EDB_MAX] = {0};
  uint8_t EDBReceivPacket[RCV_EDB_MAX] = {0};
  
  
  //Address�� ���� ����
  switch(address)
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

  //SendPacket ���� 
  EDBSendPacket[SND_EDB_ADD] = address;
  EDBSendPacket[SND_EDB_INST] = inst;
  EDBSendPacket[SND_EDB_TYPE] = type;
  EDBSendPacket[SND_EDB_BANK] = bank;
  EDBSendPacket[SND_EDB_DAT1] = value >> 24;
  EDBSendPacket[SND_EDB_DAT2] = value >> 16;
  EDBSendPacket[SND_EDB_DAT3] = value >>  8;
  EDBSendPacket[SND_EDB_DAT4] = value;
  EDBSendPacket[SND_EDB_CKSUM]= 0;

  for(dwCnt = 0; dwCnt < SND_EDB_CKSUM; dwCnt++)
  {
    EDBSendPacket[SND_EDB_CKSUM] += EDBSendPacket[dwCnt];
  }

  *gp_tStatus = busy;

  //���� ���� ����
  while(1)
  {
    HAL_Delay(50);
    EDB_QueReset();
    HAL_GPIO_WritePin(UART4_DIR_GPIO_Port, UART4_DIR_Pin, (GPIO_PinState)SET);
    dwCheck = HAL_UART_Transmit(&huart4, EDBSendPacket, SND_EDB_MAX, EDB_COM_WAIT_TIM);
    HAL_GPIO_WritePin(UART4_DIR_GPIO_Port, UART4_DIR_Pin, (GPIO_PinState)RESET);
    
    if(dwCheck)
    {
      HAL_Fail_Cnt++;
      UART4_ReInit();
      if(HAL_Fail_Cnt > 5) 
      {
        while(1);
      }
    }
    for(dwCount = 0; dwCnt < 500; dwCount++)
    {
      HAL_Delay(5);
      EDB_queue_size=EDB_QueueHaveSize();
      if(EDB_queue_size >= EDB_LENGTH)
      { 
        break;
      }  
    }
    dwCheck = Get_EDB_ReceivPacketHandle(address, EDBReceivPacket);
    if(dwCheck == EDB_RECV_OK)
    {
      if( EDBReceivPacket[RCV_EDB_STATUS] == EDB_STATUS_OK) 
      {
        break;
      }
      else
      {
        //Error Code
      }
    }
    dwReTryCnt++;
    HAL_Delay(100);
    if(dwReTryCnt > EDB_RE_TRY_MAX)
    {
      //Error code
      while(1);
    }
  }
  
  //���� ��Ŷ ����
  dwCheck = EDB_ReceivPacketExec(EDBReceivPacket);
  if(dwCheck)
  {
    while(1);//Error code  
  }
  //Error Count Stop
  
  err_tmout_en(FALSE); 
}

int32_t Get_EDB_ReceivPacketHandle(uint8_t uAddress, uint8_t *p_uGetPacketBuf)
{
  uint32_t dwCnt, dwCkSumCnt;
  uint8_t uCheckSum = 0;
  int32_t dwQueCheck;
  uint8_t uDumy;

  for(dwCnt = 0; ; dwCnt++)
  {
    if(dwCnt <= 50)
    { 
      dwQueCheck = EDB_ReceiveGetQue(&p_uGetPacketBuf[dwCnt]);
    }  
    else dwQueCheck = EDB_ReceiveGetQue(&uDumy);
    
    if(dwQueCheck == EDB_QUE_EMPTY) break;
  }
  if(dwCnt >= 9)
  {
    for(dwCkSumCnt = 0; dwCkSumCnt < RCV_EDB_CKSUM; dwCkSumCnt++)
    {
      uCheckSum += p_uGetPacketBuf[dwCkSumCnt];
    }
    if((p_uGetPacketBuf[RCV_EDB_HOST] == EDB_HOST_ID) && (p_uGetPacketBuf[RCV_EDB_DEV] == uAddress) && (p_uGetPacketBuf[RCV_EDB_CKSUM] == uCheckSum))
    {
      return EDB_RECV_OK;
    }
  }
  return EDB_RECV_FAIL;
}

int32_t EDB_ReceivPacketExec(uint8_t *p_uReceivPacket)
{
  int32_t dwCheck = 0;
  uint32_t dwReceivValue;
  uint8_t uCmd, uAddress;
  
  uCmd = p_uReceivPacket[RCV_EDB_INST];
  uAddress = p_uReceivPacket[RCV_EDB_DEV];
  dwReceivValue = p_uReceivPacket[RCV_EDB_DAT4] & 0x000000FF;
  
  switch(uCmd)
  {
    case INST_MVP:
    case INST_SAP:
    case INST_RESET:
    case INST_RUNAPP:
    case INST_STOP:
      *gp_tStatus = stanby;
      break;
      
    case INST_GAP:
      *gp_tStatus = stanby;   
      if(uAddress == ADDR_MOTOR_X){
        x_reach_pos = dwReceivValue;  
      }
      else if(uAddress == ADDR_MOTOR_Y){
        y_reach_pos = dwReceivValue;  
      }
      else if(uAddress == ADDR_MOTOR_Z){
        z_reach_pos = dwReceivValue;  
      }
      break;
      
    case INST_SGP:
      if( (dwReceivValue == HOMEING) || (dwReceivValue == CLL_EN) || (dwReceivValue == SWS_OP) || (dwReceivValue == CLLD_HOME) )
      {
        *gp_tStatus = stanby;   
      }
      else if(dwReceivValue == 0)
      {
        if(uAddress == ADDR_MOTOR_Z) *gp_tStatus = stanby;
      }
      else
      {
        if(uAddress == ADDR_MOTOR_Z) *gp_tStatus = stanby;
      }
      break;
    case INST_GGP:
      if(dwReceivValue == 1)
      {
        *gp_tStatus = stanby_home;
      }
      else if( (dwReceivValue == 5) || (dwReceivValue == 6) )
      {
        if(uAddress == ADDR_MOTOR_Z)
        {
          *gp_tStatus = stanby;   
          cll_state = dwReceivValue;
        }
      }
      else
      {
        *gp_tStatus = stanby;
      }
      break;
    default:
      dwCheck = 1;
      break;
  }
  
  return dwCheck;
}



