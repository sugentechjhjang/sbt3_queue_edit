#include "main.h"


/*****************************************************************************
PACKET Configuration
[Config]  Leanth - Opcode - Message Source - Status -   Data   - Checksum 
[Byte]      1    -   1    -     1          -    1   - Variable - 2 
[Example]  
******************************************************************************/
UART_HandleTypeDef huart2;

#define BARCODE_BAUDRATE                9600
#define READ_TMOUT 50;

/*******************OPCODE*******************/
// HOST -> DECODER
#define  OP_CAPABILITY_REQUEST 0xD3
#define  OP_IMAGER_MODE        0xF7
#define  OP_PARAM_DEFAULTS     0xC8
#define  OP_PARAM_REQUEST      0xC7
#define  OP_REQUEST_REVISION   0xA3
#define  OP_SLEEP              0xEB
#define  OP_START_SESSION      0xE4
#define  OP_STOP_SESSION       0xE5   
#define  OP_WAKEUP             0x00 // N/A

// DECODER -> HOST 
#define  OP_CAPABILITY_REPLY 0xD4
#define  OP_DECODE_DATA      0xF3
#define  OP_EVENT            0xF6
#define  OP_IMAGE_DATA       0xB1
#define  OP_REPLY_REVISION   0xA4


// Between HOST and DECODE 
#define OP_CMD_ACK    0xD0
#define OP_CMD_NAK    0xD1
#define OP_PARAM_SEND 0xC6

/****************Message Source**************/
#define HOST     0x04
#define BARCODE  0x00

/****************status**************/
#define Status_default     0x00
/****************beep code**************/
#define FF_code          0xff
/*******************PARAMTER*******************/
//Parameter 
#define Param_Baudrate            0x9C 
#define Param_LedonTime           0x88
#define Param_TriggerMode         0x8A

#define Param_Code128             0x08
#define Param_Code39              0x00
#define Param_Interleaved2of5     0x06
#define Param_Codabar             0x07

/*******************DATA*******************/
//DATA - baudrate 
#define  Data_9600               0x06
#define  Data_19200              0x07
#define  Data_38400              0x08
#define  Data_57600              0x0a     
#define  Data_115200             0x0b

//DATA - LedonTime
#define  Data_LedonTime         0x0A // RANGE 0.5S ~ 25.5S   // EX) 1S -> 10(0x0A)  , 3S ->30(0x1E)

//DATA - bacode enable/disable
#define Data_Enable               0x01
#define Data_Disable              0x00

//DATA - Trigger mode
#define Data_Continue            0x01    //������ �������Ƿ� ������� �� ��~! 
#define Data_Level               0x00
/*****************************************/

struct bar_pram bar_pm;
UART_HandleTypeDef *se655_uart_handle;

struct bar_pram bar_pm ={
  1000,
  50000,
  10,
  25000,
  540000,
};



unsigned char se655_send_data(uint8_t param, uint8_t data)
{
  uint8_t i =0;
  int16_t chksum = 0;
  uint8_t send_data[9] = {7, OP_PARAM_SEND, HOST, Status_default, FF_code, 0 , 0, 0 ,0};
  
  send_data[5] = param;
  send_data[6] = data;  

  for(i = 0; i < 7; i++)
  {
    chksum += send_data[i];
  }
  chksum = -chksum;
  send_data[7] = chksum >> 8;
  send_data[8] = chksum;
  
  return barcode_data_send(send_data, 9);
  
}

enum barcode_state br_state=BarNone;

void bar_mem_init()
{
  bar_param_read();
  if(bar_pm.start_pos==(~0)){
    bar_pm.start_pos=6000;
    bar_pm.end_pos=252000;
    bar_pm.spl_num=20;
    bar_pm.start_pos2=271000;
    bar_pm.qr_tray_pos=540000;
    bar_param_write();
 }
    //bar_pm.start_pos2=285000;
}

