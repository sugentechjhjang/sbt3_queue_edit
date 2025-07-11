
#include "main.h"



uint32_t start_time_ms = 0;
uint32_t end_time_ms = 0;
int32_t Get_SYRINGE_ReceivPacketHandle(uint8_t *p_sy_GetPacketBuf);
int32_t SYRINGE_ReceivPacketExec(uint8_t *p_sy_ReceivPacket);
uint32_t elapsed_time=0;
bool sy_flag = false;

enum sy_pump_state sy_state=pmNone;

void record_start_time() {
    start_time_ms = HAL_GetTick(); // 
}

uint32_t current_time_ms() {
    return HAL_GetTick(); 
}

#define SYRINGE_C3000_SEND_MAX_RETRY            5
#define SYRINGE_C3000_485_DIRECTION_DELAY       50
#define SYRINGE_C3000_REPONSE_TIMEOUT           1000

#define SYRINGE_BUSY  0x40
#define SYRINGE_IDLE  0x60
#define SYRINGE_ERROR 0x0F

#define C3000_OK                0x01
#define C3000_CHECKSUN_ERROR    0x02
#define C3000_TX_ERROR          0x03
#define C3000_RX_ERROR          0x04
#define C3000_BUSY              0x05

#define SY_RECV_OK             0
#define SY_RECV_FAIL           1
#define SYRINGE_C3000_RESPONSE_BUSY             0x60

#define SY_RECV_MAX 50

int32_t sy_Check;
struct syringe_param syrg_pram;

UART_HandleTypeDef huart3;


#define SYRINGE_QUE_SZ	50

#define SYRINGE_QUE_OK	0
#define SYRINGE_QUE_FULL	1
#define SYRINGE_QUE_EMPTY	2

typedef struct
{
  uint32_t sy_Tail;
  uint32_t sy_Head;
  uint8_t sy_QueBuf[SYRINGE_QUE_SZ];
}SYRINGE_Que_t;

SYRINGE_Que_t Syringe_Que_Handle;

int32_t SYRINGE_ReceivePutQue(uint8_t uPutData)
{
  int32_t sy_Tail;

  sy_Tail = (Syringe_Que_Handle.sy_Tail + 1)%SYRINGE_QUE_SZ;

  if( sy_Tail == Syringe_Que_Handle.sy_Head) return SYRINGE_QUE_FULL;

  Syringe_Que_Handle.sy_QueBuf[Syringe_Que_Handle.sy_Tail] = uPutData;

  Syringe_Que_Handle.sy_Tail = sy_Tail;
          
  return SYRINGE_QUE_OK;
}

int32_t SYRINGE_ReceiveGetQue(uint8_t *p_uGetData)
{
  if( Syringe_Que_Handle.sy_Tail == Syringe_Que_Handle.sy_Head ) return SYRINGE_QUE_EMPTY;

  *p_uGetData = Syringe_Que_Handle.sy_QueBuf[Syringe_Que_Handle.sy_Head];

  Syringe_Que_Handle.sy_Head = (Syringe_Que_Handle.sy_Head + 1)%SYRINGE_QUE_SZ;

  return SYRINGE_QUE_OK;
}

void SYRINGE_QueReset(void)
{
  NVIC_DisableIRQ(USART3_IRQn);
  Syringe_Que_Handle.sy_Head = 0;
  Syringe_Que_Handle.sy_Tail = 0;
  memset(Syringe_Que_Handle.sy_QueBuf,0,SYRINGE_QUE_SZ);
  NVIC_EnableIRQ(USART3_IRQn);
}

uint32_t SYRINGE_QueHaveDataSize(void)
{
    uint32_t dwReturnData;
    NVIC_DisableIRQ(USART3_IRQn); 

    if (Syringe_Que_Handle.sy_Head <= Syringe_Que_Handle.sy_Tail)
    {
        dwReturnData = Syringe_Que_Handle.sy_Tail - Syringe_Que_Handle.sy_Head;
    }
    else
    {
        dwReturnData = SYRINGE_QUE_SZ - Syringe_Que_Handle.sy_Head;
        dwReturnData += Syringe_Que_Handle.sy_Tail;
    }
    
    NVIC_EnableIRQ(USART3_IRQn);

    return dwReturnData;
}

void c3000_add_checksum(uint8_t *pdata)
{
  uint8_t i = 0, checksum = 0;
  
  do
  {
    checksum ^= pdata[i++];
  }while(pdata[i-1] != 0x03);
  
  pdata[i] = checksum;
}

