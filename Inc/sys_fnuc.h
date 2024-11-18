
#ifndef _SYS_FNUC_H
#define _SYS_FNUC_H

#define PS_MAX_EVENT 40

#pragma pack(push,1)
 struct barcode
 {
  enum cntrl_event event[PS_MAX_EVENT];
  int32_t sample_num;  // number//  byte maxNum[3];  // number
  uint32_t sample_wd;
  int8_t  scan_cnt;
 } ;
#pragma pack(pop)
extern struct barcode bar_pr;

#pragma pack(push,1)
 struct prime 
 {
  enum cntrl_event event[PS_MAX_EVENT];
  uint disp_pump_num;
  uint disp_vol;
  uint asp_vol;
 } ;
#pragma pack(pop)
extern struct prime prime;
  
#pragma pack(push,1)
 struct disp_single 
 {
  enum cntrl_event event[PS_MAX_EVENT];
  uint disp_pump_num;
  uint disp_vol;
  uint disp_ypin_wide;
  uint disp_xpin_wide;
  int xaixs_start_pos;
  int yaixs_start_pos;
  byte total_strip;
 } ;
#pragma pack(pop)
extern struct disp_single disp_sgl;


#pragma pack(push,1)
 struct disp_asp 
 {
  enum cntrl_event event[PS_MAX_EVENT];
  uint pump_num;
  uint vol;
  uint asp_time;
  uint ypin_wide;
  uint xpin_wide;
  int xaixs_start_pos;
  int yaixs_start_pos;
  byte total_strip;
 } ;
#pragma pack(pop)
extern struct disp_asp diasp;

#pragma pack(push,1)
 struct aspration  
 {
  enum cntrl_event event[PS_MAX_EVENT];
  uint time;
  byte total_strip;
 };
#pragma pack(pop)
extern struct aspration asp;
  

#pragma pack(push,1)
 struct sample_pr
 {
  enum cntrl_event event[PS_MAX_EVENT];
  int32_t sample_num;  // number//  byte maxNum[3];  // number
  uint32_t sample_wd;
  int8_t  sensing_cnt;
  uint16_t clld_init_vol;
  uint32_t clld_asp_pos;
 } ;
#pragma pack(pop)
extern struct sample_pr smpl_pr;


#pragma pack(push,1)
 struct sample_prime
 {
  enum cntrl_event event[PS_MAX_EVENT];
  int32_t pm_num;  // number//  byte maxNum[3];  // number
  int32_t pm_time;
  int32_t syg_down_pos;
  int8_t syg_prime_cnt;
  int32_t prov_pos;
  int32_t air_asp_pos;
 } ;
#pragma pack(pop)
extern struct sample_prime smpl_prime;


#pragma pack(push,1)
struct probe_disp 
 {
  enum cntrl_event event[PS_MAX_EVENT];
  int32_t vol;  // number//  byte maxNum[3];  // number
  int32_t total_strip;
 } ;
#pragma pack(pop)

extern struct probe_disp probe_disp;

#pragma pack(push,1)
struct drys
 {
  int32_t time;  // number//  byte maxNum[3];  // number
 } ;
#pragma pack(pop)
extern struct drys dry;

#pragma pack(push,1)
struct auto_clean
 {
  enum cntrl_event event[PS_MAX_EVENT];
  int16_t repeat;
  int32_t disp_time;  //disp
  int32_t asp_time;  //disp
  uint32_t rootin_delay;
 } ;
#pragma pack(pop)
extern struct auto_clean auto_cl;

extern bool only_washing_flg;
extern bool auto_clean_repeat_flg;
extern bool probe_disp_enable;
extern bool auto_prime_flg;
#endif   

event execute_sys_ctrl(event event);