void Se655_Barcode_Init(void)
{
  
  //HAL_GPIO_WritePin(BAR_WAKE_GPIO_Port, BAR_WAKE_Pin, GPIO_PIN_RESET);

   HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(BAR_WAKE_GPIO_Port, BAR_WAKE_Pin, GPIO_PIN_RESET);
  
  dwHsSW_Delay_ms(500);

    HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_SET);
  //HAL_GPIO_WritePin(BAR_PWR_GPIO_Port, BAR_PWR_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(BAR_WAKE_GPIO_Port, BAR_WAKE_Pin, GPIO_PIN_SET);
  dwHsSW_Delay_ms(500);
err_tmout_set(errBarcode);
  br_state=BarInit;
 while(se655_send_data(Param_Baudrate, Data_9600));         //��żӵ�
  while(br_init_pram_ack_chek() != true);
  while(se655_send_data(Param_TriggerMode, Data_Continue));      //Ʈ���Ÿ��
  while(br_init_pram_ack_chek() != true);
       // while(se655_send_data(Param_LedonTime, Data_LedonTime));    //Ʈ���Ÿ�� - LED TIME 
        //while(br_init_pram_ack_chek() != true);
  while(se655_send_data(Param_Code128, Data_Enable));         //BARCODE ENABLE -CODE128
  while(br_init_pram_ack_chek() != true);
  while(se655_send_data(Param_Code39, Data_Enable));          //BARCODE ENABLE -CODE39
   while(br_init_pram_ack_chek() != true);
  while(se655_send_data(Param_Interleaved2of5, Data_Enable)); //BARCODE ENABLE -Interleaved2of5
  while(br_init_pram_ack_chek() != true);
  while(se655_send_data(Param_Codabar, Data_Enable));         //BARCODE ENABLE -Codabar
  while(br_init_pram_ack_chek() != true);
  br_state=BarStnby;
 err_tmout_en(FALSE);
 
}


uint8_t se655_decoder_check()
{
  if(HAL_GPIO_ReadPin(BAR_DECODE_GPIO_Port, BAR_DECODE_Pin) == 0) return 0; //READ NO 
  else return 1;                                                            //READ OK
}

uint8_t se655_ack_check(uint8_t *data)
{ 
  //ACK : 04D0 00 00 FF2C
  //NAK : 05D10000FF29 
//  if(data == ACK) return 1;   // PACKET OK 
//  else            return 0;   // PACKET ERROR 
  return 0;
}

bool br_pram_ack_cmp = false;

/* USART2 init function */
uint8_t barcode_rx_data_buffer[200] ={0,};
byte br_chr=0;
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = BARCODE_BAUDRATE;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);
  
  //HAL_UART_Receive_IT(&huart2, barcode_rx_data_buffer, 1);  
  HAL_UART_Receive_IT(&huart2, &br_chr, 1);  
}

HAL_StatusTypeDef UART2_ReInit(void)
{
    HAL_StatusTypeDef status;

    status = HAL_UART_DeInit(&huart2);
    if (status != HAL_OK)
    {
        return status;
    }

    status = HAL_UART_Init(&huart2);
    if (status != HAL_OK)
    {
        return status;
    }

    HAL_UART_Receive_IT(&huart2, &br_chr, 1);
    return HAL_OK;
}

HAL_StatusTypeDef barcode_data_send(unsigned char *data, unsigned char data_length)
{
  return HAL_UART_Transmit(&huart2, data, data_length, 100);  
}


void UART2_Barcode_RxCpltCallback(void)           //time out �ʿ�
{      
  HAL_UART_Receive_IT(&huart2, &br_chr, 1);
  if(br_state==BarInit||br_state==BarInitOper)
    br_init_rs232_read(br_chr);
  else
    br_rs232_read(br_chr);
}

byte init_index=0;
void br_init_rs232_read(byte br_chr)
{
  br_state=BarInitOper;
  init_index++;
  barcode_rx_data_buffer[init_index]=br_chr;
//  if(init_index==5)
  br_read_reflash();
}


byte index=0;
int32_t bar_data_checksum=0;
void br_rs232_read(byte br_chr)
{
  br_state=BarOper;
  if(!index)
    HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_SET);
  index++;
  barcode_rx_data_buffer[index]=br_chr;
  bar_data_checksum+=br_chr;
  br_read_reflash();
}

