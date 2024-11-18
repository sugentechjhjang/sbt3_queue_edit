
#include "main.h"

SPI_HandleTypeDef cam_led;
SPI_HandleTypeDef *hspi_cam;
struct camera_pram cam_pram;


int32_t cam_strip_pcak_cnt=0;
int32_t cam_strip_pcak_gap=0;
void cam_led_spi_init()
{
  cam_led.Instance = SPI1;
  cam_led.Init.Mode = SPI_MODE_MASTER;
  cam_led.Init.Direction = SPI_DIRECTION_2LINES;
  cam_led.Init.DataSize = SPI_DATASIZE_8BIT;
  cam_led.Init.CLKPolarity = SPI_POLARITY_HIGH;
  cam_led.Init.CLKPhase = SPI_PHASE_2EDGE;
  cam_led.Init.NSS = SPI_NSS_SOFT;
  cam_led.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  cam_led.Init.FirstBit = SPI_FIRSTBIT_MSB;
  cam_led.Init.TIMode = SPI_TIMODE_DISABLED;
  cam_led.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  cam_led.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&cam_led);
  cam_led_handle(&cam_led); 
}

void cam_mem_init()
{
  cam_param_read();
 /* if(shk_pram.speed_rpm==0||shk_pram.speed_rpm==(~0)){
    shk_pram.hoiz_pos=7000;
    shk_pram.dasp_pos=10000;
    shk_pram.aly_pos=8000;
    shk_pram.dry_pos=6000;
    shk_pram.speed_rpm=500;
    shk_pram.accel_rpm=600;
    shk_pram.run_time_s=5;
    shk_pram.up_ang_pos=15000;
    shk_pram.down_ang_pos=-15000;
    
    shk_param_write();
  }*/
}


void camera_LED_off()
{
  HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_SET);
  //HAL_GPIO_WritePin(LED_CL_GPIO_Port, LED_CL_Pin, GPIO_PIN_SET);
  dwHsSW_Delay_ms(10);
  led_write_Byte(0xC000);
  //HAL_GPIO_WritePin(LED_CL_GPIO_Port, LED_CL_Pin, GPIO_PIN_RESET);
}


void cam_led_on()
{
  //seq_status = 4;
  HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_CL_GPIO_Port, LED_CL_Pin, GPIO_PIN_SET);
    dwHsSW_Delay_ms(10);
  led_write_Byte(0x4000);
  //led_write_Byte(0x00);
  
  //led_write_Byte(0x300f|cam_pram.led_vol[0]);
   led_write_Byte(0x3000|cam_pram.led_vol[0]);
  //led_write_Byte(0x18);//(LED1_quant);
  
   // led_write_Byte(0x701e|cam_pram.led_vol[0]);
   // led_write_Byte(0xb01e|cam_pram.led_vol[0]);
   // led_write_Byte(0xf01e|cam_pram.led_vol[0]);
      led_write_Byte(0x7000|cam_pram.led_vol[1]);
      led_write_Byte(0xb000|cam_pram.led_vol[2]);
      led_write_Byte(0xf000|cam_pram.led_vol[3]);

  //led_write_Byte(0x1e);//(LED2_quant);
  //HAL_GPIO_WritePin(LED_CL_GPIO_Port, LED_CL_Pin, GPIO_PIN_RESET);

}


void cam_led_set(uint8_t* led_vol_set)
{

  //seq_status = 4;
  HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_CL_GPIO_Port, LED_CL_Pin, GPIO_PIN_SET);
    dwHsSW_Delay_ms(10);
  led_write_Byte(0x4000);
  //led_write_Byte(0x00);
  
  //led_write_Byte(0x300f|cam_pram.led_vol[0]);
   led_write_Byte(0x3000|*led_vol_set);
  //led_write_Byte(0x18);//(LED1_quant);
  
   // led_write_Byte(0x701e|cam_pram.led_vol[0]);
   // led_write_Byte(0xb01e|cam_pram.led_vol[0]);
   // led_write_Byte(0xf01e|cam_pram.led_vol[0]);
      led_write_Byte(0x7000|*(led_vol_set+1));
      led_write_Byte(0xb000|*(led_vol_set+2));
      led_write_Byte(0xf000|*(led_vol_set+3));

  //led_write_Byte(0x1e);//(LED2_quant);
  //HAL_GPIO_WritePin(LED_CL_GPIO_Port, LED_CL_Pin, GPIO_PIN_RESET);

}

