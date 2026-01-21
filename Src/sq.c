#include "main.h"
#include "string.h"
enum step st=St_None;
struct sequence sq[12];
struct sample_strip smp_strip[STRIP_NUM_END+1];
struct strip_err_bit str_errbit;
byte penel_sel=0;
int pr_time_min=0;
int pr_time_sec=0;
int base_time=0;

byte manual_check_state=0;
bool Ccd_Blocker_check_state = false;
bool Ccd_beep_en = false;


bool aging_mode=false;

void sq_mem_init()
{
  sq_read();
}
/*
eventSqInMode=0x0801,
eventSqTotalStep,
eventSqSave,
eventSqId,
eventReserve29,
eventReserve30,
eventReserve31,
eventReserve32,
eventSqStepNum,
eventSqPrNum,       
eventSqPrParamByte,
eventSqPrParamWord,
eventSqPrParamDword,
*/

byte total_step=0;
byte step_cnt=0,pr_cnt=0;
byte pr_step_cnt=0;
byte full_step=0,full_pr=0;
byte full_step_cnt=0,full_pr_cnt=0,smp_retime_pr_cnt=0;
byte full_total_strip=0;
byte smp_strp_mach_cnt=0;
byte sq_smp_strp_mach_cnt=0;
byte sq_strp_mach=0;
uint rbk_pump_num=0;
uint rbk_time=0;
uint32_t smp_reult_bit[2]={0,};

