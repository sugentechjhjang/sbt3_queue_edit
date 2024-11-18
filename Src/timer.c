#include "main.h"
#include "string.h"

TIM_HandleTypeDef hWORK_TIM_Handle;


#define TIEMR0_TIK 500
#define TIMER1_TIK 100
#define TIMER0_1S_TIK 1
#define COM_TIME_OUT 3

#define LLD_TIME_OUT 35
#define SY_TIME_OUT  10

ulong timer=TIEMR0_TIK;//timer_freq;
ulong timer1=TIMER1_TIK;
byte ad_t_cnt = TIMER0_1S_TIK;

uint16_t timer_ticks=0;
byte com_timeout_cnt=COM_TIME_OUT;
byte lld_timeout_cnt=LLD_TIME_OUT;
byte sy_timeout_cnt=SY_TIME_OUT;


struct tmchan channel[50];
static const byte channels=50;

void tmchan_init()
{
  memset(channel, 0, sizeof(channel));
}

void  timer_tick_init()
{
  timer_ticks=0;

}

void timer_init()
{
  timer=TIEMR0_TIK;
}

void timer_mk_int()
{

/*100ms timer irpt*/
    if(timer1) 
      --timer1;
    else
      {
        give_event(eventTimer100ms,0);
        timer1=TIMER1_TIK;
        } 
     
}

byte com_tm_out_enable=0;
byte lld_tm_out_enable=0;
byte sy_tm_out_enable=0;

void timer_eve()
{
   if(timer)
   { 
      --timer;
   }
   else
   {
      give_event(eventTimer500ms,0);
      //timer=2000;//timer_freq; 
      timer=TIEMR0_TIK;//timer_freq;

      if(ad_t_cnt)
        --ad_t_cnt;
      else
      {
        give_event(eventTimer1s,0);
        ad_t_cnt = TIMER0_1S_TIK;
        err_tmout_cnt();

        if(com_tm_out_enable)
        {
          if (com_timeout_cnt > 0) 
          {
              com_timeout_cnt--;
          }
        }                           
        if(lld_tm_out_enable)
        {
          if (lld_timeout_cnt > 0) 
          {
              lld_timeout_cnt--;
          }
        }                           
        if(sy_tm_out_enable)
        {
          if (sy_timeout_cnt > 0) 
          {
              sy_timeout_cnt--;
          }
        }                            
      } 
  }
  timer_ticks++;	
}


void tm_execute()
  {
  byte i=0;
    if(timer_ticks)
      {
	  
        for(i=0; i<channels; i++)
          if(channel[i].event)
            {
              if(channel[i].delay<=timer_ticks) // the time
                {
                  if(channel[i].event_func)  // if have func
                    (*channel[i].event_func)(channel[i].event);
                  else  // if not have func
                    give_event(channel[i].event,0);
                   channel[i].event=0;
                }
              else
                channel[i].delay-=timer_ticks;
            }
		
        timer_ticks=0;
      }
  }


void tm_add(uint16_t evnt, uint time, void(*event_func)(uint16_t evnt))
  {
  	uint16_t i=0;
    for(i=0; i<channels; i++)
      if(!channel[i].event)
        {
          channel[i].delay=time;
          channel[i].event_func=event_func=0;
          channel[i].event=evnt;
          return;
        }
    error(errTMTimeOut,0);
  }

void tm_init()
{
  memset(channel,0,sizeof(channel));
}
void execute_timer()
  {
    tm_execute();
  }

void set_timer_(uint16_t evnt, uint time, void(*event_func)(uint16_t evnt))
  {
    tm_add(evnt, time, event_func);

  }
uint16_t pause_eve=0;
uint pause_time=0;
void set_timer_pause(uint16_t evnt, uint time, void(*event_func)(uint16_t evnt))
  {
    pause_eve=evnt;
    pause_time=time;
    beep(1000, 3);
    usb_send_pack(eventPauseRes,0);

  }


