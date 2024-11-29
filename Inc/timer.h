#ifndef TIMER_H
#define TIMER_H

void timer_mk_int();
void tm_execute();
void tmchan_init();
void timer_eve();
void tm_init();
//void tm_execute();
//void tm_add(byte evnt, uint time, void(*event_func)(byte evnt));

struct tmchan 
{
    uint16_t event;
    unsigned int delay;
    void (*event_func)(uint16_t evnt);
};

// tmchan 구조체 배열 선언
extern struct tmchan channel[50];



void set_timer_(uint16_t evnt, uint time, void(*event_func)(uint16_t evnt));
void set_timer_pause(uint16_t evnt, uint time, void(*event_func)(uint16_t evnt));

#define set_timer(event, time, func) set_timer_(event, (time)*2,func);//*timer_freq/1000., func)  // ms
#define set_etimer(event, time) set_timer_(event, (time)*2);//*timer_freq/1000.)
void HAL_SYSTICK_Callback(void);

void MX_TIM2_Init();
void beep(uint duty_ms, byte cnt);
void beep_polling();
void timer_intrrupt_stop();
void timer_intrrupt_start();
void com_time_out_set(byte value);
byte call_com_timeout_cnt();
void lld_time_out_set(byte value);
byte lld_com_timeout_cnt();
void lld_repeat_en(byte value);
void sy_repeat_en(byte value);
byte sy_com_timeout_cnt();
extern uint16_t pause_eve;
extern uint pause_time;
extern byte beep_en;
#endif 
