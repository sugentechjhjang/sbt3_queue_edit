/* Includes ------------------------------------------------------------------*/


#include "main.h"
//#include "string.h"
#include <string.h>
#include <stdio.h>

UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;


#define PCBUF_QUEUE_LENGTH 100
#define MAIN_PC_BAUDRATE   57600

#define PC_STX 0x5B
#define PC_ETX 0x5D

byte pc_chr=0;
static byte const NO_RECIVED=0xFF;
byte pc_escap=0;
byte usb_currnt_rec_buf_ind=0;
byte usb_recived_rec_buf_ind=0xff;
byte *pc_pnt=0;
byte *usb_receive_buf;
byte *usb_received_buf;
byte pc_checksum=0;
byte usb_receive_bufs[300]={0,};
uint16_t usb_checksum,usb_checksum_b;
uint16_t usb_checksums=0xffff;
/* USART1 init function */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = MAIN_PC_BAUDRATE;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
  
 HAL_UART_Receive_IT(&huart1, &pc_chr, 1);  

}

extern void UART_EDB_RxCpltCallback(UART_HandleTypeDef *);
extern int32_t hsPC_ToDev_PutQue(uint8_t);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{      
  if(huart == &huart1)
  {
    if(hsPC_ToDev_PutQue(pc_chr))
    {
      //Error Code
    }
    HAL_UART_Receive_IT(&huart1, &pc_chr, 1);
  }
  UART_EDB_RxCpltCallback(huart);
  UART_Barcode_RxCpltCallback(huart);
  UART_Syringe_RxCpltCallback(huart);
  UART_Pump_LLD_RxCpltCallback(huart);
  
}

uint8_t ascii2hex(uint8_t code)
{
  uint8_t result = 0;

  if (code >= '0' && code <= '9') {
    result = code - '0'; 
  } else if (code >= 'A' && code <= 'F') {
    result = 10 + (code - 'A'); 
  } else if (code >= 'a' && code <= 'f') {
    result = 10 + (code - 'a');
  }
  return result;
}

uint8_t usb_data_buf[4];
uint16_t event_cmd=0,etc_event_cmd=0;
uint16_t pau_queue[EVENT_QUEUE_LENGTH];
uint16_t pau_head, pau_tail;
bool pause_flag=false;

#define TRANSMIT_BUF_LENGTH 50
byte usb_transmit_bufs[TRANSMIT_BUF_LENGTH];

bool debug_serial_mode=false;

void dbg_serial(char *s)
{
   debug_serial_mode = true;
   uint8_t buffer[256];
   sprintf((char *)buffer, "DEV<-%s\n", s);
   HAL_NVIC_DisableIRQ(USART1_IRQn);
   HAL_UART_Transmit(&huart1, buffer, strlen((char *)buffer), 100);
   HAL_NVIC_EnableIRQ(USART1_IRQn); 
   debug_serial_mode = false;
}

void feedback_motor_packet(uint8_t *p_uGetPacketBuf, uint32_t dwCount)
{
  debug_serial_mode = true;
  uint8_t buffer[256]; // 데이터 저장용 버퍼
  int offset = 0; // 버퍼에 데이터를 쓸 위치를 관리

  // 버퍼에 "DEV<-" 메시지를 먼저 넣음
  offset += sprintf((char *)buffer, "[");

  // p_uGetPacketBuf의 데이터를 16진수 형식으로 buffer에 저장
  for (uint32_t i = 0; i < dwCount && offset < sizeof(buffer) - 4; i++) {
      offset += sprintf((char *)&buffer[offset], "%02X ", p_uGetPacketBuf[i]);
  }

  // 개행 문자를 추가
  offset += sprintf((char *)&buffer[offset], "]\n");

  // UART 전송 중 인터럽트를 비활성화
  HAL_NVIC_DisableIRQ(USART1_IRQn);
  HAL_UART_Transmit(&huart1, buffer, offset, 100);
  HAL_NVIC_EnableIRQ(USART1_IRQn); 

  debug_serial_mode = false;
}

