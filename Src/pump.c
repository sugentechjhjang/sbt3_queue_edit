

#include "main.h"

enum pump_com_state pm_state=PmStanby;
struct pump_param pm_pram;

UART_HandleTypeDef huart5;
byte uart5_chr;
void MX_USART5_UART_Init(void)
{
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 38400;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart5);
  
  HAL_UART_Receive_IT(&huart5, &uart5_chr, 1);
}


void UART_Pump_LLD_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart5)
  {
    int32_t dwQueCheck;

    dwQueCheck = UART5_ReceivePutQue(uart5_chr);
    if(dwQueCheck == UART5_QUE_FULL)
    {
      //Error code;
    }
    //===================================================
    dwQueCheck = HAL_UART_Receive_IT(&huart5, &uart5_chr, 1);
    if(dwQueCheck)
    {
      while(1);
    }
  }
}

HAL_StatusTypeDef UART5_ReInit(void)
{
    HAL_StatusTypeDef status;

    // UART5 비활성화 (DeInit)
    status = HAL_UART_DeInit(&huart5);
    if (status != HAL_OK)
    {
        // UART 비활성화 실패 처리
        return status;
    }

    // UART5 다시 초기화 (Init)
    status = HAL_UART_Init(&huart5);
    if (status != HAL_OK)
    {
        // UART 초기화 실패 처리
        return status;
    }

    // 초기화 성공
    HAL_UART_Receive_IT(&huart5, &uart5_chr, 1);
    return HAL_OK;
}

HAL_StatusTypeDef PUMP_Uart5_Data_Send(unsigned char *data, unsigned char data_length)
{  
  return HAL_UART_Transmit(&huart5, data, data_length, 100);  
}

void pump_mem_init()
{
  pump_param_read();
}

#define PMPBUF_QUEUE_LENGTH 100

#define FIRST_PUMP_STX 0x4B
#define SECON_PUMP_STX 0x4D
byte pm_temp_chr=0;
byte pm_escap=0;
byte pm_currnt_rec_buf_ind=0;
uint16_t pm_ck_cnt=0,pm_check_sum=0,pm_check_sum_b=0;
bool pump_reset_flag=false;
byte pm_receive_bufs[100]={0,};
byte *pm_receive_buf;
byte *pm_pnt;

#define NEW_PUMP_TRANS_RETRAY               10
#define NEW_PUMP_RESPONSE_TIME_OUT            500
#define NEW_PUMP_MAX_DSP_TIME               20000
#define NEW_PUMP_RUN_TIME_DELAY_FACTOR      0.8


#define NEW_PUMP_MAX_DATA              8
#define NEW_PUMP_FRAME_DATA            4  //Pump
#define NEW_PUMP_HEADER_DATA           0x4B4D  //Pump
#define NEW_PUMP_MCODE                 1                

#define New_PUMP_RS_MCODE                 0x02
#define New_PUMP_RS_LENGTH               0x06
#define New_PUMP_STOP_SCODE              0x01
#define New_PUMP_CW_SCODE                0x02
#define New_PUMP_CCW_SCODE               0x04
#define New_PUMP_MOTION_MCODE             0x03
#define New_PUMP_MOTION_LENGTH           0x04
#define New_PUMP_MOTION_SET_SCODE        0x41
#define New_PUMP_MOTION_READ_SCODE       0x01
#define New_PUMP_MOTION_CW_DATA          0x00
#define New_PUMP_MOTION_CCW_DATA         0x01
#define New_PUMP_RUN_TIME_MCODE             0x04
#define New_PUMP_RUN_TIME_LENGTH           0x04
#define New_PUMP_RUN_TIME_SET_SCODE        0x41
#define New_PUMP_RUN_TIME_READ_SCODE       0x01
#define New_PUMP_RESET_MCODE                0x3F
#define New_PUMP_RESET_LENGTH              0x04
#define New_PUMP_RESET_SCODE               0x01
#define New_PUMP_RESET_FIRST_DATA          0xA1B2
#define New_PUMP_RESET_SECOND_DATA         0xC3D4