void syge_mem_init()
{
syge_param_read();
if(syrg_pram.air_gap==(~0)||syrg_pram.vol==(~0)){
    syrg_pram.air_gap=150;
    syrg_pram.vol=1744;
    syrg_pram.speed=11;
    syge_param_write();
  }
  //syge_param_read();
}
void syge_init()
{
  C3000_send_command(1, "U5R\0");
  while(sy_state == sypm_busy )
  {
     C3000_srige_oper(SYRINGE_STATE_CHECK , 0);
  }       
  C3000_srige_oper(SYRINGE_INIT , 0);
}

void Syringe_485_Direction_Set(GPIO_PinState pin_state)
{
  HAL_GPIO_WritePin(UART3_DIR_GPIO_Port, UART3_DIR_Pin, pin_state);  // DIR RS-485
}


void C3000_srige_oper(byte cmd , uint data_value)
{
  char syringe_data_temp[100] = {0,};
  int8_t retry_count=0;
  //Delay Command: M
  switch(cmd)
  {
  case SYRINGE_NONE:
    break;
  case SYRINGE_INIT:
    sy_repeat_en(EN);
    C3000_send_command(1, "ZR\0");
    while(sy_state == sypm_busy)
    {
      C3000_send_command(1, "Q\0"); // 상태체크
      if(!sy_com_timeout_cnt())
      {
        sy_repeat_en(EN);
        retry_count++; 
        if(retry_count <= 3) 
        {
          C3000_send_command(1, "T\0"); //정지
          HAL_Delay(500);
          C3000_send_command(1, "ZR\0");
        }
        else
        {
          error(errSygTimeOut,0);
          while(1);
        }
      }
    }
    retry_count = 0;
    sy_repeat_en(DISEN);    
    break;
  case SYRINGE_STATUS:
    C3000_send_command(1, "?29\0");
    while(sy_state == sypm_busy)
    {
      C3000_send_command(1, "Q\0"); 
    }     
    break;
  case SYRINGE_SPEED_SET:       //SPEED CODE (0~40 Range) / default(11) - V1400  
    sprintf((char*)syringe_data_temp, "S%02dR\0", (byte)(data_value));
    C3000_send_command(1, syringe_data_temp); 
    while(sy_state == sypm_busy)
    {
      C3000_send_command(1, "Q\0"); 
    }  
    
    break;

  case SYRINGE_ACCEL_SET:       //SET ACC SLOPE CODE (1~20 ) / default(14)
    sprintf((char*)syringe_data_temp, "L%02dR\0", (byte)(data_value));
    C3000_send_command(1,syringe_data_temp);
    while(sy_state == sypm_busy)
    {
      C3000_send_command(1, "Q\0"); 
    }      
    break;

  case SYRINGE_MOVE_DOWN:
    sprintf((char*)syringe_data_temp, "P%04dR\0", data_value);
    C3000_send_command(1, syringe_data_temp);
    while(sy_state == sypm_busy && sy_flag == false)
    {
      C3000_send_command(1, "Q\0"); 
    }
    if(sy_flag == true)
    {
      sy_state = sypm_idle; 
    }
    
    break;
  case SYRINGE_MOVE_UP:
    sprintf((char*)syringe_data_temp, "D%04dR\0", data_value);
    C3000_send_command(1, syringe_data_temp);
    while(sy_state == sypm_busy)
    {
      C3000_send_command(1, "Q\0"); 
    }     
    break;
  case SYRINGE_VALVE_LEFT:
    C3000_send_command(1, "IR\0");
    while(sy_state == sypm_busy)
    {
      C3000_send_command(1, "Q\0"); 
    }    
    break;
  case SYRINGE_VALVE_RIGHT:     //default
    C3000_send_command(1, "OR\0");
    while(sy_state == sypm_busy)
    {
      C3000_send_command(1, "Q\0"); 
    }    
    break;
  case SYRINGE_VALVE_BYPASS:
    C3000_send_command(1, "BR\0");
    while(sy_state == sypm_busy)
    {
      C3000_send_command(1, "Q\0"); 
    }         
    break;
  case SYRINGE_STATE_CHECK:
    C3000_send_command(1, "Q\0");    
    break;
  case SYRINGE_STOP:  
    C3000_send_command(1, "T\0");    
    break;
  }
}

