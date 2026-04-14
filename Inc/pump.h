
#ifndef _PUMP_H
#define _PUMP_H
extern UART_HandleTypeDef huart5;



#define WS_PUMP1                 7
#define WS_PUMP2                 8

#define WS_PUMP                  9
#define BATH_ASP1                10 
#define BATH_ASP2                11

#define BATH_DW_ON               HAL_GPIO_WritePin(ACC_VALVE_WASTE_GPIO_Port,ACC_VALVE_WASTE_Pin,GPIO_PIN_RESET)
#define BATH_DW_OFF              HAL_GPIO_WritePin(ACC_VALVE_WASTE_GPIO_Port,ACC_VALVE_WASTE_Pin,GPIO_PIN_SET)

#define PRIME_DW_ON              12//HAL_GPIO_WritePin(ACC_PUMP_WASTE_GPIO_Port,ACC_PUMP_WASTE_Pin,GPIO_PIN_RESET) //pump ch 15
#define PRIME_DW_OFF             HAL_GPIO_WritePin(ACC_PUMP_WASTE_GPIO_Port,ACC_PUMP_WASTE_Pin,GPIO_PIN_SET)

#define PRIME_DW                 15//12

#define RD_PUMP1                 1
#define RD_PUMP2                 2
#define RD_PUMP3                 3
#define RD_PUMP4                 4
#define RD_PUMP5                 5
#define RD_PUMP6                 6

#define ASP_PUMP_ON              HAL_GPIO_WritePin(ACC_PUMP_PRIME_GPIO_Port,ACC_PUMP_PRIME_Pin,GPIO_PIN_RESET)
#define ASP_PUMP_OFF             HAL_GPIO_WritePin(ACC_PUMP_PRIME_GPIO_Port,ACC_PUMP_PRIME_Pin,GPIO_PIN_SET)
enum pump_com_state
{
  PmNone,
  PmStanby,
  PmBusy,
  PmStanby_op,
};

#pragma pack(push,1)
struct pump_param 
{
  uint vol[15];
  uint use_tm[15];
} ;
#pragma pack(pop)

extern struct pump_param pm_pram;


void MX_USART5_UART_Init(void);
void UART5_Pump_RxCpltCallback(void);
void UART_Pump_LLD_RxCpltCallback(UART_HandleTypeDef *huart);
void New_Pump_Data_Trans_s(unsigned short int *pump_go);
void New_Pump_Run(unsigned short int pump_channel, unsigned long int pump_run_time);
void New_Pump_RollBack(unsigned short int pump_channel, unsigned long int pump_run_time);
void New_Pump_Reset(void);
void give_pm_buf_data(byte buf, byte nomore);
void pump_pack_aly(byte pm_chr);
void pump_mem_init();

HAL_StatusTypeDef UART5_ReInit(void);

extern volatile bool SUB_FW_DOWN_MODE;
extern volatile bool SubToPC;
extern volatile bool isAckReceiving;
extern byte uart5_chr;

#define ACK_BUF_SIZE 9

extern uint8_t ACKBuf[ACK_BUF_SIZE];
extern uint8_t ack_idx;
extern uint16_t SUB_TOTAL_LENGTH;

event execute_pump_ctrl(event event);

#endif   