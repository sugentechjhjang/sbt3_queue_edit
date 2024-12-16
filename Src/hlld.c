

#include "main.h"

#define PLLD_TOTAL_INDEX 100

enum lld_com_state lld_state=lldStanby;
enum lld_func lld_fc=lldFuncNone;
struct smple_pram smp_pram;
unsigned char hlld_rx_index =0;
unsigned char hlld_rx_data_buffer[100] = {0,};
uint8_t current_uSendToPC_Buf[4];
bool CLLD_VOL_Check = false;


void  hlld_mem_init()
{
  hlld_param_read();
}

void UART5_Hlld_RxCpltCallback(byte chr) 
{
  hlld_rx_index++;
  hlld_rx_data_buffer[hlld_rx_index]=chr;
  //  HAL_UART_Receive_IT(&huart5, &pump_rx_data_buffer[pump_rx_index], 1);
}
void hlld_485_Direction_Set(GPIO_PinState pin_state)
{
  HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, pin_state);  // DIR RS-485
}
HAL_StatusTypeDef Hlld_Uart5_Data_Send(unsigned char *data, unsigned char data_length)
{ 
  return HAL_UART_Transmit(&huart5, data, data_length, 100);
  
}
unsigned char hex2ascii(unsigned char code)
{
  unsigned char result = 0;
  
  result = code + 0x30;
  if(result > 0x39)
    result += 7;
  return result;
}

#define HLLD_STX 0x3A
#define FIRST_HLLD_ETX 0x0D
#define END_HLLD_ETX   0x0A
#define HLLD_NO_RECIVED 0xFF

byte lld_escap=0;
byte lld_currnt_rec_buf_ind=0;
byte lld_check_sum=0,lld_check_sum_b=0;
byte lld_receive_bufs[100]={0,};
byte *lld_receive_buf;
byte *lld_pnt;
byte lld_recived_rec_buf_ind=0xff;


uint16_t lld_event_cmd=0;
uint16_t cl1=0;
uint16_t cl2=0;
int32_t init_clld_vol=0;

byte  rb=255,ra=100;
uint16_t clld_indx=0;
uint16_t hlld_vol=0;
byte sws_arry[10]={0,};
uint16_t sws_sum=0;

byte plld_min=0,plld_max=0, plld_slope=0;
bool plld_qc=false;
uint16_t plld_index=0;


//2024 03 05 -------------------------------------------------------------------

enum
{
  COM_hLLD_STX = 0,
  COM_hLLD_ADD1,
  COM_hLLD_ADD2,
  COM_hLLD_CMD1,
  COM_hLLD_CMD2,
  COM_hLLD_CNTNT1,
  COM_hLLD_CNTNT2,
  COM_hLLD_CNTNT3,
  COM_hLLD_CNTNT4,
  COM_hLLD_DAT1,
  COM_hLLD_DAT2,
  COM_hLLD_DAT3,
  COM_hLLD_DAT4,
  COM_hLLD_CKSM1,
  COM_hLLD_CKSM2,
  COM_hLLD_ETX1,
  COM_hLLD_ETX2,

  COM_hLLD_MAX
};

typedef struct
{
  uint8_t uAddress;
  uint8_t uCmd;
  uint16_t wContent;
  uint16_t wData;
  int16_t ReceiveLRC; 
}hsCOM_hLLD_Cmd_t;

#define COM_hLLD_OK                 0
#define COM_hLLD_FAIL               5

#define COM_hLLD_RECV_OK            (COM_hLLD_OK)
#define COM_hLLD_RECV_FAIL          (COM_hLLD_FAIL + 1)

#define COM_hLLD_RE_TRY_MAX         5
#define COM_hLLD_RX_MIN_WAIT_DELAY  5
#define COM_hLLD_RX_MAX_WAIT_CNTMAX 1000


hsCOM_hLLD_Cmd_t g_tSubRun_hLLD_Data;
const hsCOM_hLLD_Cmd_t g_tResetDumySubRun_hLLD_Dat = {0};

