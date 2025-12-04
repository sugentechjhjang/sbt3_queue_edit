
#ifndef _HLLD_H
#define _HLLD_H

#define HLLD_ADD 0x03

#define HLLD_CLLD_RES  0x01
#define HLLD_PLLD_RUN_MINMAX  0x02
#define HLLD_PLLD_RUN 0x03
#define HLLD_CLLD_RUN  0x05
#define HLLD_CLLD_VOL  0x06
#define HLLD_PLLD_STREAM  0x07
#define HLLD_PLLD_GET_SENS  0x08
#define HLLD_PLLD_SLOP_CMD  0x09
#define HLLD_FW_DATE  0x0A
#define HLLD_PLLD_INDEX_SET 0x0B
#define HLLD_PLLD_RESULT  0x0C
#define HLLD_PLLD_SLOP_RESULT  0x0D
#define HLLD_CLLD_INIT  0x0E
#define HLLD_SYSTEM_RESET 0x0F
#define HLLD_DEVELOPER_VER_H 0x10
#define HLLD_DEVELOPER_VER_L 0x11
#define HLLD_CLLD_VALUE_SET 0x12

#define CLLD_STANBY          0 
#define CLLD_START           2
#define CLLD_FISRT_END       3
#define CLLD_SECOND_START    4
#define CLLD_SECOND_END      5

#define SWS_STANBY           0 
#define SWS_START            4
#define SWS_END              5



enum lld_Result
{
  JudeNone,
  CLLD_OK,
  CLLD_AIR,
  CLLD_Clot,
  Limit,
  Limit_AIR,
  Limit_Clot
};

enum lld_com_state
{
  lldNone,
  lldStanby,
  lldBusy,
  lldstanby_home,
};

enum lld_func
{
  lldFuncNone,
  lldFuncDispVol,
  lldFuncOnlyClean,
  lldFuncClld,
  lldFuncPlld,
  lldFullFunc,
};
extern enum lld_func lld_fc;

#pragma pack(push,1)
 __packed  struct smple_pram
 {
  uint r1;
  uint r2;
  uint sws;
  uint off_set;
  int16_t begin_index;
  int16_t end_index;
  int16_t min_std;
  int16_t max_std;
  int16_t slop_min_std;
  int16_t slop_max_std;
  int16_t clld_value_set;
 };
#pragma pack(pop)
 
extern struct smple_pram smp_pram;
extern uint16_t plld_index;

void UART5_Hlld_RxCpltCallback(byte chr);
void hlld_send_pack(unsigned char addr, unsigned char cmd, unsigned short int content, unsigned short int data);
void lld_pack_aly(byte lld_chr);
void hlld_rs232();
void  hlld_mem_init();
event execute_hlld_ctrl(event event);


extern bool plld_qc;
extern bool CLLD_VOL_Check;
#endif   
