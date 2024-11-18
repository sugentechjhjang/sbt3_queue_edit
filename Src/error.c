#include "main.h"

#include <string.h>



struct hspState error_state, prev_state;

bool error(enum ERROR err, uint8_t err_st)

{
    uint8_t error_st;
    if(state!=stErr)  // Not error state or err priority is higer 
    {
     //shutdown(); 
     //beep(0.5,1);
      error_state.error=err_st;
      //print("err %d\r\n",err);
      state=stErr;
      error_st=err;
      //send_lcd_sq_data(err, error_state.error);
      beep(10, 30);
      //usb_send_pack(eventError,&error_st);
      hsErr_PutQue(error_st);

      ASP_PUMP_OFF;
      BATH_DW_OFF;
      //head.send(hseState,sizeof(error_state),&error_state);
    }
    return 1;
}

uint16_t get_error()
{
 return error_state.error; 
}

void clear_error()
{
 error_state.error=0; 
}

void send_error(uint16_t err)
{
 error_state.error=err;
 //state=stErr;
// head.send(hseState,sizeof(error_state),&error_state);    
}

#define ERR_CNT 20
bool err_enable=FALSE;
byte err_cnt=ERR_CNT;
enum ERROR errs;

void err_tmout_en (bool en)
{
  err_enable=en;
}

void err_tmout_cnt()
{
  if(err_enable)
  err_cnt--;
  
  if(!err_cnt){
    error(errs,0);
    err_tmout_en(FALSE);
  }
    
}


void err_tmout_set(enum ERROR err)
{
  err_cnt=ERR_CNT;
  err_tmout_en(TRUE);
  errs=err;
}

void err_tmout_cnt_set(enum ERROR err,byte cnt)
{
  err_cnt=cnt;
  err_tmout_en(TRUE);
  errs=err;
}


//------------------------------------------------------------------------------
// 2024 03 11
#define hsERR_QUE_SZ            10

#define hsERR_QUE_OK            0
#define hsERR_QUE_FULL	        1
#define hsERR_QUE_EMPTY	        2


typedef struct
{
  uint32_t dwTail;
  uint32_t dwHead;
  uint8_t uQueBuf[hsERR_QUE_SZ];
}hsERR_QUE_t;


hsERR_QUE_t g_tErr_QueHandle;

int32_t hsErr_PutQue(uint8_t uPutData)
{
  int32_t dwTail;

  dwTail = (g_tErr_QueHandle.dwTail + 1)%hsERR_QUE_SZ;

  if( dwTail == g_tErr_QueHandle.dwHead) return hsERR_QUE_FULL;

  g_tErr_QueHandle.uQueBuf[g_tErr_QueHandle.dwTail] = uPutData;

  g_tErr_QueHandle.dwTail = dwTail;
          
  return hsERR_QUE_OK;
}

int32_t hsErr_GetQue(uint8_t *p_uGetData)
{
  if( g_tErr_QueHandle.dwTail == g_tErr_QueHandle.dwHead ) return hsERR_QUE_EMPTY;

  *p_uGetData = g_tErr_QueHandle.uQueBuf[g_tErr_QueHandle.dwHead];

  g_tErr_QueHandle.dwHead = (g_tErr_QueHandle.dwHead + 1)%hsERR_QUE_SZ;

  return hsERR_QUE_OK;
}

void hsErr_QueReset(void)
{
  __disable_irq();
  g_tErr_QueHandle.dwHead = 0;
  g_tErr_QueHandle.dwTail = 0;
  __enable_irq();
}