#define NEW_ASPIRATION_PUMP1      0x01
#define NEW_ASPIRATION_PUMP2      0x02
#define NEW_ASPIRATION_PUMP3      0x04   
#define NEW_PUMP_ALL                   0x3FFE
#define NEW_PUMP_DSPI_ALL              0x3FF0  // 11,1111,1111,0000
#define NEW_PUMP_ASPI_ALL              0x000E  // 00,0000,0000,1110
#define PUMP_ALL_ROLL_BACK_LIMIT_TIME  40000
#define PUMP_ASP_LIMIT_TIME            40000
#define PUMP_DSP_LIMIT_TIME            40000

enum new_pump_frame
{
  new_pumpheader =0,
  new_pumpcode,
  new_pumplength,
  new_pump_check_sum,
  new_pump_data_first,
  new_pump_data_second,
  new_pump_data_third,
  new_pump_data_fourth
};


  unsigned short int new_pump_go[10] ={
    0x0000,0x0000,0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,0x0000
  };
void New_Pump_Run(unsigned short int pump_channel, unsigned long int pump_run_time)
{
  unsigned int pump_run_time_buffer =0;
  memset(new_pump_go,0,sizeof(new_pump_go));
//  pump_active_flag=1;
  new_pump_go[new_pumpcode] = (New_PUMP_RS_MCODE << 8) | (New_PUMP_CW_SCODE);
  new_pump_go[new_pumplength] = New_PUMP_RS_LENGTH;
 
  new_pump_go[new_pump_data_first] = pump_channel;    
  new_pump_go[new_pump_data_second]= (unsigned short int)((pump_run_time >> 16) & 0xFFFF);
  new_pump_go[new_pump_data_third]= (unsigned short int)(pump_run_time & 0xFFFF);
  New_Pump_Data_Trans_s(new_pump_go);
  pump_run_time_buffer = (unsigned int)(pump_run_time * NEW_PUMP_RUN_TIME_DELAY_FACTOR);
  if(pump_run_time_buffer > PUMP_DSP_LIMIT_TIME) pump_run_time_buffer = PUMP_DSP_LIMIT_TIME;
 // P_Delay_ms(pump_run_time_buffer);

}


void New_Pump_RollBack(unsigned short int pump_channel, unsigned long int pump_run_time)
{
  unsigned int pump_run_time_buffer =0;
  memset(new_pump_go,0,sizeof(new_pump_go));
//  pump_active_flag=1;
  new_pump_go[new_pumpcode] = (New_PUMP_RS_MCODE << 8) | (New_PUMP_CCW_SCODE);
  new_pump_go[new_pumplength] = New_PUMP_RS_LENGTH;
 
  new_pump_go[new_pump_data_first] = pump_channel;    
  new_pump_go[new_pump_data_second]= (unsigned short int)((pump_run_time >> 16) & 0xFFFF);
  new_pump_go[new_pump_data_third]= (unsigned short int)(pump_run_time & 0xFFFF);
  New_Pump_Data_Trans_s(new_pump_go);
  pump_run_time_buffer = (unsigned int)(pump_run_time * NEW_PUMP_RUN_TIME_DELAY_FACTOR);
  if(pump_run_time_buffer > PUMP_DSP_LIMIT_TIME) pump_run_time_buffer = PUMP_DSP_LIMIT_TIME;
 // P_Delay_ms(pump_run_time_buffer);
}

void New_Pump_Reset(void)
{
  pump_reset_flag = true;
  memset(new_pump_go,0,sizeof(new_pump_go));
  new_pump_go[new_pumpcode] = (New_PUMP_RESET_MCODE << 8) | (New_PUMP_RESET_SCODE);
  new_pump_go[new_pumplength] = New_PUMP_RESET_LENGTH;
  new_pump_go[new_pump_data_first] = New_PUMP_RESET_FIRST_DATA;    
  new_pump_go[new_pump_data_second]= New_PUMP_RESET_SECOND_DATA;
  New_Pump_Data_Trans_s(new_pump_go);
  while(pm_state==PmBusy);

}
void Pump_485_Direction_Set(GPIO_PinState pin_state)
{
  HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, pin_state);  // DIR RS-485
}


