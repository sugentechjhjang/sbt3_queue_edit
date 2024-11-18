

#include "Robot.h"











unsigned char Robot_Axis_Command_Run(unsigned char motor_add, unsigned char commnad_list, unsigned char motor_bank, signed long int data_value);








//기본 Parameter 설정, 초기 Speed, 가속도, 위치등 설정
void Robot_Init(void)
{
  
  //X축 설정
  
  //Y축 설정
  Edb2000_Motor_Init(ADDR_MOTOR_X);
 // Edb2000_Motor_Init(ADDR_MOTOR_Y);
 // Edb2000_Motor_Init(ADDR_MOTOR_Z);
    
}

//Position 제어


unsigned char Robot_Conrtrol(unsigned char motor_no, unsigned char commnad_list, unsigned char motor_bank, signed long int* data_value)
{
  //motor_no : Bit 연산(1: X_Axis, 2: Y_Axis, 4: Z_Axis)
  //         : ex)0b0000 0101 -> X축과 Z축 동작
  static unsigned char status_flag[4] ={1, 1, 1, 1};
  unsigned char i=0;
  
  for(i=1;i<4;i++)
  {
    if(((motor_no >> (i-1)) & 0x01) && (status_flag[i] == 1))
    {
      if(i==3) status_flag[i] = Robot_Axis_Command_Run(i, commnad_list, motor_bank, data_value[4]);
      else status_flag[i] = Robot_Axis_Command_Run(i, commnad_list, motor_bank, data_value[i]);      
    }
    else status_flag[i] = 0;
  }  
  if(status_flag[i] > 1) return 2; //error
  if(status_flag[ADDR_MOTOR_X] | status_flag[ADDR_MOTOR_Y] | status_flag[ADDR_MOTOR_Z]) return 1;
  else
  {
    for(i=0;i<4;i++) status_flag[i] =1;
    return 0;
  }    
}




unsigned char Robot_Axis_Command_Run(unsigned char motor_add, unsigned char commnad_list, unsigned char motor_bank, signed long int data_value)
{
  static unsigned char status_flag[4] ={1,1,1,1};
  static unsigned char emergency_commnad_flag =0;
  
  if((commnad_list == EDB2000_STOP) && !emergency_commnad_flag)
  {
    status_flag[motor_add] = 1;
    emergency_commnad_flag = 1;
  }
  
  if(status_flag[motor_add] == 1)
  {    
    if((motor_add == ADDR_MOTOR_Z) && ((commnad_list == EDB2000_MOVE_ABS) || (commnad_list == EDB2000_MOVE_REL))) data_value *= -1;
    status_flag[motor_add] = EDB2000_Command_Send(commnad_list, motor_add, motor_bank, data_value); //완료를 확인 할 필요 없는 명령
    return 1;
  }
  else if(!status_flag[motor_add])
  {
    if(commnad_list == EDB2000_HOMING)
    {
      if(!EDB2000_Motor_Homing_Reached_Check(motor_add))
      {
        status_flag[motor_add] = 1;
        emergency_commnad_flag = 0;        
        return 0;
      }
      else return 1;
    }
    else if(commnad_list == EDB2000_MOVE_ABS)
    {
      if(EDB2000_Motor_Position_Reached_Check(motor_add))
      {
        status_flag[motor_add] = 1;
        emergency_commnad_flag = 0;
        return 0;
      }
      else return 1;
    }
    else
    {
      status_flag[motor_add] = 1;
      emergency_commnad_flag = 0;
      return 0;
    }
  }
  else return status_flag[motor_add];        //error
}  




