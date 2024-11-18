#include "main.h"

  
  uint16_t port_prev=0, chatter_cycles=0;
  bool button_changed=0;//,locked=0;
  uint16_t port=eventBathOk;
  #define CHATTER_CYCLES   30//30
  
void water_level_sensor()
{
   if(HAL_GPIO_ReadPin(LEVEL_1_GPIO_Port,LEVEL_1_Pin)||HAL_GPIO_ReadPin(LEVEL_2_GPIO_Port,LEVEL_2_Pin)) 
   port=eventBathFull;
   else 
   port=eventBathOk;

   if(port!=port_prev)
      {
        // long repeat or pushed
        port_prev=port;
        chatter_cycles=CHATTER_CYCLES;
        return;
      }
    else if(chatter_cycles)
      {
        chatter_cycles--;
        if(!chatter_cycles)
         button_changed=true;       
        return;
      }
    
     
    if(button_changed)
     {  
        give_event(port,0);
        button_changed=false;
        //sysOnTimer=0;
     }

}


  uint16_t door_port_prev=eventDoorClose, door_chatter_cycles=0;
  bool door_button_changed=0;//,locked=0;
  uint16_t door_port=eventDoorClose;

  
bool door_sen_flg=false;
void door_sensor()
{
  if(door_sen_flg)
   if(HAL_GPIO_ReadPin(DOOR_1_GPIO_Port,DOOR_1_Pin)&&HAL_GPIO_ReadPin(DOOR_2_GPIO_Port,DOOR_2_Pin)) 
    door_port=eventDoorClose;
   else 
    door_port=eventDoorOpen;

   if(door_port!=door_port_prev)
      {
        // long repeat or pushed
        door_port_prev=door_port;
        door_chatter_cycles=CHATTER_CYCLES;
        return;
      }
    else if(door_chatter_cycles)
      {
        door_chatter_cycles--;
        if(!door_chatter_cycles)
         door_button_changed=true;       
        return;
      }
    
     
    if(door_button_changed)
     {  
        give_event(door_port,0);
        door_button_changed=false;
        //sysOnTimer=0;
     }

}

  