int32_t hsGet_hLLD_ReceivPacketHandle(uint8_t uAddress, uint8_t *p_uGetPacketBuf, hsCOM_hLLD_Cmd_t *p_tReturnConvertData);
int32_t hsGet_hLLD_ReceivDataExec(hsCOM_hLLD_Cmd_t *p_tRunData);
int32_t hsCommunication_hLLD_Handle(uint8_t uAddress, uint8_t uCmd, uint16_t wContent, uint16_t wData);


void hlld_send_pack(unsigned char addr, unsigned char cmd, unsigned short int content, unsigned short int data)
{
  int32_t dwCheck;
  dwCheck = hsCommunication_hLLD_Handle(addr, cmd, content, data);
}



int32_t hsCommunication_hLLD_Handle(uint8_t uAddress, uint8_t uCmd, uint16_t wContent, uint16_t wData)
{
  int16_t uLRC=0;
  uint8_t hLLDSendPacket[COM_hLLD_MAX]={0};
  uint8_t hLLDReceivPacket[UART5_QUE_SZ]={0};
  hsCOM_hLLD_Cmd_t tConvertCOM_Packet;
  int32_t dwCheck = COM_hLLD_OK, dwQueHaveDataSize;
  uint32_t dwCnt, dwReTrayCnt = 0, HAL_Fail_Cnt = 0;
  int32_t i = 0;
  
  err_tmout_cnt_set(errHlld,20);

  //Packet Generator
  uLRC = (uAddress + uCmd + ((wContent>>8)&0x00FF) + (wContent&0x00FF) + ((wData>>8)&0x00FF) + (wData & 0x00FF)) * (-1);

  hLLDSendPacket[COM_hLLD_STX] = HLLD_STX;
  hLLDSendPacket[COM_hLLD_ADD1] = hex2ascii((uAddress >> 4) & 0x0F);
  hLLDSendPacket[COM_hLLD_ADD2] = hex2ascii(uAddress & 0x0F);
  hLLDSendPacket[COM_hLLD_CMD1] = hex2ascii((uCmd >> 4) & 0x0F);
  hLLDSendPacket[COM_hLLD_CMD2] = hex2ascii(uCmd & 0x0F);
  hLLDSendPacket[COM_hLLD_CNTNT1] = hex2ascii((wContent >> 12) & 0x0F);
  hLLDSendPacket[COM_hLLD_CNTNT2] = hex2ascii((wContent >> 8) & 0x0F);
  hLLDSendPacket[COM_hLLD_CNTNT3] = hex2ascii((wContent >> 4) & 0x0F);
  hLLDSendPacket[COM_hLLD_CNTNT4] = hex2ascii(wContent & 0x0F);
  hLLDSendPacket[COM_hLLD_DAT1] = hex2ascii((wData >> 12) & 0x0F);
  hLLDSendPacket[COM_hLLD_DAT2] = hex2ascii((wData >> 8) & 0x0F);
  hLLDSendPacket[COM_hLLD_DAT3] = hex2ascii((wData >> 4) & 0x0F);
  hLLDSendPacket[COM_hLLD_DAT4] = hex2ascii(wData & 0x0F);
  hLLDSendPacket[COM_hLLD_CKSM1] = hex2ascii((uLRC >> 4) & 0x0F);
  hLLDSendPacket[COM_hLLD_CKSM2] = hex2ascii(uLRC & 0x0F);
  hLLDSendPacket[COM_hLLD_ETX1] = FIRST_HLLD_ETX;
  hLLDSendPacket[COM_hLLD_ETX2] = END_HLLD_ETX;

 
  //Transter Loop
  while(1)
  {
    HAL_Delay(50);
    UART5_QueReset();
    HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, (GPIO_PinState)SET);
    dwCheck = HAL_UART_Transmit(&huart5, hLLDSendPacket, COM_hLLD_MAX, 100);
    HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, (GPIO_PinState)RESET);
    if(dwCheck)
    {
      //Error Code
      HAL_Fail_Cnt++;
      UART5_ReInit();
      if(HAL_Fail_Cnt > COM_hLLD_RE_TRY_MAX)
      {
        while(1); 
      }
    }

    for(dwCnt = 0; dwCnt < COM_hLLD_RX_MAX_WAIT_CNTMAX ; dwCnt++) 
    {
      HAL_Delay(5);
      dwQueHaveDataSize = UART5_QueHaveDataSize();
      for(i=0; i<UART5_QUE_SZ; i++)
      {
        if(g_tUART5_Que_Handle.uQueBuf[i] == HLLD_STX) break;
      }
      if(dwQueHaveDataSize >= i+COM_hLLD_MAX)  break;
    }

    dwCheck = hsGet_hLLD_ReceivPacketHandle(uAddress, hLLDReceivPacket, &tConvertCOM_Packet);
    if(dwCheck == COM_hLLD_RECV_OK) break;

    dwReTrayCnt++;
    HAL_Delay(100);
    
    if(dwReTrayCnt > COM_hLLD_RE_TRY_MAX) 
    {
      //Error Code
      while(1);
    }

  }
  
  dwCheck = hsGet_hLLD_ReceivDataExec(&tConvertCOM_Packet);
  
  if(dwCheck == COM_hLLD_OK)
  {
    g_tSubRun_hLLD_Data = tConvertCOM_Packet;
  }
  err_tmout_en(FALSE);
  
  return dwCheck;
}

