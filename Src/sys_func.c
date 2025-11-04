
#include "main.h"
struct drys dry;
struct barcode bar_pr= 
   {
    {eventBarScanInit,eventBarScan,eventBarScanOper,eventBarScanResult,eventBarScanOk,eventBarScanFail,eventBarScanEnd,eventBarScanReOper,eventBarScanCheck,
    },
    12,
    0,
    3,
  };

struct prime prime= 
   {
    {eventPrimeIinit,eventPrimeZixsCheck,eventPrimeBathPsn,eventPrimeAixsCheck,eventPrimeDspAspOut,eventPrimeDspAspEnd,\
     eventPrimeAixsCheck,eventPrimeEnd,
    },
    0,
    9000,
    15000,
  };

struct disp_single disp_sgl= 
{
  {eventDspIinit,eventDspSkAngle,eventDspStripMove,eventDspAixsCheck,eventDspOper,eventDspCheck,\
  eventDspAixsCheck,eventDspEnd
  },
  0,
  100,
  5000,
  5000,
  0,
  0,
  STRIP_NUM,
  
};


struct disp_asp diasp = 
{
  {eventDspAspIinit,eventDspAspSkAngle, eventDspAspStripMove,eventDspAspAixsCheck,\
   eventDspAspOut, eventDspAspOper, eventDspAspEnd,eventDspAspCheck,eventDspAspAixsCheck,eventDspAspFinalEnd,
    
  },
  0,
  1200,
  1700,
  5000,
  5000,
  0,
  0,
  STRIP_NUM,
} ;

struct aspration asp= 
{
  {eventAspIinit, eventAspSkAngle, eventAspStripMove,eventAspAixsCheck, eventAspOut,\
   eventAspOper, eventAspOperEnd,eventAspCheck,eventAspAixsCheck,eventAspEnd,
  },
  3000,
  STRIP_NUM,
};

struct probe_disp probe_disp= 
{
  {eventProbeDispIinit/*,eventProbeDispZMoveCheck*/,eventProbeDispSkAngle,eventProbeDispStripMove,eventProbeDispAixsCheck,\
   eventProbeDispProveMove, eventProbeDispZMoveCheck,eventProbeDispOper, eventProbeDispEnd, \
   eventProbeDispProbeHome, eventProbeDispZMoveCheck,  eventProbeDispCheck, eventProbeDispAixsCheck,eventProbeDispFinalEnd,
  },
  1000,
  47,

};


/*
struct sample_pr smpl_pr= 
{
  {
    eventSmpInit,eventSmpRackMove,eventSmpAbsMoveCheck,  
    eventSmpProbeSpeedSet,eventSmpClldMinPosMoveZ,eventSmpMoveZCheck,
    eventSmpClldVolInit,eventSmpClldRageSpeedSet, eventSmpClldRagePosMoveZ,\
    eventSmpCllCheck,
      
    eventSmpCll, eventSmpCllCheck,eventSmpStripMove,eventSmpAbsMoveCheck, \
    eventSmpStripDspMove,eventSmpStripMoveZCheck,                         \
    eventSmpStripHome,eventSmpStripMoveZCheck,                            \
    eventSmpNext,eventSmpEnd,eventSmpStripMoveXYCheck,                    \
    eventSmpPass
  },
  10,
  5000,
  3,
  30000,
  110000,
  0,
  80,
};
*/
struct sample_pr smpl_pr= 
{
  {
    eventSmpInit, eventSmpZinit, eventSmpMoveZCheck, eventSmpRackMove,eventSmpAbsMoveCheck,  
    eventSmpSWSInitCheck, eventSmpCll, eventSmpCllCheck, eventSmpSWSCheck, eventSmpSygAspOper,eventSmpCllVolCheck,//
    eventSmpProbeZHome,eventSmpMoveZCheck,eventSmpCllCheck,
    eventSmpStripMove,eventSmpAbsMoveCheck, eventSmpStripDspMove,eventSmpMoveZCheck,
    eventSmpSygDspOper,eventSmpClldRageSpeedSet, eventSmpEnd,eventSmpMoveZCheck, eventSmpFinalEnd,eventSmpAbsMoveCheck, //eventSmpPass,
    eventSmpNext,
  /*  eventSmpClldVolInit,eventSmpClldRageSpeedSet, eventSmpClldRagePosMoveZ,\

      
    eventSmpCll, eventSmpCllCheck,eventSmpStripMove,eventSmpAbsMoveCheck, \
    eventSmpStripDspMove,eventSmpStripMoveZCheck,                         \
    eventSmpStripHome,eventSmpStripMoveZCheck,                            \
    eventSmpNext,eventSmpEnd,eventSmpStripMoveXYCheck,                    \
    eventSmpPass*/
  },
  0,
  5000,
  3,
  0,
  5000,
};
  

