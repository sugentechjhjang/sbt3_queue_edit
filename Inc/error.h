#ifndef ERROR_H
#define ERROR_H

#define ATTR_PACKED

#define CMD_ERROR               1
#define DATA_ERROR              2
#define BUSY_ERROR              3
#define BUSY_INT_ERROR          4

enum ERROR
  {
    errOK,                 // 0   no alarm
    errBathFull,                // bath�� ���� ����
    errTMTimeOut,
    errRS232,             // ��� �ҷ�
    errBoot,              //����
    errSygTimeOut,
    errShaker,
    errBarcode,
    errXaxi,
    errYaxi,
    errZaxi,//10
    errMemory,
    errPump,
    errHlld,
    errWaterLevel,
    errClld,
   _errEnd,
    errMoveTimeoutXaxi,   //18번째
    errMoveTimeoutYaxi,
    errMoveTimeoutZaxi,
    errMoveTimeoutSyg,
    errMoveTimeoutShaker
  };



struct hspState
{
  uint16_t error;
}ATTR_PACKED;

bool error(enum ERROR err, uint8_t err_st);
uint16_t get_error();
void clear_error();
void send_error(uint16_t err);

void err_tmout_set(enum ERROR err);
void err_tmout_cnt_set(enum ERROR err,byte cnt);
void err_tmout_cnt();
void err_tmout_en (bool en);

void move_err_tmout_cnt_set(enum ERROR err,uint16_t cnt);
void move_err_tmout_cnt();
void move_err_tmout_en (bool en);

#endif

