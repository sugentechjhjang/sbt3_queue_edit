#ifndef MACROS_H
#define MACROS_H


//#define Motor_EDB  
#define Motor_LEAD

#define FW_VER "1.0.0.8"
#define HW_VER "0.0.0.1"
#define DEV_SN 1
#define FW_YEAR "26"
#define FW_DATE "0414"

#ifdef Motor_EDB
#define DEVELOPER_MAIN_VER "9.0.0.6"
#endif

#ifdef Motor_LEAD
#define DEVELOPER_MAIN_VER "8.0.0.6"
#endif

#define BEEP

#define ASP_NUM 2
#define ASP_NUM1 8

#define FAN 1
#define FAN_OFF 1
#define FAN_ON 0

#define HEATER 3

#define HEATER_OFF 0
#define HEATER_ON 1

#define EN 1
#define DISEN 0



typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned int uint;
typedef unsigned long ulong;

#define MONITOR(arg) { byte si=__save_interrupt(); __disable_interrupt(); arg; __restore_interrupt(si); }


#endif
