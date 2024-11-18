#ifndef __EDB2000_STEPMOTOR_H
#define __EDB2000_STEPMOTOR_H


#include "stm32f1xx_hal.h"
#include "macro.h"


typedef enum edb2000_motor_address
{
  ADDR_MOTOR_X =0x0B,
  ADDR_MOTOR_Y =0x0C,
  ADDR_MOTOR_Z =0x0D 
}edb2000_motor_address;


typedef enum edb2000_command_list
{
  EDB2000_NONE = 0,

      EDB2000_STOP,

      EDB2000_MOVE_ABS,
      EDB2000_MOVE_REL,  

      EDB2000_ACTUAL_POSITION, 
  EDB2000_INIT_START,
      EDB2000_MAX_SPEED,
      EDB2000_ACCELERATOR,
      EDB2000_DECELERATE,
      EDB2000_RFS_MODDE,  
      EDB2000_RFS_SPEED,        // 10
      EDB2000_RFS_SWITCH_SPEED,  
      EDB2000_SET_CURRENT,
      EDB2000_SET_STANBY_CURRENT,
      EDB2000_ENCODER_RESOLUTION,      
    //---------- Closed Loop Parameter ---------//  
      EDB2000_GAMMA_VMIN,
      EDB2000_GAMMA_VMAX,
      EDB2000_CL_MAX_GAMMA,
      
      EDB2000_CL_BETA,
      EDB2000_CL_CURRENT_MIN,
      EDB2000_CL_CURRENT_MAX,   // 20
      EDB2000_CL_START_UP_CURRENT_SCALE,
      EDB2000_CL_CORRECTION_TOLERANCE,
      EDB2000_CL_POSITION_WINDOW,
      EDB2000_CL_UPSCALE_DELAY,
      EDB2000_CL_DOWNSCALE_DELAY,
      EDB2000_CL_CORRECTION_VELOCITY_P,
      EDB2000_CL_CORRECTION_VELOCITY_I,
      EDB2000_CL_CORRECTION_VELOCITY_I_CLIPPING,
      EDB2000_CL_CORRECTION_VELOCITY_DV_CLOCK,
      EDB2000_CL_CORRECTION_VELOCITY_DV_CLIPPING,       // 30
      EDB2000_CL_CORRECTION_POSITION_P,
      EDB2000_CL_CHOPPER_BLANK_TIME,
      EDB2000_CL_CORRECTION_RELATIVE_POSITION_OPTION,
      EDB2000_CL_TURN_ON_CLOSED_LOOP,
      //EDB2000_CL_GET_CLLOOP_CHECK,
    //---------- Closed Loop Parameter ---------//            

      EDB2000_SET_AUTO_MODE,
      EDB2000_SET_BUADRATE,
      EDB2000_SET_SYSTEM_ERROR,
  EDB2000_INIT_END,
      EDB2000_CL_GET_CLLOOP_CHECK,
      
      EDB2000_POSITION_REACHED,         //40
      EDB2000_LEFT_SWITCH_STATUS,

      EDB2000_HOMING,
      EDB2000_RFS_STATUS,  

  
  EDB2000_COMMAND_LIST_NO
}edb2000_command_list;


typedef struct
{
  GPIO_TypeDef  *port;
  uint16_t      pin;
}GPIO_PIN_BIND;

typedef enum 
{
  No_ERROR        = 0x00,
  TX_HAL_ERROR    = 0x01,
  TX_HAL_BUSY     = 0x02,
  TX_HAL_TIMEOUT  = 0x03,
  RX_HAL_ERROR    = 0X04,
  RX_HAL_BUSY     = 0x05,
  RX_HAL_TIMEOUT  = 0x06,
  CHECKSUM_ERROR  = 0x07,
  DEVICE_ERROR    = 0x08
} COM_StatusTypeDef;


#define LEAD_RUNNING_STATE 0x1003 //bit 0=Faulty, bit 1=Enable, bit 2=Running,  bit 4=Command completed, bit 5=Path completed, bit 6=Homing completed

#define LEAD_READ 0x03
#define LEAD_WRITE 0x06

#define LEAD_MOVE_ABS 0x6200

#define LEAD_POSITION_H_ABS 0x6201
#define LEAD_POSITION_L_ABS 0x6202
#define LEAD_SPEED_ABS 0x6203
#define LEAD_ACC_ABS 0x6204
#define LEAD_DEC_ABS 0x6205
#define LEAD_HOME_OFFSET_H 0x600D
#define LEAD_HOME_OFFSET_L 0x600E

#define Servo_Address 0x0145
      #define Servo_On  0x88
      #define Servo_Off 0x08

#define LEAD_PATH_0_SET 0x6207
      #define PATH_0 0x10     

#define LEAD_TRIGGER_REG 0x6002
      #define LEAD_HOMING 0x0020
      #define LEAD_E_STOP 0x0040





#define EDB2000_INST    0
#define EDB2000_TYPE    1


#define INST_STGP 0
#define INST_ROR  1
#define INST_ROL  2
#define INST_STOP 3
      #define EDB2000_STOP_TYPE                         0
