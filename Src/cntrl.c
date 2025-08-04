#include "main.h"
#include <string.h>
enum blot_state state=stPowerUp;


uint pos=8333;
byte bar_rack_num=0;
byte bar_scan_cnt=0;

byte smple_ev_cnt=0;
byte smple_rack_cnt=0;
byte prob_disp_cnt=0;
byte prime_eve_cnt=0;
byte smp_prime_eve_cnt=0;
byte auto_clean_eve_cnt=0;
byte probe_disp_eve_cnt=0;
byte disp_eve_cnt=0,disp_yaixs_wide_offset=0,disp_xaixs_wide_offset=0,disp_cnt=0;
byte diasp_eve_cnt=0,diasp_yaixs_wide_offset=0,diasp_xaixs_wide_offset=0\
  ,diasp_cnt=0;
byte asp_eve_cnt=0,asp_cnt=0, syg_prime_oper_cnt=0;
uint asp_xaixs_init_pos=0;
byte cll_state=0;
bool home_flag=false;
bool servo_continue=false;
byte syg_add_sel=0;
int16_t slope_vol=0;

bool LLD_homeing_flag=false;
bool PRIME_homeing_flag=false;

int32_t clld_vol=0;
int32_t strip_pack_cnt=0;
int32_t strip_pack_gap=0;
unsigned short int smp_prime_pm_ch=0;
int clld_speed_probz=0;
byte jude_cnt=0;
int16_t p_min=0 , p_max=0;
int32_t smp_prime_pvol=0;
bool dry_cnt=false;
bool lld_retry=false;
bool di_asp_onplus_flag=false;
byte diasp_cnt_offset=0;
byte asp_re_cunt=0;
byte auto_cl_re_cunt=0;
uint16_t asp_roll_back=20;
uint16_t diasp_eve_cnt2=0;
byte disasp_pack_cnt=0;
byte diasp_cnt_even=0;
bool smp_prime_press_chek_flg=false;
bool smp_prime_dw_check_flg=false;
byte smp_prime_dw_check_cnt=0;
bool smp_prime_oper_flg=false;
bool odd_flg=false;
bool asp_flg=false;
byte smp_prime_dw_erorr_cnt=0;
int32_t lld_strip_gap_qc=0;
bool press_sens_flg=false;
bool channel_empty = true; 