uint dw_prime_vol=0;
bool sq_start_dw_washing_flg=false;
byte state_time_min=60;
event sq_ctrl(event event)
{
    byte dev_send_buf[4]={0,}; 
  switch(event)
  {
    //--------------sq generator--------------
  case eventSqInMode:
    if(usb_data_buf[3]){
      step_cnt=0;
      pr_cnt=0;
      total_step=0;
      memset(sq,0,sizeof(sq));
      //    sq_read();
    }else {
      step_cnt=0;
      pr_cnt=0;
      total_step=0;
      sq_write();
      beep(1000, 1);
    }
    
    usb_send_pack(eventSqInMode,usb_data_buf);
    break;
  case eventSqTotalStep:
    total_step=usb_data_buf[3];
    usb_send_pack(eventSqTotalStep,usb_data_buf);
    break;
  case eventSqSave:
    usb_send_pack(eventSqSave,usb_data_buf);
    break;
  case eventSqId:
    usb_send_pack(eventSqId,usb_data_buf);
    break;
  case eventSqStepNum:
    sq[step_cnt].stNum=usb_data_buf[3];
    step_cnt++;
    usb_send_pack(eventSqStepNum,usb_data_buf);
    break;
  case eventSqPrNum:
    sq[step_cnt-1].prNum[usb_data_buf[3]]=usb_data_buf[1];
    pr_cnt=usb_data_buf[3];
    usb_send_pack(eventSqPrNum,usb_data_buf);
    break;
  case eventSqPrParamDword:
    sq[step_cnt-1].dword[pr_cnt]=merge_32bit( sq[step_cnt].dword[pr_cnt],usb_data_buf);
    usb_send_pack(eventSqPrParamDword,usb_data_buf);
    break;
    //---------------------sq start-----------
  case eventSeqStart:
    sq_start_dw_washing_flg=true;
    if(usb_data_buf[3]){
      state=stReady;
      full_step_cnt=0;
      full_pr_cnt=0;
      full_step=0;
      full_pr=0;
      full_total_strip=smp_strp_mach_cnt;
      smp_strp_mach_cnt=0;
      //pump_param_read();
      give_event(eventSpuResetAsp,0);
    }
    else
    {
      state=stReady;
      full_step_cnt=usb_data_buf[2]-1;
      full_pr_cnt=0;
      full_step=0;
      full_pr=0;
      full_total_strip=smp_strp_mach_cnt;
      smp_strp_mach_cnt=0;
      //pump_param_read();
      set_timer_(eventSpuResetAsp,100,0);
    }
    step_time_cal(full_step_cnt);
    usb_send_pack(eventSeqStart,usb_data_buf);
    break;
    
  case eventSampStripMatch:
    smp_strip[smp_strp_mach_cnt].strip=usb_data_buf[0];
    smp_strip[smp_strp_mach_cnt].smpl=usb_data_buf[1];
    smp_strip[smp_strp_mach_cnt].penel=usb_data_buf[2];
    smp_strip[smp_strp_mach_cnt].manual=usb_data_buf[3];
    full_total_strip=smp_strp_mach_cnt++;
    usb_send_pack(eventSampStripMatch,usb_data_buf);
    break;
  
  case eventSampStripMatchRun:
    smp_strp_mach_cnt=0;
    memset(smp_strip,0,sizeof(smp_strip));
    usb_send_pack(eventSampStripMatchRun,0);
    break;
    
    
    //-----------------full sq ruting-------
  case eventSqFullSqAly:
  //////////////////////////////////////////////////////////////////////   
    if(aging_mode == true && full_step_cnt==10)  // aging TEST
    {
      full_step_cnt = 0;
      full_pr_cnt = 0;
      smple_rack_cnt=0;
      sq_strp_mach=0;
      sk_state=stShkrStby;
      state = stBoot;
      set_timer_(eventSpuOn,2000,0);
      break;
    }
  //////////////////////////////////////////////////////////////////////       

    state_led_init();

    stmt_speed_set(ADDR_MOTOR_X,300);
    stmt_speed_set(ADDR_MOTOR_Y,300);
    door_sen_flg=true;
    full_step=sq[full_step_cnt].stNum;
    full_pr=sq[full_step_cnt].prNum[full_pr_cnt];
    
    dbg_serial("eventSqFullSqAly");

    switch(full_step)
    {
    case Wetting:
      break;
    case Sample:
      pr_step_cnt=Sample;
      break;
    case Washing_WS:
      if(pr_step_cnt==Sample){
        pr_step_cnt=0;
        usb_data_buf[0]=full_step_cnt+1;
        usb_data_buf[1]=full_step;
        usb_data_buf[2]=full_pr;
        usb_send_pack(eventCtAndPt,usb_data_buf);
      }
    case Conj_ANA:
    case Anti_ALG:
    case Enzy_ALG:
    case Stop_ANA:
    case Substrate:
    case Washing_DW:
    case Drying:
    case Auto_Clean:
    case Analysis:
      break;
    default:
      state=stStby;
      beep(1000, 3);
      break;
    }
   


    // if(full_step!=Sample)
    switch(full_pr)
    {
    case Prime:
      disp_sgl.disp_pump_num=prime.disp_pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff);
      dw_prime_vol=prime.disp_vol=(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)*1000;
      if(((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff)==PRIME_DW)
      {
        smpl_prime.pm_num=SMP_PRME_PP;
        lld_fc=lldFuncNone;
        smple_rack_cnt=1;
        auto_prime_flg=false;
        C3000_srige_oper(SYRINGE_INIT , 0);
        HAL_Delay(2000);
        dbg_serial("PRIME_eventSmpPrimeInit"); 
        set_timer_(eventSmpPrimeInit,200,0);
      }
      else
      {
        set_timer_(eventPrimeIinit,200,0);
      }
      break;
    case Prime1:
      disp_sgl.disp_pump_num=prime.disp_pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff);
      disp_sgl.disp_pump_num|=prime.disp_pump_num|=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x000000ff);
      prime.disp_vol=(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)*1000;
      set_timer_(eventPrimeIinit,200,0);
      break;
    case Prime2:
      disp_sgl.disp_pump_num=prime.disp_pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>28)&0x0000000f);
      disp_sgl.disp_pump_num|=prime.disp_pump_num|=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x0000000f);
      disp_sgl.disp_pump_num|=prime.disp_pump_num|=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x000000ff);
      prime.disp_vol=(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)*1000;
      set_timer_(eventPrimeIinit,200,0);
      break;
    case DispAsp:
      HAL_GPIO_WritePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin,GPIO_PIN_RESET);
      diasp.pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff);
      probe_disp.total_strip=diasp.total_strip=full_total_strip;
      if(((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff)==PRIME_DW)
      {
        probe_disp.vol=(pm_pram.vol[PRIME_DW-1]*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff))/500;//(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff);
        set_timer_(eventProbeDispIinit,200,0);
      }
      else
      {
        diasp.vol=pm_pram.vol[((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff)-1]*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)/500;
        set_timer_(eventDspAspIinit,200,0);
      }
      break;
    case DispAsp2:
      HAL_GPIO_WritePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin,GPIO_PIN_RESET);
      diasp.pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff);
      diasp.pump_num|=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x000000ff);
      //  diasp.vol=(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff);
      if(!only_washing_flg)
      {
        diasp.total_strip=full_total_strip;
      }
      diasp.vol=((pm_pram.vol[WS_PUMP1-1]+pm_pram.vol[WS_PUMP2-1])/2);
      diasp.vol=((diasp.vol*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff))/500);
      set_timer_(eventDspAspIinit,200,0);
      break;
    case DispAsp3:
      HAL_GPIO_WritePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin,GPIO_PIN_RESET);
      if((smp_strip[0].penel==Inhalant)||(smp_strip[0].penel==Inhalant_CON)){
        diasp.pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>28)&0x0000000f);
        diasp.vol=pm_pram.vol[((sq[full_step_cnt].dword[full_pr_cnt]>>28)&0x0000000f)-1]*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)/500;
      }else {//if(smp_strip[0].penel==Food){
        diasp.pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x0000000f);
        diasp.vol=pm_pram.vol[((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x0000000f)-1]*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)/500;
      }/*else if(smp_strip[0].penel==Special1){
        diasp.pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x0000000f);
        diasp.vol=pm_pram.vol[((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x0000000f)-1]*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)/500;
      }//else if(smp_strip[0].penel==Special2){*/
      //}
      penel_sel=DispAsp3;
      diasp.total_strip=full_total_strip;
      set_timer_(eventDspAspIinit,200,0);
      break;
    case Disp:
      HAL_GPIO_WritePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin,GPIO_PIN_RESET);
      disp_sgl.disp_pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff);
      //disp_sgl.disp_vol=(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff);
      disp_sgl.total_strip=full_total_strip;
      disp_sgl.disp_vol=pm_pram.vol[((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff)-1]*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)/500;
      set_timer_(eventDspIinit,200,0);
      break;
    case Disp2:
      HAL_GPIO_WritePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin,GPIO_PIN_RESET);
      disp_sgl.disp_pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff);
      disp_sgl.disp_pump_num|=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x000000ff);
      //disp_sgl.disp_vol=(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff);
      disp_sgl.disp_vol=((pm_pram.vol[WS_PUMP1-1]+pm_pram.vol[WS_PUMP2-1])/2);
      disp_sgl.disp_vol=(disp_sgl.disp_vol*(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff))/500;
      disp_sgl.total_strip=full_total_strip;
      set_timer_(eventDspIinit,200,0);
      break;
    case Asp:
      HAL_GPIO_WritePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin,GPIO_PIN_RESET);
      asp_re_cunt=((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x0000000f)-1;
      asp.total_strip=full_total_strip;
      set_timer_(eventAspIinit,200,0);
      break;
    case Incubation:
      HAL_GPIO_WritePin(M_INCUBATION_GPIO_Port,M_INCUBATION_Pin,GPIO_PIN_RESET);
      base_time = (sq[full_step_cnt].dword[full_pr_cnt] & 0x0000ffff) * 60;
      strip_num_shake_time(); //edit incubation
      sk_state=stShkrStby;
      shake_run();
      break;
    case Incubation2:  
      HAL_GPIO_WritePin(M_INCUBATION_GPIO_Port,M_INCUBATION_Pin,GPIO_PIN_RESET);
      shk_pram.run_time_s = (sq[full_step_cnt].dword[full_pr_cnt] & 0x0000ffff) * 60;
      sk_state=stShkrStby;
      shake_run();
      break;
    case Incubation_Ws:
      step_time_cal(full_step_cnt);
      HAL_GPIO_WritePin(M_INCUBATION_GPIO_Port,M_INCUBATION_Pin,GPIO_PIN_RESET);
      base_time = (sq[full_step_cnt].dword[full_pr_cnt] & 0x0000ffff) * 60;
      strip_num_shake_time();
      sk_state=stShkrStby;
      shake_run();
      //prime.disp_pump_num=0x01<<PRIME_DW;
      auto_clean_repeat_flg=true;
      smpl_prime.pm_num=0x01<<WS_PUMP;//SMP_PRME_PP;
      lld_fc=lldFuncNone;
      smple_rack_cnt=0;
      set_timer_(eventSmpPrimeInit,500,0);
      break;

    case Incubation_Ws2:
      step_time_cal(full_step_cnt);
      HAL_GPIO_WritePin(M_INCUBATION_GPIO_Port,M_INCUBATION_Pin,GPIO_PIN_RESET);
      shk_pram.run_time_s = (sq[full_step_cnt].dword[full_pr_cnt] & 0x0000ffff) * 60;
      sk_state=stShkrStby;
      shake_run();
      auto_clean_repeat_flg=true;
      smpl_prime.pm_num=0x01<<WS_PUMP;
      lld_fc=lldFuncNone;
      smple_rack_cnt=0;
      set_timer_(eventSmpPrimeInit,500,0);
      break;

    case Sample_LLD:
      auto_prime_flg=false;
      smp_prime_oper_flg=false;
      sq_smp_strp_mach_cnt=0;
      sq_strp_mach=0;
      smple_rack_cnt=0;
      smpl_pr.sample_num=full_total_strip;//smp_strip[0].smpl;
      smple_rack_cnt=smp_strip[sq_smp_strp_mach_cnt].smpl-1;//full_total_strip;
      sq_strp_mach=smp_strip[sq_smp_strp_mach_cnt].strip-1;
      dw_prime_vol=4500;
      // jude_cnt=smpl_pr.sensing_cnt;
      smpl_prime.pm_num=SMP_PRME_PP;
      lld_fc=lldFullFunc;
      jude_cnt=smpl_pr.sensing_cnt-1;

      if(aging_mode == true)
      {
        smple_rack_cnt = 0;
        sq_strp_mach = 0;
      }  

      if(smp_strip[sq_smp_strp_mach_cnt].manual)
      {
        smp_reult_bit[0]=1<<sq_smp_strp_mach_cnt;
        set_timer_(eventSqSamplNumMach,10,0);
      }
      else
      {
        set_timer_(eventSmpPrimeInit,10,0);
      }
        
      beep(1000, 1);
      usb_data_buf[0]=full_step_cnt+1;
      usb_data_buf[1]=full_step;
      usb_data_buf[2]=full_pr;
      usb_send_pack(eventCtAndPt,usb_data_buf);
      break;
    case Dry:
      HAL_GPIO_WritePin(M_DRY_GPIO_Port,M_DRY_Pin,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(FAN_L_GPIO_Port,FAN_L_Pin,GPIO_PIN_RESET);
      dwHsSW_Delay_ms(1000);
      dSPIN_Go_To( shk_pram.dry_pos);
      dry.time=(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)*60;
      set_timer_(eventdryOper,1000,0);     
      break;
    case RollBack:
      if(((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff)==0x0f)
         rbk_pump_num=0x01<<PRIME_DW_ON;
      else
      rbk_pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff);
      rbk_time=(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)*1000;

      New_Pump_RollBack(rbk_pump_num,rbk_time);
      
      set_timer_(eventSqNext,rbk_time+500,0); 
      break;
    case RollBack1:
      rbk_pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x000000ff);
      rbk_pump_num|=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x000000ff);
      rbk_time=(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)*1000;;

      New_Pump_RollBack(rbk_pump_num,rbk_time);
      
      set_timer_(eventSqNext,rbk_time+500,0); 
      break;
    case RollBack2:
      rbk_pump_num=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>28)&0x0000000f);
      rbk_pump_num|=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>24)&0x0000000f);
      rbk_pump_num|=0x01<<((sq[full_step_cnt].dword[full_pr_cnt]>>16)&0x000000ff);
      rbk_time=(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff)*1000;;

      New_Pump_RollBack(rbk_pump_num,rbk_time);
      
      set_timer_(eventSqNext,rbk_time+500,0); 
      break;
    case Analy:
      HAL_GPIO_WritePin(M_ANALYSIS_GPIO_Port,M_ANALYSIS_Pin,GPIO_PIN_RESET); 
      usb_data_buf[0]=full_step_cnt+1;
      usb_data_buf[1]=full_step;
      usb_data_buf[2]=full_pr;
      usb_send_pack(eventCtAndPt,usb_data_buf);
      dwHsSW_Delay_ms(1000);
      dSPIN_Go_To( shk_pram.aly_pos);
      break;
    case Pause:
      dSPIN_Go_To( shk_pram.down_ang_pos);
      state=stPauseSq;
      beep(1000, 3);
      break;
    default:break;
    }
    usb_data_buf[0]=full_step_cnt+1;
    usb_data_buf[1]=full_step;
    usb_data_buf[2]=full_pr;
 //   usb_send_pack(eventCtAndPt,usb_data_buf);
    break;
    
  case eventCtAndPt:
    usb_data_buf[0]=full_step_cnt+1;
    usb_data_buf[1]=full_step;
    usb_data_buf[2]=full_pr;
    usb_send_pack(eventCtAndPt,usb_data_buf);
    break;
  case eventSqSamplNumMach:
    if(sq_smp_strp_mach_cnt<full_total_strip-1){
      sq_smp_strp_mach_cnt++;
      if(aging_mode==false)
      {
        smple_rack_cnt=smp_strip[sq_smp_strp_mach_cnt].smpl-1;
        sq_strp_mach=smp_strip[sq_smp_strp_mach_cnt].strip-1;
      }
      else
      {
        smple_rack_cnt++;
        sq_strp_mach++;

        if(smple_rack_cnt>=48 && sq_strp_mach>=48)
        {
          smple_rack_cnt=0;
          sq_strp_mach=0;
        }
      }
      if(smp_strip[sq_smp_strp_mach_cnt].manual)
      {
        set_timer_(eventSqSamplNumMach,10,0);
      }  
      else
      {
        set_timer_(eventSmpPrimeInit,100,0);
      }  
    }
    else
    {
      for(byte i=0;i<STRIP_NUM_END+1;i++){
        if(smp_strip[i].manual){
                  if(smp_strip[i].strip<33)
                     smp_reult_bit[0]|=1<<smp_strip[i].strip-1;
                  else
                     smp_reult_bit[1]|=1<<((smp_strip[i].strip-1)-32);
          /*if(i<32)
            smp_reult_bit[0]|=1<<smp_strip[i].strip-1;
          else
            smp_reult_bit[1]|=1<<(smp_strip[i].strip-1)-32;
          */
        }
      }
      pr_time_sec=0;
      beep(80, 2);
      smpl_pr.sample_num=0;
      #ifdef Motor_EDB
        stmt_abs_move(ADDR_MOTOR_X,0);
      #endif
      
      #ifdef Motor_LEAD
        stmt_abs_move(ADDR_MOTOR_X,xmt_ctrl.bath_pos);
      #endif
      if((full_pr==Sample_LLD)&&(full_step==Sample)){
        smp_retime_pr_cnt=full_pr_cnt;
        for(;;){
        smp_retime_pr_cnt++;
        pr_time_cal(full_step_cnt,smp_retime_pr_cnt);
         if(!sq[full_step_cnt].prNum[smp_retime_pr_cnt])
         break;
        }
        smp_retime_pr_cnt=0;
        dev_send_buf[0]=pr_time_sec/60;
        dev_send_buf[1]=0;
        dev_send_buf[2]=0;
        dev_send_buf[3]=0;
        usb_send_pack(eventFullSmpTime,dev_send_buf);
      }
      if(lld_fc==lldFullFunc){
        lld_fc=lldFuncNone;
        set_timer_(eventSmpPrimeInit,100,0);
      }
    }
    break;
  case eventSqManualCheck:  
    if(!manual_check_state){
      beep(1000, 1);
        set_timer_(eventSqManualCheck,2000,0);
    }else
        set_timer_(eventSqNext,2000,0);

    //for(manual_check_cnt =0;manual_check_cnt<full_total_strip-1;manual_check_cnt++)
  break;
  
  case eventSqManualStart:
    manual_check_state=1;
    if(press_sens_flg){
      press_sens_flg=false;
      set_timer_(eventSqNext,1000,0);     //pressuser  exception handing 
    }
    usb_send_pack(eventSqManualStart,dev_send_buf);
    break;

  case eventSqCcdBlockerCheck:
    if(Ccd_Blocker_check_state)
    {
      if(Ccd_beep_en)
      {
        beep(1000, 1);
      }  
      set_timer_(eventSqCcdBlockerCheck,2000,0);
    }
    else
    {
      dSPIN_Go_To(0);
      set_timer_(eventSqNext,2000,0);
    }
    break;
    
  case eventSqCcdBlockerFlagOn:
    Ccd_Blocker_check_state = true;
    Ccd_beep_en = true;
    usb_send_pack(eventSqCcdBlockerFlagOn,dev_send_buf);
    break;

  case eventSqCcdBlockerStart:
    Ccd_Blocker_check_state = false;
    usb_send_pack(eventSqCcdBlockerStart,dev_send_buf); 
    break;

  case eventSqCcdBlockerBeepControl:
    Ccd_beep_en = false;
    usb_send_pack(eventSqCcdBlockerBeepControl,dev_send_buf); 
    break;

  case eventSqNext:
    if(state==stReady){
      if(Ccd_Blocker_check_state)
      {
        if(sq[full_step_cnt].prNum[full_pr_cnt+1]==Sample_LLD){
          Ccd_beep_en = true;
          dSPIN_Go_To(shk_pram.down_ang_pos);
          usb_send_pack(eventSqOpenCcdBlockerWindow,dev_send_buf);
          give_event(eventSqCcdBlockerCheck,0);
          break;
        }
      }  
      
      if((smp_reult_bit[0]||smp_reult_bit[1])&&!manual_check_state)
      {
        if(sq[full_step_cnt].prNum[full_pr_cnt]==Sample_LLD){
          dSPIN_Go_To(shk_pram.down_ang_pos);
          sort_8bit(smp_reult_bit[0],dev_send_buf);
          usb_send_pack(eventManualErrorSampBit1,dev_send_buf);
          sort_8bit(smp_reult_bit[1],dev_send_buf);
          usb_send_pack(eventManualErrorSampBit2,dev_send_buf);
          give_event(eventSqManualCheck,0);
          manual_check_state=0;
          break;
        }
      }

      sq[full_step_cnt].prNum[full_pr_cnt++];
      if(!sq[full_step_cnt].prNum[full_pr_cnt]){
        full_pr_cnt=0;
        full_step_cnt++;
        step_time_cal(full_step_cnt);  
      }
      
      HAL_GPIO_WritePin(FAN_L_GPIO_Port,FAN_L_Pin,GPIO_PIN_SET);
      manual_check_state=0;
      
      set_timer_(eventSqFullSqAly,2000,0);
    }else if(state==stPause)
      set_timer_pause(eventSqNext,100,0);
    else{
      sq[full_step_cnt].prNum[full_pr_cnt++];
      if(!sq[full_step_cnt].prNum[full_pr_cnt]){
        full_pr_cnt=0;
        full_step_cnt=0;
        only_washing_flg=false; 
        beep(1000, 1);
        usb_send_pack(eventWashingFunc, usb_data_buf);
      }else{
        if( only_washing_flg)
          set_timer_(eventSqFullSqAly,2000,0);
        else{
          full_pr_cnt=0;
          full_step_cnt=0;
        }
      }
      
    }
    break;
  case eventTotalSN:
    if( sq[full_step_cnt].stNum!=0xfe){
      full_step_cnt++;
      if(full_step_cnt>12){
        dev_send_buf[3]=0;
        //full_step_cnt=0;
        usb_send_pack(eventTotalSN,dev_send_buf);
      }else
      give_event(eventTotalSN,0);
    }else{
       dev_send_buf[3]=full_step_cnt;
    // full_step_cnt=0;
     usb_send_pack(eventTotalSN,dev_send_buf);
    }
    break;
  case eventStepTime:
    step_time_cal(usb_data_buf[3]-1); 
    dev_send_buf[0]=0;
    dev_send_buf[1]=usb_data_buf[3];
    dev_send_buf[2]=0;
    dev_send_buf[3]=pr_time_min/60;