#define INST_MVP  4
      #define EDB2000_MOVE_ABS_TYPE                     0       //absolute 
      #define EDB2000_MOVE_REL_TYPE                     1       //relative
#define INST_SAP  5
      #define EDB2000_ACTUAL_POSITION_TYPE              1        
      #define EDB2000_MAX_SPEED_TYPE                    4         //Max positioning speed
      #define EDB2000_ACCELERATOR_TYPE                  5
      #define EDB2000_SET_CURRENT_TYPE                  6
      #define EDB2000_SET_STANBY_CURRENT_TYPE           7
      #define EDB2000_DECELERATE_TYPE                   17 

      #define EDB2000_GAMMA_VMIN_TYPE                   108
      #define EDB2000_GAMMA_VMAX_TYPE                   109
      #define EDB2000_CL_MAX_GAMMA_TYPE                 110
      #define EDB2000_CL_BETA_TYPE                      111
      #define EDB2000_CL_CURRENT_MIN_TYPE               113
      #define EDB2000_CL_CURRENT_MAX_TYPE               114
      #define EDB2000_CL_CORRECTION_VELOCITY_P_TYPE     115
      #define EDB2000_CL_CORRECTION_VELOCITY_I_TYPE     116
      #define EDB2000_CL_COR_VELOCITY_I_CLIPPING_TYPE   117
      #define EDB2000_CL_COR_VELOCITY_DV_TYPE           118
      #define EDB2000_CL_COR_VELOCITY_DV_CLIPPING_TYPE  119    
      #define EDB2000_CL_UPSCALE_DELAY_TYPE             120
      #define EDB2000_CL_DOWNSCALE_DELAY_TYPE           121
      #define EDB2000_CL_CORRECTION_POSITION_P_TYPE     124
      #define EDB2000_CL_CORRECTION_TOLERAANCE_TYPE     125
      #define EDB2000_CL_STARTUP_CURRENT_SCALE_TYPE     126
      #define EDB2000_CL_RELATIVE_POSITION_TYPE         127
      #define EDB2000_CL_TURN_ON_CLOSED_LOOP_TYPE       129
      #define EDB2000_CL_CLOSED_LOOP_CHECK_TYPE         133
      #define EDB2000_CL_POSITION_WINDOW_TYPE           134
      #define EDB2000_CL_CHOPPER_BLANK_TIME_TYPE        162

      #define EDB2000_RFS_MODE_TYPE                     193       //reference search mode
      #define EDB2000_RFS_SPEED_TYPE                    194       //reference search speed
      #define EDB2000_RFS_SWITCH_SPEED_TYPE             195       //reference switch speed
      #define EDB2000_ENCODER_RESOLUTION_TYPE           210       //reference switch speed
#define INST_GAP  6
      #define EDB2000_POSITION_REACHED_TYPE             8
      #define EDB2000_LEFT_SWITCH_STATUS_TYPE           11
#define INST_SGP  9
      #define EDB2000_AUTO_MODE_TYPE                    77
      #define EDB2000_BAUDRATE_TYPE                     65
      #define EDB2000_SYSTEM_ERROR_MASK_TYPE            72      
#define INST_GGP  10
#define INST_RFS 13
      #define EDB2000_HOMING_TYPE                       0
      #define EDB2000_RFS_STATUS_TYPE                   2
#define INST_WAIT 27
#define INST_SCO  30
#define INST_GCO  31
#define INST_SETTING 88
#define INST_RUNAPP 129
      #define EDB2000_FROME_PC                          0
      #define EDB2000_FROME_SPECIL_ADD                  1
#define INST_RESET 131

#define INST_CLE 36


#define EDB2000_SUCCESSFULLY            100
#define EDB2000_COMMAND LOAD            101
#define EDB2000_WRONG_CHECKSUM          1
#define EDB2000_INVALID_COMMAND         2
#define EDB2000_WRONG_TYPE              3
#define EDB2000_INVALID_VALUE           4
#define EDB2000_CONFIGURATION_EEPROM    5
#define EDB2000_COMMAND_NOT_AVAILABLE   6



#define MOTOR_SEND_MAX_RETRY            5
#define MOTOR_485_DIRECTION_DELAY       50


void Edb2000_Motor_Init(unsigned char motor_add);


unsigned char EDB2000_Command_Send(unsigned char commnad_list, unsigned char address, unsigned char bank, long int value);

unsigned char EDB2000_Motor_Rx_Check(unsigned char motor_add_temp);
unsigned char EDB2000_Motor_Position_Reached_Check(unsigned char motor_add_temp);
unsigned char EDB2000_Motor_Homing_Reached_Check(unsigned char motor_add_temp);


void edbmt_cmd_send(byte address, byte inst, byte type ,byte bank, signed long int value);
void edbmt_cmd_send_no_error(byte address, byte inst, byte type ,byte bank, signed long int value);


void EDB2000_Parameter_Define(void);


#endif                  //__EDB2000_MOTOR