uint read_time=READ_TMOUT;
void br_read_time()
{
  if(br_state==BarOper||br_state==BarInitOper){
    read_time--;
    if(!read_time){
      read_time=READ_TMOUT;
      //index=0;
      if(br_state==BarInitOper){
       br_pram_ack_cmp=true;
      }else{
        give_event(eventBarReadComp,0);
        br_state=BarReady;
      }
    }
  }
}

bool br_init_pram_ack_chek()
{
  byte i=0;
  unsigned char ack_data[7] = {0x06, 0x04, 0xD0, 0x00, 0x00, 0xFF, 0x2C};
  if(br_pram_ack_cmp){
    
      barcode_rx_data_buffer[0]=init_index;
      init_index=0;
      for(i=0;i<7;i++)
      {
        if(barcode_rx_data_buffer[i] != ack_data[i]) return false;//error  or nak        
      }
      br_pram_ack_cmp=false;
      return true;
     
  }
  return false;
}

void br_read_reflash()
{
  read_time=READ_TMOUT;
}


void UART_Barcode_RxCpltCallback(UART_HandleTypeDef *huart)
{      

  if(huart == &huart2)
  UART2_Barcode_RxCpltCallback();
   
}       
void barcode_triger()
{
    HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_SET);
    br_state=BarReady;
    set_timer_(eventBarTri,10,0);
}
void barcode_off()
{
  // HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_SET);
   br_state=BarStnby;
}