int32_t hsGet_hLLD_ReceivPacketHandle(uint8_t uAddress, uint8_t *p_uGetPacketBuf, hsCOM_hLLD_Cmd_t *p_tReturnConvertData)
{
  uint32_t dwCnt;
  int32_t dwQueCheck=0;
  int16_t ReceiveLRC=0;
  uint8_t uDumy, uConvertBuf[UART5_QUE_SZ];

  for(dwCnt = 0; ; dwCnt++)
  {
    if(dwCnt < UART5_QUE_SZ)
    { 
      dwQueCheck = UART5_ReceiveGetQue(&p_uGetPacketBuf[dwCnt]);
    }  
    else 
    {
      dwQueCheck = UART5_ReceiveGetQue(&uDumy);
    }
    if(dwQueCheck == UART5_QUE_EMPTY) break;
  }
  for(int Detect_STX = 0; Detect_STX<UART5_QUE_SZ; Detect_STX++)
  {
    if((p_uGetPacketBuf[Detect_STX] == HLLD_STX)&&(p_uGetPacketBuf[Detect_STX+COM_hLLD_ETX1] == FIRST_HLLD_ETX)&&(p_uGetPacketBuf[Detect_STX+COM_hLLD_ETX2] == END_HLLD_ETX))
    {
      for(dwCnt = 0; dwCnt< UART5_QUE_SZ; dwCnt++)
      {
        uConvertBuf[dwCnt] = ascii2hex(p_uGetPacketBuf[dwCnt]);
      }

      p_tReturnConvertData->uAddress = (uConvertBuf[Detect_STX + COM_hLLD_ADD1] << 4) | uConvertBuf[Detect_STX + COM_hLLD_ADD2];

      p_tReturnConvertData->uCmd = (uConvertBuf[Detect_STX + COM_hLLD_CMD1] << 4) | uConvertBuf[Detect_STX + COM_hLLD_CMD2];

      p_tReturnConvertData->wContent = (uConvertBuf[Detect_STX + COM_hLLD_CNTNT1] << 12) | (uConvertBuf[Detect_STX + COM_hLLD_CNTNT2] << 8) 
                                        |(uConvertBuf[Detect_STX + COM_hLLD_CNTNT3] << 4) | uConvertBuf[Detect_STX + COM_hLLD_CNTNT4];  

      p_tReturnConvertData->wData = (uConvertBuf[Detect_STX + COM_hLLD_DAT1] << 12) | (uConvertBuf[Detect_STX + COM_hLLD_DAT2] << 8) 
                                        | (uConvertBuf[Detect_STX + COM_hLLD_DAT3] << 4) | uConvertBuf[Detect_STX + COM_hLLD_DAT4];

      p_tReturnConvertData->ReceiveLRC = (uConvertBuf[Detect_STX + COM_hLLD_CKSM1] << 4)|(uConvertBuf[Detect_STX + COM_hLLD_CKSM2]);

      ReceiveLRC = (int16_t)(p_tReturnConvertData->uAddress + p_tReturnConvertData->uCmd + ((p_tReturnConvertData->wContent>>8)&0xFF) + (p_tReturnConvertData->wContent&0xFF)
        + ((p_tReturnConvertData->wData>>8)&0xFF) + (p_tReturnConvertData->wData & 0xFF)) * (-1);
      
      ReceiveLRC = ((ReceiveLRC) &0x00FF);         
                                    
      if(uAddress == p_tReturnConvertData->uAddress && ReceiveLRC == p_tReturnConvertData->ReceiveLRC )
      {
        return COM_hLLD_RECV_OK;
      } 
    }
  }  
  return COM_hLLD_RECV_FAIL;
}


