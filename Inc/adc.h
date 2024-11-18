#ifndef ADC_H
#define ADC_H

#define BEEP_EN_MASK 0x0001 
#define HANT_PAD_OFF  HAL_GPIO_WritePin(FAN_EX1_GPIO_Port,FAN_EX1_Pin,GPIO_PIN_SET)
#define HANT_PAD_ON   HAL_GPIO_WritePin(FAN_EX1_GPIO_Port,FAN_EX1_Pin,GPIO_PIN_RESET)


void MX_ADC3_Init(void);
void adc_ctrl();
void THERMISTOR1_INT (); 

extern ADC_HandleTypeDef hTHERM_ADC_Handle;
extern ADC_ChannelConfTypeDef sConfig;

extern byte serve_amp;
extern bool heat_pad_en;
uint Air_temp_res_vol(uint volt);


#pragma pack(push,1)
 __packed  struct temper
 {
  int32_t air_volt;
  int32_t tray_volt;
 };
#pragma pack(pop)
 
extern struct temper tmp;

#endif
