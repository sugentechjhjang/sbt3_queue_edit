
#ifndef _CAM_LED_H
#define _CAM_LED_H
extern SPI_HandleTypeDef cam_led;
void cam_led_spi_init();
void cam_led_handle(SPI_HandleTypeDef *h_spi);
void cam_led_delay(__IO uint32_t nCount);
uint16_t led_write_Byte(uint16_t byte);
void cam_led_on();
void cam_led_off();
void camera_LED_off();
void cam_led_set(uint8_t* led_vol_set);

#pragma pack(push,1)
 __packed  struct camera_pram
 {

  int16_t led_vol[4];
  int32_t cam_aly_x_pos;
  int32_t cam_aly_xend_pos;
 };
#pragma pack(pop)
extern struct camera_pram cam_pram;
 
void cam_mem_init();
event execute_cam_ctrl(event event);
#endif   