char *pnt;
//char *date;
event dev_cfg_ctrl(event event)
{
byte dev_send_buf[4]={0,}; 


//uint32_t start_time, end_time;
//char buffer[50];
  switch(event)
  {
  case eventEquiSN:
    pnt=FW_VER;
    dev_send_buf[0]=*pnt;
    dev_send_buf[1]=*(pnt+2);
    dev_send_buf[2]=*(pnt+4);
    dev_send_buf[3]=*(pnt+6);
    usb_send_pack(eventEquiSN,dev_send_buf);
    break;
    
  case eventReserve0:
    usb_send_pack(eventReserve0,dev_send_buf);
    break;

  case MCU_FW_DATE_CHECK:
    uint16_t year = (uint16_t)atoi(FW_YEAR);
    uint16_t date = (uint16_t)atoi(FW_DATE);

    dev_send_buf[0] = (uint8_t)((year >> 8) & 0xFF); 
    dev_send_buf[1] = (uint8_t)(year & 0xFF);       
    dev_send_buf[2] = (uint8_t)((date >> 8) & 0xFF); 
    dev_send_buf[3] = (uint8_t)(date & 0xFF);      
    usb_send_pack(MCU_FW_DATE_CHECK,dev_send_buf);
    break;

  case LLD_FW_DATE_CHECK:
    hlld_send_pack(HLLD_ADD, HLLD_FW_DATE,0, 0);
    break;
    
  default: 
    if((event_cmd&0xf000)==0xc000){
      app_data_write(event_cmd&0x0fff,usb_data_buf);
      pc_send_pack(event_cmd,usb_data_buf);
      event_cmd=0;
    }else if((event_cmd&0xf000)==0xd000){
      app_data_read(event_cmd&0x0fff,dev_send_buf);
      pc_send_pack(event_cmd,dev_send_buf);
      event_cmd=0;
    }
  break;
  }
  return event;
}


int32_t merge_32bit(int32_t out_data, byte* in_data )
{
  out_data=*in_data<<24;
  out_data|=*(in_data+1)<<16;
  out_data|=*(in_data+2)<<8;
  out_data|=*(in_data+3);
  
  return out_data;
}

void sort_8bit(int32_t in_data, byte* out_data )
{
  *out_data=in_data>>24;
  *(out_data+1)=in_data>>16;
  *(out_data+2)=in_data>>8;
  *(out_data+3)=in_data;
}
                                         
                                         
                                         
//-------------------------------------------------------------------------------
// ���� �ڵ� 
// 2024 03 11 �߰�
#define hsDEV_TO_PC_QUE_SZ      512

#define hsDEV_TO_PC_QUE_OK	0
#define hsDEV_TO_PC_QUE_FULL	1
#define hsDEV_TO_PC_QUE_EMPTY	2


typedef struct
{
  uint32_t dwTail;
  uint32_t dwHead;
  uint8_t uQueBuf[hsDEV_TO_PC_QUE_SZ];
}hsDevToPC_Que_t;

uint32_t dwUseDevToPCPutQue;

hsDevToPC_Que_t g_tDevToPC_QueHandle;

int32_t hsDevToPC_PutQue(uint8_t uPutData)
{
  int32_t dwTail;

  dwTail = (g_tDevToPC_QueHandle.dwTail + 1)%hsDEV_TO_PC_QUE_SZ;

  if( dwTail == g_tDevToPC_QueHandle.dwHead) return hsDEV_TO_PC_QUE_FULL;

  g_tDevToPC_QueHandle.uQueBuf[g_tDevToPC_QueHandle.dwTail] = uPutData;

  g_tDevToPC_QueHandle.dwTail = dwTail;
          
  return hsDEV_TO_PC_QUE_OK;
}

int32_t hsDevToPC_GetQue(uint8_t *p_uGetData)
{
  if( g_tDevToPC_QueHandle.dwTail == g_tDevToPC_QueHandle.dwHead ) return hsDEV_TO_PC_QUE_EMPTY;

  *p_uGetData = g_tDevToPC_QueHandle.uQueBuf[g_tDevToPC_QueHandle.dwHead];

  g_tDevToPC_QueHandle.dwHead = (g_tDevToPC_QueHandle.dwHead + 1)%hsDEV_TO_PC_QUE_SZ;

  return hsDEV_TO_PC_QUE_OK;
}

void hsDevToPC_QueReset(void)
{
  __disable_irq();
  g_tDevToPC_QueHandle.dwHead = 0;
  g_tDevToPC_QueHandle.dwTail = 0;
  __enable_irq();
}

uint32_t hsDevToPC_QueUseCheck(void)
{
  return dwUseDevToPCPutQue;
}

extern unsigned char hex2ascii();

