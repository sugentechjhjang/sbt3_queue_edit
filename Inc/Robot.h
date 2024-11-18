

#ifndef __ROBOT
#define __ROBOT


#include "EDB2000_STEPMOTOR.h"

typedef enum
{
  ROBOT_HOME_POSITION = 1,
  X_HOME_POSITION,
  Y_HOME_POSITION,
  Z_HOME_POSITION,
  BATH_POSITION,
  ASP_POSITION,
  DSP_POSITION,
  DRY_POSITION,
  BARCODE_POSITION,
  SAMPLE_POSITION  
}robot_position;



#define Robot_Control_X         1
#define Robot_Control_Y         2
#define Robot_Control_Z         4

void Robot_Init(void);
#if 0
unsigned char Robot_Conrtrol(unsigned char* motor_no, unsigned char commnad_list, unsigned char motor_bank, signed long int* data_value);
#else
unsigned char Robot_Conrtrol(unsigned char motor_no, unsigned char commnad_list, unsigned char motor_bank, signed long int* data_value);
#endif




#endif                  //__ROBOT