void cam_led_off()
{
  //seq_status = 4;
  HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_CL_GPIO_Port, LED_CL_Pin, GPIO_PIN_SET);
    dwHsSW_Delay_ms(10);
  led_write_Byte(0x4000);
  //led_write_Byte(0x00);
  
  led_write_Byte(0x3000);
  //led_write_Byte(0x18);//(LED1_quant);
  
  led_write_Byte(0x7000);
  led_write_Byte(0xb000);
  led_write_Byte(0xf000);
  //led_write_Byte(0x1e);//(LED2_quant);
  //HAL_GPIO_WritePin(LED_CL_GPIO_Port, LED_CL_Pin, GPIO_PIN_RESET);

}


void cam_led_handle(SPI_HandleTypeDef *h_spi) 
{
  hspi_cam = h_spi;
}

void cam_led_delay(__IO uint32_t nCount)
{
  for(; nCount!= 0;nCount--);
}


uint16_t led_write_Byte(uint16_t byte)
{
  uint8_t tmp =0;
 // uint16_t rec_tmp=0;
  uint8_t buf[2];
  //uint8_t msb=0,lsb=0;
  buf[0]=byte>>8;
  buf[1]=byte;
  /* nSS signal activation - low */
  HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_RESET);
  /* SPI byte send */
  
  tmp = HAL_SPI_Transmit(hspi_cam, &buf[0], 1, 500);
  tmp = HAL_SPI_Transmit(hspi_cam, &buf[1], 1, 500);
  //rec_tmp=tmp<<8;
  //tmp = HAL_SPI_Transmit(hspi_cam, &buf[1], 1, 100);
  //rec_tmp|=tmp;
  if(tmp) tmp++;
  // if(rec_tmp) rec_tmp++;
  // rec_tmp
  //while(HAL_SPI_Transmit(hspi, &byte, 1, 100));
  /* Wait for SPIx Busy flag */
  while(HAL_SPI_GetState(hspi_cam) != HAL_SPI_STATE_READY);
  
  /* nSS signal deactivation - high */
   HAL_GPIO_WritePin(LED_CS_GPIO_Port, LED_CS_Pin, GPIO_PIN_SET);
  //  tmp = HAL_SPI_Receive(hspi_cam, &msb, 1, 100);
  //   if(tmp) tmp++;
  //while(HAL_SPI_Receive(hspi, &byte, 1, 100));
  return byte;
  
}