//------------------------------------------------------------------------------
//0 STX, 1 ~ 4 CMD, 
enum
{
  hsDEV_TO_PC_STX = 0,
  hsDEV_TO_PC_CMD_1,
  hsDEV_TO_PC_CMD_2,
  hsDEV_TO_PC_CMD_3,
  hsDEV_TO_PC_CMD_4,
  hsDEV_TO_PC_LEN_1,
  hsDEV_TO_PC_LEN_2,
  hsDEV_TO_PC_DAT_1,
  hsDEV_TO_PC_DAT_2,
  hsDEV_TO_PC_DAT_3,
  hsDEV_TO_PC_DAT_4,
  hsDEV_TO_PC_DAT_5,
  hsDEV_TO_PC_DAT_6,
  hsDEV_TO_PC_DAT_7,
  hsDEV_TO_PC_DAT_8,
  hsDEV_TO_PC_CKSM_1,
  hsDEV_TO_PC_CKSM_2,  
  hsDEV_TO_PC_ETX,
  
  hsDEV_TO_PC_MAX
};

int32_t hsDevToPC_DataSendQueHandle(enum cntrl_event cmd, uint8_t *data);
int32_t hsDevToPC_NoConvertSendQueHandle(uint8_t *p_uData, uint32_t dwLen);

void usb_send_pack(enum cntrl_event cmd, uint8_t *data)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDevToPC_DataSendQueHandle(cmd, data);
  if(dwCheck)
  {
    //Error Code
    
  }
  
}

void pc_send_pack(uint16_t cmd, uint8_t *data)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDevToPC_DataSendQueHandle((enum cntrl_event)cmd, data);
  if(dwCheck)
  {
    //Error Code
    
  }
}

void send_test(uint8_t *data, unsigned char length)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDevToPC_NoConvertSendQueHandle(data, (uint32_t)length);
  if(dwCheck)
  {
    //Error Code
    
  }  
}


int32_t hsDevToPC_DataSendQueHandle(enum cntrl_event cmd, uint8_t *data)
{
  uint32_t dwCmd, dwCnt, dwDataSZ, dwCheck = 0;
  uint8_t uSendDataBuf[hsDEV_TO_PC_MAX];
  uint8_t *p_uData, uCheckSum = 0;
  
  dwUseDevToPCPutQue = SET;
  
  dwCmd = cmd;
  dwDataSZ = (hsDEV_TO_PC_CKSM_1 - hsDEV_TO_PC_DAT_1)/2;
  
  uSendDataBuf[hsDEV_TO_PC_STX] = PC_STX;
  uSendDataBuf[hsDEV_TO_PC_CMD_1] = hex2ascii((dwCmd>>12) & 0x000F);
  uSendDataBuf[hsDEV_TO_PC_CMD_2] = hex2ascii((dwCmd>>8) & 0x000F);
  uSendDataBuf[hsDEV_TO_PC_CMD_3] = hex2ascii((dwCmd>>4) & 0x000F);
  uSendDataBuf[hsDEV_TO_PC_CMD_4] = hex2ascii((dwCmd) & 0x000F);
  uSendDataBuf[hsDEV_TO_PC_LEN_1] = '0';
  uSendDataBuf[hsDEV_TO_PC_LEN_2] = '8';
  
  p_uData = &uSendDataBuf[hsDEV_TO_PC_DAT_1];
  
  for(dwCnt = 0; dwCnt < dwDataSZ; dwCnt++)
  {
    p_uData[dwCnt*2] = hex2ascii((data[dwCnt]>>4) & 0x0F);
    p_uData[(dwCnt*2)+1] = hex2ascii((data[dwCnt]) & 0x0F);
  }
  
  for(dwCnt = 0; dwCnt < hsDEV_TO_PC_DAT_8 ; dwCnt++)
  {
    uCheckSum += uSendDataBuf[dwCnt + hsDEV_TO_PC_CMD_1];
  }
  
  uSendDataBuf[hsDEV_TO_PC_CKSM_1] = hex2ascii( (uCheckSum >> 4) & 0x0F);
  uSendDataBuf[hsDEV_TO_PC_CKSM_2] = hex2ascii( (uCheckSum) & 0x0F);
  
  uSendDataBuf[hsDEV_TO_PC_ETX] = PC_ETX;
  
  for(dwCnt = 0; dwCnt < hsDEV_TO_PC_MAX; dwCnt++)
  {
    dwCheck = hsDevToPC_PutQue(uSendDataBuf[dwCnt]);
     
     if(dwCheck) goto DEV_TO_PC_SND_QUE_EXIT;

  }
  DEV_TO_PC_SND_QUE_EXIT: 
    dwUseDevToPCPutQue = RESET;
  
  return dwCheck;
}