event execute_pump_ctrl(event event)
{
 byte dev_send_buf[4]={0,}; 
  switch(event)
  {

  case hseDWPage:
        state=stStEng;
    pump_param_read();
    usb_send_pack(hseDWPage,0); 
    break;
  case hseP15DispSet:
    pm_pram.vol[PRIME_DW-1]=merge_32bit(pm_pram.vol[PRIME_DW-1],usb_data_buf);
    // probe_disp.total_strip=diasp.total_strip=full_total_strip;
   // New_Pump_Run(0x01<<PRIME_DW,pm_pram.vol[PRIME_DW-1]);
    usb_send_pack(hseP15DispSet,usb_data_buf);
    pump_param_write();
    break;
  case hseP15DispRead:
    pump_param_read();
    sort_8bit( pm_pram.vol[PRIME_DW-1],dev_send_buf);
    usb_send_pack(hseP15DispRead, dev_send_buf);
    break;    
  case hseP15DispSave:
    pm_pram.vol[PRIME_DW-1]=merge_32bit(pm_pram.vol[PRIME_DW-1],usb_data_buf);
    // probe_disp.total_strip=diasp.total_strip=full_total_strip;
   // New_Pump_Run(0x01<<PRIME_DW,pm_pram.vol[PRIME_DW-1]);
    usb_send_pack(hseP15DispSave,usb_data_buf);
    pump_param_write();
    break;


  case hseP15DispMove:
     probe_disp.total_strip=merge_32bit(probe_disp.total_strip,usb_data_buf);
     probe_disp.vol=pm_pram.vol[PRIME_DW-1];
     
     smpl_prime.pm_num=SMP_PRME_PP;
     lld_fc=lldFuncNone;
     
     smple_rack_cnt=1;
     auto_prime_flg=false;
     dw_prime_vol=12000;
     probe_disp_enable=true;
     give_event(eventSmpPrimeInit,0);
    // give_event(eventProbeDispIinit,0);
    //prime.disp_pump_num=0x01<<DI_PUMP;
    //give_event(eventPrimeIinit,0);
    usb_send_pack(hseP15DispMove, 0);
    break;
  case hseP15DispPause:
    usb_send_pack(hseP15DispPause, 0);
    break;
//--------------------
  case hseWSSetPage:
        state=stStEng;
    pump_param_read();
    usb_send_pack(hseWSSetPage,0); 
    break;
  case hseP7DispSet:
    pm_pram.vol[WS_PUMP1-1]=merge_32bit(pm_pram.vol[WS_PUMP1-1],usb_data_buf);
    pm_pram.vol[WS_PUMP2-1]=merge_32bit(pm_pram.vol[WS_PUMP2-1],usb_data_buf);
    //New_Pump_Run(0x01<<WS_PUMP1,pm_pram.vol[WS_PUMP1-1]);
    disp_sgl.disp_vol=((pm_pram.vol[WS_PUMP1-1]+pm_pram.vol[WS_PUMP2-1])/2);
    usb_send_pack(hseP7DispSet,usb_data_buf);
    pump_param_write();
    break;

  case hseP7DispSave:
    pm_pram.vol[WS_PUMP1-1]=merge_32bit(pm_pram.vol[WS_PUMP1-1],usb_data_buf);
    pm_pram.vol[WS_PUMP2-1]=merge_32bit(pm_pram.vol[WS_PUMP2-1],usb_data_buf);
    //New_Pump_Run(0x01<<WS_PUMP1,pm_pram.vol[WS_PUMP1-1]);
    disp_sgl.disp_vol=((pm_pram.vol[WS_PUMP1-1]+pm_pram.vol[WS_PUMP2-1])/2);
    usb_send_pack(hseP7DispSave,usb_data_buf);
    pump_param_write();
    break;
  
  case hseP7DispRead:
    pump_param_read();
    sort_8bit( pm_pram.vol[WS_PUMP1-1],dev_send_buf);
    usb_send_pack(hseP7DispRead, dev_send_buf);
    break;



  case hseP8DispSet:
    //pm_pram.vol[WS_PUMP2-1]=merge_32bit(pm_pram.vol[WS_PUMP2-1],usb_data_buf);
  //  New_Pump_Run(0x01<<WS_PUMP2,pm_pram.vol[WS_PUMP2-1]);
   // usb_send_pack(hseP8DispSet,usb_data_buf);
   // pump_param_write();
     usb_send_pack(hseP8DispSet,usb_data_buf);
    break;

  case hseP8DispSave:
    //pm_pram.vol[WS_PUMP2-1]=merge_32bit(pm_pram.vol[WS_PUMP2-1],usb_data_buf);
  //  New_Pump_Run(0x01<<WS_PUMP2,pm_pram.vol[WS_PUMP2-1]);
   // usb_send_pack(hseP8DispSet,usb_data_buf);
   // pump_param_write();
     usb_send_pack(hseP8DispSave,usb_data_buf);
    break;
  
  case hseP8DispRead:
    pump_param_read();
    sort_8bit( pm_pram.vol[WS_PUMP2-1],dev_send_buf);
    usb_send_pack(hseP8DispRead, dev_send_buf);
    break;



  case hseP7DispMove:
   // disp_sgl.disp_pump_num=prime.disp_pump_num=(0x01<<WS_PUMP1)|(0x01<<WS_PUMP1);
   // diasp.total_strip=disp_sgl.total_strip=merge_32bit(disp_sgl.total_strip,usb_data_buf);
   
    disp_sgl.disp_pump_num=prime.disp_pump_num=(0x01<<WS_PUMP1)|(0x01<<WS_PUMP2);//0x01<<WS_PUMP1;
    disp_sgl.total_strip=merge_32bit(disp_sgl.total_strip,usb_data_buf);
    //give_event(eventAspIinit,0);
     disp_sgl.disp_vol=((pm_pram.vol[WS_PUMP1-1]+pm_pram.vol[WS_PUMP2-1])/2);
    give_event(eventPrimeIinit,0);
    
    
    //give_event(eventPrimeIinit,0);
    //give_event(eventDspAspIinit,0);
    usb_send_pack((enum cntrl_event)event, 0);
    break;
//  case hseP7DispPause:
//    usb_send_pack(hseP7DispPause, 0);
//    break;
  case hseP8DispMove:
   // disp_sgl.disp_pump_num=prime.disp_pump_num=0x01<<WS_PUMP2;
   // disp_sgl.total_strip=merge_32bit(disp_sgl.total_strip,usb_data_buf);
  //  give_event(eventPrimeIinit,0);
    usb_send_pack((enum cntrl_event)event, 0);
    break;
//  case hseP8DispPause:
//    usb_send_pack(hseP8DispPause, 0);
//    break;
//------------------------
  case hseReagSetPage:
        state=stStEng;
    pump_param_read();
    usb_send_pack(hseReagSetPage,0); 
    break;
  case hseP1DispSet:
    pm_pram.vol[RD_PUMP1-1]=merge_32bit(pm_pram.vol[RD_PUMP1-1],usb_data_buf);
    New_Pump_Run(0x01<<RD_PUMP1,pm_pram.vol[RD_PUMP1-1]);
    usb_send_pack(hseP1DispSet,usb_data_buf);
    pump_param_write();
    break; 
  case hseP1DispSave:
    pm_pram.vol[RD_PUMP1-1]=merge_32bit(pm_pram.vol[RD_PUMP1-1],usb_data_buf);
    usb_send_pack(hseP1DispSave,usb_data_buf);
    pump_param_write();
    break; 

  case hseP1DispRead:
    pump_param_read();
    sort_8bit( pm_pram.vol[RD_PUMP1-1],dev_send_buf);
    usb_send_pack(hseP1DispRead, dev_send_buf);
    break;
  case hseP2DispSet:
    pm_pram.vol[RD_PUMP2-1]=merge_32bit(pm_pram.vol[RD_PUMP2-1],usb_data_buf);
    New_Pump_Run(0x01<<RD_PUMP2,pm_pram.vol[RD_PUMP2-1]);
    usb_send_pack(hseP2DispSet,usb_data_buf);
    pump_param_write();
    break;
  case hseP2DispSave:
    pm_pram.vol[RD_PUMP2-1]=merge_32bit(pm_pram.vol[RD_PUMP2-1],usb_data_buf);
    usb_send_pack(hseP2DispSave,usb_data_buf);
    pump_param_write();
    break;    
  case hseP2DispRead:
    pump_param_read();
    sort_8bit( pm_pram.vol[RD_PUMP2-1],dev_send_buf);
    usb_send_pack(hseP2DispRead, dev_send_buf);
    break;
  case hseP3DispSet:
    pm_pram.vol[RD_PUMP3-1]=merge_32bit(pm_pram.vol[RD_PUMP3-1],usb_data_buf);
    New_Pump_Run(0x01<<RD_PUMP3,pm_pram.vol[RD_PUMP3-1]);
    usb_send_pack(hseP3DispSet,usb_data_buf);
    pump_param_write();
    break;
  case hseP3DispSave:
    pm_pram.vol[RD_PUMP3-1]=merge_32bit(pm_pram.vol[RD_PUMP3-1],usb_data_buf);
    usb_send_pack(hseP3DispSave,usb_data_buf);
    pump_param_write();
    break;
  case hseP3DispRead:
    pump_param_read();
    sort_8bit( pm_pram.vol[RD_PUMP3-1],dev_send_buf);
    usb_send_pack(hseP3DispRead, dev_send_buf);
    break;
  case hseP4DispSet:
    pm_pram.vol[RD_PUMP4-1]=merge_32bit(pm_pram.vol[RD_PUMP4-1],usb_data_buf);
    New_Pump_Run(0x01<<RD_PUMP4,pm_pram.vol[RD_PUMP4-1]);
    usb_send_pack(hseP4DispSet,usb_data_buf);
    pump_param_write();
    break;
  case hseP4DispSave:
    pm_pram.vol[RD_PUMP4-1]=merge_32bit(pm_pram.vol[RD_PUMP4-1],usb_data_buf);
    usb_send_pack(hseP4DispSave,usb_data_buf);
    pump_param_write();
    break;
  case hseP4DispRead:
    pump_param_read();
    sort_8bit( pm_pram.vol[RD_PUMP4-1],dev_send_buf);
    usb_send_pack(hseP4DispRead, dev_send_buf);
    break;
  case hseP5DispSet:
    pm_pram.vol[RD_PUMP5-1]=merge_32bit(pm_pram.vol[RD_PUMP5-1],usb_data_buf);
    New_Pump_Run(0x01<<RD_PUMP5,pm_pram.vol[RD_PUMP5-1]);
    usb_send_pack(hseP5DispSet,usb_data_buf);
    pump_param_write();
    break;
   case hseP5DispSave:
    pm_pram.vol[RD_PUMP5-1]=merge_32bit(pm_pram.vol[RD_PUMP5-1],usb_data_buf);
    usb_send_pack(hseP5DispSave,usb_data_buf);
    pump_param_write();
    break;   
  case hseP5DispRead:
    pump_param_read();
    sort_8bit( pm_pram.vol[RD_PUMP5-1],dev_send_buf);
    usb_send_pack(hseP5DispRead, dev_send_buf);
    break;
  case hseP6DispSet:
    pm_pram.vol[RD_PUMP6-1]=merge_32bit(pm_pram.vol[RD_PUMP6-1],usb_data_buf);
    New_Pump_Run(0x01<<RD_PUMP6,pm_pram.vol[RD_PUMP6-1]);
    usb_send_pack(hseP6DispSet,usb_data_buf);
    pump_param_write();
    break;
  case hseP6DispSave:
    pm_pram.vol[RD_PUMP6-1]=merge_32bit(pm_pram.vol[RD_PUMP6-1],usb_data_buf);
    usb_send_pack(hseP6DispSave,usb_data_buf);
    pump_param_write();
    break;

  case hseP6DispRead:
    pump_param_read();
    sort_8bit( pm_pram.vol[RD_PUMP6-1],dev_send_buf);
    usb_send_pack(hseP6DispRead, dev_send_buf);
    break;

  case hseP1DispMove:
    disp_sgl.disp_pump_num=prime.disp_pump_num=0x01<<RD_PUMP1;
    disp_sgl.total_strip=merge_32bit(disp_sgl.total_strip,usb_data_buf);
   //give_event(eventAspIinit,0);
    disp_sgl.disp_vol=pm_pram.vol[RD_PUMP1-1];
     give_event(eventPrimeIinit,0);
   // give_event(eventDspIinit,0);
    usb_send_pack((enum cntrl_event)event, 0);
    break;
  case hseP2DispMove:
    disp_sgl.disp_pump_num=prime.disp_pump_num=0x01<<RD_PUMP2;
    disp_sgl.total_strip=merge_32bit(disp_sgl.total_strip,usb_data_buf);
      disp_sgl.disp_vol=pm_pram.vol[RD_PUMP2-1];
    give_event(eventPrimeIinit,0);
    //give_event(eventDspIinit,0);
    usb_send_pack((enum cntrl_event)event, 0);
    break;
  case hseP3DispMove:
    disp_sgl.disp_pump_num=prime.disp_pump_num=0x01<<RD_PUMP3;
    disp_sgl.total_strip=merge_32bit(disp_sgl.total_strip,usb_data_buf);
       disp_sgl.disp_vol=pm_pram.vol[RD_PUMP3-1];
    give_event(eventPrimeIinit,0);
    //give_event(eventDspIinit,0);
    usb_send_pack((enum cntrl_event)event, 0);
    break;
  case hseP4DispMove:
    disp_sgl.disp_pump_num=prime.disp_pump_num=0x01<<RD_PUMP4;
    disp_sgl.total_strip=merge_32bit(disp_sgl.total_strip,usb_data_buf);
     disp_sgl.disp_vol=pm_pram.vol[RD_PUMP4-1];
    give_event(eventPrimeIinit,0);
    //give_event(eventDspIinit,0);
    usb_send_pack((enum cntrl_event)event, 0);
    break;
  case hseP5DispMove:
    disp_sgl.disp_pump_num=prime.disp_pump_num=0x01<<RD_PUMP5;
    disp_sgl.total_strip=merge_32bit(disp_sgl.total_strip,usb_data_buf);
    disp_sgl.disp_vol=pm_pram.vol[RD_PUMP5-1];
    give_event(eventPrimeIinit,0);
    //give_event(eventDspIinit,0);
    usb_send_pack((enum cntrl_event)event, 0);
    break;
  case hseP6DispMove:
    disp_sgl.disp_pump_num=prime.disp_pump_num=0x01<<RD_PUMP6;
    disp_sgl.total_strip=merge_32bit(disp_sgl.total_strip,usb_data_buf);
    disp_sgl.disp_vol=pm_pram.vol[RD_PUMP6-1];
    give_event(eventPrimeIinit,0);
    //give_event(eventDspIinit,0);
    usb_send_pack((enum cntrl_event)event, 0);
    break;
    
  case hseP1DispPause:
  case hseP2DispPause:
  case hseP3DispPause:
  case hseP4DispPause:
  case hseP5DispPause:
  case hseP6DispPause:
    usb_send_pack((enum cntrl_event)event, 0);
    break;
    
    
    
    
    
    
    
  default:
    break;
  }
  return event;
}

