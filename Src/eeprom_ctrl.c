

#include "main.h"
#include "string.h"

#define MEM_ADD 0xA0
#define MEM_PAGE 64
#define MEM_START_ADD 0

#define XMT_MEM_START_ADD 0//100
#define XMT_MEM_SIZE_ADD 128//100

#define YMT_MEM_START_ADD XMT_MEM_START_ADD+XMT_MEM_SIZE_ADD//100
#define YMT_MEM_SIZE_ADD YMT_MEM_START_ADD+128//100

#define ZMT_MEM_START_ADD YMT_MEM_START_ADD+YMT_MEM_SIZE_ADD//100
#define ZMT_MEM_SIZE_ADD ZMT_MEM_START_ADD+128//100\

#define SHK_MEM_START_ADD ZMT_MEM_START_ADD+ZMT_MEM_SIZE_ADD//100
#define SHK_MEM_SIZE_ADD SHK_MEM_START_ADD+64//100

#define SEV0_MEM_START_ADD SHK_MEM_START_ADD+SHK_MEM_SIZE_ADD//100
#define SEVO_MEM_SIZE_ADD SEV0_MEM_START_ADD+64//100

#define PUMP_MEM_START_ADD SEV0_MEM_START_ADD+SEVO_MEM_SIZE_ADD//100
#define PUMP_MEM_SIZE_ADD PUMP_MEM_START_ADD+128//100

#define SYGE_MEM_START_ADD PUMP_MEM_START_ADD+PUMP_MEM_SIZE_ADD//100
#define SYGE_MEM_SIZE_ADD SYGE_MEM_START_ADD+64//100

#define BAR_MEM_START_ADD SYGE_MEM_START_ADD+SYGE_MEM_SIZE_ADD//100
#define BAR_MEM_SIZE_ADD BAR_MEM_START_ADD+64//100

#define HLLD_MEM_START_ADD BAR_MEM_START_ADD+BAR_MEM_SIZE_ADD//100
#define HLLD_MEM_SIZE_ADD HLLD_MEM_START_ADD+64//100

#define CAM_MEM_START_ADD HLLD_MEM_START_ADD+HLLD_MEM_SIZE_ADD//100
#define CAM_MEM_SIZE_ADD CAM_MEM_START_ADD+64//100


#define SQ_MEM_START_ADD 1280
#define SQ_MEM_SIZE_ADD SQ_MEM_START_ADD+2560//1536

#define SERVO2_MEM_START_ADD SQ_MEM_START_ADD+SQ_MEM_SIZE_ADD//100
#define SERVO2_SIZE_ADD SERVO2_MEM_START_ADD+64//100

#define TEMP_MEM_START_ADD SERVO2_MEM_START_ADD+SERVO2_SIZE_ADD//100
#define TEMP_SIZE_ADD TEMP_MEM_START_ADD+64//100

//#define PUMP_MEM_START_ADD SQ_MEM_START_ADD9+1280//100
//#define PP_MEM_START_ADD PUMP_MEM_START_ADD+556
#define APP_MEM_START_ADD  6400
#define APP_MEM_SIZE_ADD   APP_MEM_START_ADD+12800

#define SHK_MEM_START_ADD2 APP_MEM_START_ADD+APP_MEM_SIZE_ADD//100
#define SHK_MEM_SIZE_ADD2 SHK_MEM_START_ADD2+64//100

hsAT24xx_ConfigType xmtCfg;
hsAT24xx_ConfigType ymtCfg;
hsAT24xx_ConfigType zmtCfg;

hsAT24xx_ConfigType sakeCfg;
hsAT24xx_ConfigType servoCfg;
hsAT24xx_ConfigType pumpCfg;
hsAT24xx_ConfigType sygeCfg;
hsAT24xx_ConfigType barCfg;
hsAT24xx_ConfigType hlldCfg;
hsAT24xx_ConfigType camCfg;
hsAT24xx_ConfigType sqCfg;
hsAT24xx_ConfigType servoCfg2;
hsAT24xx_ConfigType tempCfg;
hsAT24xx_ConfigType appCfg;
hsAT24xx_ConfigType shakCfg2;

