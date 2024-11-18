
#ifndef __EEPROM_CTRL_H
#define __EEPROM_CTRL_H
void eeprom_init();
void xmt_param_read();
void xmt_param_write();
void ymt_param_read();
void ymt_param_write();
void zmt_param_read();
void zmt_param_write();
void shk_param_read();
void shk_param_write();
void servo_param_read();
void servo_param_write();
void pump_param_read();
void pump_param_write();
void syge_param_read();
void syge_param_write();
void bar_param_read();
void bar_param_write();
void hlld_param_read();
void hlld_param_write();
void cam_param_read();
void cam_param_write();
void sq_write();
void sq_read();
void servo_param_read2();
void servo_param_write2();
void temp_param_read();
void temp_param_write();
void app_data_write(uint16_t addr, uint8_t* data );
void app_data_read(uint16_t addr, uint8_t* data);
#endif   //__K_BLOT_UART