int32_t cam_strip_wd=0;
event execute_cam_ctrl(event event)
{
  byte dev_send_buf[4]={0,};
  switch(event)
  {
  case hseCamLedPramWrite:
    cam_pram.led_vol[0]=usb_data_buf[0];
    cam_pram.led_vol[1]=usb_data_buf[1];
    cam_pram.led_vol[2]=usb_data_buf[2];
    cam_pram.led_vol[3]=usb_data_buf[3];
    cam_param_write();
    
    usb_send_pack(hseCamLedPramWriteResp, usb_data_buf);  
    break;
  case hseCamLedPramRead:
    cam_param_read();
    dev_send_buf[0]=cam_pram.led_vol[0];
    dev_send_buf[1]=cam_pram.led_vol[1];
    dev_send_buf[2]=cam_pram.led_vol[2];
    dev_send_buf[3]=cam_pram.led_vol[3];
    usb_send_pack(hseCamLedPramReadResp, dev_send_buf);  
    break;
  case hseCamLedOnOff:
    if(usb_data_buf[3]==0xff)cam_led_on();
    else cam_led_off();
    usb_send_pack(hseCamLedOnOffResp, dev_send_buf);  
    break;
  case hseCamAnlyXPosSet:
    cam_pram.cam_aly_x_pos=merge_32bit(cam_pram.cam_aly_x_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X, cam_pram.cam_aly_x_pos);
    cam_param_write();
    usb_send_pack(hseCamAnlyXEndPosSetResp,usb_data_buf);
    break;
  case hseCamAnlyXPosSave:
    cam_pram.cam_aly_x_pos=merge_32bit(cam_pram.cam_aly_x_pos,usb_data_buf);
    cam_param_write();
    usb_send_pack(hseCamAnlyXPosSave,usb_data_buf);
    break;    
  case hseCamAnlyXPosRead:
    cam_param_read();
    sort_8bit(cam_pram.cam_aly_x_pos,dev_send_buf);
    usb_send_pack(hseCamAnlyXPosReadResp, dev_send_buf);
    break;
  case hseCamAnlyXEndPosSet:
    cam_pram.cam_aly_xend_pos=merge_32bit(cam_pram.cam_aly_xend_pos,usb_data_buf);
    stmt_abs_move(ADDR_MOTOR_X, cam_pram.cam_aly_xend_pos);
    cam_param_write();
    usb_send_pack(hseCamAnlyXEndPosReadResp,usb_data_buf);
    break;
  case hseCamAnlyXEndPosSave:
    cam_pram.cam_aly_xend_pos=merge_32bit(cam_pram.cam_aly_xend_pos,usb_data_buf);
    cam_param_write();
    usb_send_pack(hseCamAnlyXEndPosSave,usb_data_buf);
    break;    
  case hseCamAnlyXEndPosRead:
    cam_param_read();
    sort_8bit(cam_pram.cam_aly_xend_pos,dev_send_buf);
    usb_send_pack(hseCamAnlyXEndPosReadResp, dev_send_buf);
    break;
  case hseCamStripGo:
    cam_strip_pcak_cnt=(usb_data_buf[0]-1)/12;
  //  cam_strip_pcak_gap=xmt_ctrl.cam_pos-xmt_ctrl.dsp_pos_end;//xmt_ctrl.dsp_pos;
    cam_strip_pcak_gap=xmt_ctrl.cam_pos-cam_pram.cam_aly_xend_pos;//xmt_ctrl.dsp_pos;
    cam_strip_wd=(cam_pram.cam_aly_xend_pos-cam_pram.cam_aly_x_pos)/(STRIP_NUM-1);
    stmt_abs_move(ADDR_MOTOR_X,cam_pram.cam_aly_x_pos+(cam_strip_wd*(usb_data_buf[0]-1))+((cam_strip_pcak_gap-cam_strip_wd)*cam_strip_pcak_cnt));
   // (cam_strip_pcak_gap*cam_strip_pcak_cnt)+((cam_strip_wd*(usb_data_buf[0]-1)-(12*cam_strip_pcak_cnt))));
    //stmt_abs_move(ADDR_MOTOR_X, ((usb_data_buf[0]-1)*cam_strip_wd)+cam_pram.cam_aly_x_pos);
   // stmt_abs_move(ADDR_MOTOR_X, ((usb_data_buf[0]-1)*cam_strip_wd)+cam_pram.cam_aly_x_pos);
    usb_send_pack(hseCamStripGoResp, usb_data_buf);
    dSPIN_Go_To( shk_pram.aly_pos);
    break;
  case hseCamStripWaitGo:
    cam_strip_pcak_cnt=(usb_data_buf[0]-1)/12;
    cam_strip_pcak_gap=xmt_ctrl.cam_pos-cam_pram.cam_aly_xend_pos;//xmt_ctrl.dsp_pos;
    //cam_strip_pcak_gap=xmt_ctrl.cam_pos-xmt_ctrl.dsp_pos_end;//xmt_ctrl.dsp_pos;
    cam_strip_wd=(cam_pram.cam_aly_xend_pos-cam_pram.cam_aly_x_pos)/(STRIP_NUM-1);
    stmt_abs_move(ADDR_MOTOR_X,cam_pram.cam_aly_x_pos+(cam_strip_wd*(usb_data_buf[0]-1))+((cam_strip_pcak_gap-cam_strip_wd)*cam_strip_pcak_cnt));
    //stmt_abs_move(ADDR_MOTOR_X, ((usb_data_buf[0]-1)*cam_strip_wd)+cam_pram.cam_aly_x_pos);
    set_timer_(hseCamStripWaitGoResp,100,0);
    break;
  case hseCamStripWaitGoResp:
      if(x_reach_pos)
      usb_send_pack(hseCamStripWaitGoResp, 0);
      else
      set_timer_(hseCamStripWaitGoResp,100,0);
    break;
  /*      hseCamLedPramWriteResp,
          hseCamLedPramRead,
          hseCamLedPramReadResp,
          hseCamLedOnOff,
          hseCamLedOnOffResp,
          hseCamAnlyXPosSet,
          hseCamAnlyXPosRead,
          hseCamAnlyXEndPosSet,
          hseCamAnlyXEndPosRead,
    */
  }
  return event;
}