/*
    dev_send_buf[0]=full_step;
    dev_send_buf[1]=0;
    dev_send_buf[2]=pr_time_min/60;
    dev_send_buf[3]=(pr_time_min-pr_time_sec)/60*/
    usb_send_pack(eventStepTime,dev_send_buf);
    break;
  case eventSetpId:
     dev_send_buf[3]=sq[usb_data_buf[3]-1].stNum;
    usb_send_pack(eventSetpId,dev_send_buf);
    break;
    
  case eventSeqStop:
    New_Pump_Reset();
    door_sen_flg=false;
    beep(1000, 2);
    init_event();
    dSPIN_Hard_Stop();
    state=stStopPrepare;
    cam_led_off();
    BATH_DW_OFF;
    PRIME_DW_OFF;
    ASP_PUMP_OFF;
    HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ACC_VALVE_PROBE_GPIO_Port,ACC_VALVE_PROBE_Pin,GPIO_PIN_SET);
    usb_send_pack(eventSeqStopRes,0);
    HAL_Delay(50);
    HAL_NVIC_SystemReset();
    break;
    
    
    
  case eventDevReset:
    New_Pump_Reset();
    state_led_init();
    door_sen_flg=false;
    init_event();
    cam_led_off();
    dSPIN_Hard_Stop();
    BATH_DW_OFF;
    PRIME_DW_OFF;
    ASP_PUMP_OFF;
    HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ACC_VALVE_PROBE_GPIO_Port,ACC_VALVE_PROBE_Pin,GPIO_PIN_SET);
    usb_send_pack(eventDevResetRes,0);
    if(state==stReady||state==stStby||state==stStEng){
      set_timer_(eventSpuResetAsp,800,0);
      state=stStopPrepare;
    }
    break;
    
    
  case eventSpuResetAsp:
    HAL_GPIO_WritePin(FAN_L_GPIO_Port,FAN_L_Pin,GPIO_PIN_RESET);
    servo_mv(asp_mt.up_pos);
    set_timer_(eventSpuResetZ,300,0);
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
        set_timer_(eventSpuResetEnd,8000,0);
        home_flag=FALSE;
      }else
        set_timer_(eventSpuResetXY,50,0);
    }
    break;
  case eventSpuResetEnd:
    cam_led_off();
    full_pr_cnt=0;
    HAL_GPIO_WritePin(FAN_L_GPIO_Port,FAN_L_Pin,GPIO_PIN_SET);
    switch(state)
    {
    case stStEng:
      //give_event(eventDspIinit,0);
      break;
    case stReady:
      if(sq_start_dw_washing_flg){
        auto_cl.rootin_delay=500;
        auto_cl.repeat=1;
        // smp_prime_dw_check_flg=false;
        auto_clean_repeat_flg=true;
        smpl_prime.pm_num=0x01<<WS_PUMP;//SMP_PRME_PP;
        smple_rack_cnt=0;
        lld_fc=lldFuncNone;
        probe_disp_enable=false;
        auto_prime_flg=true;
        set_timer_(eventSmpPrimeInit,1000,0);
        give_event(eventAutoCleanInit,0);
       // set_timer_(eventSqFullSqAly,300,0);
      }else
        set_timer_(eventSqFullSqAly,300,0);
      break;
    case stStopPrepare:
      state=stStby;
      beep(1000, 1);
      
      break;
    }
    break;  
    
    
    
  case eventCrrtTotalTime:
    dev_send_buf[0]=full_step;
    dev_send_buf[1]=0;
    dev_send_buf[2]=pr_time_min/60;
    dev_send_buf[3]=(pr_time_min-pr_time_sec)/60;
    usb_send_pack( eventCrrtTotalTime,dev_send_buf);
    break;
  case eventCntinRun:       
    usb_send_pack(eventCntinRunRes,0);
    break;
  case eventStepSigRun:      
    usb_send_pack(eventStepSigRunRes,0);
    break;
  case eventStPsSigRun:     
    usb_send_pack(eventStPsSigRunRes,0);
    break;
  case eventDoorClose:
    break;
  case eventDoorOpen:
    state=stPause;
    usb_send_pack(eventDoorAlarm,0);
    break;
  case eventTimer1s: //볼것
    if(state==stReady)
    {
      if(full_step!=Analysis && full_pr!=Sample_LLD)
      {
        if(!state_time_min--)
        {
          usb_data_buf[0]=full_step_cnt+1;
          usb_data_buf[1]=full_step;
          usb_data_buf[2]=full_pr;
          usb_send_pack(eventCtAndPt,usb_data_buf);
          dev_send_buf[0]=full_step;
          dev_send_buf[1]=0;
          dev_send_buf[2]=pr_time_min/60;
          dev_send_buf[3]=(pr_time_min-pr_time_sec)/60;
          usb_send_pack(eventCrrtTotalTime,dev_send_buf);

          set_timer_(eventAirTempRes,5,0);
          state_time_min=120;
        }
      }  
    }
    break;

  case TEST_AGING_MODE:
    state = stReady;
    sq_start_dw_washing_flg=true;
    aging_mode = true;
    full_step_cnt = 0x01;
    full_pr_cnt = 0x00;
    full_total_strip = 1;
    set_timer_(eventSqFullSqAly,10,0);
    break;
    


    // case eventCtAndPt:
    //   break;
  default:break;
  }
  
  return event;
}