//------------------------------------------------------------------------------
// 2024 03 11 추가
enum
{
  hsHD_PUMP_COM_STX_1 = 0,
  hsHD_PUMP_COM_STX_2,
  hsHD_PUMP_COM_MCODE,
  hsHD_PUMP_COM_SCODE,
  hsHD_PUMP_COM_LEN_1,
  hsHD_PUMP_COM_LEN_2,  
  hsHD_PUMP_COM_CKSUM_1,
  hsHD_PUMP_COM_CKSUM_2,
  
  hsHD_PUMP_COM_MAX
};

typedef struct
{
  uint8_t uM_Code;
  uint8_t uS_Code;
  uint16_t wLen;
  uint16_t wCheckSum;
  uint16_t wDataBuf[5];
}hsPump_COM_Convert_t;

#define hsPUMP_COM_BUF_SZ                       50
#define hsPUMP_COM_RETRY_CNT_MAX                1

#define hsPUMP_COM_RECEIV_WAIT_TM               5
#define hsPUMP_COM_RECEIV_WAIT_CNT_MAX          700

#define hsPUMP_COM_OK                           0
#define hsPUMP_COM_FAIL                         5
#define hsPUMP_RECEIV_OK                        (hsPUMP_COM_OK)
#define hsPUMP_RECEIV_FAIL                      (hsPUMP_COM_FAIL + 1)