void eeprom_init()
{
  // void hsAT24xx_Config(hsAT24xx_ConfigType *p_tMemConfig, uint16_t wPageSize, uint16_t wDev_Address, uint16_t wMemStart_Adrress, uint16_t wMemAddress_Size, uint16_t wMemMaxSize)
  hsAT24xx_Config(&xmtCfg,MEM_PAGE,MEM_ADD, XMT_MEM_START_ADD,4,XMT_MEM_SIZE_ADD);
  hsAT24xx_Config(&ymtCfg,MEM_PAGE,MEM_ADD, YMT_MEM_START_ADD,4,YMT_MEM_SIZE_ADD);
  hsAT24xx_Config(&zmtCfg,MEM_PAGE,MEM_ADD, ZMT_MEM_START_ADD,4,ZMT_MEM_SIZE_ADD);
  hsAT24xx_Config(&sakeCfg,MEM_PAGE,MEM_ADD, SHK_MEM_START_ADD,4,SHK_MEM_SIZE_ADD);
  hsAT24xx_Config(&servoCfg,MEM_PAGE,MEM_ADD, SEV0_MEM_START_ADD,4,SEVO_MEM_SIZE_ADD);
  hsAT24xx_Config(&pumpCfg,MEM_PAGE,MEM_ADD, PUMP_MEM_START_ADD,4,PUMP_MEM_SIZE_ADD);
  hsAT24xx_Config(&sygeCfg,MEM_PAGE,MEM_ADD, SYGE_MEM_START_ADD,4,SYGE_MEM_SIZE_ADD);
  hsAT24xx_Config(&barCfg,MEM_PAGE,MEM_ADD, BAR_MEM_START_ADD,4,BAR_MEM_SIZE_ADD);
  hsAT24xx_Config(&hlldCfg,MEM_PAGE,MEM_ADD, HLLD_MEM_START_ADD,4,HLLD_MEM_SIZE_ADD);
  hsAT24xx_Config(&camCfg,MEM_PAGE,MEM_ADD, CAM_MEM_START_ADD,4,CAM_MEM_SIZE_ADD);
  hsAT24xx_Config(&sqCfg,MEM_PAGE,MEM_ADD, SQ_MEM_START_ADD,4,SQ_MEM_SIZE_ADD);
 // sAT24xx_Config(&sqCfg,MEM_PAGE,MEM_ADD, SQ_MEM_START_ADD,4,SQ_MEM_SIZE_ADD);
  hsAT24xx_Config(&servoCfg2,MEM_PAGE,MEM_ADD, SERVO2_MEM_START_ADD,4,SERVO2_SIZE_ADD);
  hsAT24xx_Config(&tempCfg,MEM_PAGE,MEM_ADD, TEMP_MEM_START_ADD,4,TEMP_SIZE_ADD);
  hsAT24xx_Config(&appCfg,MEM_PAGE,MEM_ADD, APP_MEM_START_ADD,4,APP_MEM_SIZE_ADD);
  hsAT24xx_Config(&shakCfg2,MEM_PAGE,MEM_ADD, SHK_MEM_START_ADD2,4,SHK_MEM_SIZE_ADD2);
}


