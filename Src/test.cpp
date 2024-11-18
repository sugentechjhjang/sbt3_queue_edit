
#include "main.h"

class adc_ctrl adc1;

uint adc_ctrl::adc_pd_average; // PD average  

adc_ctrl::adc_ctrl()
{
        adc_pd_average=0; // PD average  
        
}

void adc_ctrl::test()
{
     //set_timer_(eventReserve9,100,0);
  x_homeing();
}

byte kdm=0;
void adc_ttest()
{

  kdm=22;
}