hsPump_COM_Convert_t g_tPumpCovertData;

//int32_t hsPump_COM_PacketGeneration(uint16_t *p_wOriginDatBuf, uint8_t *p_uSendPacket);
//int32_t hsPump_COM_HeaderReceivPacketHandle(uint8_t *p_uReceivPacketBuf, hsPump_COM_Convert_t *p_tConvertData);
//int32_t hsPump_COM_TailReceivPacketHandle(uint8_t *p_uReceivPacketBuf, hsPump_COM_Convert_t *p_tConvertData);
int32_t hsPump_Communication_Handle(uint16_t *p_wOriginDatBuf);

void New_Pump_Data_Trans_s(unsigned short int *pump_go)
{
  int32_t dwCheck = 0;

  pm_state=PmBusy;
  
  dwCheck = hsPump_Communication_Handle(pump_go);
  if(dwCheck == hsPUMP_COM_OK)
  {
    pm_state = PmStanby;
  }
  else
  {
    while(1);
  }
}

/*
* Pump 통신의 경우, 재전송으로 인해 펌프가 중복 동작할 수 있는 위험이 있음 
* --> 재전송으로 인한 중복 동작 위험으로 1번 만 명령을 내려야 함.
* Pump 보드가 전송 데이터를 받았다고 추정 할 수 있으면 통신 성공으로 처리 
* 추정 방법은 응답 데이터 중 연속적으로 "KM"이 있으면 통신 성공으로 추정 함
*/