struct sample_prime smpl_prime= 
{
  {eventSmpPrimeInit, eventSmpPrimeStripMoveZCheck, eventSmpPrimeBathMove, eventSmpPrimeAixsCheck, eventSmpPrimeStripMoveZ, eventSmpPrimeStripMoveZCheck,\
   eventSmpPrimeSygeSet, eventSmpPrimePumpOn,eventSmpPrimeStructInit,eventSmpPrimeSygeOper, eventSmpPrimeProveSet, 
   eventSmpPrimeProveMove, eventSmpPrimeStripMoveZCheck, eventSmpPrimeBathSuck,eventSmpPrimeBathInit,eventSmpPrimeEnd,
   eventSmpPrimeStripMoveZCheck,eventSmpPrimeFinalEnd
  },
  9,     //pump number
  12000, // pump timer
  3000,  // syge full move postion 
  5,     // syge rotation count
  14000, // prove move postion
  150,   // syge air struct position
};
      
       
struct auto_clean auto_cl=   
{
  {eventAutoCleanInit,eventAutoCleanZCheck,eventAutoCleanBathMove,eventAutoCleanAixsCheck,eventAutoCleanAspOut,\
   eventAutoCleanEnd, eventAutoCleanNext,eventAutoCleanAixsCheck,eventAutoCleanPass,
  },
  5,
  9000,
  30000,
  1000*60,
};
       
uint32_t qc_dry_time=0;
uint air_temper=0;
uint temper_volt = 135;  //13.5V
byte *ver_pnt=0;

bool only_washing_flg=false;
bool auto_clean_repeat_flg=false;
bool probe_disp_enable =false;
bool auto_prime_flg=false;
bool admin_positon_flag=false;

bool usb_retry_flag = false;

