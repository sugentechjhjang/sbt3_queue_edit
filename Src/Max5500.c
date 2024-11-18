//#include "Max5500.h"
//
//
//
//
//SPI_HandleTypeDef hspi1;
//
//void MX_SPI1_Init(void);
//
//unsigned char Max5500_Spi_Data_Send(unsigned char* max5500_data unsigned short int max5500_data_size)
//unsigned char Max5500_Spi_Data_Send(unsigned char* max5500_data unsigned short int max5500_data_size);
//unsigned char Max5500_Current_Change(unsigned char channel, unsigned short int current_value);
//unsigned char Max5500_Current_All_Off(void);
//
//
///* SPI1 init function */
//void MX_SPI1_Init(void)
//{
//
//  hspi1.Instance = SPI1;
//  hspi1.Init.Mode = SPI_MODE_MASTER;
//  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
//  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
//  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
//  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
//  hspi1.Init.NSS = SPI_NSS_SOFT;
//  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
//  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
//  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
//  hspi1.Init.CRCPolynomial = 10;
//  HAL_SPI_Init(&hspi1);  
//	//hspi1.Instance->CR1|=0x00000040;
//}
//
//
//
////typedef enum max5500_command_list
////{
////  NONE           =0,
////  CURRENT_CHANGE =1,
////  CURRENT_ON,
////  CURRENT_OFF,
////  CURRENT_ALL_OFF
////}max5500_command_list;
//
//unsigned char Max5500_Spi_Data_Send(unsigned char* max5500_data unsigned short int max5500_data_size)
//{
//}
//
//unsigned char Max5500_Current_Control(unsigned char command_list, unsigned char channel, unsigned short int current_value)
//{  
//  switch(command_list)
//  {
//  case CURRENT_CHANGE:    
//    break;
//  case CURRENT_ON:
//    break;
//  case CURRENT_OFF:
//    break;
//  case CURRENT_ALL_OFF:
//    break;
//  }
//}
//
//
//unsigned char Max5500_Current_Change(unsigned char channel, unsigned short int current_value)
//{
//}
//
//unsigned char Max5500_Current_All_Off(void)
//{
//}
//
//
////SPI_HandleTypeDef *max5500_hspi;
////GPIO_PIN_BIND max5500_cs_pin, max5500_cl_pin;
////uint8_t max5500_com_status;
////uint8_t Txdata[2];
////uint8_t Rxdata[2];
////
////void def_spi_handle(SPI_HandleTypeDef *hspi){max5500_hspi = hspi;}
////
////void def_cs_pin(GPIO_PIN_BIND pin_cs){max5500_cs_pin = pin_cs;}
////
////void set_cs(GPIO_PinState set){HAL_GPIO_WritePin(max5500_cs_pin.port, max5500_cs_pin.pin, set);}
////
/////*
////return val com_status
////0: No error
////1: TX HAL_Error
////2: TX HAL_Busy
////3: TX HAL_TIMEOUT
////4: RX HAL_Error
////5: RX HAL_Busy
////6: RX HAL_TIMEOUT
////*/
////uint8_t send_data(uint8_t *txdata, uint16_t txsize, uint8_t *rxdata, uint16_t rxsize)
////{
////  HAL_StatusTypeDef spi_status;
////  uint8_t cnt_tx_error = 0, cnt_rx_error = 0;
////  
////  do
////  {     
////    do
////    {
////      set_cs(GPIO_PIN_RESET);
////      spi_status = HAL_SPI_Transmit(max5500_hspi, txdata, txsize, 100);
////      set_cs(GPIO_PIN_SET);
////    }while(max5500_com_status = spi_status && cnt_tx_error++ < 10);
////    
////    if(cnt_tx_error) break;
////    
////    set_cs(GPIO_PIN_RESET);
////    spi_status = HAL_SPI_Receive(max5500_hspi, rxdata, rxsize, 100);
////    set_cs(GPIO_PIN_SET);
////    
////    if(spi_status)
////    {
////      max5500_com_status = spi_status + 3;
////      cnt_rx_error++;
////    }
////  }while(max5500_com_status = spi_status && cnt_tx_error++ < 10);
////  
////  return max5500_com_status;
////}
////
////void def_cl_pin(GPIO_PIN_BIND pin_cl){max5500_cl_pin = pin_cl;}
////
////void set_cl(GPIO_PinState set){HAL_GPIO_WritePin(max5500_cl_pin.port, max5500_cl_pin.pin, set);}
////
////uint8_t sw_shutdown(){return send_data((uint8_t*)"\xC0\x00", 2, Rxdata, 2);}
////
////uint8_t startup(){return send_data((uint8_t*)"\x40\x00", 2, Rxdata, 2);}
////
////uint8_t dac_update(uint8_t ch, uint16_t data)
////{
////  Txdata[0] = (ch << 6) | 0x30;
////  Txdata[0] |= (data >> 8) & 0x0F;
////  Txdata[1] = data & 0xFF;
////  
////  return send_data(Txdata, 2, Rxdata, 2);
////}
////
////uint8_t all_ch_update(uint16_t *data)
////{
////  uint8_t i;
////  
////  startup();
////  for(i=0;i<4;i++)
////  {
////    if(max5500_com_status = dac_update(i, data[i]))
////      break;
////  }
////  
////  return max5500_com_status;
////}
//
//
//
//
//
//