int32_t hsPump_COM_PacketGeneration(uint16_t *p_wOriginDatBuf, uint8_t *p_uSendPacket);
int32_t hsPump_COM_ReceivPacketHandle(uint8_t *p_uReceivPacketBuf, hsPump_COM_Convert_t *p_tConvertData);

int32_t hsPump_Communication_Handle(uint16_t *p_wOriginDatBuf)
{
  int32_t dwCheck = 0, dwHAL_Err_Cnt = 0, dwReTryCnt = 0;
  uint32_t dwSendDataSZ=0,dwCnt=0,dwQueHaveDataSize=0;
  uint8_t uSendPacket[hsPUMP_COM_BUF_SZ] = {0};
  uint8_t uReceivePacket[hsPUMP_COM_BUF_SZ] = {0};
  uint8_t i=0;
  hsPump_COM_Convert_t tConvertData;
  
  //Packet Generation ---------------------------------------------------------
  hsPump_COM_PacketGeneration(p_wOriginDatBuf, uSendPacket);
  dwSendDataSZ = p_wOriginDatBuf[new_pumplength] + hsHD_PUMP_COM_MAX;
  
  err_tmout_cnt_set(errPump,20);
  //Communication --------------------------------------------------------------
  HAL_Delay(10);
  while(1)
  {
    //Packet Send
    UART5_QueReset();
    HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, (GPIO_PinState)SET);
    dwCheck = HAL_UART_Transmit(&huart5, uSendPacket, dwSendDataSZ, 100);
    HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, (GPIO_PinState)RESET);
    if(dwCheck)
    {
      dwHAL_Err_Cnt++;
      UART5_ReInit();
      if(dwHAL_Err_Cnt > 5) 
      {
        while(1);
      }
    }
    //Header Packet 수신 
    for(dwCnt = 0; dwCnt < hsPUMP_COM_RECEIV_WAIT_CNT_MAX ; dwCnt++)
    {
      HAL_Delay(hsPUMP_COM_RECEIV_WAIT_TM);
      
      dwQueHaveDataSize = UART5_QueHaveDataSize();

      for(i=0; i<UART5_QUE_SZ; i++)
      {
        if(g_tUART5_Que_Handle.uQueBuf[i] == 0x4B) break; //헤더데이터의 첫번째 배열 추적
      }
      if(dwQueHaveDataSize >= i+hsHD_PUMP_COM_MAX)  break;
    }

    //Packet Receive -----------------------------------------------------------
    dwCheck = hsPump_COM_ReceivPacketHandle(uReceivePacket, &tConvertData);
    if(dwCheck == hsPUMP_RECEIV_OK)
    { 
      break;
    }

    dwReTryCnt++;
    HAL_Delay(100);
    
    if(dwReTryCnt >= 5)
    {
      while(1);
    } 
  }
  
  err_tmout_en(FALSE);
  dwCheck = hsPUMP_COM_OK;

  return dwCheck;
}