bool get_pasue_event(uint16_t eve)
{
  switch(eve)
  {
  case 0:
  case eventAspOut:
  case eventAspOper:
  case eventAspOperEnd:
  case eventAspAixsCheck:
  case eventAspCheck:
  case eventTimer500ms:
  case eventTimer100ms:
  case eventTimer1s:
  case eventAspTimeOut:
  case eventDspAspOut:
  case eventDspAspOper:
  case eventDspAspEnd:
  case eventProbeDispOper:
  case eventProbeDispEnd:
  case eventSmpSygAspOper:
    
    return true;
    break;
  default: 
    return false;
    break;
  }
}


void strip_num_shake_time() {
  
  if (base_time < 600) {  //min 
    shk_pram.run_time_s = base_time;
  } else {
    int additional_time = 0; //sec

    switch (full_total_strip) {
      case 1 ... 12:
        additional_time = 240;
        break;
      case 13 ... 24:
        additional_time = 180;
        break;
      case 25 ... 36:
        additional_time = 120;
        break;
      case 37 ... 48:
        additional_time = 60;
        break;
      case 49 ... 60:
        break;
    }
    shk_pram.run_time_s = base_time + additional_time;
  }
}

void pr_incubation_time(){
 
  if (pr_time_sec < 600) {  //min 
    return;
  } else {
    int additional_pr_time = 0; //sec

    switch (full_total_strip) {
      case 1 ... 12:
        additional_pr_time = 240;
        break;
      case 13 ... 24:
        additional_pr_time = 180;
        break;
      case 25 ... 36:
        additional_pr_time = 120;
        break;
      case 37 ... 48:
        additional_pr_time = 60;
        break;
      case 49 ... 60:
        break;
    }
    pr_time_sec = pr_time_sec + additional_pr_time;
  }
}

