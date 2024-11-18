/* Includes ------------------------------------------------------------------*/

#include "main.h"

#define STEP_BAUDRATE                  115200

UART_HandleTypeDef huart4;

struct erea_rs_pack er_rs_pack;


#define RECEIVE_BUF_LENGTH 500
byte step_receive_bufs[RECEIVE_BUF_LENGTH];

byte mt_chr=0;
void MX_USART4_UART_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = STEP_BAUDRATE;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart4);
  
  HAL_UART_Receive_IT(&huart4, &mt_chr, 1); 
}

                               
static byte const NO_RECIVED=0xFF;
byte step_recived_rec_buf_ind=0xFF;