int32_t hsPump_COM_PacketGeneration(uint16_t *p_wOriginDatBuf, uint8_t *p_uSendPacket)
{
  int32_t dwCheck = 0;
  uint32_t dwCnt, dwQriginDataSZ, dwDataLen, dwHeaderCKSumSZ, dwDataStartIndex;
  int16_t wCheckSum = 0;
  uint8_t *p_uPacket;
  
  dwDataLen = p_wOriginDatBuf[new_pumplength];
  dwQriginDataSZ = dwDataLen / 2;
  dwHeaderCKSumSZ = hsHD_PUMP_COM_CKSUM_1 - hsHD_PUMP_COM_MCODE; //hsHD_PUMP_COM_CKSUM_1(6) - hsHD_PUMP_COM_MCODE(2) = 4(M_CODE(1), S_CODE(1), Len(2))
  dwDataStartIndex = hsHD_PUMP_COM_MAX;
  
  //SendPacket 생성 ------------------------------------------------------------
  p_uSendPacket[hsHD_PUMP_COM_STX_1] = NEW_PUMP_HEADER_DATA >> 8;
  p_uSendPacket[hsHD_PUMP_COM_STX_2] = (uint8_t)NEW_PUMP_HEADER_DATA;  
  p_uSendPacket[hsHD_PUMP_COM_MCODE] = p_wOriginDatBuf[new_pumpcode] >> 8;
  p_uSendPacket[hsHD_PUMP_COM_SCODE] = p_wOriginDatBuf[new_pumpcode];
  p_uSendPacket[hsHD_PUMP_COM_LEN_1] = dwDataLen >> 8;
  p_uSendPacket[hsHD_PUMP_COM_LEN_2] = dwDataLen;
  
  p_uPacket = &p_uSendPacket[hsHD_PUMP_COM_MCODE];
  for(dwCnt = 0; dwCnt < dwHeaderCKSumSZ ; dwCnt++)
  {
    wCheckSum += p_uPacket[dwCnt];
  }
  
  p_uPacket = &p_uSendPacket[dwDataStartIndex]; //dwDataStartIndex = hsHD_PUMP_COM_MAX(8)
  for(dwCnt = 0; dwCnt < dwQriginDataSZ ; dwCnt++)
  {
    p_uPacket[dwCnt*2] = ( ((&p_wOriginDatBuf[new_pump_data_first]))[dwCnt]) >> 8;
    p_uPacket[(dwCnt*2) + 1] = ( ((&p_wOriginDatBuf[new_pump_data_first]))[dwCnt]);
  }
  
  for(dwCnt = 0; dwCnt < dwDataLen ; dwCnt++)
  {
    wCheckSum += p_uPacket[dwCnt];
  }
  
  wCheckSum = ((int16_t )wCheckSum) * (-1);
  
  p_uSendPacket[hsHD_PUMP_COM_CKSUM_1] = wCheckSum >> 8;
  p_uSendPacket[hsHD_PUMP_COM_CKSUM_2] = wCheckSum;
  
  return dwCheck;
}