bool bar_indx_oper_flg= false;
bool barcode_flag = false;
bool bar_index_num_flag = false;

 event execute_barcode_ctrl(event event)
{
   byte dev_send_buf[4]={0,}; 
  switch(event)
  {
  case eventBarTri:
    if( br_state==BarReady)
      HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_RESET); 
    //else if(br_state==BarReadComp){
    //  HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_RESET); 
    //  br_state=BarReady;
    //}
    break;
  case eventBarReadComp:
    //index+=1;
    //barcode_rx_data_buffer[index]='\n';
    memset(dev_send_buf,0,sizeof(dev_send_buf));

    if(bar_index_num_flag == false)
    {  
      bar_pm.spl_num--;
      dev_send_buf[1]=bar_pr.sample_num-bar_pm.spl_num;
    }  
    
    else if(bar_index_num_flag == true)
    {
      dev_send_buf[1]=bar_pm.spl_num;
      bar_index_num_flag = false; 
    }

    dev_send_buf[3]=index;

    usb_send_pack(hseBarDataStart, dev_send_buf);
    send_test(&barcode_rx_data_buffer[1], index);
    memset(dev_send_buf,0,sizeof(dev_send_buf));
    sort_8bit(bar_data_checksum,dev_send_buf);
    usb_send_pack(hseBarDataEnd, dev_send_buf);
    bar_data_checksum=0;
    index=0;
    memset(barcode_rx_data_buffer,0,sizeof(barcode_rx_data_buffer));
      
    beep(100, 1);
    br_state=BarReadComp;
   // set_timer_(eventBarTri,800,0);
    break;
  case eventBarPr:
    //usb_send_pack(eventBarPr,0);
    give_event(bar_pr.event[0],0);
   // dev_send_buf[1]= bar_pm.spl_num;
   // usb_send_pack(hseBarDataStart, dev_send_buf);
   //  give_event(smpl_pr.event[0],0);
    break;
  case hseBarPage:
        state=stStEng;
    usb_send_pack(hseBarPage, 0);
    break;
  case hseBarRead:
    barcode_flag = true;
    bar_data_checksum=0;
    bar_pr.sample_num=bar_pm.spl_num=usb_data_buf[3];
    sort_8bit(bar_pm.spl_num,dev_send_buf);
    usb_send_pack(hseBarRead, dev_send_buf);
    give_event(eventBarPr,0);
    break;
    
  case hseBarFirstPosSet:
    bar_pm.start_pos=merge_32bit( bar_pm.start_pos,usb_data_buf);
    bar_param_write();
    stmt_abs_move(ADDR_MOTOR_X,bar_pm.start_pos);
    usb_send_pack(hseBarFirstPosSet, dev_send_buf);
    HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_RESET);
    set_timer_(eventQcTriger,3000,0);
    break;
  case hseBarFirstPosSave:
    bar_pm.start_pos=merge_32bit( bar_pm.start_pos,usb_data_buf);
    bar_param_write();
    bar_param_read();
    sort_8bit(bar_pm.start_pos,dev_send_buf);
    usb_send_pack(hseBarFirstPosSave, dev_send_buf);
    break;    
  case hseBarFirstPosRead:
    bar_param_read();
    sort_8bit(bar_pm.start_pos,dev_send_buf);
    usb_send_pack(hseBarFirstPosRead, dev_send_buf);
    break;
  case hseBarLastPosSet:
    bar_pm.end_pos=merge_32bit( bar_pm.end_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X,bar_pm.end_pos);
    bar_param_write();
    usb_send_pack(hseBarLastPosSet, dev_send_buf);
    HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_RESET);
    set_timer_(eventQcTriger,5000,0);
    break;
  case hseBarLastPosSave:
    bar_pm.end_pos=merge_32bit( bar_pm.end_pos,usb_data_buf);
    bar_param_write();
    bar_param_read();
    sort_8bit(bar_pm.end_pos,dev_send_buf);
    usb_send_pack(hseBarLastPosSave, dev_send_buf);
    break;    
  case hseBarLastPosRead:
    bar_param_read();
    sort_8bit(bar_pm.end_pos,dev_send_buf);
    usb_send_pack(hseBarLastPosRead, dev_send_buf);
    break;
  case hseBarDataStart:
    break;
  case hseBarDataEnd:
    break;
  case eventQcTriger:
    HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_SET); 
    break;
  case hseBarRacGapSet:
    bar_pm.start_pos2=merge_32bit( bar_pm.start_pos2,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X,bar_pm.start_pos2);
    bar_param_write();
    usb_send_pack(hseBarRacGapSet, dev_send_buf);
    HAL_GPIO_WritePin(BAR_TRIG_GPIO_Port, BAR_TRIG_Pin, GPIO_PIN_RESET);
    set_timer_(eventQcTriger,5000,0);
    break;
  case hseBarRacGapSave:
    bar_pm.start_pos2=merge_32bit( bar_pm.start_pos2,usb_data_buf);
    bar_param_write();
    bar_param_read();
    sort_8bit(bar_pm.start_pos2,dev_send_buf);
    usb_send_pack(hseBarRacGapSave, dev_send_buf);
    break;    
  case hseBarRacGapRead:
    bar_param_read();
    sort_8bit(bar_pm.start_pos2,dev_send_buf);
    usb_send_pack(hseBarRacGapRead, dev_send_buf);
    break;
  case hseQrTrayPosSet:
    bar_pm.qr_tray_pos=merge_32bit( bar_pm.qr_tray_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X,  bar_pm.qr_tray_pos);
    bar_param_write();
    usb_send_pack(hseQrTrayPosSet, usb_data_buf);
    break;
  case hseQrTrayPosSave:
    bar_pm.qr_tray_pos=merge_32bit( bar_pm.qr_tray_pos,usb_data_buf);
    bar_param_write();
    bar_param_read();
    sort_8bit(bar_pm.qr_tray_pos,dev_send_buf);
    usb_send_pack(hseQrTrayPosSave, dev_send_buf);
    break;    
  case hseQrTrayPosRead:
    bar_param_read();
    sort_8bit(bar_pm.qr_tray_pos,dev_send_buf);
    usb_send_pack(hseQrTrayPosRead, dev_send_buf);
    break;
  case hseQrDataStart:
    stmt_abs_move(ADDR_MOTOR_X,  bar_pm.qr_tray_pos);
    usb_send_pack(hseQrDataStart, dev_send_buf);
    break;
  case hseBarNumbRead:
    bar_data_checksum=0;
    bar_pr.sample_num=bar_pm.spl_num=1;
    bar_pm.spl_num=usb_data_buf[3];
    bar_indx_oper_flg=true;
    bar_index_num_flag = true;
    usb_send_pack(hseBarNumbRead, dev_send_buf);
    give_event(eventBarPr,0);
    break;
  case hseQrLEDSet:
     cam_led_set(usb_data_buf);
     usb_send_pack(hseQrTrayPosSet, usb_data_buf);
    break;
  default: break;
  }
  return event;
}