int32_t hsDevToPC_NoConvertSendQueHandle(uint8_t *p_uData, uint32_t dwLen)
{
  int32_t dwCheck = 0, dwCnt;
  
  dwUseDevToPCPutQue = SET;
  
  for(dwCnt = 0; dwCnt < dwLen ; dwCnt++)
  {
    dwCheck = hsDevToPC_PutQue(p_uData[dwCnt]);
    if(dwCheck) break;
  }
  
  dwUseDevToPCPutQue = RESET;
  
  return dwCheck;
}

//------------------------------------------------------------------------------                                         
void hsDevToPC_ErrHandle(void) //Timer Run
{
  int32_t dwCheck, dwCnt;
  uint8_t uData[4] = {0};
  
  if(dwUseDevToPCPutQue == SET) return;
  
  for(dwCnt = 0; dwCnt < 10 ; dwCnt++)
  {
    dwCheck = hsErr_GetQue(&uData[0]);
    if(dwCheck == 0) 
    {
      dwCheck = hsDevToPC_DataSendQueHandle(eventError, uData);
      if(dwCheck) return;
    }
    else return;
  }
}
//------------------------------------------------------------------------------
#define DEV_TO_PC_SND_BUF_SZ            128


uint8_t g_uDevToPCSendBuffer[DEV_TO_PC_SND_BUF_SZ];

uint32_t g_dwDEV_TO_SND_TX_BUSY_Check;

void hsDevToPC_PacketSendHandle(void) //Timer Run
{
  int32_t dwCheck = 0;
  uint32_t dwSendDataSZCnt;
  if(g_dwDEV_TO_SND_TX_BUSY_Check == SET) return;
  
  for(dwSendDataSZCnt = 0; dwSendDataSZCnt < DEV_TO_PC_SND_BUF_SZ; dwSendDataSZCnt++)
  {
    dwCheck = hsDevToPC_GetQue( &g_uDevToPCSendBuffer[dwSendDataSZCnt]);
    if(dwCheck == hsDEV_TO_PC_QUE_EMPTY) break;
  }
  
  if(dwSendDataSZCnt > 0)
  {
    g_dwDEV_TO_SND_TX_BUSY_Check = SET;
    dwCheck = HAL_UART_Transmit_DMA(&huart1, g_uDevToPCSendBuffer, dwSendDataSZCnt);
    if(dwCheck)
    {
      //Error Code
    }
  }
}

//------------------------------------------------------------------------------
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if((huart->Instance) == USART1)
  {
    g_dwDEV_TO_SND_TX_BUSY_Check =  RESET;
  }
}

//=============================================================================
//2024 03 12
//------------------------------------------------------------------------------
#define hsPC_TO_DEV_QUE_SZ      64

#define hsPC_TO_DEV_QUE_OK	0
#define hsPC_TO_DEV_QUE_FULL	1
#define hsPC_TO_DEV_QUE_EMPTY	2


typedef struct
{
  uint32_t dwTail;
  uint32_t dwHead;
  uint8_t uQueBuf[hsPC_TO_DEV_QUE_SZ];
}hsPC_ToDev_Que_t;

hsPC_ToDev_Que_t g_tPC_ToDev_QueHandle;

int32_t hsPC_ToDev_PutQue(uint8_t uPutData)
{
  int32_t dwTail;

  dwTail = (g_tPC_ToDev_QueHandle.dwTail + 1)%hsPC_TO_DEV_QUE_SZ;

  if( dwTail == g_tPC_ToDev_QueHandle.dwHead) return hsPC_TO_DEV_QUE_FULL;

  g_tPC_ToDev_QueHandle.uQueBuf[g_tPC_ToDev_QueHandle.dwTail] = uPutData;

  g_tPC_ToDev_QueHandle.dwTail = dwTail;
          
  return hsPC_TO_DEV_QUE_OK;
}

int32_t hsPC_ToDev_GetQue(uint8_t *p_uGetData)
{
  if( g_tPC_ToDev_QueHandle.dwTail == g_tPC_ToDev_QueHandle.dwHead ) return hsPC_TO_DEV_QUE_EMPTY;

  *p_uGetData = g_tPC_ToDev_QueHandle.uQueBuf[g_tPC_ToDev_QueHandle.dwHead];

  g_tPC_ToDev_QueHandle.dwHead = (g_tPC_ToDev_QueHandle.dwHead + 1)%hsPC_TO_DEV_QUE_SZ;

  return hsPC_TO_DEV_QUE_OK;
}