byte syring_receive_bufs[50]={0,};
byte syringe_chr=0;
/* USART3 init function */
void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart3);
  
  HAL_UART_Receive_IT(&huart3, &syringe_chr, 1);
  sy_state=sypm_idle;
}

HAL_StatusTypeDef UART3_ReInit(void)
{
    HAL_StatusTypeDef status;

    // UART5 비활성화 (DeInit)
    status = HAL_UART_DeInit(&huart3);
    if (status != HAL_OK)
    {
        // UART 비활성화 실패 처리
        return status;
    }

    // UART5 다시 초기화 (Init)
    status = HAL_UART_Init(&huart3);
    if (status != HAL_OK)
    {
        // UART 초기화 실패 처리
        return status;
    }

    // 초기화 성공
    HAL_UART_Receive_IT(&huart3, &syringe_chr, 1);
    return HAL_OK;
}

void C3000_send_command(uint8_t addr, char *pdata)
{
  
  uint8_t i = 0; 
  uint32_t dwCnt,queue_size,HAL_Fail_Cnt;  
  int32_t sy_ReTryCnt=0;
  byte send_data_t[30] = {0};
  byte sy_ReceivPacket[50] = {0};

  
  sprintf((char*)send_data_t, "\2%1d1%s\3", addr, pdata); 
  c3000_add_checksum(send_data_t);
  i = strlen(pdata); //PACKET ���� 
  

  err_tmout_cnt_set(errSygTimeOut,20);

  while(1)
  {
    HAL_Delay(50);
    SYRINGE_QueReset();
    Syringe_485_Direction_Set(GPIO_PIN_SET); // TX DIR ���� prime
    sy_Check=HAL_UART_Transmit(&huart3, send_data_t, i+5, 200); 
    while (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC) == RESET);
    Syringe_485_Direction_Set(GPIO_PIN_RESET);  // RX DIR ����
   
    if(sy_Check)
    {
      HAL_Fail_Cnt++;
      UART3_ReInit();
      if(HAL_Fail_Cnt > 5) 
      {
        while(1);
      }
    }
   
    
    for(dwCnt = 0; dwCnt < 500; dwCnt++)
    {
      HAL_Delay(5);
      queue_size=SYRINGE_QueHaveDataSize();
      if(queue_size>=6)
      { 
        break;
      }  
    }

    sy_Check = Get_SYRINGE_ReceivPacketHandle(sy_ReceivPacket);
    
    
    if(sy_Check == SY_RECV_OK)
    {
      if (sy_ReceivPacket[3] == SYRINGE_BUSY || sy_ReceivPacket[3] == SYRINGE_IDLE)
      {
        break;
      }
    }
    
    sy_ReTryCnt++;
    HAL_Delay(100);
    
    if(sy_ReTryCnt > 5)
    {
      while(1);
    }

  }

  sy_Check=SYRINGE_ReceivPacketExec(sy_ReceivPacket);
  if(sy_Check == SY_RECV_OK)
  {
    
  }
  
  else if(sy_Check == SY_RECV_FAIL)
  {
    error(errSygTimeOut,0);
    while(1);
  }
  
  err_tmout_en(FALSE);
}



void UART_Syringe_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart3)
  {
    int32_t dwQueCheck;

    dwQueCheck = SYRINGE_ReceivePutQue(syringe_chr);
    if(dwQueCheck == SYRINGE_QUE_FULL)
    {
      //Error code
    }
    dwQueCheck = HAL_UART_Receive_IT(&huart3, &syringe_chr, 1);
    if(dwQueCheck)
    {
      while(1);
    }
  }
}


#define SYRINGE_STX 0x02
#define SYRINGE_ETX 0x03
byte syringe_escap=0;
byte sy_currnt_rec_buf_ind=0;
byte sy_recived_rec_buf_ind=0xff;
byte *sy_pnt=0;
byte *sy_receive_buf=0;
byte sy_checksum=0;