int32_t hsGet_hLLD_ReceivDataExec(hsCOM_hLLD_Cmd_t *p_tRunData)
{
  int32_t dwCheck = 0;
  uint8_t uSendToPC_Buf[4];


  switch(p_tRunData->uCmd)
  {
  case 0:
    break;
  
  case HLLD_CLLD_RES:
    break;
    
  case HLLD_CLLD_INIT:
    break;
    
  case HLLD_CLLD_VOL:
    if(state == stBoot)
    {
      init_clld_vol = ((p_tRunData->wContent) << 16)|(p_tRunData->wData);
      if(init_clld_vol < 32000000)
      {
        set_timer_(eventSpuStnby,5000,0);
      } 
      else
      {
        error(errClld,0);
        while(1);
      } 
    }
    else
    {
      uSendToPC_Buf[0] = p_tRunData->wContent >> 8;
      uSendToPC_Buf[1] = p_tRunData->wContent ;
      uSendToPC_Buf[2] = p_tRunData->wData >> 8;
      uSendToPC_Buf[3] = p_tRunData->wData;
      usb_send_pack(hseClldVolH, uSendToPC_Buf);
      
      CLLD_VOL_Check = true;
    }
    break;
    
  case HLLD_PLLD_RESULT:
    p_min = p_tRunData->wContent;
    p_max = p_tRunData->wData;
    if(smp_prime_press_chek_flg)
    {
      smp_prime_press_chek_flg = false;
      set_timer_(eventSmpPrimeProbePressResult,100,0);
    }
    else set_timer_(eventSmpPlldJude,100,0);
    break;
    
  case HLLD_PLLD_STREAM:
    if(p_tRunData->wData == 1) set_timer_(eventSmpPlldJude,100,0);
    break;
    
  case HLLD_PLLD_GET_SENS:
    give_event(eventSmpQcStreamGo,0);
    break;
    
  case HLLD_PLLD_SLOP_RESULT:
    slope_vol = p_tRunData->wData;  
    set_timer_(hsePlldJudgSlope,100,0);    
    break;
    
  case HLLD_FW_DATE:
    uSendToPC_Buf[0] = p_tRunData->wContent >> 8;
    uSendToPC_Buf[1] = p_tRunData->wContent;
    uSendToPC_Buf[2] = p_tRunData->wData >> 8;
    uSendToPC_Buf[3] = p_tRunData->wData;
    usb_send_pack(HLLD_FW_DATE, uSendToPC_Buf);
    break;  
    
  default:
    break;
  }
  
  return dwCheck;
}

