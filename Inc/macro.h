#ifndef MACROS_H
#define MACROS_H

#define FW_VER "1.0.08"
#define HW_VER "0.0.01"
#define DEV_SN 1
#define FW_YEAR "24"
#define FW_DATE "1212"

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