void hsPC_ToDev_QueReset(void)
{
  __disable_irq();
  g_tPC_ToDev_QueHandle.dwHead = 0;
  g_tPC_ToDev_QueHandle.dwTail = 0;
  __enable_irq();
}

int32_t hsPC_ToDev_QueHaveDataSize(void)
{
  int32_t dwReturnData;
  
  __disable_irq();
  if(g_tPC_ToDev_QueHandle.dwHead <= g_tPC_ToDev_QueHandle.dwTail) dwReturnData = g_tPC_ToDev_QueHandle.dwTail - g_tPC_ToDev_QueHandle.dwHead;
  else
  {
    dwReturnData = hsPC_TO_DEV_QUE_SZ - g_tPC_ToDev_QueHandle.dwHead;
    dwReturnData = dwReturnData + g_tPC_ToDev_QueHandle.dwTail;
  }  
  __enable_irq();

  return dwReturnData;
}


//------------------------------------------------------------------------------
enum
{
  hsPC_TO_DEV_STX = 0,
  hsPC_TO_DEV_CMD_1,
  hsPC_TO_DEV_CMD_2,
  hsPC_TO_DEV_CMD_3,
  hsPC_TO_DEV_CMD_4,
  hsPC_TO_DEV_LEN_1,
  hsPC_TO_DEV_LEN_2,
  hsPC_TO_DEV_DAT_1,
  hsPC_TO_DEV_DAT_2,
  hsPC_TO_DEV_DAT_3,
  hsPC_TO_DEV_DAT_4,
  hsPC_TO_DEV_DAT_5,
  hsPC_TO_DEV_DAT_6,
  hsPC_TO_DEV_DAT_7,
  hsPC_TO_DEV_DAT_8,
  hsPC_TO_DEV_CKSM_1,
  hsPC_TO_DEV_CKSM_2,  
  hsPC_TO_DEV_ETX,
  
  hsPC_TO_DEV_MAX
};

typedef struct
{
  uint32_t dwRxCnt;
  uint8_t uRxPacketBuf[hsPC_TO_DEV_MAX];
}hsPC_To_DevCOM_CTRL_t;

hsPC_To_DevCOM_CTRL_t g_tPC_To_DevCOMHandle;

int32_t dwPC_To_Dev_PacketExec(uint16_t wCmd, uint8_t *p_Data);