void xmt_param_read(){
  hsAT24xx_Read(&xmtCfg, (uint8_t*)&xmt_ctrl, XMT_MEM_START_ADD, sizeof(xmt_ctrl));
}
void xmt_param_write(){
  hsAT24xx_Write(&xmtCfg, (uint8_t*)&xmt_ctrl, XMT_MEM_START_ADD,  sizeof(xmt_ctrl));
}
void ymt_param_read(){
   hsAT24xx_Read(&ymtCfg, (uint8_t*)&ymt_ctrl, YMT_MEM_START_ADD, sizeof(ymt_ctrl));
}
void ymt_param_write(){
  hsAT24xx_Write(&ymtCfg, (uint8_t*)&ymt_ctrl, YMT_MEM_START_ADD,  sizeof(ymt_ctrl));
}
void zmt_param_read(){
   hsAT24xx_Read(&zmtCfg, (uint8_t*)&zmt_ctrl, ZMT_MEM_START_ADD, sizeof(zmt_ctrl));
}
void zmt_param_write(){
  hsAT24xx_Write(&zmtCfg, (uint8_t*)&zmt_ctrl, ZMT_MEM_START_ADD,  sizeof(zmt_ctrl));
}
void shk_param_read(){
 //  hsAT24xx_Read(&sakeCfg, (uint8_t*)&shk_pram, SHK_MEM_START_ADD, sizeof(shk_pram));
  hsAT24xx_Read(&shakCfg2, (uint8_t*)&shk_pram, 0, sizeof(shk_pram));
}
void shk_param_write(){
  //hsAT24xx_Write(&sakeCfg, (uint8_t*)&shk_pram, SHK_MEM_START_ADD,  sizeof(shk_pram));
  hsAT24xx_Write(&shakCfg2, (uint8_t*)&shk_pram, 0, sizeof(shk_pram));
}
void servo_param_read(){
   hsAT24xx_Read(&servoCfg, (uint8_t*)&asp_mt, SEV0_MEM_START_ADD, sizeof(asp_mt));
}
void servo_param_write(){
  hsAT24xx_Write(&servoCfg, (uint8_t*)&asp_mt, SEV0_MEM_START_ADD,  sizeof(asp_mt));
}
void pump_param_read(){
   hsAT24xx_Read(&pumpCfg, (uint8_t*)&pm_pram, PUMP_MEM_START_ADD, sizeof(pm_pram));
}
void pump_param_write(){
  hsAT24xx_Write(&pumpCfg, (uint8_t*)&pm_pram, PUMP_MEM_START_ADD,  sizeof(pm_pram));
}
void syge_param_read(){
 hsAT24xx_Read(&sygeCfg, (uint8_t*)&syrg_pram, SYGE_MEM_START_ADD, sizeof(syrg_pram));
}
void syge_param_write(){
 hsAT24xx_Write(&sygeCfg, (uint8_t*)&syrg_pram, SYGE_MEM_START_ADD,  sizeof(syrg_pram));
}
void bar_param_read(){
 hsAT24xx_Read(&barCfg, (uint8_t*)&bar_pm, BAR_MEM_START_ADD, sizeof(bar_pm));
}
void bar_param_write(){
 hsAT24xx_Write(&barCfg, (uint8_t*)&bar_pm, BAR_MEM_START_ADD,  sizeof(bar_pm));
}

void hlld_param_read(){
 hsAT24xx_Read(&hlldCfg, (uint8_t*)&smp_pram, HLLD_MEM_START_ADD, sizeof(smp_pram));
}
void hlld_param_write(){
 hsAT24xx_Write(&hlldCfg, (uint8_t*)&smp_pram, HLLD_MEM_START_ADD,  sizeof(smp_pram));
}

void cam_param_read(){
 hsAT24xx_Read(&camCfg, (uint8_t*)&cam_pram, CAM_MEM_START_ADD, sizeof(cam_pram));
}
void cam_param_write(){
 hsAT24xx_Write(&camCfg, (uint8_t*)&cam_pram, CAM_MEM_START_ADD,  sizeof(cam_pram));
}
void sq_read(){
  hsAT24xx_Read(&sqCfg, (uint8_t*)&sq, 0, sizeof(sq));
 ///hsAT24xx_Read(&sqCfg, (uint8_t*)&sq, SQ_MEM_START_ADD, sizeof(sq));
}
void sq_write(){
  hsAT24xx_Write(&sqCfg, (uint8_t*)&sq, 0,  sizeof(sq));
 //hsAT24xx_Write(&sqCfg, (uint8_t*)&sq, SQ_MEM_START_ADD,  sizeof(sq));
}



void servo_param_read2(){
   hsAT24xx_Read(&servoCfg2, (uint8_t*)&asp_mt, SERVO2_MEM_START_ADD, sizeof(asp_mt));
}
void servo_param_write2(){
  hsAT24xx_Write(&servoCfg2, (uint8_t*)&asp_mt, SERVO2_MEM_START_ADD,  sizeof(asp_mt));
}

void temp_param_read(){
   hsAT24xx_Read(&tempCfg, (uint8_t*)&tmp, SERVO2_MEM_START_ADD, sizeof(tmp));
}
void temp_param_write(){
  hsAT24xx_Write(&tempCfg, (uint8_t*)&tmp, SERVO2_MEM_START_ADD,  sizeof(tmp));
}


void app_data_read(uint16_t addr, uint8_t* data){
   hsAT24xx_Read(&appCfg, data,(addr*4), 4);
}

void app_data_write(uint16_t addr, uint8_t* data ){
  hsAT24xx_Write(&appCfg, data,(addr*4),  4);
}