event execute_sys_ctrl(event event)
{
  uint16_t etc_vol=0;
  byte dev_send_buf[4]={0}; 
  uint16_t year=0,date=0;  

  switch(event)
  {
  case eventPosPag:
    state=stStby;
    admin_positon_flag=true;
    usb_send_pack(eventPosPag,0); 
    break;
  case eventLLDPag:
    state=stStEng;
    usb_send_pack(eventLLDPag,0); 
    break;
  case eventDispPag:
    state=stStEng;
    usb_send_pack(eventDispPag,0); 
    break;
  case eventCamPag:
    state=stStby;
    usb_send_pack(eventCamPag,0); 
    break;
  case eventEtcPag:
    state=stStby;
    usb_send_pack(eventEtcPag,0); 
    break;
    
  case eventXAxisHome:
    x_homeing();
    usb_send_pack(eventXAxisHome,0); 
    break;
  case eventYAxisHome:
    y_homeing();
    usb_send_pack(eventYAxisHome,0); 
    break;
  case eventZAxisHome:
    z_homeing();
    usb_send_pack(eventZAxisHome,0); 
    break;
  case eventTrayHome:
    give_event(eventSkrRollBack,0);
    usb_send_pack(eventTrayHome,0); 
    break;
  case eventASPHome:
    servo_mv(asp_mt.up_pos);
    servo_move_flag = false;
    dSPIN_Go_To(0);
    usb_send_pack(eventASPHome,0); 
    break;
  case hseDrySet:
    qc_dry_time=merge_32bit(qc_dry_time,usb_data_buf);
    HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_RESET);
    usb_send_pack(hseDrySet,usb_data_buf);
    set_timer_(hseDryOff,qc_dry_time,0);
    break;
  case hseDryOff:
    HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_SET);  
    break;
  case hseStausLedOn:
    cam_led_on();
    usb_send_pack(hseStausLedOn,0);
    break;
  case hseStausLedOff:
    cam_led_off();
    usb_send_pack(hseStausLedOff,0);
    break;
  case hseFanOn:
    HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_RESET);
    usb_send_pack(hseFanOn,0);
    break;
  case hseFanOff:
    HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_SET);
    usb_send_pack(hseFanOff,0);
  case hseDoorOn:
    usb_send_pack(hseDoorOn,0);
    break;
  case hseDoorOff:
    usb_send_pack(hseDoorOff,0);
    break;
  case eventPrimeFunc:
    prime.disp_pump_num=0x01<<usb_data_buf[0];
    if(prime.disp_pump_num==0x01<<WS_PUMP1)
      prime.disp_pump_num=(0x01<<WS_PUMP1)|(0x01<<WS_PUMP2);
    give_event(eventPrimeIinit,0);
    usb_send_pack((enum cntrl_event)event, usb_data_buf);
    break;
  case eventDispFunc:
    disp_sgl.disp_pump_num=0x01<<usb_data_buf[0];
    if(disp_sgl.disp_pump_num==0x01<<WS_PUMP1)
      disp_sgl.disp_pump_num=(0x01<<WS_PUMP1)|(0x01<<WS_PUMP2);
    disp_sgl.total_strip=usb_data_buf[1];
    etc_vol=usb_data_buf[2]<<8;
    etc_vol|=usb_data_buf[3];
    disp_sgl.disp_vol=(pm_pram.vol[usb_data_buf[0]-1]*etc_vol)/500;
    // give_event(eventDispAspFunc,0);
    stmt_speed_set(ADDR_MOTOR_X,300);
    stmt_speed_set(ADDR_MOTOR_Y,300);
    give_event(eventDspIinit,0);
    usb_send_pack((enum cntrl_event)event, usb_data_buf);
    break;
  case eventDispAspFunc:
    diasp.pump_num=0x01<<usb_data_buf[0];
    if(diasp.pump_num==0x01<<WS_PUMP1)
      diasp.pump_num=(0x01<<WS_PUMP1)|(0x01<<WS_PUMP2);
    diasp.total_strip=usb_data_buf[1];
    etc_vol=usb_data_buf[2]<<8;
    etc_vol|=usb_data_buf[3];
    diasp.vol=(pm_pram.vol[usb_data_buf[0]-1]*etc_vol)/500;
    stmt_speed_set(ADDR_MOTOR_X,300);
    stmt_speed_set(ADDR_MOTOR_Y,300);
    give_event(eventDspAspIinit,0);
    usb_send_pack((enum cntrl_event)event, usb_data_buf);
    break;
  case eventAspFunc:
    asp.total_strip=usb_data_buf[0];
    stmt_speed_set(ADDR_MOTOR_X,300);
    stmt_speed_set(ADDR_MOTOR_Y,300);
    give_event(eventAspIinit,0);
    usb_send_pack(eventAspFunc, usb_data_buf);
    break;
  case eventSakerFunc:
    shk_pram.run_time_s=merge_32bit( shk_pram.run_time_s,usb_data_buf);
    sk_state=stShkrStby;
    shake_run();
    usb_send_pack((enum cntrl_event)event, usb_data_buf);
    break;
  case eventSmplPrimeFunc:
    smpl_prime.pm_num=0x01<<usb_data_buf[0];
    lld_fc=lldFuncNone;
    smple_rack_cnt=0;
    give_event(eventSmpPrimeInit,0);
    usb_send_pack((enum cntrl_event)event, usb_data_buf);
    break;
  case eventAutoCleanFunc:
    auto_cl.rootin_delay=500;
    auto_cl.repeat=usb_data_buf[1];
    // smp_prime_dw_check_flg=false;
    auto_clean_repeat_flg=true;
    smpl_prime.pm_num=0x01<<WS_PUMP;//SMP_PRME_PP;
    smple_rack_cnt=0;
    lld_fc=lldFuncNone;
    probe_disp_enable=false;
    auto_prime_flg=true;
    set_timer_(eventSmpPrimeInit,1000,0);
    give_event(eventAutoCleanInit,0);
    usb_send_pack(eventAutoCleanFunc, 0);
    break;
  case eventRinseFunc:
    auto_cl.rootin_delay=1000*60*10;
    // smp_prime_dw_check_flg=false;
    auto_cl.repeat=usb_data_buf[1];
    auto_clean_repeat_flg=true;
    smpl_prime.pm_num=0x01<<WS_PUMP;//SMP_PRME_PP;
    smple_rack_cnt=0;
    lld_fc=lldFuncNone;
    set_timer_(eventSmpPrimeInit,1000,0);
    give_event(eventAutoCleanInit,0);
    usb_send_pack(eventAutoCleanFunc, 0);
    break;
    
  case eventWashingFunc:
    only_washing_flg=true; 
    full_step_cnt=0;
    for( ;;full_step_cnt++){
      if(sq[full_step_cnt].stNum==Washing_WS)
        break;
    }
    set_timer_(eventSqFullSqAly,500,0);
    diasp.total_strip=usb_data_buf[0];
    break;
  case eventAppExit:
    state=stPrepare;
    give_event(eventSpuStnby,0);
   // set_timer_(eventSpuStnby,5000,0);
    usb_send_pack(eventAppExitResp, usb_data_buf);
    break;
    
  case eventEquiType:
    if(state==stStby){
      dev_send_buf[0]=0x05;
      dev_send_buf[1]=0x08;

      ///FW_date//////////////////////////////////      
      dbg_serial_fw_date();
      //dbg_serial("MAIN_FW_DATE_250519");  //FW DATE
      hlld_send_pack(HLLD_ADD, HLLD_FW_DATE,0, 0);
      ////////////////////////////////////////////
      usb_send_pack(eventEquiType, dev_send_buf);
    }else{
      if(state==stReady)
        dev_send_buf[0]=0xff;
      else
        dev_send_buf[0]=0;
      
      usb_send_pack(_eventEnd, usb_data_buf);
    }
    break;
    
  case eventSeqSels:
    usb_send_pack(eventSeqSelRes, usb_data_buf);
   break;
   
    
  case eventAirTempRes:
    air_temper=Air_temp_res_vol(temper_volt);
    dev_send_buf[0]=usb_data_buf[0];
    dev_send_buf[1]=usb_data_buf[1];
    dev_send_buf[2]=air_temper>>8;
    dev_send_buf[3]=air_temper;
    usb_send_pack(eventAirTempRes, dev_send_buf);

    break;
  case eventFwVer:
    ver_pnt=FW_VER;
    dev_send_buf[0]=*ver_pnt;
    dev_send_buf[1]=*(ver_pnt+2);
    dev_send_buf[2]=*(ver_pnt+4);
    dev_send_buf[3]=*(ver_pnt+6);
    usb_send_pack(eventFwVer, dev_send_buf);
    break;
  case eventHwVer:
    ver_pnt=HW_VER;
    dev_send_buf[0]=*ver_pnt;
    dev_send_buf[1]=*(ver_pnt+2);
    dev_send_buf[2]=*(ver_pnt+4);
    dev_send_buf[3]=*(ver_pnt+6);
    usb_send_pack(eventHwVer, dev_send_buf);
    break;
  case eventConfig:
    beep_en=usb_data_buf[3]&0x01;
    if(beep_en)
      beep(80, 1);
    usb_send_pack(eventConfig, usb_data_buf);
    break;

  case TEST_MCU_FW_DATE_CHECK:
    year = (uint16_t)atoi(FW_YEAR);
    date = (uint16_t)atoi(FW_DATE);

    dev_send_buf[0] = (uint8_t)((year >> 8) & 0xFF); 
    dev_send_buf[1] = (uint8_t)(year & 0xFF);       
    dev_send_buf[2] = (uint8_t)((date >> 8) & 0xFF); 
    dev_send_buf[3] = (uint8_t)(date & 0xFF);      
    usb_send_pack(TEST_MCU_FW_DATE_CHECK,dev_send_buf);
    break;

  case TEST_LLD_FW_DATE_CHECK:
    hlld_send_pack(HLLD_ADD, HLLD_FW_DATE,0, 0);
    break;

  case event_Developer_MAIN_VER: // 버전 역순 표시 Blot2_Easy와 구조통일
    parse_version_string(DEVELOPER_MAIN_VER, dev_send_buf);
    usb_send_pack(event_Developer_MAIN_VER,dev_send_buf);
    break;
    
  case event_Developer_LLD_VER: // 버전 역순 표시 Blot2_Easy와 구조통일
    hlld_send_pack(HLLD_ADD, HLLD_DEVELOPER_VER_H,0, 0); 
    hlld_send_pack(HLLD_ADD, HLLD_DEVELOPER_VER_L,0, 0);
    break;

  case eventUSBreset:
    usb_send_pack(eventUSBreset,NULL);
    HAL_Delay(50);
    HAL_GPIO_WritePin(USB5V_OnOff_GPIO_Port,USB5V_OnOff_Pin,GPIO_PIN_RESET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(USB5V_OnOff_GPIO_Port,USB5V_OnOff_Pin,GPIO_PIN_SET);
    HAL_Delay(500);
    break;


    case eventUSBretry:
    usb_retry_flag = true;
    usb_repeat_en(EN);
    usb_send_pack(eventUSBretry,0);
    /*usb_send_pack(eventUSBreset,0);
    HAL_Delay(50);
    HAL_GPIO_WritePin(USB5V_OnOff_GPIO_Port,USB5V_OnOff_Pin,GPIO_PIN_RESET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(USB5V_OnOff_GPIO_Port,USB5V_OnOff_Pin,GPIO_PIN_SET);
    HAL_Delay(500);*/
    break;  


  default: break;
  }
  return event;
}