int32_t hsPump_COM_ReceivPacketHandle(uint8_t *p_uReceivPacketBuf, hsPump_COM_Convert_t *p_tConvertData)
{
  int32_t dwCheck = 0;
  uint32_t dwCnt, dwReceivDataSZ,dwDataLen; 
  int16_t CheckCnt,rCheckSum=0,rCheckSumCompare=0;
  //Tail Packet 수신 대기
  HAL_Delay(5);
  
  for(dwReceivDataSZ = 0; ; dwReceivDataSZ++)
  {
    dwCheck = UART5_ReceiveGetQue(&p_uReceivPacketBuf[dwReceivDataSZ]);
    if(dwCheck == UART5_QUE_EMPTY) break;
  }
  
  //연속하는 'K', 'M' 찾기 
  for(dwCnt = 0; dwCnt < dwReceivDataSZ; dwCnt++)
  {
    if( (p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_STX_1] == ((NEW_PUMP_HEADER_DATA >> 8) & 0x00FF)) && (p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_STX_2] == ((NEW_PUMP_HEADER_DATA) & 0x00FF)) )
    {
      if(pump_reset_flag == true)
      {
        pump_reset_flag = false;
        return hsPUMP_RECEIV_OK;
      }
      
      dwDataLen = p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_LEN_1] << 8 | p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_LEN_2]; 
      rCheckSumCompare = p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_CKSUM_1] << 8 | p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_CKSUM_2];
 
      rCheckSum += p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_MCODE];
      rCheckSum += p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_SCODE];
      rCheckSum += p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_LEN_1];
      rCheckSum += p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_LEN_2];
      
      for(CheckCnt = 0; CheckCnt < dwDataLen; CheckCnt++)
      {
        rCheckSum+= p_uReceivPacketBuf[dwCnt+hsHD_PUMP_COM_LEN_2 + 3 + CheckCnt];
      }  
      
      rCheckSum = ((int16_t)rCheckSum) * (-1) ;
            
      if(rCheckSumCompare == rCheckSum )
      {
         return hsPUMP_RECEIV_OK;
      }
    }
  }
  
  return hsPUMP_RECEIV_FAIL;
}
