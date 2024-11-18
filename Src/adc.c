#include "main.h"
#include "string.h"

ADC_HandleTypeDef hadc3;
ADC_ChannelConfTypeDef sConfig;



ulong thermistor1_temp_average;
ulong thermistor1_temp_sum;
byte thermistor1_adc_temp_num;
uint thermistor1_temp_vol;

uint Air_temp_res=0;

byte serve_amp=0;
struct temper tmp;
bool heat_pad_en=false;
/* ADC1 init function */
void MX_ADC3_Init(void)
{

  //ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc3.Instance = ADC3;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc3);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADC_ConfigChannel(&hadc3, &sConfig);

}



void adc_ctrl()
{
	thermistor1_temp_average=0;
	thermistor1_temp_sum=0;
	thermistor1_adc_temp_num=100;
	thermistor1_temp_vol=0;	
	
}

void THERMISTOR1_INT() 
{

  HAL_ADC_Start(&hadc3);
  //HAL_ADC_Start(&hadc2);
	

	switch(sConfig.Channel)
	{

    case ADC_CHANNEL_2:
			if(thermistor1_adc_temp_num--)
			{
        HAL_ADC_PollForConversion(&hadc3,1000);
				thermistor1_temp_sum+=HAL_ADC_GetValue(&hadc3);;
			}else
			{
				thermistor1_temp_average=thermistor1_temp_sum/100;
				thermistor1_temp_sum=0;
				thermistor1_adc_temp_num=100;
				thermistor1_temp_vol=(uint)(thermistor1_temp_average*0.8057+125);//mV
                                //temp_res_vol();
                                //degree 24 : 2705
                                 //degree 25.5 : 2667
                                if(thermistor1_temp_vol<2755)
                                  heat_pad_en=false;
                                else 
                                  heat_pad_en=true;;
			}

		break;
		default:
        sConfig.Channel=ADC_CHANNEL_0;
		break;		
	}

	HAL_ADC_Stop(&hadc3);
        
}


uint Air_temp_res_vol(uint volt)
{
  float Air_input_volt=0;
  float Air_compos_res=0;
  float Air_res_1th=10000.0,Air_res_2th=3300.0;
  Air_input_volt=(float)(volt/10.0);
  
  Air_compos_res=(float)((thermistor1_temp_vol/1000.0)*Air_res_1th)/(Air_input_volt-(thermistor1_temp_vol/1000.0));
  Air_temp_res=(uint)((Air_compos_res*Air_res_2th)/(Air_res_2th-Air_compos_res));
  
  return Air_temp_res;
}