//Exec hLLD
event hsExec_hLLD_CTRL(event event);

event execute_hlld_ctrl(event event)
{
  return hsExec_hLLD_CTRL(event);
}

//Recoding 
event hsExec_hLLD_CTRL(event event)
{
  uint8_t uSendToPC_Buf[4];
 
  switch(event)
  {
  case hseClldPage:
    state=stStEng;
    lld_fc=lldFuncClld;
    smpl_prime.pm_num=SMP_PRME_PP;
    set_timer_(eventSmpPrimeInit,10,0);
    usb_send_pack(hseClldPage, 0);    
    break;
    
  case hseClldSensControl:
    uSendToPC_Buf[0]= g_tSubRun_hLLD_Data.wData >> 8;
    uSendToPC_Buf[1]= g_tSubRun_hLLD_Data.wData;
    uSendToPC_Buf[2]= clld_indx>>8;
    uSendToPC_Buf[3]= clld_indx;
        
    hlld_send_pack(HLLD_ADD, HLLD_CLLD_VOL,0, 0);
    usb_send_pack(hseClldSensControlResp, uSendToPC_Buf);
    break;
    
  case hseClldOutputVol:
    uSendToPC_Buf[0]= g_tSubRun_hLLD_Data.wData >> 8;
    uSendToPC_Buf[1]= g_tSubRun_hLLD_Data.wData;

    uSendToPC_Buf[2]= 0;
    uSendToPC_Buf[3]= 1;
    
    if(!ra--)
    {
      rb--;
      ra=100;
    }     
    usb_send_pack(hseClldOutputVolResp, uSendToPC_Buf);
    break;
    
  case hseClldOffSet:
    smp_pram.off_set=usb_data_buf[0];
    hlld_param_write();
    usb_send_pack(hseClldOffSetResp, usb_data_buf);
    break;
    
  case hseClldOffRead:
    hlld_param_read();
    uSendToPC_Buf[0]=smp_pram.off_set;
    usb_send_pack(hseClldOffReadResp, uSendToPC_Buf);
    break;
    
  case hseClldTestRun:
    clld_indx=0;
    give_event(eventClldVarResCtrl,0);
    break;
    
  case eventClldVarResCtrl:
    clld_indx++;
    usb_send_pack(hseClldTestRunResp, 0);
    break;
    
  case hseLLDPage:
    state=stStEng;
    usb_send_pack(hseLLDPage, 0);
    break;
    
  case hseLLDQcStart:
    smp_prime_oper_flg=false;
    state=stStby;
    lld_fc=lldFullFunc;
    smpl_pr.sample_num=usb_data_buf[0];
    smple_rack_cnt=0;
    jude_cnt=smpl_pr.sensing_cnt-1;
    dw_prime_vol=4500;
    smpl_prime.pm_num=SMP_PRME_PP;
    set_timer_(eventSmpPrimeInit,10,0);
    usb_send_pack(hseLLDQcStartResp, 0);    
    break;
    
  case hseClldTestStop:
    beep(80, 1);
    rb=255,ra=100;
    usb_send_pack(hseClldTestStopResp, 0);    
    break;
    
  case hseClldRaRbSet:
    smp_pram.r1=usb_data_buf[2]<<8;
    smp_pram.r1|=usb_data_buf[3];
    smp_pram.r2=usb_data_buf[0]<<8;
    smp_pram.r2|=usb_data_buf[1];
    hlld_param_write();    
    
    usb_send_pack(hseClldRaRbSetResp, usb_data_buf);
    hlld_param_read();    
    break;
    
  case hseClldRaRbRead:
    hlld_param_read();    
    uSendToPC_Buf[3]=smp_pram.r1;
    uSendToPC_Buf[2]=smp_pram.r1>>8;
    uSendToPC_Buf[1]=smp_pram.r2;
    uSendToPC_Buf[0]=smp_pram.r2>>8;
    usb_send_pack(hseClldRaRbReadResp, uSendToPC_Buf);
    break;

  case hseClldSWSSet:
    cll_state=0;
    ra=100;
    if(!clld_indx){
      rb=usb_data_buf[1];
      usb_send_pack(hseClldSWSResp, uSendToPC_Buf);
    }
    edbmt_cmd_send(ADDR_MOTOR_Z,INST_SGP,0,2,SWS_OP);
    give_event(eventSWSVarResCtrl,0);
    break;
    
  case eventSWSVarResCtrl:
    give_event(eventSWSVarCheck,0);
    break;
    
  case eventSWSVarCheck:
    if(cll_state==5){
      hlld_send_pack(HLLD_ADD, HLLD_CLLD_VOL,0, 0);
      edbmt_cmd_send(ADDR_MOTOR_Z,INST_SGP,2,2,0);
      give_event(eventSWSVarEnd,0);
    }else { 
      edbmt_cmd_send(ADDR_MOTOR_Z, INST_GGP, 2, 2, 0);
      ra--;
      give_event(eventSWSVarResCtrl,0);
    }
    
    if(!ra)
      give_event(eventSWSVarFail,0);
    break;
    
  case eventSWSVarEnd:
    uSendToPC_Buf[0]= g_tSubRun_hLLD_Data.wData;
    sws_arry[clld_indx]=uSendToPC_Buf[0];
    clld_indx++;
    if(clld_indx==10)
    {
      clld_indx=0;
      for(int i=0; i<10; i++) sws_sum+=sws_arry[i];
      sws_sum=sws_sum/10;
      uSendToPC_Buf[0]=(byte)sws_sum;
      usb_send_pack(hseClldSWSReadResp, uSendToPC_Buf);
      smp_pram.sws=sws_sum;
      beep(80, 2);
      hlld_param_write();
      memset(sws_arry,0,sizeof(sws_arry));
      sws_sum=0;
    }
    else give_event(hseClldSWSSet,0); 
    break;
    
  case eventSWSVarFail:
    uSendToPC_Buf[0]=0xff; 
    uSendToPC_Buf[1]=0xff;
    usb_send_pack(hseClldSWSReadResp, uSendToPC_Buf);
    beep(10, 10);    
    break;
    
  case hseClldSWSRead:
    hlld_param_read();
    uSendToPC_Buf[0]=smp_pram.sws<<8;
    uSendToPC_Buf[1]=smp_pram.sws;
    usb_send_pack(hseClldSWSReadResp, uSendToPC_Buf);    
    break;
    
  case hseClldSWSReadResp:
    break;

  case hseClldSWSWrite: 
    smp_pram.sws=usb_data_buf[0];
    smp_pram.sws|=usb_data_buf[1];
    hlld_param_write();
    usb_send_pack(hseClldSWSWriteResp, 0);
    break;
    
  //PLLD   
  case hsePlldPage:
    state=stStEng;
    usb_send_pack(hsePlldPage, 0);    
    break;
    
  case hsePlldQcStart:
    plld_qc=true;
    smp_prime_oper_flg=false;
    smple_rack_cnt=0;
    lld_fc=lldFuncPlld;
    smpl_prime.pm_num=SMP_PRME_PP;
    set_timer_(eventSmpPrimeInit,10,0);
    usb_send_pack(hsePlldQcStartResp, 0);    
    break;
    
  case hsePlldJudgResult:
    break;
  case hsePlldStreamStart:
    break;
  case hsePlldStreamData:
    break;
  
  case eventSmpQcStreamInit:
    uSendToPC_Buf[2]=PLLD_TOTAL_INDEX>>8;
    uSendToPC_Buf[3]=(byte)PLLD_TOTAL_INDEX;
    plld_index=1;
    usb_send_pack(hsePlldStreamStart, uSendToPC_Buf);
    break;
    
  case hsePlldStreamStartResp:
    hlld_send_pack(HLLD_ADD, HLLD_PLLD_GET_SENS,0, plld_index-1);
    break;
    
  case eventSmpQcStreamGo:
    uSendToPC_Buf[0]= g_tSubRun_hLLD_Data.wContent >> 8;
    uSendToPC_Buf[1]= g_tSubRun_hLLD_Data.wContent;
    uSendToPC_Buf[2]= g_tSubRun_hLLD_Data.wData >> 8;
    uSendToPC_Buf[3]= g_tSubRun_hLLD_Data.wData;

    usb_send_pack(hsePlldStreamData, uSendToPC_Buf);
    break;
  
    /*uint32_t combined_value = 0; 추후 추가가능성
    uint8_t prev_uSendToPC_Buf[4];
    
    for (int i = 0; i < 4; i++) 
    {
      prev_uSendToPC_Buf[i] = current_uSendToPC_Buf[i];
    }
    
    current_uSendToPC_Buf[0]= g_tSubRun_hLLD_Data.wContent >> 8;
    current_uSendToPC_Buf[1]= g_tSubRun_hLLD_Data.wContent;
    current_uSendToPC_Buf[2]= g_tSubRun_hLLD_Data.wData >> 8;
    current_uSendToPC_Buf[3]= g_tSubRun_hLLD_Data.wData;

    combined_value |= current_uSendToPC_Buf[0] << 24;
    combined_value |= current_uSendToPC_Buf[1] << 16;
    combined_value |= current_uSendToPC_Buf[2] << 8;
    combined_value |= current_uSendToPC_Buf[3];
    
    if(combined_value > 100000)
    {
      usb_send_pack(hsePlldStreamData, current_uSendToPC_Buf);
    } 
    else
    {
      usb_send_pack(hsePlldStreamData, prev_uSendToPC_Buf); 
    }  
    break;*/

  case hsePlldStreamDataResp:
    plld_index++;
  
    if(PLLD_TOTAL_INDEX >= plld_index )
      set_timer_(hsePlldStreamStartResp,50,0);
    else
      //set_timer_(eventSmpPass,500,0);
    break;
    
  case hsePlldMinPramSet:
    smp_pram.min_std=usb_data_buf[2]<<8;
    smp_pram.min_std|=usb_data_buf[3];
    hlld_param_write();
    usb_send_pack(hsePlldMinPramSetResp, usb_data_buf);  
    break;
  case hsePlldMinPramSave:  
    smp_pram.min_std=usb_data_buf[2]<<8;
    smp_pram.min_std|=usb_data_buf[3];
    hlld_param_write();
    usb_send_pack(hsePlldMinPramSave, usb_data_buf);  
    break;

  case  hsePlldMinPramRead:
    hlld_param_read();
    uSendToPC_Buf[0]=smp_pram.min_std>>8;
    uSendToPC_Buf[1]=smp_pram.min_std;
    usb_send_pack(hsePlldMinPramReadResp, uSendToPC_Buf); 
    break;
  case hsePlldMaxPramSet:
    smp_pram.max_std=usb_data_buf[2]<<8;
    smp_pram.max_std|=usb_data_buf[3];
    hlld_param_write();
    usb_send_pack(hsePlldMaxPramSetResp, usb_data_buf); 
    break;

  case hsePlldMaxPramSave:
    smp_pram.max_std=usb_data_buf[2]<<8;
    smp_pram.max_std|=usb_data_buf[3];
    hlld_param_write();
    usb_send_pack(hsePlldMaxPramSave, usb_data_buf); 
    break;

  case hsePlldMaxPramRead:
    hlld_param_read();  
    uSendToPC_Buf[0]=smp_pram.max_std>>8;
    uSendToPC_Buf[1]=smp_pram.max_std;
    usb_send_pack(hsePlldMaxPramReadResp, uSendToPC_Buf);
    break;
    //hsePlldMaxPramReadResp,
  case hsePlldSlopPramSet:
    smp_pram.slop_min_std=usb_data_buf[0]<<8;
    smp_pram.slop_min_std|=usb_data_buf[1];
    smp_pram.slop_max_std=usb_data_buf[2]<<8;
    smp_pram.slop_max_std|=usb_data_buf[3];
    hlld_param_write();
    usb_send_pack(hsePlldMaxPramSetResp, usb_data_buf); 
    break;

  case hsePlldSlopPramSave:
    smp_pram.slop_min_std=usb_data_buf[0]<<8;
    smp_pram.slop_min_std|=usb_data_buf[1];
    smp_pram.slop_max_std=usb_data_buf[2]<<8;
    smp_pram.slop_max_std|=usb_data_buf[3];
    hlld_param_write();
    usb_send_pack(hsePlldSlopPramSave, usb_data_buf); 
    break;

  case hsePlldSlopPramRead:
    hlld_param_read();
    uSendToPC_Buf[0]=smp_pram.slop_min_std>>8;
    uSendToPC_Buf[1]=smp_pram.slop_min_std;
    uSendToPC_Buf[2]=smp_pram.slop_max_std>>8;
    uSendToPC_Buf[3]=smp_pram.slop_max_std;
    usb_send_pack(hsePlldSlopPramReadResp, uSendToPC_Buf); 
    break;
/////////////////////////////현재 LLD보드에서 상수로 인덱스범위를 조정 아래 프로토콜은 미구현 상태. 
   case hsePlldBeginIndexSet:
    smp_pram.begin_index=usb_data_buf[2]<<8;
    smp_pram.begin_index|=usb_data_buf[3];
    hlld_param_write();
    hlld_send_pack(HLLD_ADD, HLLD_PLLD_INDEX_SET,smp_pram.begin_index, smp_pram.end_index);
    usb_send_pack(hsePlldBeginIndexSet, usb_data_buf); 
    break;

   case hsePlldBeginIndexSave:
    smp_pram.begin_index=usb_data_buf[2]<<8;
    smp_pram.begin_index|=usb_data_buf[3];
    hlld_param_write();
    hlld_send_pack(HLLD_ADD, HLLD_PLLD_INDEX_SET,smp_pram.begin_index, smp_pram.end_index);
    usb_send_pack(hsePlldBeginIndexSave, usb_data_buf); 
    break;

  case hsePlldBeginIndexRead:
    hlld_param_read();
    uSendToPC_Buf[2]=smp_pram.begin_index<<8;
    uSendToPC_Buf[3]|=smp_pram.begin_index;
    usb_send_pack(hsePlldBeginIndexRead, uSendToPC_Buf); 
    break;

   case hsePlldEndIndexSet:
    smp_pram.end_index=usb_data_buf[2]<<8;
    smp_pram.end_index|=usb_data_buf[3];
    hlld_param_write();
    hlld_send_pack(HLLD_ADD, HLLD_PLLD_INDEX_SET,smp_pram.begin_index, smp_pram.end_index);
    usb_send_pack(hsePlldEndIndexSet, usb_data_buf); 
    break;

   case hsePlldEndIndexSave:
    smp_pram.end_index=usb_data_buf[2]<<8;
    smp_pram.end_index|=usb_data_buf[3];
    hlld_param_write();
    hlld_send_pack(HLLD_ADD, HLLD_PLLD_INDEX_SET,smp_pram.begin_index, smp_pram.end_index);
    usb_send_pack(hsePlldEndIndexSave, usb_data_buf); 
    break;
    
   case hsePlldEndIndexRead:
    hlld_param_read();
    uSendToPC_Buf[2]=smp_pram.end_index<<8;
    uSendToPC_Buf[3]|=smp_pram.end_index;
    usb_send_pack(hsePlldEndIndexRead, uSendToPC_Buf); 
    break;
/////////////////////////////////////////////////////////////////////////////////////////////

  default:
    break;
  }
  return event;
}