void HAL_SYSTICK_Callback(void)
{

  timer_mk_int();
  timer_eve();
  br_read_time();
 // THERMISTOR1_INT();

// HAL_GPIO_WritePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin,GPIO_PIN_SET);
// HAL_GPIO_WritePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin,GPIO_PIN_RESET);
 // HAL_GPIO_TogglePin(M_ASP_DSP_GPIO_Port,M_ASP_DSP_Pin);
}

void MX_TIM2_Init()
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  //TIM2
  hWORK_TIM_Handle.Instance = TIM2;
  hWORK_TIM_Handle.Init.Prescaler = 72;
  hWORK_TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  hWORK_TIM_Handle.Init.Period = 999;
  hWORK_TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  
  while(HAL_TIM_Base_Init(&hWORK_TIM_Handle) != HAL_OK);

  while(HAL_TIM_ConfigClockSource(&hWORK_TIM_Handle, &sClockSourceConfig) != HAL_OK);
    
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  while(HAL_TIMEx_MasterConfigSynchronization(&hWORK_TIM_Handle, &sMasterConfig) != HAL_OK);
    
  adc_ctrl();  
  HAL_TIM_Base_Start_IT(&hWORK_TIM_Handle);

}

void timer_intrrupt_stop()
{
  HAL_TIM_Base_Stop_IT(&hWORK_TIM_Handle);
}

void timer_intrrupt_start()
{
   HAL_TIM_Base_Start_IT(&hWORK_TIM_Handle);
}

void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&hWORK_TIM_Handle);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//1ms
{
   uint32_t deDevCheck;
   if(htim->Instance == TIM2)
   {
     THERMISTOR1_INT();
     beep_polling();
     deDevCheck = hsDevToPC_QueUseCheck();
     
     if(deDevCheck == RESET) //usb_rs232();
     {
       hsPC_To_DevRxPacket_Handle();
       
       hsDevToPC_ErrHandle();
       
       hsDevToPC_PacketSendHandle();
     }
     
     if(state!=stBoot)
     {
       water_level_sensor();
       door_sensor();
     }
    
   }
}

uint beep_tick_cnt=0;
byte beep_duty=0;
uint beep_cnt=0;
byte beep_en=0;
void beep(uint duty_ms, byte cnt)
{
  if(cnt){
    beep_duty=duty_ms;
    beep_cnt=cnt*2;
   // HAL_GPIO_WritePin(M_Buzzer_GPIO_Port, M_Buzzer_Pin, GPIO_PIN_SET);
  }
}

void beep_polling()
{
  if(beep_duty)
  beep_tick_cnt++;
  
    if(beep_tick_cnt>beep_duty){
      if(!beep_cnt)
      {
        beep_tick_cnt=0;
        beep_duty=0;
        HAL_GPIO_WritePin(M_Buzzer_GPIO_Port, M_Buzzer_Pin, GPIO_PIN_RESET);
        return;
      }else{
      beep_tick_cnt=0;
      beep_cnt--;
      }
      if(beep_en)
       HAL_GPIO_TogglePin(M_Buzzer_GPIO_Port, M_Buzzer_Pin);
      else
        HAL_GPIO_WritePin(M_Buzzer_GPIO_Port, M_Buzzer_Pin, GPIO_PIN_RESET);
    }
  
}

void com_time_out_set(byte value)
{
  com_tm_out_enable=value;
  com_timeout_cnt=COM_TIME_OUT;
  
  if(rollback_flag == false)
  {
    com_timeout_cnt=COM_TIME_OUT+(rbk_time/1000);
  }
}

byte call_com_timeout_cnt()
{
  return com_timeout_cnt;
}

void lld_time_out_set(byte value)
{
  lld_timeout_cnt=value;
}

byte lld_com_timeout_cnt()
{
  return lld_timeout_cnt;
}

void lld_repeat_en(byte value)
{
  lld_tm_out_enable=value;
  lld_timeout_cnt=LLD_TIME_OUT;  
}

byte sy_com_timeout_cnt()
{
  return sy_timeout_cnt;
}

void sy_repeat_en(byte value)
{
  sy_tm_out_enable=value;
  sy_timeout_cnt=SY_TIME_OUT;  
}
  