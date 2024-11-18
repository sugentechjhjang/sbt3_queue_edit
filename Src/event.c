#include "main.h"

#include<string.h>



struct
{
  uint16_t head;
  uint16_t tail;
  uint16_t queue[EVENT_QUEUE_LENGTH];
}events={0,0};

void init_event()
{
  memset(&events,0,sizeof(events));
  tm_init();
}

void give_event(uint16_t event, byte nomore)
{
  if(events.tail==65535)  // if it's end 
  {
    events.tail=events.head;
  }
  if(nomore)  // no more needed
  {
    uint16_t ev=events.head;
    for(ev; ev!=events.tail; )
    {
      if(events.queue[ev]==event)  // return;
        return;
      if(++ev==EVENT_QUEUE_LENGTH)  // end of Queue
        ev=0;
    }
  }

  events.queue[events.tail++]=event;  // input the event
  
  
  if(events.tail==EVENT_QUEUE_LENGTH) // event tail 
    events.tail=0;
  if(events.tail==events.head) 
  {
    events.head=events.tail;
    events.tail=65535;
  }
  
}


void save_pause(uint16_t* qu,uint16_t* p_head,uint16_t* p_tail)
{
  *p_head=events.head;
  *p_tail=events.tail;
  memcpy(qu,events.queue,sizeof(events.queue));
}


void get_pause(uint16_t* qu,uint16_t p_head,uint16_t p_tail)
{
  events.head=p_head;
  events.tail=p_tail;
  memcpy(events.queue,qu,sizeof(events.queue));
}
/*
byte del_event(byte event)
{
if(events.tail==255)
return 0;
for(byte ev=events.head; ev!=events.tail; )
{
if(events.queue[ev]==event)  // there is event
{
events.queue[ev]=0;  // delete
return 1;
          }
if(++ev==EVENT_QUEUE_LENGTH)  // e reset
ev=0;
      }
return 0;
  }
*/

uint16_t get_event()
{
  uint16_t tmp;
  if(events.tail==65535)
  {
    events.tail=events.head;
    // limo_ctrl.error_handling(ERR_QUEUEFULL);	XYZ	
    return 0;
  }
 
    if(events.head==events.tail) 
      return 0;

    tmp=events.queue[events.head++];
    if(events.head==EVENT_QUEUE_LENGTH)
      events.head=0;
    
    return tmp;

}