void hsPC_To_DevRxPacket_Handle(void)
{
  int32_t dwRxQueHaveSZ = 0;
  uint32_t dwCnt;
  uint16_t wCmd;
  uint8_t uData[4];
  uint8_t uChecksum = 0, uCheckSumPacket;
  uint8_t uRxCovertBuf[hsPC_TO_DEV_MAX];
  uint8_t *p_uPacket;
  
  //Rx Packet generation 
  if(g_tPC_To_DevCOMHandle.dwRxCnt == hsPC_TO_DEV_STX)
  {
    dwRxQueHaveSZ = hsPC_ToDev_QueHaveDataSize();
    if(dwRxQueHaveSZ > 0)
    {
      hsPC_ToDev_GetQue( &(g_tPC_To_DevCOMHandle.uRxPacketBuf[g_tPC_To_DevCOMHandle.dwRxCnt]));
      if( g_tPC_To_DevCOMHandle.uRxPacketBuf[g_tPC_To_DevCOMHandle.dwRxCnt] == PC_STX) 
      {
        g_tPC_To_DevCOMHandle.dwRxCnt = hsPC_TO_DEV_CMD_1;
      }
    }
    
    return;
  }
  else
  {
    dwRxQueHaveSZ = hsPC_ToDev_QueHaveDataSize();
    if(dwRxQueHaveSZ < (hsPC_TO_DEV_MAX - 1) ) return;
    
    for(dwCnt = 0; dwCnt < (hsPC_TO_DEV_MAX -1) ; dwCnt++)
    {
      hsPC_ToDev_GetQue( &(g_tPC_To_DevCOMHandle.uRxPacketBuf[g_tPC_To_DevCOMHandle.dwRxCnt]));
      
      if( g_tPC_To_DevCOMHandle.uRxPacketBuf[g_tPC_To_DevCOMHandle.dwRxCnt] == PC_STX) 
      {
        g_tPC_To_DevCOMHandle.dwRxCnt = hsPC_TO_DEV_CMD_1;
        return;
      }
      
      g_tPC_To_DevCOMHandle.dwRxCnt = g_tPC_To_DevCOMHandle.dwRxCnt + 1;
      
      if(g_tPC_To_DevCOMHandle.dwRxCnt >= hsPC_TO_DEV_MAX) break;
    }
    

    if( g_tPC_To_DevCOMHandle.uRxPacketBuf[hsPC_TO_DEV_ETX] != PC_ETX) 
    {
      g_tPC_To_DevCOMHandle.dwRxCnt = hsPC_TO_DEV_STX;
      
      return;
    }
    

    for(dwCnt = hsPC_TO_DEV_CMD_1 ; dwCnt < hsPC_TO_DEV_CKSM_1 ; dwCnt++)
    {
      uChecksum +=g_tPC_To_DevCOMHandle.uRxPacketBuf[dwCnt];
    }
    
    //Packet Pre-Convert
    for(dwCnt = 0; dwCnt < hsPC_TO_DEV_MAX ; dwCnt++)
    {
      uRxCovertBuf[dwCnt] = ascii2hex(g_tPC_To_DevCOMHandle.uRxPacketBuf[dwCnt]);
    }
    
    //Convert CMD
    wCmd = (uRxCovertBuf[hsPC_TO_DEV_CMD_1] <<12) | (uRxCovertBuf[hsPC_TO_DEV_CMD_2] <<8) |(uRxCovertBuf[hsPC_TO_DEV_CMD_3] <<4) | uRxCovertBuf[hsPC_TO_DEV_CMD_4] ;
    
    
    //Convert Data
    p_uPacket = &uRxCovertBuf[hsPC_TO_DEV_DAT_1];
    for(dwCnt = 0; dwCnt < 4; dwCnt++)
    {
      uData[dwCnt] = (p_uPacket[dwCnt*2] << 4) | p_uPacket[(dwCnt*2)+1];
    }
    
    //Convert Checkssum
    uCheckSumPacket = (uRxCovertBuf[hsPC_TO_DEV_CKSM_1] << 4) | uRxCovertBuf[hsPC_TO_DEV_CKSM_2];
    
    //RxCnt Reset
    g_tPC_To_DevCOMHandle.dwRxCnt = hsPC_TO_DEV_STX;
    
    //Checksum Check 
    if( uChecksum != uCheckSumPacket)
    {
      dwCnt++;
      return;
    }
    
  }
  
  dwPC_To_Dev_PacketExec(wCmd, uData);
  
}

int32_t dwPC_To_Dev_PacketExec(uint16_t wCmd, uint8_t *p_Data)
{
  int32_t dwCheck = 0;
  uint32_t dwCnt;
  
  event_cmd = wCmd;
  
  for(dwCnt = 0; dwCnt < 4 ; dwCnt++)
  {
    usb_data_buf[dwCnt] = p_Data[dwCnt];
  }
  
  switch(event_cmd)
  {
    case 0:
      break;
    
    case eventFwDownReset: 
     HAL_NVIC_SystemReset(); 
     break;


    case eventPause:

    if(state==stPauseSq)
    {
      if(!usb_data_buf[3])
      {
        beep(1000, 1);
        state=stReady;
        set_timer_(eventSqNext,100,0);
      }
    }
    else
    {
      if(!usb_data_buf[3])
      {
        if(door_port==eventDoorClose)
        {
          beep(1000, 1);
          state=stReady;
          set_timer_(pause_eve,pause_time,0);
          pause_eve=0;
          pause_time=0;
          usb_send_pack(eventPauseRes,usb_data_buf);

          if(full_pr==Dry)
          { 
            HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_RESET);
          }
        }
        else usb_send_pack(eventDoorAlarm,0); 

      }
      else
      {
        state=stPause;
        if(full_pr==Dry)
        {
          beep(1000, 3);
          HAL_GPIO_WritePin(FAN_ARR_GPIO_Port,FAN_ARR_Pin,GPIO_PIN_SET);
          usb_send_pack(eventPauseRes,usb_data_buf);
        }
      }      
    }

    break;

    default:
      break;  
  }
  
  set_timer_(event_cmd,100,0);
  
  return dwCheck;
}                                         
                                         
                                         
                                         
                                    