int32_t Get_SYRINGE_ReceivPacketHandle(uint8_t *p_sy_GetPacketBuf)
{
  uint32_t sy_Cnt;
  uint8_t sy_CheckSum = 0;
  int32_t sy_QueCheck;
  
  for(sy_Cnt = 0; ; sy_Cnt++)
  {
    if(sy_Cnt < SY_RECV_MAX) sy_QueCheck = SYRINGE_ReceiveGetQue( &p_sy_GetPacketBuf[sy_Cnt]);
    
    if(sy_QueCheck == SYRINGE_QUE_EMPTY) break;
  }

  if(p_sy_GetPacketBuf[0] == 0xFF && p_sy_GetPacketBuf[1] == SYRINGE_STX && !syringe_escap)
  {
    syringe_escap=1;
    if(p_sy_GetPacketBuf[4] == SYRINGE_ETX) 
    {
      sy_CheckSum=SYRINGE_STX^p_sy_GetPacketBuf[2]^p_sy_GetPacketBuf[3]^SYRINGE_ETX;
    }

    if((p_sy_GetPacketBuf[5] == sy_CheckSum) )
    {
      syringe_escap = 0;
      return SY_RECV_OK;
    }
  }

  return SY_RECV_FAIL;
  
  
}

int32_t SYRINGE_ReceivPacketExec(uint8_t *p_sy_ReceivPacket)
{
    
    if(p_sy_ReceivPacket[3]==0x60){
      sy_state=sypm_idle;
      return SY_RECV_OK;
    }
    
    else if(p_sy_ReceivPacket[3]==0x40){
      sy_state=sypm_busy;
      return SY_RECV_OK;
    }

    else if(p_sy_ReceivPacket[3]&0x0f){
      sy_state=sypm_error;
      return SY_RECV_FAIL;
    }
}

 event execute_syringe_ctrl(event event)
{
   byte dev_send_buf[4]={0,}; 
  switch(event)
  {
  case eventSygeSendCheck:
  break;
  case hseDSSetPage:
        state=stStEng;
    syge_param_read();
    usb_send_pack(hseDSSetPage,0); 
    C3000_srige_oper(SYRINGE_INIT, 0);  
    break;
  case hseSygeSet:
    syrg_pram.vol=merge_32bit(syrg_pram.vol,usb_data_buf);
    usb_send_pack(hseSygeSet,usb_data_buf);
    syge_param_write();
    break;
  case hseSygeSave:
    syrg_pram.vol=merge_32bit(syrg_pram.vol,usb_data_buf);
    syge_param_write();
    syge_param_read();
    sort_8bit( syrg_pram.vol,dev_send_buf);
    usb_send_pack(hseSygeSave, dev_send_buf);
    break;
  case hseSygeRead:
    syge_param_read();
    sort_8bit( syrg_pram.vol,dev_send_buf);
    usb_send_pack(hseSygeRead, dev_send_buf);
    break;
  case hseSygeMove:
    lld_fc=lldFuncDispVol;
    smpl_prime.pm_num=SMP_PRME_PP;
    set_timer_(eventSmpPrimeInit,10,0);
    usb_send_pack(hseSygeMove, 0);

    break;
  case hseSygePause:
     usb_send_pack(hseSygePause, 0);
    break;
    
  case hseAirGapSet:
    syrg_pram.air_gap=merge_32bit(syrg_pram.air_gap,usb_data_buf);
    usb_send_pack(hseAirGapSet,usb_data_buf);
    syge_param_write();
    break;
  case hseAirGapSave:
    syrg_pram.air_gap=merge_32bit(syrg_pram.air_gap,usb_data_buf);
    syge_param_write();
    syge_param_read();
    sort_8bit( syrg_pram.air_gap,dev_send_buf);
    usb_send_pack(hseAirGapSave, dev_send_buf);    
    break;   
  case hseAirGapRead:
    syge_param_read();
    sort_8bit( syrg_pram.air_gap,dev_send_buf);
    usb_send_pack(hseAirGapRead, dev_send_buf);
    break;

  case hseSygeSpeedSet:
    syrg_pram.speed=merge_32bit(syrg_pram.speed,usb_data_buf);
    usb_send_pack(hseSygeSpeedSet,usb_data_buf);
    syge_param_write();
    break;

  case hseSygeSpeedSave:
    syrg_pram.speed=merge_32bit(syrg_pram.speed,usb_data_buf);
    syge_param_write();
    syge_param_read();
    sort_8bit(syrg_pram.speed,dev_send_buf);
    usb_send_pack(hseSygeSpeedRead, dev_send_buf);  
    break;  

  case hseSygeSpeedRead:
    syge_param_read();
    sort_8bit(syrg_pram.speed,dev_send_buf);
    usb_send_pack(hseSygeSpeedRead, dev_send_buf);
    break;
    
  default: break;
  }
  return event;
}