event execute_main_ctrl(event event)
{
  //  byte ste=0;
  float jude_slop=0;
  byte dev_send_buf[4]={0,}; 
  switch(event)
  {
  case eventSpuOn:
    stm_memory_init();
    stm_reset();
    z_pram_set();
    x_pram_set();
    y_pram_set();
    
    cam_led_off();
    give_event(eventMemoryInit,0);
    break;
    
  case eventMemoryInit:
    err_tmout_set(errMemory);
    shake_mem_init();
    servo_mem_init();
    pump_mem_init();
    syge_mem_init();
    bar_mem_init();
    hlld_mem_init();
    cam_mem_init();
    sq_mem_init();
    err_tmout_en(FALSE);
    set_timer_(eventAspHomeInit,100,0);
    break;
  case eventAspHomeInit:
    New_Pump_Run(0x01<<BATH_ASP1,10000);
    servo_mv(asp_mt.up_pos);
    servo_move_flag = false;
    set_timer_(eventZHomeInit,100,0);
    break;
  case eventZHomeInit:
    z_homeing();
    set_timer_(eventZHomeInitCheck,5000,0);
    break;
  case eventZHomeInitCheck:
    if(z_homeing_success){
      y_homeing();
      set_timer_(eventShakeInit,5000,0);
    }else  set_timer_(eventZHomeInitCheck,100,0);
    break;
  case eventShakeInit:
    if(y_homeing_success)
    {
      set_timer_(eventSpuInit,100,0);
    }
    else set_timer_(eventShakeInit,100,0);
    break;
  case eventShakeHomeInit:
    if(stShkrOper==shake_home()){
      set_timer_(eventShakeHomeInit,200,0);
    }else set_timer_(eventSygInit,200,0);    
    break;
  case eventSygInit:
    syge_init();
    set_timer_(eventPumpInit,200,0);  
    break;
  case eventPumpInit:
    New_Pump_Reset();
    set_timer_(eventLldInit,200,0);  
    break;
  case eventLldInit:
    hlld_send_pack(HLLD_ADD, HLLD_CLLD_VALUE_SET,0,smp_pram.clld_value_set); 
    hlld_send_pack(HLLD_ADD, HLLD_CLLD_VOL,0, 0);
    //  hlld_send_pack(HLLD_ADD, HLLD_CLLD_RES,smp_pram.r1, smp_pram.r2); 
    //set_timer_(eventSpuInit,200,0);
    //set_timer_(eventLldInit,100,0);  
    break;
  case eventSpuInit:
    if(!home_flag){
      home_flag=TRUE;
      x_homeing();
      set_timer_(eventSpuInit,300,0);
    }else{
      if(x_homeing_success){
        set_timer_(eventShakeHomeInit,100,0);
        Se655_Barcode_Init();
        home_flag=FALSE;
      }else
        set_timer_(eventSpuInit,50,0);
    }
    break;
  case eventSpuStnby:
    
    if(state==stPrepare){
      z_pram_set();
      x_pram_set();
      y_pram_set();
      cam_led_off();
      
      // stmt_abs_move(ADDR_MOTOR_Y, ymt_ctrl.sample_width);
      state=stPrepare;
      // state=stStby;
      beep(80, 1);
    }
    
    if(state!=stStby){
      #ifdef Motor_EDB
        motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,4,2, zmt_ctrl.sample_pos);
      #endif
      set_timer_(eventDevStateRep,3000,0);
    }
    break;
  case eventDevStateRep:
    state=stStby;
    beep(80, 2);
    
    break;
    
    // --------init------------
    /* 
  case eventSpuResetAsp:
    servo_mv(asp_mt.up_pos);
    set_timer_(eventSpuResetZ,100,0);
    break;
  case eventSpuResetZ:
    z_homeing();
    set_timer_(eventSpuResetZCheck,100,0);
    break;
  case eventSpuResetZCheck:
    if(z_reach_pos){
    set_timer_(eventSpuResetShak,50,0);
  }else  set_timer_(eventSpuResetZCheck,100,0);
    break;
  case eventSpuResetShak:
    if(stShkrOper==shake_home()){
    set_timer_(eventSpuResetShak,200,0);
  }else set_timer_(eventSpuResetSyg,200,0); 
    break;
  case eventSpuResetSyg:
    syge_init();
    set_timer_(eventSpuResetPump,200,0); 
    break;
  case eventSpuResetPump:
    New_Pump_Reset();
    set_timer_(eventSpuResetXY,200,0); 
    break;
  case eventSpuResetXY:
    if(!home_flag){
    home_flag=TRUE;
    all_homeing();
    set_timer_(eventSpuResetXY,300,0);
  }else{
    if(mt_xstate==stanby_home){
    set_timer_(eventSpuResetEnd,5000,0);
    home_flag=FALSE;
  }else
    set_timer_(eventSpuResetXY,50,0);
  }
    break;
  case eventSpuResetEnd:
    switch(state)
    {
    
  case stStEng:
    //give_event(eventDspIinit,0);
    break;
  case stReady:
    give_event(eventSqFullSqAly,0);
    break;
  case stPrepare:
    state=stStby;
    beep(a, 1);
    
    break;
  }
    break;
    */
    //---------------------------pro_barcode---------------------   
  case eventBarScanInit:   
    stmt_speed_set(ADDR_MOTOR_X,100);
    bar_rack_num=1;
    
    xmt_ctrl.bar_pos=bar_pm.start_pos;
    
    bar_pr.sample_wd=(bar_pm.end_pos-bar_pm.start_pos)/(SAMPLE_NUM-1);
    bar_scan_cnt=bar_pr.scan_cnt;
    
    if(bar_indx_oper_flg){
      bar_rack_num= bar_pm.spl_num-1;
      if(bar_rack_num>=SAMPLE_NUM){
        xmt_ctrl.bar_pos=((bar_rack_num-SAMPLE_NUM)*bar_pr.sample_wd)+bar_pm.start_pos2;
        bar_rack_num=1;
      }else{
        xmt_ctrl.bar_pos=(bar_rack_num*bar_pr.sample_wd)+xmt_ctrl.bar_pos;
        bar_rack_num=1;
      }
    }
    
    stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.bar_pos);
    if(!bar_indx_oper_flg)
      barcode_triger();
    set_timer_(bar_pr.event[1],10,0);
    break;
  case eventBarScan:
    if(x_reach_pos){
      if(bar_indx_oper_flg)
        barcode_triger();

      stmt_speed_set(ADDR_MOTOR_X,30);    
      set_timer_(bar_pr.event[2],100,0);
    }else
      set_timer_(bar_pr.event[1],50,0);
    break;
  case eventBarScanReOper:
    if(bar_rack_num>bar_pr.sample_num){
      set_timer_(bar_pr.event[6],10,0);
      break;
    }else {
      if(br_state==BarReadComp){
        barcode_triger();
        set_timer_(bar_pr.event[2],300,0);
      }else
        set_timer_(bar_pr.event[2],10,0);
    }
    break;
  case eventBarScanOper:
    // stmt_abs_move(ADDR_MOTOR_X,(bar_rack_num*bar_pr.sample_wd)+xmt_ctrl.bar_pos);
    if(bar_rack_num<SAMPLE_NUM){
      stmt_abs_move(ADDR_MOTOR_X,(bar_rack_num*bar_pr.sample_wd)+xmt_ctrl.bar_pos);
    }else{
      if(bar_indx_oper_flg)
        stmt_abs_move(ADDR_MOTOR_X,(bar_rack_num*bar_pr.sample_wd)+bar_pm.start_pos2);
      else
        stmt_abs_move(ADDR_MOTOR_X,((bar_rack_num-SAMPLE_NUM)*bar_pr.sample_wd)+bar_pm.start_pos2);
    }
    set_timer_(bar_pr.event[3],30,0);
    break;
  case eventBarScanResult:
    if(x_reach_pos){
      bar_scan_cnt--;
      if(br_state==BarReadComp)
        set_timer_(bar_pr.event[4],50,0);
      else
        set_timer_(bar_pr.event[5],50,0);
    }else
      set_timer_(bar_pr.event[3],100,0);
    break;
  case eventBarScanOk:
    bar_rack_num++;
    if(bar_scan_cnt%2){
      bar_scan_cnt=bar_pr.scan_cnt;
      if(bar_rack_num<SAMPLE_NUM)    
        stmt_abs_move(ADDR_MOTOR_X,(bar_rack_num*bar_pr.sample_wd)+xmt_ctrl.bar_pos);
      else{
        stmt_abs_move(ADDR_MOTOR_X,((bar_rack_num-SAMPLE_NUM)*bar_pr.sample_wd)+bar_pm.start_pos2);
      }
      set_timer_(bar_pr.event[8],50,0);
      break;
    }
    bar_scan_cnt=bar_pr.scan_cnt;
    set_timer_(bar_pr.event[7],100,0);
    break;
  case eventBarScanFail:
    if(!bar_scan_cnt){
      bar_rack_num++;
      bar_scan_cnt=bar_pr.scan_cnt;
      bar_pm.spl_num--;
      beep(10, 10);
      set_timer_(bar_pr.event[7],100,0);
      break;
    }
    if(!(bar_scan_cnt%2))
      bar_rack_num-=1;
    else
      bar_rack_num++;
    
    set_timer_(bar_pr.event[2],100,0);
    break;
  case eventBarScanCheck:
    if(x_reach_pos){
      bar_rack_num++;
      set_timer_(bar_pr.event[7],30,0);
    }else
      set_timer_(bar_pr.event[8],30,0);
    break;
  case eventBarScanEnd:
    barcode_flag = false;
    bar_indx_oper_flg=false;
    barcode_off();
    stmt_speed_set(ADDR_MOTOR_X,100);
    #ifdef Motor_EDB
      stmt_abs_move(ADDR_MOTOR_X,0);
    #endif
    
    #ifdef Motor_LEAD
      stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.bath_pos);
    #endif

    usb_send_pack(hseBarSqEnd, 0);
    break;
    
    //----------prime---------------------------   
  case eventPrimeIinit:
    prime_eve_cnt=0;
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);
    set_timer_(prime.event[++prime_eve_cnt],30,0);
    break;
  case eventPrimeZixsCheck:
    if(z_reach_pos){
      set_timer_(prime.event[++prime_eve_cnt],50,0);
    }else  set_timer_(prime.event[prime_eve_cnt],100,0);
    break;
  case eventPrimeBathPsn:
    stmt_abs_move(ADDR_MOTOR_Y,asp_mt.disp_bath_y);
    set_timer_(prime.event[++prime_eve_cnt],100,0);
    break;
  case eventPrimeAixsCheck:
    if(x_reach_pos&&y_reach_pos){
      if(state==stPause)
        set_timer_pause(prime.event[++prime_eve_cnt],50,0);
      else
        set_timer_(prime.event[++prime_eve_cnt],50,0);
    }else  set_timer_(prime.event[prime_eve_cnt],100,0);
    break;
  case eventPrimeDspAspOut:
    servo_mv(asp_mt.down_pos);
    if(prime.disp_pump_num==((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2)))
    {
      New_Pump_Run(prime.disp_pump_num,prime.disp_vol+4000);
    }else
      New_Pump_Run(prime.disp_pump_num,prime.disp_vol);
    ASP_PUMP_ON;
    set_timer_(prime.event[++prime_eve_cnt],prime.disp_vol+8000,0);  
    break;
  case eventPrimeDspAspEnd:
    ASP_PUMP_OFF;
    servo_mv(asp_mt.up_pos);
    //stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
    //stmt_abs_move(ADDR_MOTOR_Y, ymt_ctrl.sample_width);
    if(state==stPause)
      set_timer_pause(prime.event[++prime_eve_cnt],100,0);
    else
      set_timer_(prime.event[++prime_eve_cnt],100,0);
    
    break;
  case eventPrimeEnd:
    prime_eve_cnt=0;
    prime.disp_pump_num=0;
    
    switch(state)
    {
    case stPause:
      usb_send_pack(eventPauseRes,0);
      break;
    case stStEng:
      give_event(eventDspIinit,0);
      break;
    case stReady:
      set_timer_(eventSqNext,100,0);
      break;
    case stStby:
      set_timer_(eventSqNext,100,0);
      break;
    }
    break;
    //------------sample prime-------------------
  case eventSmpPrimeInit:
    smp_prime_dw_check_flg=false;
    smpl_prime.air_asp_pos=syrg_pram.air_gap;
    smp_prime_eve_cnt=0;
    syg_prime_oper_cnt=0;
    syg_add_sel=0;
    smp_prime_pvol=0;
    p_max=0;
    p_min=0;
    smp_prime_pm_ch=0;
    syg_prime_oper_cnt=smpl_prime.syg_prime_cnt*2; 
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);
    dbg_serial("[eventSmpPrimeInit]"); 

    set_timer_(smpl_prime.event[++smp_prime_eve_cnt],200,0);

    break;
  case eventSmpPrimeBathMove:
    if(z_reach_pos)
    {
      #ifdef Motor_EDB
        stmt_abs_move(ADDR_MOTOR_Y,0);
      #endif
      
      #ifdef Motor_LEAD
        stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
      #endif
      dbg_serial("(eventSmpPrimeBathMove)");
      set_timer_(smpl_prime.event[++smp_prime_eve_cnt],50,0);
    }
    else 
    {
      dbg_serial("(eventSmpPrimeBathMove_LOOP)");
      set_timer_(smpl_prime.event[smp_prime_eve_cnt],50,0);
    }
    break;
 
    break;
  case eventSmpPrimeAixsCheck:
    if(x_reach_pos&&y_reach_pos){
      set_timer_(smpl_prime.event[++smp_prime_eve_cnt],50,0);
      dbg_serial("[eventSmpPrimeAixsCheck]");
    }else{        
      dbg_serial("[eventSmpPrimeAixsCheck_LOOP]");

      set_timer_(smpl_prime.event[smp_prime_eve_cnt],50,0);

    }  
    break;
  case eventSmpPrimeStripMoveZ:
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.dsp_pos_end);
    set_timer_(smpl_prime.event[++smp_prime_eve_cnt],50,0);
    break;
  case eventSmpPrimeStripMoveZCheck:
    if(z_reach_pos){
      set_timer_(smpl_prime.event[++smp_prime_eve_cnt],50,0);
    }else  set_timer_(smpl_prime.event[smp_prime_eve_cnt],50,0);//
    break;
  case eventSmpPrimeSygeSet:
    if((!smple_rack_cnt)||lld_retry){
      C3000_srige_oper(SYRINGE_INIT , 0);
      lld_retry=false;
    }
    // else{
    //   if(jude_cnt<smpl_pr.sensing_cnt-1)
    //      C3000_srige_oper(SYRINGE_INIT , 0);
    // }
    
    if(smpl_prime.pm_num==SMP_PRME_PP){
      C3000_srige_oper(SYRINGE_VALVE_BYPASS , 0);
      //C3000_send_command(1, "ER\0");  
      HAL_GPIO_WritePin(ACC_VALVE_PROBE_GPIO_Port,ACC_VALVE_PROBE_Pin,GPIO_PIN_SET);
    }else{
      C3000_srige_oper(SYRINGE_VALVE_LEFT , 0);
      HAL_GPIO_WritePin(ACC_VALVE_PROBE_GPIO_Port,ACC_VALVE_PROBE_Pin,GPIO_PIN_RESET);
    }
    if(state==stPause){
      ++smp_prime_eve_cnt;
      set_timer_pause(smpl_prime.event[smp_prime_eve_cnt],1000,0);//
    }else
      set_timer_(smpl_prime.event[++smp_prime_eve_cnt],50,0);//
    break;
    
  case eventSmpPrimePumpOn:
    
    //  smp_prime_pm_ch=0x01<<BATH_ASP1;//smpl_prime.pm_num|(0x01<<BATH_ASP1)|(0x01<<BATH_DW);
    
    if(smpl_prime.pm_num==SMP_PRME_PP){
      smp_prime_pm_ch=(0x01<<BATH_ASP1);
      //  PRIME_DW_ON;
      if(smple_rack_cnt){
        New_Pump_Run((0x01<<PRIME_DW_ON)|(0x01<<BATH_ASP1),dw_prime_vol);
        set_timer_(smpl_prime.event[++smp_prime_eve_cnt],dw_prime_vol+1000,0); //compatible with lld pirme
      }else{
        if(auto_prime_flg){
          New_Pump_Run((0x01<<PRIME_DW_ON)|(0x01<<BATH_ASP1),21000);
         // New_Pump_Run(0x01<<BATH_ASP1,21000);
          set_timer_(smpl_prime.event[++smp_prime_eve_cnt],23000,0);
        }else{
          //  New_Pump_Run(0x01<<BATH_ASP1,33000);
          New_Pump_Run((0x01<<PRIME_DW_ON)|(0x01<<BATH_ASP1),33000);
          set_timer_(smpl_prime.event[++smp_prime_eve_cnt],3000,0);
        }
        
      }
    }else{
      smp_prime_pm_ch=smpl_prime.pm_num|(0x01<<BATH_ASP1);
      New_Pump_Run(smp_prime_pm_ch,(unsigned long int)smpl_prime.pm_time);
      set_timer_(smpl_prime.event[++smp_prime_eve_cnt],smpl_prime.pm_time,0);
    }
    
    BATH_DW_ON;
    HAL_Delay(500);
    
    // if(smple_rack_cnt){
    //  New_Pump_Run(smp_prime_pm_ch,4000);
    // }else{
    // New_Pump_Run(0x01<<BATH_ASP1,(unsigned long int)smpl_prime.pm_time);
    // New_Pump_Run(0x01<<WS_PUMP,(unsigned long int)smpl_prime.pm_time);
    
    //   New_Pump_Run(0x01<<BATH_ASP1,23000);
    // }
    // New_Pump_Run(smp_prime_pm_ch,(unsigned long int)smpl_prime.pm_time);
    if(smp_prime_oper_flg)
      set_timer_(eventSmpPrimeProbePressCheck,1000,0);
    smp_prime_dw_check_cnt=0;
    smp_prime_oper_flg=false;
    
    // give_event(eventSmpPrimeSygeOper,0);
    break;
    
  case eventSmpPrimeStructInit:
    // C3000_srige_oper(SYRINGE_MOVE_DOWN , smpl_prime.syg_down_pos);
    if(smpl_prime.pm_num==SMP_PRME_PP){
      C3000_srige_oper(SYRINGE_SPEED_SET , 1);
      if(auto_prime_flg){
        New_Pump_Run((0x01<<BATH_ASP1)|(0x01<<BATH_ASP2),30000);
        auto_prime_flg=false;
      }
      ASP_PUMP_OFF;
    }
    
    
    set_timer_(smpl_prime.event[++smp_prime_eve_cnt],300,0);
    break;
  case eventSmpPrimeSygeOper:
    if((smpl_prime.pm_num==SMP_PRME_PP)&&!smple_rack_cnt){
      if(syg_prime_oper_cnt--){
        //C3000_srige_oper(SYRINGE_MOVE_DOWN , smpl_prime.syg_down_pos);
        if(syg_add_sel==SYRINGE_MOVE_DOWN)
          syg_add_sel=SYRINGE_MOVE_UP;
        else if(syg_add_sel==SYRINGE_MOVE_UP)
          syg_add_sel=SYRINGE_MOVE_DOWN;
        else
          syg_add_sel=SYRINGE_MOVE_DOWN;
        C3000_srige_oper(syg_add_sel , smpl_prime.syg_down_pos);
        set_timer_(smpl_prime.event[smp_prime_eve_cnt],3000,0);
      }else{
        syg_prime_oper_cnt=0;
        BATH_DW_OFF;
        // PRIME_DW_OFF;
        set_timer_(smpl_prime.event[++smp_prime_eve_cnt],100,0);
      }
    }else{
      BATH_DW_OFF;
      //  PRIME_DW_OFF;
      set_timer_(smpl_prime.event[++smp_prime_eve_cnt],100,0);
    }
    break;
  case eventSmpPrimeProveSet:
    stmt_speed_set(ADDR_MOTOR_Z,5);
    set_timer_(smpl_prime.event[++smp_prime_eve_cnt],300,0);
    break;
  case eventSmpPrimeProveMove:
    C3000_srige_oper(SYRINGE_VALVE_LEFT , 0);
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);//zmt_ctrl.dsp_pos_end/2);//-smpl_prime.prov_pos);
    set_timer_(smpl_prime.event[++smp_prime_eve_cnt],1500,0);
    break;
  case eventSmpPrimeBathSuck:
    if(smpl_prime.pm_num==SMP_PRME_PP){
      C3000_srige_oper(SYRINGE_SPEED_SET , 14);
    }  
    C3000_srige_oper(SYRINGE_MOVE_DOWN , smpl_prime.air_asp_pos);
    // smp_prime_pm_ch=0x0c00;
    New_Pump_Run((0x01<<BATH_ASP1)|(0x01<<BATH_ASP2),3000);
    //New_Pump_Run(smp_prime_pm_ch,3000);
    //set_timer_(smpl_prime.event[++smp_prime_eve_cnt],3100,0);
    set_timer_(smpl_prime.event[++smp_prime_eve_cnt],100,0);
    z_pram_set();
    
    if(smp_prime_dw_check_flg)
    {
      dev_send_buf[0]=0;
      dev_send_buf[1]=0;
      dev_send_buf[2]=0;
      dev_send_buf[3]=0;
      if(smp_prime_pvol<100)
      {
        dev_send_buf[0]=1;
        for(int i=0;STRIP_NUM_END<=i;i++)
        {
          smp_strip[i].manual=1;
        }
        press_sens_flg=true;
      }
      usb_send_pack(eventProbeAlarm,dev_send_buf);
      beep(1000, 3);
    }

    if(!auto_clean_repeat_flg){
      if(lld_fc==lldFuncPlld||lld_fc==lldFullFunc||lld_fc==lldFuncDispVol){
        //if(state==stPause)
        //  set_timer_pause(eventSmpInit,500,0);
        // else{
        if(!smp_prime_dw_check_flg)
          set_timer_(eventSmpInit,500,0);
        //  }
      }else if(lld_fc==lldFuncNone){
        if(!smp_prime_dw_check_flg)
          if(state==stReady&&full_pr!=Incubation_Ws&&full_pr!=Incubation){         
            set_timer_(eventSqNext,100,0);
          }else if(state==stStEng){
            if(probe_disp_enable)
              give_event(eventProbeDispIinit,0);
          }else if(state==stPause)
            set_timer_pause(eventSqNext,1000,0);
        break;
        
      }
    }else{
      if(state==stPause){
        set_timer_pause(eventSmpPrimeInit,1000,0);
        //   else if(state==stStby)
        //     break;
      }else{
        smpl_prime.pm_num=SMP_PRME_PP;
        auto_clean_repeat_flg=false;
        if(!sq_start_dw_washing_flg)
          set_timer_(eventSmpPrimeInit,500,0);
        // else
        //  sq_start_dw_washing_flg=false;
      }
      
      
    }
    
    break;
  case eventSmpPrimeBathInit:
    //  z_pram_set();
    // if(state!=stStEng)
    //C3000_srige_oper(SYRINGE_INIT , 0);
    
    HAL_GPIO_WritePin(ACC_VALVE_PROBE_GPIO_Port,ACC_VALVE_PROBE_Pin,GPIO_PIN_SET);
    set_timer_(smpl_prime.event[++smp_prime_eve_cnt],200,0);
    break;
  case eventSmpPrimeEnd:
    // stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);
    // stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
    // set_timer_(smpl_prime.event[++smp_prime_eve_cnt],100,0);
    set_timer_(eventSmpPrimeFinalEnd,100,0);
    break;
  case eventSmpPrimeFinalEnd:
    smp_prime_eve_cnt=0;
    //syg_prime_oper_cnt=0;
    syg_add_sel=0;
    /*    if(lld_fc==lldFuncPlld||lld_fc==lldFullFunc||lld_fc==lldFuncDispVol)
    set_timer_(eventSmpInit,100,0);
    else if(lld_fc==lldFuncNone){
    if(state==stReady&&full_pr!=Incubation_Ws){
    set_timer_(eventSqNext,10,0);
  }else if(state==stStEng)
    give_event(eventProbeDispIinit,0);
  }*/
    break;
    
    
  case eventSmpPrimeProbePressCheck:
    hlld_send_pack(HLLD_ADD, HLLD_PLLD_RUN,0, 0);
    smp_prime_press_chek_flg=true;
    break;
  case eventSmpPrimeProbePressResult:
    if((smpl_prime.pm_num==SMP_PRME_PP)&&!smple_rack_cnt) {
      if(smp_prime_dw_check_cnt<3){
        smp_prime_dw_check_cnt++;
        smp_prime_pvol+=p_max;
        set_timer_(eventSmpPrimeProbePressCheck,100,0);
      }else{
        smp_prime_pvol=smp_prime_pvol/3;
        if(smp_prime_pvol<500){//smkang
          smp_prime_dw_check_flg=true;
          // usb_send_pack(eventProbeAlarm,0);
        }
      }
      //state=stPause;
    }
    
    break;
    
  case eventProbeAlarmRes:
  //  smp_prime_dw_check_flg=false;
    give_event(eventSmpPrimeInit,0);
    break;
    /*case eventSmpPrimeSygeUp:
    C3000_srige_oper(SYRINGE_MOVE_UP , 0);
    break;
  case eventSmpPrimeSygeDown:
    C3000_srige_oper(SYRINGE_MOVE_DOWN , smpl_prime.syg_down_pos);
    break;*/
    
    //-----------process sample hlld---------
  case eventSmpInit:
    if(plld_qc != true) 
    {
      lld_repeat_en(EN);
    }  
    smple_ev_cnt=0;
    strip_pack_cnt=0;
    clld_vol=0;
    cll_state=0; 
    clld_speed_probz=0;
    p_min=0; p_max=0;
    strip_pack_gap=0;
    slope_vol=0;
 
    smpl_pr.sample_wd=(xmt_ctrl.sample_pos_end- xmt_ctrl.sample_pos)/(SAMPLE_NUM-1);
    xmt_ctrl.strip_width=(cam_pram.cam_aly_xend_pos-cam_pram.cam_aly_x_pos)/(STRIP_NUM-1);

    set_timer_(smpl_pr.event[++smple_ev_cnt],30,0);
    
    break;
  case eventSmpZinit:
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);
    set_timer_(smpl_pr.event[++smple_ev_cnt],50,0);
    break;
  case eventSmpRackMove:
    if(smple_rack_cnt<SAMPLE_NUM)    
      stmt_abs_move(ADDR_MOTOR_X,(smpl_pr.sample_wd*smple_rack_cnt)+xmt_ctrl.sample_pos);
    else
      stmt_abs_move(ADDR_MOTOR_X,((smple_rack_cnt-SAMPLE_NUM)*smpl_pr.sample_wd)+ymt_ctrl.cam_pos);
    
    // dwHsSW_Delay_ms(100);
    if(!(smple_rack_cnt%2))
      stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.sample_pos);
    else
      stmt_abs_move(ADDR_MOTOR_Y, ymt_ctrl.sample_pos2);
    

    hlld_send_pack(HLLD_ADD, HLLD_CLLD_VOL,0, 0);
    set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    break;
    
  case eventSmpAbsMoveCheck:
    if(x_reach_pos&&y_reach_pos){
      set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    }else  set_timer_(smpl_pr.event[smple_ev_cnt],100,0);
    break;
    
  case eventSmpMoveZCheck:
    if(z_reach_pos){
      set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    }else  set_timer_(smpl_pr.event[smple_ev_cnt],100,0);
    break;
    
  case eventSmpSWSInitCheck: 
    hlld_send_pack(HLLD_ADD, HLLD_CLLD_INIT,0, 0); // 새로 추가한 건.
    HAL_Delay(100);
    hlld_send_pack(HLLD_ADD, HLLD_CLLD_RES,smp_pram.r1, smp_pram.r2);
    hlld_send_pack(HLLD_ADD, HLLD_CLLD_RUN,0, 0);
    set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    break;
    
  case eventSmpCll:
    #ifdef Motor_EDB
      motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,0,2,CLL_EN);
      cll_state=0;
      set_timer_(smpl_pr.event[++smple_ev_cnt],50,0);
    #endif
    
    #ifdef Motor_LEAD
      cll_state=0;
      CLLD_OP();
      set_timer_(smpl_pr.event[++smple_ev_cnt],50,0);
    #endif

    break;
    
  case eventSmpCllCheck:
    #ifdef Motor_EDB
      if(z_reach_pos && (cll_state==5||cll_state==6))
      {
        motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,2,2,0);
        set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);//clld pass
      }
      else 
      { 
        motor_cmd_send(ADDR_MOTOR_Z, INST_GGP, 2, 2, 0);    
        set_timer_(smpl_pr.event[smple_ev_cnt],100,0);
      }
    #endif  
    
    #ifdef Motor_LEAD
      if(z_reach_pos)
      {
        cll_state = 0;
        set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);//clld pass
      }
      else 
      {     
        set_timer_(smpl_pr.event[smple_ev_cnt],100,0);
      }
    #endif  
    break;
    
  case eventSmpSWSCheck:
    set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    break;
    
  case eventSmpSygAspOper: //
    CLLD_VOL_Check = false;
    sy_flag = TRUE;
    stmt_speed_set(ADDR_MOTOR_X,300);
    stmt_speed_set(ADDR_MOTOR_Y,300);
    C3000_srige_oper(SYRINGE_ACCEL_SET , 20);
    C3000_srige_oper(SYRINGE_SPEED_SET , 6); //jjh
    hlld_send_pack(HLLD_ADD, HLLD_CLLD_VOL,0, 0);

    set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    break;

  case eventSmpCllVolCheck:
    if(CLLD_VOL_Check == true){
      C3000_srige_oper(SYRINGE_MOVE_DOWN , syrg_pram.vol);
      hlld_send_pack(HLLD_ADD, HLLD_PLLD_RUN,0, 0);
    }
    else 
      set_timer_(smpl_pr.event[smple_ev_cnt],100,0);
    break;
    
  case eventSmpPlldJude:
    sy_flag = FALSE;
    dev_send_buf[0]=p_min>>8;
    dev_send_buf[1]=p_min&0xff;
    dev_send_buf[2]=p_max>>8;;
    dev_send_buf[3]=p_max&0xff;
    usb_send_pack(hsePlldJudgResult, dev_send_buf); 
    set_timer_(hsePlldJudgResultResp,50,0);
    break;
  case hsePlldJudgResultResp:
    hlld_send_pack(HLLD_ADD, HLLD_PLLD_SLOP_CMD,0, 0);
    break;
  case hsePlldJudgSlope:
    dev_send_buf[2]=slope_vol>>8;
    dev_send_buf[3]=slope_vol;
    usb_send_pack(hsePlldJudgSlope, dev_send_buf);
    set_timer_(hsePlldJudgResp,100,0);
    break;
  case hsePlldJudgResp:
    if(lld_fc==lldFuncPlld) 
      give_event(eventSmpQcStreamInit,0);
    else if(lld_fc==lldFullFunc){
      
      jude_slop=((float)slope_vol/40)*100;
      if(smp_pram.slop_min_std>jude_slop){
        dev_send_buf[3]=CLLD_Clot;
      }else if(smp_pram.slop_max_std<jude_slop){
        dev_send_buf[3]=Limit_AIR;
      }else{ 
        dev_send_buf[3]=CLLD_OK;
      }

      dev_send_buf[1]=sq_strp_mach;//smple_rack_cnt;
      usb_send_pack(hseLLDJudgDataInxRult, dev_send_buf);  
      if((dev_send_buf[3]==CLLD_OK)){
        jude_cnt=smpl_pr.sensing_cnt-1;
        //give_event(smpl_pr.event[++smple_ev_cnt],0);
          set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
      }else{
        stmt_speed_set(ADDR_MOTOR_Z,100);
        if(!jude_cnt--){
          beep(10, 10);
          smp_strip[sq_smp_strp_mach_cnt].manual=1;
          //smp_strip[smple_rack_cnt].manual=1;
          jude_cnt=smpl_pr.sensing_cnt-1;
          smple_ev_cnt=20;
          set_timer_(smpl_pr.event[smple_ev_cnt],100,0);
         // give_event(smpl_pr.event[smple_ev_cnt],0);
        }else{
          pr_time_min+=100;
          set_timer_(eventSmpPrimeInit,500,0);
        }
        lld_repeat_en(DISEN);
        lld_retry=true;
        
      }
      //set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
      // give_event(smpl_pr.event[++smple_ev_cnt],3000);
    }else//jude
      set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    
    break;
    
  case eventSmpClldRageSpeedSet:
    stmt_speed_set(ADDR_MOTOR_Z,clld_speed_probz);
    set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    break; 

  case eventSmpProbeZHome:   
    #ifdef Motor_EDB
      cll_state=0;
      dSPIN_Go_To( shk_pram.down_ang_pos);
      motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,0,2,CLLD_HOME);
      cll_state=0;
      set_timer_(smpl_pr.event[++smple_ev_cnt],1500,0);
    #endif
   
    #ifdef Motor_LEAD
      cll_state=0;
      dSPIN_Go_To(shk_pram.down_ang_pos);
      CLLD_HOME();
      set_timer_(smpl_pr.event[++smple_ev_cnt],1500,0);
    #endif

    break;
    
  case eventSmpStripMove:
    stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.dsp_pos);
    if((state==stReady)||(state==stPause)){
      strip_pack_cnt=sq_strp_mach/12;
      //  strip_pack_gap=xmt_ctrl.cam_pos-xmt_ctrl.dsp_pos;
      strip_pack_gap=xmt_ctrl.cam_pos-cam_pram.cam_aly_x_pos;
      stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.dsp_pos+(strip_pack_gap*strip_pack_cnt)+(xmt_ctrl.strip_width*(sq_strp_mach-(12*strip_pack_cnt))));
    }else{
      
      strip_pack_cnt=smple_rack_cnt/12;
      strip_pack_gap=xmt_ctrl.cam_pos-cam_pram.cam_aly_x_pos;
      stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.dsp_pos+(strip_pack_gap*strip_pack_cnt)+(xmt_ctrl.strip_width*(smple_rack_cnt-(12*strip_pack_cnt))));
    }
    
    set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    break;
    
    
    
  case eventSmpStripDspMove:
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.dsp_pos);
    set_timer_(smpl_pr.event[++smple_ev_cnt],50,0);
    break;
  case eventSmpSygDspOper:
    C3000_srige_oper(SYRINGE_SPEED_SET , 4);
    C3000_srige_oper(SYRINGE_MOVE_UP ,syrg_pram.vol+(syrg_pram.air_gap/2));// 400);
    clld_speed_probz=200;
    set_timer_(smpl_pr.event[++smple_ev_cnt],800,0);
    break;
  case eventSmpEnd:
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);
    set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    break;
  case eventSmpFinalEnd:
    
    dSPIN_Go_To( 0);
    //if(!smple_rack_cnt)
    // stmt_abs_move(ADDR_MOTOR_X,0);
    #ifdef Motor_EDB
      stmt_abs_move(ADDR_MOTOR_Y,0);
    #endif
    
    #ifdef Motor_LEAD
      stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
    #endif
    set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    break;
    
  case eventSmpNext:
    C3000_srige_oper(SYRINGE_MOVE_UP ,(syrg_pram.air_gap/2));
    lld_repeat_en(DISEN);
    if((state!=stReady)&&(state!=stPause)){
      if(smple_rack_cnt<smpl_pr.sample_num-1){
        smple_rack_cnt++;
        // set_timer_(smpl_pr.event[smple_ev_cnt],50,0); 
        set_timer_(eventSmpPrimeInit,10,0);
      }else{
        beep(80, 2);
        smpl_pr.sample_num=0;
        #ifdef Motor_EDB
          stmt_abs_move(ADDR_MOTOR_X,0);
        #endif
        
        #ifdef Motor_LEAD
          stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.bath_pos);
        #endif
        if(lld_fc==lldFullFunc){
          lld_fc=lldFuncNone;
          set_timer_(eventSmpPrimeInit,100,0);
        }
      }
    }else{     
      if(state==stPause)
        set_timer_pause(eventSqSamplNumMach,2000,0);
      else
        set_timer_(eventSqSamplNumMach,1000,0); 
    }
    break;
 
  case eventSmpStripMoveZCheck:
    if(z_reach_pos){
      set_timer_(smpl_pr.event[++smple_ev_cnt],100,0);
    }else  set_timer_(smpl_pr.event[smple_ev_cnt],100,0);
    break;
  case eventSmpStripHome:
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);
    set_timer_(smpl_pr.event[++smple_ev_cnt],50,0);
    break;
    
    
  case eventSmpPass:
    
    beep(80, 2);
    //mt_pram_set(ADDR_MOTOR_Z,200,200,200);
    set_timer_(smpl_pr.event[++smple_ev_cnt],50,0);
    break;
    
    //----------------Aspiration----------------
  case eventAspIinit:
    asp_eve_cnt=0;
    asp_cnt=0;
    strip_pack_cnt=0;
    strip_pack_gap=0;
    //xmt_ctrl.strip_width=(xmt_ctrl.dsp_pos_end-xmt_ctrl.dsp_pos)/STRIP_NUM;
    // xmt_ctrl.strip_width=(xmt_ctrl.dsp_pos_end-xmt_ctrl.dsp_pos)/(STRIP_NUM-1);
    xmt_ctrl.strip_width=(cam_pram.cam_aly_xend_pos-cam_pram.cam_aly_x_pos)/(STRIP_NUM-1);
    asp_xaixs_init_pos=asp_mt.asp_tray_x-(xmt_ctrl.strip_width*2);
    set_timer_(asp.event[++asp_eve_cnt],100,0);
    break;
  case eventAspSkAngle:
    //  dSPIN_Go_To(shk_pram.up_ang_pos);shk_pram.dasp_pos
    dSPIN_Go_To(shk_pram.dasp_pos);
    set_timer_(asp.event[++asp_eve_cnt],100,0);
    break;
  case eventAspStripMove:
    if(!dSPIN_Busy_HW()){
      stmt_abs_move(ADDR_MOTOR_Y,asp_mt.disp_tray_y);
      stmt_abs_move(ADDR_MOTOR_X,asp_xaixs_init_pos);
      set_timer_(asp.event[++asp_eve_cnt],100,0);
    }else{
      set_timer_(asp.event[asp_eve_cnt],50,0);
    }
    break;
  case eventAspAixsCheck:
    if(x_reach_pos&&y_reach_pos){
      set_timer_(asp.event[++asp_eve_cnt],50,0);
    }else  set_timer_(asp.event[asp_eve_cnt],100,0);
    break;
  case eventAspOut:
    //servo_mv(asp_mt.down_pos); 
    servo_continue = true;
    servo_mv( asp_mt.asp_tray_z);
    set_timer_(asp.event[++asp_eve_cnt],100,0);
    break;
  case eventAspOper:
    asp_cnt+=2;
    ASP_PUMP_ON;
    set_timer_(asp.event[++asp_eve_cnt],asp.time+300,0);
    break;
  case eventAspOperEnd:
    ASP_PUMP_OFF;
    servo_mv(asp_mt.up_pos); 
    if(state==stPause)
      set_timer_pause(asp.event[++asp_eve_cnt],100,0);
    else
      set_timer_(asp.event[++asp_eve_cnt],300,0);
    break;
  case eventAspCheck:
    if(asp_cnt>=asp.total_strip){
      dSPIN_Go_To(0);
      #ifdef Motor_EDB
        stmt_abs_move(ADDR_MOTOR_X,0);
      #endif
      
      #ifdef Motor_LEAD
        stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.bath_pos);
      #endif
      // stmt_abs_move(ADDR_MOTOR_Y, ymt_ctrl.sample_width);
      //stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
      set_timer_(asp.event[++asp_eve_cnt],100,0);
    }else{
      strip_pack_cnt=asp_cnt/12;
      //strip_pack_gap=xmt_ctrl.cam_pos-xmt_ctrl.dsp_pos;
      strip_pack_gap=xmt_ctrl.cam_pos-cam_pram.cam_aly_x_pos;
      stmt_abs_move(ADDR_MOTOR_X,asp_xaixs_init_pos+(strip_pack_gap*strip_pack_cnt)+(xmt_ctrl.strip_width*(asp_cnt-(12*strip_pack_cnt))));
      //stmt_abs_move(ADDR_MOTOR_X,asp_xaixs_init_pos+(xmt_ctrl.strip_width*asp_cnt));
      set_timer_(eventAspCon,50,0);
    }
    break;
  case eventAspEnd:
    servo_continue = false;
    set_timer_(eventAspTimeOut,1200,0);
    asp_eve_cnt=0;
    asp_cnt=0;
    switch(state)
    {
    case stStEng:
      break;
    case stReady:
      if(!asp_re_cunt){
        set_timer_(eventSqNext,100,0);
      }else{
        set_timer_(eventAspIinit,1000,0);
        asp_re_cunt--;
      }
      break;
    case stPause:
      usb_send_pack(eventPauseRes,0);
      break;
      
    }
    break;
  case eventAspCon:
    if(x_reach_pos&&y_reach_pos){
      asp_eve_cnt-=3;
      set_timer_(asp.event[asp_eve_cnt],450,0);
    }else  set_timer_(eventAspCon,50,0);
    break;
    //----------------disp 1---------------------
  case eventDspIinit:
    disp_eve_cnt=0;
    diasp.xaixs_start_pos=0;
    diasp_xaixs_wide_offset=0;
    strip_pack_cnt=0;
    strip_pack_gap=0;
    //xmt_ctrl.strip_width=(xmt_ctrl.dsp_pos_end-xmt_ctrl.dsp_pos)/STRIP_NUM;
    xmt_ctrl.strip_width=(cam_pram.cam_aly_xend_pos-cam_pram.cam_aly_x_pos)/(STRIP_NUM-1);
    
    switch(disp_sgl.disp_pump_num)
    {
    case 0x01<<RD_PUMP1: disp_yaixs_wide_offset=5;
    disp_xaixs_wide_offset=0;
    //disp_sgl.disp_vol=pm_pram.vol[RD_PUMP1-1];
    break;
    case 0x01<<RD_PUMP2: disp_yaixs_wide_offset=5;
    disp_xaixs_wide_offset=1;
    //disp_sgl.disp_vol=pm_pram.vol[RD_PUMP2-1];
    break;
    case 0x01<<RD_PUMP3: disp_yaixs_wide_offset=4;
    disp_xaixs_wide_offset=0;
    //disp_sgl.disp_vol=pm_pram.vol[RD_PUMP3-1];
    break;
    case 0x01<<RD_PUMP4: disp_yaixs_wide_offset=2;
    disp_xaixs_wide_offset=1;
    //disp_sgl.disp_vol=pm_pram.vol[RD_PUMP4-1];
    break;
    case 0x01<<RD_PUMP5: disp_yaixs_wide_offset=4;
    disp_xaixs_wide_offset=0;
    //disp_sgl.disp_vol=pm_pram.vol[RD_PUMP5-1];
    break;
    case 0x01<<RD_PUMP6: disp_yaixs_wide_offset=2;
    disp_xaixs_wide_offset=1;
    //disp_sgl.disp_vol=pm_pram.vol[RD_PUMP6-1];
    break;
    case 0x01<<WS_PUMP1: disp_yaixs_wide_offset=0;
    //disp_sgl.disp_vol=pm_pram.vol[WS_PUMP1-1];
    break;
    //diasp.pump_num=((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2));
    break;
    case 0x01<<WS_PUMP2: disp_yaixs_wide_offset=0;
    //disp_sgl.disp_vol=pm_pram.vol[WS_PUMP2-1];
    break;
    //diasp.pump_num=((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2));
    break;
    case(0x01<<WS_PUMP1)|(0x01<<WS_PUMP2): disp_yaixs_wide_offset=0;
    disp_xaixs_wide_offset=0;
    //disp_sgl.disp_vol=pm_pram.vol[WS_PUMP1-1];
    diasp.pump_num=((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2));
    break;
    //    case 0x01<<WS_PUMP1|0x01<<WS_PUMP2: disp_yaixs_wide_offset=0;
    //    diasp.vol=pm_pram.vol[WS_PUMP1-1];
    //    diasp.pump_num=((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2));
    }
    disp_sgl.yaixs_start_pos=asp_mt.disp_tray_y-(disp_sgl.disp_ypin_wide*disp_yaixs_wide_offset);
    // if(disp_sgl.disp_pump_num==((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2)))
    // xmt_ctrl.strip_width=xmt_ctrl.strip_width*2;
    disp_sgl.xaixs_start_pos=asp_mt.asp_tray_x-(xmt_ctrl.strip_width*disp_xaixs_wide_offset);
    //  if(diasp.pump_num==((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2))){
    // disp_sgl.yaixs_start_pos+=asp_mt.disp_tray_y;
    // }
    set_timer_(disp_sgl.event[++disp_eve_cnt],100,0);
    break;
  case eventDspSkAngle:
    dSPIN_Go_To(shk_pram.dasp_pos);// dSPIN_Go_To(shk_pram.up_ang_pos);
    set_timer_(disp_sgl.event[++disp_eve_cnt],100,0);
    break;
  case eventDspStripMove:
    if(!dSPIN_Busy_HW()){
      stmt_abs_move(ADDR_MOTOR_Y,disp_sgl.yaixs_start_pos);
      stmt_abs_move(ADDR_MOTOR_X,disp_sgl.xaixs_start_pos);
      set_timer_(disp_sgl.event[++disp_eve_cnt],100,0);
    }else{
      set_timer_(disp_sgl.event[disp_eve_cnt],50,0);
    }
    break;
  case eventDspAixsCheck:
    if(x_reach_pos&&y_reach_pos){
      if(state==stPause)
        set_timer_pause(disp_sgl.event[++disp_eve_cnt],100,0);
      else      
        set_timer_(disp_sgl.event[++disp_eve_cnt],100,0);
    }else  set_timer_(disp_sgl.event[disp_eve_cnt],50,0);
    break;
  case eventDspOper:
    
    if(disp_sgl.disp_pump_num==((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2))){
      
      if((disp_sgl.total_strip-disp_cnt)==1||disp_sgl.total_strip==1)
        disp_sgl.disp_pump_num=0x01<<WS_PUMP1;  
      disp_cnt=disp_cnt+2;
    }else
      disp_cnt++;
    
    New_Pump_Run( disp_sgl.disp_pump_num, disp_sgl.disp_vol);
    // beep(80, 1);
    
    
    set_timer_(disp_sgl.event[++disp_eve_cnt],disp_sgl.disp_vol+300,0);
    break;
  case eventDspCheck:
    if(disp_cnt>=disp_sgl.total_strip){
      #ifdef Motor_EDB
        stmt_abs_move(ADDR_MOTOR_X,0);
      #endif
      
      #ifdef Motor_LEAD
        stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.bath_pos);
      #endif
      // stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
      // stmt_abs_move(ADDR_MOTOR_Y, ymt_ctrl.sample_width);
      dSPIN_Go_To(0);
      set_timer_(disp_sgl.event[++disp_eve_cnt],disp_sgl.disp_vol+50,0);
    }else{
      strip_pack_cnt=disp_cnt/12;
      // strip_pack_gap=xmt_ctrl.cam_pos-xmt_ctrl.dsp_pos;
      strip_pack_gap=xmt_ctrl.cam_pos-cam_pram.cam_aly_x_pos;
      stmt_abs_move(ADDR_MOTOR_X,disp_sgl.xaixs_start_pos+(strip_pack_gap*strip_pack_cnt)+(xmt_ctrl.strip_width*(disp_cnt-(12*strip_pack_cnt))));
      // stmt_abs_move(ADDR_MOTOR_X,disp_sgl.xaixs_start_pos+(xmt_ctrl.strip_width*disp_cnt));
      
      set_timer_(eventDspByStripPass,100,0);
    }
    break;
  case eventDspByStripPass:
    if(x_reach_pos&&y_reach_pos){
      if(state==stPause)
        set_timer_pause(disp_sgl.event[--disp_eve_cnt],100,0);
      else     
        set_timer_(disp_sgl.event[--disp_eve_cnt],100,0);
    }else  set_timer_(eventDspByStripPass,50,0);
    break;
  case eventDspEnd:
    disp_cnt=0;
    disp_eve_cnt=0;
    switch(state)
    {
    case stStEng:
      break;
    case stReady:
      if(state==stPause)
        set_timer_pause(eventSqNext,100,0);
      else      
        set_timer_(eventSqNext,100,0);
      break;
    case stPause:
      usb_send_pack(eventPauseRes,0);
      break;
    case stStby:
      set_timer_(eventSqNext,100,0);
      break;
    }
    break;
    //---------------------dispense&asp--------------
    
  case eventDspAspIinit:
    diasp_eve_cnt=0;
    diasp_cnt=0;
    diasp.xaixs_start_pos=0;
    diasp_xaixs_wide_offset=0;
    diasp_cnt_offset=0;
    strip_pack_cnt=0;
    strip_pack_gap=0;
    diasp_eve_cnt2=0;
    disasp_pack_cnt=0;
    diasp_cnt_even=0;
    asp_flg=false;
    odd_flg=true;
    xmt_ctrl.strip_width=(cam_pram.cam_aly_xend_pos-cam_pram.cam_aly_x_pos)/(STRIP_NUM-1);
    
    switch( diasp.pump_num)
    {
    case 0x01<<RD_PUMP1: disp_yaixs_wide_offset=5;
    disp_xaixs_wide_offset=2;
    diasp_cnt_offset=1;
    //diasp.vol=pm_pram.vol[RD_PUMP1-1];
    break;
    case 0x01<<RD_PUMP2: 
      if(!penel_sel)
        disp_yaixs_wide_offset=5;
      else   disp_yaixs_wide_offset=2;
      disp_xaixs_wide_offset=1;
      diasp_cnt_offset=1;
      diasp_cnt_even=1;
      //diasp.vol=pm_pram.vol[RD_PUMP2-1];
      break;
    case 0x01<<RD_PUMP3: 
      if(!penel_sel)
        disp_yaixs_wide_offset=4;
      else disp_yaixs_wide_offset=2;
      disp_xaixs_wide_offset=2;
      diasp_cnt_offset=1;
      // diasp.vol=pm_pram.vol[RD_PUMP3-1];
      break;
    case 0x01<<RD_PUMP4: disp_yaixs_wide_offset=2;
    disp_xaixs_wide_offset=1;
    diasp_cnt_offset=1;
    diasp_cnt_even=1;
    //diasp.vol=pm_pram.vol[RD_PUMP4-1];
    break;
    case 0x01<<RD_PUMP5: disp_yaixs_wide_offset=4;
    disp_xaixs_wide_offset=2;
    diasp_cnt_offset=1;
    //diasp.vol=pm_pram.vol[RD_PUMP5-1];
    break;
    case 0x01<<RD_PUMP6: disp_yaixs_wide_offset=2;
    disp_xaixs_wide_offset=1;
    diasp_cnt_offset=1;
    diasp_cnt_even=1;
    //diasp.vol=pm_pram.vol[RD_PUMP6-1];
    break;
    case 0x01<<WS_PUMP1: disp_yaixs_wide_offset=0;
    diasp_cnt_offset=2;
    //diasp.vol=pm_pram.vol[RD_PUMP6-1];
    diasp.pump_num=((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2));
    break;
    case 0x01<<WS_PUMP2: disp_yaixs_wide_offset=0;
    // diasp.vol=pm_pram.vol[WS_PUMP2-1];
    diasp_cnt_offset=1;
    diasp.pump_num=((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2));
    break;
    case (0x01<<WS_PUMP1)|(0x01<<WS_PUMP2): disp_yaixs_wide_offset=0;
    // diasp.vol=pm_pram.vol[WS_PUMP1-1];
    diasp_cnt_offset=0;
    diasp.pump_num=((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2));
    odd_flg=false;
    break;
    //    case 0x01<<WS_PUMP1|0x01<<WS_PUMP2: disp_yaixs_wide_offset=0;
    //    diasp.vol=pm_pram.vol[WS_PUMP1-1];
    //    diasp.pump_num=((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2));
    }
    diasp.yaixs_start_pos=asp_mt.disp_tray_y-(disp_sgl.disp_ypin_wide*disp_yaixs_wide_offset);
    diasp.xaixs_start_pos=asp_mt.asp_tray_x-(xmt_ctrl.strip_width*2); 
    
    
    set_timer_(diasp.event[++diasp_eve_cnt],100,0);
    break;
  case eventDspAspSkAngle:
    // dSPIN_Go_To(shk_pram.up_ang_pos);
    dbg_serial("[eventDspAspSkAngle]"); 
    dSPIN_Go_To(shk_pram.dasp_pos);
    set_timer_(diasp.event[++diasp_eve_cnt],100,0);
    break;
  case eventDspAspStripMove:
    if(!dSPIN_Busy_HW()){
      stmt_abs_move(ADDR_MOTOR_Y,diasp.yaixs_start_pos);
      stmt_abs_move(ADDR_MOTOR_X,diasp.xaixs_start_pos);
      set_timer_(diasp.event[++diasp_eve_cnt],100,0);
    }else{
      set_timer_(diasp.event[diasp_eve_cnt],50,0);
    }
    break;
  case eventDspAspAixsCheck:
    if(x_reach_pos&&y_reach_pos){
      if(state==stPause)
        set_timer_pause(diasp.event[++diasp_eve_cnt],100,0);
      else    
        set_timer_(diasp.event[++diasp_eve_cnt],50,0);
    }else  set_timer_(diasp.event[diasp_eve_cnt],50,0);
    break;
  case eventDspAspOut:
    servo_continue = true;
    if((diasp_cnt<diasp.total_strip-diasp_cnt_offset)||!diasp_cnt){
      //servo_mv(asp_mt.down_pos);
      if(diasp.pump_num==((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2))){
        if((diasp_cnt%12)!=0&&(diasp_cnt%12<=10)||!diasp_cnt)
          servo_mv( asp_mt.asp_tray_z);
      }else{
        if((diasp_cnt%12)!=0&&(diasp_cnt%12<(10+diasp_cnt_even))||!diasp_cnt)
          servo_mv( asp_mt.asp_tray_z);
      }
      set_timer_(diasp.event[++diasp_eve_cnt],50,0);
    }else
      set_timer_(diasp.event[++diasp_eve_cnt],10,0);
    
    break;
  case eventDspAspOper:
    
    if(diasp.pump_num==((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2))){
      if((diasp_cnt%12)!=0&&(diasp_cnt%12<=10)||!diasp_cnt){
        ASP_PUMP_ON;
        asp_flg=true;
      }
    }else{
      if((diasp_cnt%12)!=0&&(diasp_cnt%12<(10+diasp_cnt_even))||!diasp_cnt){
        ASP_PUMP_ON;
        asp_flg=true;
      }
    } 
    
   
    if(diasp_cnt){
      
      New_Pump_Run( diasp.pump_num, diasp.vol);
      disp_xaixs_wide_offset=1;
    }
    set_timer_(diasp.event[++diasp_eve_cnt],diasp.asp_time+400,0);
    break;
  case eventDspAspEnd:
    if((diasp_cnt<diasp.total_strip-diasp_cnt_offset)||!diasp_cnt){
      ASP_PUMP_OFF;
      if(asp_flg){
        asp_flg=false;
        servo_mv(asp_mt.up_pos); 
      }
    }
    ASP_PUMP_OFF;
    HAL_Delay(200);
    if( diasp.pump_num!=((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2)))
      if((disp_xaixs_wide_offset==2)&&odd_flg&&!diasp_cnt_even){
        //  diasp.xaixs_start_pos+xmt_ctrl.strip_width; 
        odd_flg=false;
        diasp_eve_cnt-=3;
        stmt_abs_move(ADDR_MOTOR_X,diasp.xaixs_start_pos+xmt_ctrl.strip_width);
        set_timer_(diasp.event[diasp_eve_cnt],300,0);
        break;
      }else
        odd_flg=false;
    
    di_asp_onplus_flag=false;
    if(state==stPause)
      set_timer_pause(diasp.event[++diasp_eve_cnt],100,0);
    else 
      set_timer_(diasp.event[++diasp_eve_cnt],100,0);
    break;
  case eventDspAspCheck:
    if(diasp_cnt) 
      if(!(diasp_cnt%12))
        strip_pack_cnt=1;
      else
        strip_pack_cnt=0;
      
      //strip_pack_gap=xmt_ctrl.cam_pos-xmt_ctrl.dsp_pos_end;
      strip_pack_gap=xmt_ctrl.cam_pos-cam_pram.cam_aly_xend_pos;
      // stmt_abs_move(ADDR_MOTOR_X,disp_sgl.xaixs_start_pos+(strip_pack_gap*strip_pack_cnt)+(xmt_ctrl.strip_width*(disp_cnt-(12*strip_pack_cnt))));
      
      if(diasp.pump_num==((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2))){
        diasp_cnt+=2;
        if(diasp_cnt-diasp.total_strip==1){
          di_asp_onplus_flag=true;
        }
      }else
        diasp_cnt+=1;  
      
      if((diasp_cnt<=diasp.total_strip)||di_asp_onplus_flag){
        
        if(diasp.pump_num==((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2))){
          //diasp.xaixs_start_pos+=(xmt_ctrl.strip_width*2);
          if(strip_pack_cnt)diasp.xaixs_start_pos=diasp.xaixs_start_pos+(strip_pack_gap*strip_pack_cnt)-(xmt_ctrl.strip_width);
          else diasp.xaixs_start_pos=diasp.xaixs_start_pos+(strip_pack_gap*strip_pack_cnt)+(xmt_ctrl.strip_width*2);
          
          if(di_asp_onplus_flag)
            diasp.pump_num=0x01<<WS_PUMP1;
          
        }else{
          if(di_asp_onplus_flag){
            if(strip_pack_cnt)diasp.xaixs_start_pos=diasp.xaixs_start_pos+(strip_pack_gap*strip_pack_cnt)-(xmt_ctrl.strip_width*2);
            else diasp.xaixs_start_pos=diasp.xaixs_start_pos+(strip_pack_gap*strip_pack_cnt)+(xmt_ctrl.strip_width*disp_xaixs_wide_offset);
          }else{
            if(strip_pack_cnt)diasp.xaixs_start_pos=diasp.xaixs_start_pos+(strip_pack_gap*strip_pack_cnt)-(xmt_ctrl.strip_width*2)+(diasp_cnt_even*xmt_ctrl.strip_width);
            else diasp.xaixs_start_pos=diasp.xaixs_start_pos+(strip_pack_gap*strip_pack_cnt)+(xmt_ctrl.strip_width*disp_xaixs_wide_offset);
            //diasp.xaixs_start_pos=diasp.xaixs_start_pos+(strip_pack_gap*strip_pack_cnt)+(xmt_ctrl.strip_width*disp_xaixs_wide_offset*(diasp_cnt-(12*strip_pack_cnt)));
            //diasp.xaixs_start_pos+=(xmt_ctrl.strip_width*disp_xaixs_wide_offset);
          }
        }
        
        /*
        switch( diasp.pump_num)
        {
      case 0x01<<RD_PUMP1: 
      case 0x01<<RD_PUMP3: 
      case 0x01<<RD_PUMP5:
        diasp_cnt_offset=2;
        diasp.xaixs_start_pos+=(xmt_ctrl.strip_width*disp_xaixs_wide_offset);
        break;
      case 0x01<<RD_PUMP2: 
      case 0x01<<RD_PUMP4: 
      case 0x01<<RD_PUMP6:
        diasp_cnt_offset=1;
        diasp.xaixs_start_pos+=(xmt_ctrl.strip_width*disp_xaixs_wide_offset);
        break;
      case 0x01<<WS_PUMP1:
        diasp_cnt_offset=1;
        if(di_asp_onplus_flag)
        diasp.xaixs_start_pos+=(xmt_ctrl.strip_width*2);
        break;
      case 0x01<<WS_PUMP2: 
        break;
      case 0x01<<(WS_PUMP1|WS_PUMP2):
        diasp_cnt_offset=1;
        diasp.xaixs_start_pos+=(xmt_ctrl.strip_width*2);
        break;
      }
        
        */
        if(penel_sel)
          if((smp_strip[diasp_cnt-1].penel==Inhalant)||(smp_strip[diasp_cnt-1].penel==Inhalant_CON)){
            diasp.pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>28)&0x0000000f);
            diasp.vol=pm_pram.vol[((sq[full_step_cnt].dword[full_pr_cnt]>>28)&0x0000000f)-1]*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)/500;
          }else {//if(smp_strip[diasp_cnt-1].penel==Food){
            diasp.pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x0000000f);
            diasp.vol=pm_pram.vol[((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x0000000f)-1]*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)/500;
          }/*else if(smp_strip[diasp_cnt-1].penel==Special1){
        diasp.pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x0000000f);
        diasp.vol=pm_pram.vol[((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x0000000f)-1]*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)/500;
      }*/
        if(strip_pack_cnt){
          disasp_pack_cnt=0;
          diasp_eve_cnt2=0;
          if( diasp.pump_num!=((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2))){
            if(!diasp_cnt_even)
              odd_flg=true;
          }
          asp_flg=false;
          set_timer_(eventDspAspStripPackCheck,10,0);  
        }else{
          stmt_abs_move(ADDR_MOTOR_X,diasp.xaixs_start_pos);
          diasp_eve_cnt-=4;
          set_timer_(diasp.event[diasp_eve_cnt],50,0);
        }
        
      }else{
        dSPIN_Go_To(0);
        // stmt_abs_move(ADDR_MOTOR_Y, ymt_ctrl.sample_width);
        // stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
      //  stmt_abs_move(ADDR_MOTOR_X,0);
        set_timer_(diasp.event[++diasp_eve_cnt],1000,0);
      }
      break;
  case eventDspAspFinalEnd:
    #ifdef Motor_EDB
      stmt_abs_move(ADDR_MOTOR_X,0);
    #endif
    
    #ifdef Motor_LEAD
      stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.bath_pos);
    #endif
    penel_sel=0;
    diasp_eve_cnt=0;
    diasp_cnt=0;
    diasp.xaixs_start_pos=0;
    diasp_xaixs_wide_offset=0;
    ASP_PUMP_OFF;
    asp_flg=false;
    servo_continue = false;
    set_timer_(eventAspTimeOut,1200,0);
    // servo_mv(asp_mt.up_pos);
    switch(state)
    {
    case stStEng:
      break;
    case stReady:
      if(state==stPause)
        set_timer_pause(eventSqNext,100,0);
      else    
        set_timer_(eventSqNext,2000,0);
      break;
    case stPause:
      usb_send_pack(eventPauseRes,0);
      break;
    case stStby:
      set_timer_(eventSqNext,2000,0);
      break;
      
    }
    break;
    
  case eventDspAspStripPackCheck:
    stmt_abs_move(ADDR_MOTOR_X,diasp.xaixs_start_pos);
    diasp_eve_cnt2=eventDspAspStripPackAspOut;
    set_timer_(eventDspAspStripPackAixsCheck,200,0);
    break;
  case eventDspAspStripPackAixsCheck:
    if(x_reach_pos&&y_reach_pos){
      set_timer_(diasp_eve_cnt2,50,0);
    }else  set_timer_(eventDspAspStripPackAixsCheck,50,0);
    break;
  case eventDspAspStripPackAspOut:
    if(disasp_pack_cnt<1){
      servo_mv(asp_mt.asp_tray_z);
      diasp_eve_cnt2=eventDspAspStripPackAspOper;
      set_timer_(diasp_eve_cnt2,300,0);
    }else{
      diasp_eve_cnt-=4;
      set_timer_(diasp.event[diasp_eve_cnt],50,0);   
    }
    break;
  case eventDspAspStripPackAspOper:
    ASP_PUMP_ON;
    asp_flg=true;
    diasp_eve_cnt2=eventDspAspStripPackAspEnd;
    set_timer_(diasp_eve_cnt2,diasp.asp_time,0);
    break;
  case eventDspAspStripPackAspEnd:
    ASP_PUMP_OFF;
    if(asp_flg){
      asp_flg=false;
      servo_mv(asp_mt.up_pos); 
    }
    
    HAL_Delay(200);
    if(!diasp_cnt_even&&odd_flg){
      //  diasp.xaixs_start_pos+xmt_ctrl.strip_width; 
      odd_flg=false;
      stmt_abs_move(ADDR_MOTOR_X,diasp.xaixs_start_pos+xmt_ctrl.strip_width);
      diasp_eve_cnt2=eventDspAspStripPackAspOut;
      set_timer_(diasp_eve_cnt2,700,0);
      break;
    }else
      odd_flg=false;
    
    disasp_pack_cnt++;  
    if(diasp.pump_num==((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2))){
      diasp.xaixs_start_pos+=(xmt_ctrl.strip_width*2);
    }else    
      diasp.xaixs_start_pos+=(xmt_ctrl.strip_width*2*disp_xaixs_wide_offset-(diasp_cnt_even*xmt_ctrl.strip_width));
    diasp_eve_cnt2=eventDspAspStripPackCheck;
    if(state==stPause)
      set_timer_pause(diasp_eve_cnt2,100,0);
    else 
      set_timer_(diasp_eve_cnt2,100,0);
    break;
    
    //----------probe disp-------------------------------
  case eventProbeDispIinit:
    probe_disp_enable=false;
    probe_disp_eve_cnt=0;
    strip_pack_cnt=0;
    strip_pack_gap=0;
    xmt_ctrl.strip_width=(cam_pram.cam_aly_xend_pos-cam_pram.cam_aly_x_pos)/(STRIP_NUM-1);
    C3000_srige_oper(SYRINGE_VALVE_BYPASS , 0);
    HAL_GPIO_WritePin(ACC_VALVE_PROBE_GPIO_Port,ACC_VALVE_PROBE_Pin,GPIO_PIN_SET);
    //stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);
    set_timer_(probe_disp.event[++probe_disp_eve_cnt],100,0);
    break;
  case eventProbeDispSkAngle:
    dSPIN_Go_To( shk_pram.up_ang_pos);
    set_timer_(probe_disp.event[++probe_disp_eve_cnt],10,0);
    break;
  case eventProbeDispStripMove:
    stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.dsp_pos);
    strip_pack_cnt=prob_disp_cnt/12;
    //  strip_pack_gap=xmt_ctrl.cam_pos-xmt_ctrl.dsp_pos;
    strip_pack_gap=xmt_ctrl.cam_pos-cam_pram.cam_aly_x_pos;
    stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.dsp_pos+(strip_pack_gap*strip_pack_cnt)+(xmt_ctrl.strip_width*(prob_disp_cnt-(12*strip_pack_cnt))));
    //stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.dsp_pos+(xmt_ctrl.strip_width*prob_disp_cnt));
    set_timer_(probe_disp.event[++probe_disp_eve_cnt],100,0);
    break;
  case eventProbeDispAixsCheck:
    if(x_reach_pos&&y_reach_pos){
      if(state==stPause)
        set_timer_pause(probe_disp.event[++probe_disp_eve_cnt],100,0);
      else  
        set_timer_(probe_disp.event[++probe_disp_eve_cnt],50,0);
    }else  set_timer_(probe_disp.event[probe_disp_eve_cnt],100,0);
    break;
  case eventProbeDispProveMove:
    // stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.dsp_pos);
    stmt_abs_move(ADDR_MOTOR_Z,ymt_ctrl.sample_width);
    set_timer_(probe_disp.event[++probe_disp_eve_cnt],50,0);
    break;
  case eventProbeDispZMoveCheck:
    if(z_reach_pos){
      if(state==stPause)
        set_timer_pause(probe_disp.event[++probe_disp_eve_cnt],100,0);
      else
        set_timer_(probe_disp.event[++probe_disp_eve_cnt],50,0);
    }else  set_timer_(probe_disp.event[probe_disp_eve_cnt],100,0);
    break;
  case eventProbeDispOper:
    // PRIME_DW_ON
    New_Pump_Run(0x01<<PRIME_DW_ON,probe_disp.vol);
    set_timer_(probe_disp.event[++probe_disp_eve_cnt],probe_disp.vol,0);
    break;
  case eventProbeDispEnd:
    // PRIME_DW_OFF;
    set_timer_(probe_disp.event[++probe_disp_eve_cnt],1000,0);
    break;
  case eventProbeDispProbeHome:
    stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);
    set_timer_(probe_disp.event[++probe_disp_eve_cnt],1500,0);
    break;
  case eventProbeDispCheck:
    prob_disp_cnt++;
    if(probe_disp.total_strip-1<prob_disp_cnt){
      #ifdef Motor_EDB
        stmt_abs_move(ADDR_MOTOR_X,0);
        stmt_abs_move(ADDR_MOTOR_Y,0);
      #endif
      
      #ifdef Motor_LEAD
        stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.bath_pos);
        stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
      #endif
      
      dSPIN_Go_To(0);
      set_timer_(probe_disp.event[++probe_disp_eve_cnt],100,0);
    }else{
      probe_disp_eve_cnt=2;
      set_timer_(probe_disp.event[probe_disp_eve_cnt],100,0);
    }
    break;
  case eventProbeDispFinalEnd:
    prob_disp_cnt=0;
    switch(state)
    {
    case stStEng:
      break;
    case stReady:
      if(state==stPause)
        set_timer_pause(eventSqNext,100,0);
      else  
        set_timer_(eventSqNext,100,0);
      break;
    case stPause:
      usb_send_pack(eventPauseRes,0);
      break;
    }
    break;
    
    
  case eventDspAspPanelCheck:
    
    break;
    
    //---------------------Auto Clean--------------------------------
  case eventAutoCleanInit:
    if(x_reach_pos&&y_reach_pos){
      auto_clean_eve_cnt=0;
      auto_cl_re_cunt=0;
      C3000_srige_oper(SYRINGE_VALVE_LEFT , 0);
      HAL_GPIO_WritePin(ACC_VALVE_PROBE_GPIO_Port,ACC_VALVE_PROBE_Pin,GPIO_PIN_RESET);
      stmt_abs_move(ADDR_MOTOR_Z,zmt_ctrl.bath_pos);
      set_timer_(auto_cl.event[++auto_clean_eve_cnt],200,0);
    }
    else
      set_timer_(eventAutoCleanInit,200,0);
    break;
  case eventAutoCleanZCheck:
    if(z_reach_pos){
      set_timer_(auto_cl.event[++auto_clean_eve_cnt],50,0);
    }else  set_timer_(auto_cl.event[auto_clean_eve_cnt],50,0);//
    break;
  case eventAutoCleanBathMove:
    // stmt_abs_move(ADDR_MOTOR_Y,asp_mt.disp_bath_y);
    set_timer_(auto_cl.event[++auto_clean_eve_cnt],1000,0);
    break;
  case eventAutoCleanAixsCheck:
    if(x_reach_pos&&y_reach_pos){
      set_timer_(auto_cl.event[++auto_clean_eve_cnt],50,0);
    }else  set_timer_(auto_cl.event[++auto_clean_eve_cnt],100,0);
    break;
  case eventAutoCleanAspOut:
    servo_mv(asp_mt.down_pos);
    HAL_Delay(1000);
    if(sq_start_dw_washing_flg)
      New_Pump_Run(((0x01<<WS_PUMP1)|(0x01<<WS_PUMP2)),auto_cl.disp_time);
    else
      New_Pump_Run(0x1FE,auto_cl.disp_time);
    ASP_PUMP_ON;
    set_timer_(auto_cl.event[++auto_clean_eve_cnt],auto_cl.asp_time+100,0);  
    break;
  case eventAutoCleanEnd:
    ASP_PUMP_OFF;
    servo_mv(asp_mt.up_pos);
    //stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
    //stmt_abs_move(ADDR_MOTOR_Y, ymt_ctrl.sample_width);
    auto_cl_re_cunt++;
    set_timer_(auto_cl.event[++auto_clean_eve_cnt],500,0);
    break;
  case eventAutoCleanNext:
    if(auto_cl_re_cunt<auto_cl.repeat)
    {
      auto_clean_eve_cnt=2;//eventAutoCleanBathMove
      set_timer_(auto_cl.event[auto_clean_eve_cnt],auto_cl.rootin_delay,0); 
    }else{
      #ifdef Motor_EDB
        stmt_abs_move(ADDR_MOTOR_X,0); 
        stmt_abs_move(ADDR_MOTOR_Y,0);
      #endif 
      #ifdef Motor_LEAD
        stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.bath_pos); 
        stmt_abs_move(ADDR_MOTOR_Y,ymt_ctrl.bath_pos);
      #endif    

      usb_send_pack(eventAutoCleanFuncEnd, dev_send_buf);
      auto_cl_re_cunt=0;
      beep(80, 2);
      set_timer_(auto_cl.event[++auto_clean_eve_cnt],100,0); 
    }
    break;
    
  case eventAutoCleanPass:
    if(sq_start_dw_washing_flg){
      sq_start_dw_washing_flg=false;
      set_timer_(eventSpuResetEnd,100,0);
    }//else
     //New_Pump_RollBack(0x1FE,16000);
    break;
    
  case eventBathFull:
    error(errWaterLevel,0);
    HAL_Delay(500);
    HAL_NVIC_SystemReset(); 
    break;
  case eventRbOper:
    break;
  case eventRbEnd:
    break;
  case eventdryOper:
    dry_cnt=true;
    HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_RESET);
    break;
  case eventdryEnd:
    dry_cnt=false;
    HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_SET);
    dry.time=0;
    dSPIN_Go_To(0);
    switch(state)
    {
    case stStEng:
      break;
    case stReady:
      set_timer_(eventSqNext,100,0);
      break;
    }
    break;
    
  case eventTimer500ms:
    //send_test(&serve_amp, 1);
    //HAL_GPIO_WritePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin,GPIO_PIN_RESET);
    break;
  case eventTimer100ms:
    THERMISTOR1_INT();
    break;
  case eventTimer1s:
    if(state!=stPause){
      if(dry_cnt)
        if(!dry.time--)
          give_event(eventdryEnd,0);
      
      if(state==stReady) 
        if(pr_time_sec)
          pr_time_sec--;
      
      #ifdef Motor_EDB
        if(!lld_com_timeout_cnt())
        {
          Sample_LLD_ClearEvents();
          dbg_serial("(eventSmpInit_Retry)");
          smple_ev_cnt=0;
          motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,2,2,0);
          stm_reset();
          motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,4,2, zmt_ctrl.sample_pos);
          z_homeing();
          HAL_Delay(1000);
          sk_state=stShkrStby;
          shake_home();

          LLD_homeing_flag = true;
          lld_repeat_en(DISEN);
        }

        if(LLD_homeing_flag) 
        {
          if(z_reach_pos && sk_state == stShkrOperEnd)
          {
            LLD_homeing_flag = false;
            set_timer_(smpl_pr.event[smple_ev_cnt],100,0);          
          }
        }  

        if(!call_com_timeout_cnt()) 
        {
          Sample_LLD_ClearPrimeEvents();
          dbg_serial("(eventSmpPrimeInit_Retry)");
          smp_prime_eve_cnt = 0;
          motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,2,2,0);
          stm_reset();
          motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,4,2, zmt_ctrl.sample_pos);
          z_homeing();
          HAL_Delay(1000);
          sk_state=stShkrStby;
          shake_home();

          PRIME_homeing_flag = true;
          com_time_out_set(DISEN);  // Retry code
        }
          
        if(PRIME_homeing_flag) 
        {
          if(z_reach_pos && sk_state == stShkrOperEnd)
          {
            PRIME_homeing_flag = false;
            set_timer_(smpl_prime.event[smp_prime_eve_cnt],200,0);         
          }
        }  
      #endif

      #ifdef Motor_LEAD
        if(!lld_com_timeout_cnt())
        {
          Sample_LLD_ClearEvents();
          dbg_serial("(eventSmpInit_Retry)");
          smple_ev_cnt=0;
          //motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,2,2,0);
          stm_reset();
          //motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,4,2, zmt_ctrl.sample_pos);
          z_homeing();
          HAL_Delay(1000);
          sk_state=stShkrStby;
          shake_home();

          LLD_homeing_flag = true;
          lld_repeat_en(DISEN);
        }

        if(LLD_homeing_flag) 
        {
          if(z_reach_pos && sk_state == stShkrOperEnd)
          {
            LLD_homeing_flag = false;
            set_timer_(smpl_pr.event[smple_ev_cnt],100,0);          
          }
        }  

        if(!call_com_timeout_cnt()) 
        {
          Sample_LLD_ClearPrimeEvents();
          dbg_serial("(eventSmpPrimeInit_Retry)");
          smp_prime_eve_cnt = 0;
          //motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,2,2,0);
          stm_reset();
          //motor_cmd_send(ADDR_MOTOR_Z,INST_SGP,4,2, zmt_ctrl.sample_pos);
          z_homeing();
          HAL_Delay(1000);
          sk_state=stShkrStby;
          shake_home();

          PRIME_homeing_flag = true;
          com_time_out_set(DISEN);  // Retry code
        }
          
        if(PRIME_homeing_flag) 
        {
          if(z_reach_pos && sk_state == stShkrOperEnd)
          {
            PRIME_homeing_flag = false;
            set_timer_(smpl_prime.event[smp_prime_eve_cnt],200,0);         
          }
        }  
      #endif

    }
    
    if(heat_pad_en)
      HANT_PAD_ON;
    else
      HANT_PAD_OFF;
    
    switch(state)
    {
    case stStEng:
      break;
    case stReady:
      if((full_pr==Sample_LLD)||(full_pr==Incubation)||(full_pr==Incubation_Ws)||(full_pr==Dry))
        if(!asp_roll_back--){
          ASP_PUMP_ON;
          set_timer_(eventAspRollBackEnd,3000,0);
        }
      
      break;
    }
    //HAL_GPIO_TogglePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin);
    break;
  case eventAspRollBackEnd:
    ASP_PUMP_OFF;
    if((full_pr==Incubation)||(full_pr==Incubation_Ws)||(full_pr==Dry))
      asp_roll_back=60*5;
    else
      asp_roll_back=60;
    break;
    
    
    
    
  default: break;
  }
  
  return event;
}