void step_time_cal(byte step)
{
  byte pr_ct=0;
  pr_time_sec=0;
  for(;;){
    if(sq[step].prNum[pr_ct]!=0){
      pr_time_cal(step,pr_ct);
      pr_ct++;
    }else{
      pr_time_min=pr_time_sec;
      pr_time_min+=60;
      break;
    }
  }
}

void pr_time_cal(byte step, byte pro)
{
  
  switch(sq[step].prNum[pro])
  {
  case Prime2:
  case Prime1:
  case Prime:
    pr_time_sec+=(sq[step].dword[pro]&0x0000ffff)+4;
    break;
  case DispAsp: 
    //  full_total_strip;
    if(((sq[step].dword[pro]>>24)&0x000000ff)==PRIME_DW){
      pr_time_sec+=(((((pm_pram.vol[PRIME_DW-1]*(sq[step].dword[pro]&0x0000ffff))/500)+1000)*full_total_strip)+3000)/1000;//(sq[full_step_cnt].dword[full_pr_cnt]&0x0000ffff);
    }else
      pr_time_sec+=(((diasp.asp_time+1000)*full_total_strip)+3000)/1000;    
    break;
  case DispAsp2:
    diasp.vol=((pm_pram.vol[WS_PUMP1-1]+pm_pram.vol[WS_PUMP2-1])/2);
    diasp.vol=(diasp.vol*(sq[step].dword[pro]&0x0000ffff))/500;
    pr_time_sec+=((diasp.vol+1000)*(full_total_strip/2)+3000)/1000;
    break;
  case DispAsp3:
    break;
  case Disp:
    //disp_sgl.total_strip=full_total_strip;
    pr_time_sec+=((((pm_pram.vol[((sq[step].dword[pro]>>24)&0x000000ff)-1]*(sq[step].dword[pro]&0x0000ffff)/500)+1000)*full_total_strip)+3000)/1000;
    break;
  case Disp2:
    disp_sgl.disp_vol=((pm_pram.vol[WS_PUMP1-1]+pm_pram.vol[WS_PUMP2-1])/2);
    disp_sgl.disp_vol=(disp_sgl.disp_vol*(sq[step].dword[pro]&0x0000ffff))/500;
    pr_time_sec+=((disp_sgl.disp_vol+1000)*(full_total_strip/2))/1000;
    break;
  case Asp:
    asp_re_cunt=((sq[step].dword[pro]>>16)&0x0000000f)-1;
    pr_time_sec+=(((asp.time+300+1000)*(full_total_strip/2))*asp_re_cunt)/1000;
    break;
  case Incubation:
    pr_time_sec+=(sq[step].dword[pro]&0x0000ffff)*60;
    pr_incubation_time();
    break;
  case Incubation2:
    pr_time_sec+=(sq[step].dword[pro]&0x0000ffff)*60;
    break;  
  case Incubation_Ws:
    pr_time_sec+=(sq[step].dword[pro]&0x0000ffff)*60;
    pr_incubation_time();
    break;
  case Incubation_Ws2:
    pr_time_sec+=(sq[step].dword[pro]&0x0000ffff)*60;
    break;      
  case Sample_LLD:
    pr_time_sec+=(full_total_strip*23)+60;
    break;
  case Dry:
    pr_time_sec+=(sq[step].dword[pro]&0x0000ffff)*60;
    break;
  case RollBack:
    break;
  case RollBack1:
    break;
  case RollBack2:
    break;
  case Analy:
    break;
  case Pause:
    break;
  default:break;

  }
  
  
}

void state_led_init()
{
 HAL_GPIO_WritePin(M_Power_GPIO_Port,M_Power_Pin,GPIO_PIN_RESET);
 HAL_GPIO_WritePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin,GPIO_PIN_SET);
 HAL_GPIO_WritePin(M_INCUBATION_GPIO_Port,M_INCUBATION_Pin,GPIO_PIN_SET);
 HAL_GPIO_WritePin(M_DRY_GPIO_Port,M_DRY_Pin,GPIO_PIN_SET);
 HAL_GPIO_WritePin(M_ANALYSIS_GPIO_Port,M_ANALYSIS_Pin,GPIO_PIN_SET);
 //HAL_GPIO_WritePin(M_ANALYSIS_GPIO_Port,M_ANALYSIS_Pin,GPIO_PIN_RESET);
}


/*
int32_t sq_val_ch(byte *data, byte size)
{
int32_t temp=0;
if(size==4){
temp|=data[0]<<24;
temp|=data[1]<<16;
temp|=data[2]<<8;
temp|=data[3];
  }else{
temp|=data[2]<<8;
temp|=data[3];
  }
return temp;  
}

void sq_init()
{
if(sq[0].stNum==0xff || sq[0].stNum==0){
memset(&sq,0,sizeof(sq));
sq[0].stNum=1;
sq[0].prCnt=1;
sq[0].prNum[0]=1;
sq[0].dword[0]=0x03001F40;

sq[1].stNum=0xfe;
sq_pram_write(0);

sq_pram_read(0); 
  
*/

