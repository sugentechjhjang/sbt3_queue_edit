/**
  ******************************************************************************
  * @file    
  * @author  전자제어팀 
  * @version V1.0.0
  * @date    08/24/2011
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 


/* Includes ------------------------------------------------------------------*/
#include "UC1608x_Drive.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HS_GLCD_DEBUG

#define UC1608X_DELAY               5

#define UC1608X_SYN_DELAY           5
#define UC1608X_READ_DELAY          30

#define GLCDRAM_X_MAX               240
#define GLCDRAM_PAGE                16
#define GLCDRAM_MAX                 3840
#define GLCDRAM_BANK_MAX            960

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//uint32_t GLCDRamBank[GLCDRAM_BANK_MAX] = {0};
//uint8_t* GLCDRam[] = (uint8_t**)GLCDRamBank;
uint8_t GLCDRam[GLCDRAM_PAGE][GLCDRAM_X_MAX] = {0};

extern const uint8_t fontEng8x16[128][16];
extern const uint8_t fontEng5x7[][6];

extern const uint8_t  KMAC_Log[6][110]; 
/* Private function prototypes -----------------------------------------------*/
void vUC1608x_GPIO_Init(void);

void vUC1608xCTRL_GPIO_Set(void);
void vUC1608xDAT_GPIO_Read_Set(void);
void vUC1608xDAT_GPIO_Write_Set(void);

void vUC1608xDAT_Write(uint8_t Data);
uint32_t uUC1608xDAT_Read(void);

void vUC1608X_Init(void);

/* Private functions ---------------------------------------------------------*/
/* ****************************************************************************
 * Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vUC1608x_GPIO_Init(void)
{
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_UC1608X_IO, ENABLE);

  /* GPIO Pin 별 상태 설정 */  
  vUC1608xCTRL_GPIO_Set();   // 제어 IO 설정
  vUC1608xDAT_GPIO_Write_Set();
}
/* ****************************************************************************
 * Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vUC1608xCTRL_GPIO_Set(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin   = nUC1608X_CS|nUC1608X_RSTn|nUC1608X_CD|nUC1608X_WR|nUC1608X_RD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(UC1608X_IO, &GPIO_InitStructure);  
}

/* ****************************************************************************
 * Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vUC1608xDAT_GPIO_Read_Set(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 내부 pull up */
  GPIO_InitStructure.GPIO_Pin  = nUC1608X_D0_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(UC1608X_IO, &GPIO_InitStructure);     
}

/* ****************************************************************************
 * Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vUC1608xDAT_GPIO_Write_Set(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin   = nUC1608X_D0_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(UC1608X_IO, &GPIO_InitStructure);    
}

/* ****************************************************************************
 * Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vUC1608xDAT_Write(uint8_t Data)
{
  IO_UC1608X_WR = (IO_UC1608X_WR & 0xFF00) | Data;
}

/* ****************************************************************************
 * Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
uint32_t uUC1608xDAT_Read(void)
{
  return IO_UC1608X_RD;
}

/******************************************************************************
*         HW IO Setting End.
******************************************************************************/

/* ****************************************************************************
 * Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vUC1608xBusy_Check(void)
{
  uint32_t temReadState = 0;
    
  vUC1608xDAT_GPIO_Read_Set();
  
  UC1608X_CS(SET);
  UC1608X_CD(nUC1608X_CMD);
  UC1608X_READ();
  SYS_DELAY(UC1608X_DELAY);
  
  while(1)
  {
    temReadState = uUC1608xDAT_Read();
    if( !(temReadState & (1<<nBZ)) ) break;
  }
  
  UC1608X_RW_CL();
  UC1608X_CS(RESET);
}
/*
void wcomd(uchar c)  
{
   // CS1=0;RS=0;WWR=0;P1=c;RRD=1;RRD=0;delayms(2);CS1=1;
    CS1=1;RS=0;RRD=1;P1=c;WWR=0;delayms(2);WWR=1;CS1=0;
}
*/

/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vUC1608x_WCommand(uint8_t Cmd)
{
  vUC1608xDAT_GPIO_Write_Set();

  UC1608X_CS(SET);
  UC1608X_CD(nUC1608X_CMD);
  
  vUC1608xDAT_Write(Cmd);
  UC1608X_WRITE();
#ifdef UC1608X_SYN_DELAY
  SYS_DELAY(UC1608X_SYN_DELAY);
#else
  vUC1608xBusy_Check();
#endif
  
  UC1608X_RW_CL();
  UC1608X_CS(RESET); 
}

/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vUC1608x_WData(uint8_t Data)
{
  vUC1608xDAT_GPIO_Write_Set();
  
  UC1608X_CS(SET);
  UC1608X_CD(nUC1608X_DAT);
  
  vUC1608xDAT_Write(Data);
  UC1608X_WRITE();
#ifdef UC1608X_SYN_DELAY
  SYS_DELAY(UC1608X_SYN_DELAY);
#else
  vUC1608xBusy_Check();
#endif
  
  UC1608X_RW_CL();
  UC1608X_CS(RESET);   
}
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
uint8_t vUC1608x_RData(void)
{
  uint16_t tempData = 0;
  vUC1608xDAT_GPIO_Read_Set();
    
  UC1608X_CS(SET);
  UC1608X_CD(nUC1608X_DAT);
  
  UC1608X_READ();

#ifdef UC1608X_SYN_DELAY
  SYS_DELAY(UC1608X_READ_DELAY);
#else
  vUC1608xBusy_Check();
#endif

  tempData = uUC1608xDAT_Read();
  SYS_DELAY(UC1608X_DELAY);
  //SYS_DELAY(20);
  //tempData = uUC1608xDAT_Read();
  //SYS_DELAY(UC1608X_DELAY);
  
  UC1608X_RW_CL();
  UC1608X_CS(RESET); 
  return (uint8_t)tempData;
}

/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
uint8_t* vUC1608x_SetContrast(uint8_t conData)
{
  static uint8_t ConstrData = 0;
  uint8_t tempData  = 0;
  
  if( conData > 63) conData = 63;
  
  ConstrData = conData;
  
  tempData = (nUC1608X_G_PM_DATA & (~0x3F)) + ConstrData; //(~0x3F)  GN1 GN0 PM5 PM4 PM3 PM2 PM1 PM0 , GN1 과 GN0 외에 0으로 초기화 
  
  vUC1608x_WCommand(nUC1608X_G_PM);
  vUC1608x_WCommand(tempData);
  
  return &ConstrData;
}

/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vUC1608x_SetAdress(uint8_t x,uint8_t Page)
{
  uint8_t x_Low = 0, x_High = 0;

  x_High = (x >> 4) & 0x0F;
  x_Low = x & 0x0F;
  
  vUC1608x_WCommand(0xb0+Page);
  vUC1608x_WCommand(0x10 + x_High);
  vUC1608x_WCommand(x_Low);
}
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
uint8_t vUC1608x_GetAddressData(uint8_t x, uint8_t Page)
{
  uint8_t temp = 0;
  vUC1608x_SetAdress(x, Page);
  return temp;  
}

/******************************************************************************
*        UC1608X Control End
******************************************************************************/
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
uint8_t uGLCDGetAdressData(uint8_t x, uint8_t Page)
{
  return GLCDRam[Page][x];
}

/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vGlcdAreaClear(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  uint8_t temp = 0;
  uint8_t x, Page;
  
  if( (x1 >= 240) || (x2 >= 240) ) return;
  if( (y1 >= 128) || (y2 >= 128) ) return;
 
  if( x1 > x2 ) 
  {
    temp = x1; x1 = x2; x2 = temp;
  }
  
  if( y1 > y2)
  {
    temp = y1; y1 = y2 ; y2 = temp;
  }
  
  for( Page = y1 ; Page <= y2 ; Page++)
  {
    for( x = x1 ; x <= x2 ; x++)
    {
      vUC1608x_SetAdress(x, Page);
      vUC1608x_WData((uint8_t)RESET);
      GLCDRam[Page][x] = (uint8_t)RESET;
    }
  }
  
}
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vGlcdAllClear(void)
{
  vGlcdAreaClear(0, 0, GLCDRAM_X_MAX-1, GLCDRAM_PAGE-1);
}
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
//void vGlcdRamToDisplay(
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vGlcdPixel(uint8_t x, uint8_t y, uint8_t put)
{
  uint8_t dotPagePos = y%8;
  uint8_t dotPage = y/8;
  uint8_t temp = 0;
  
  //temp = vUC1608x_GetAddressData(x, dotPage);
  temp = GLCDRam[dotPage][x];
  
  vUC1608x_SetAdress(x, dotPage);
  
  if(put) 
  {
    vUC1608x_WData(temp | (1<<dotPagePos));
    
    GLCDRam[dotPage][x] = temp | (1<<dotPagePos);
  }
  else 
  {
    vUC1608x_WData(temp & ~(1<<dotPagePos));
    GLCDRam[dotPage][x] = temp & ~(1<<dotPagePos);
  }
}

/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vGlcdLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t put)
{
  //uint8_t x1 = yp1, y1 = xp1, x2 = yp2, y2 = xp2;
  uint32_t x, y; 
  uint32_t tmp;
  //uint8_t index=0;

  if( (x1 >= 240) || (x2 >= 240) ) return;
  if( (y1 >= 128) || (y2 >= 128) ) return;
  
  vGlcdPixel(x1, y1, put);
  if(x1>x2)
  {
    tmp=x1; x1=x2; x2=tmp; 
    tmp=y1; y1=y2; y2=tmp; 
    //index=SET;
  }
  
  if(y1==y2)
  {
    for(x=x1 ; x <= x2 ; x++) vGlcdPixel(x, y1, put);
  }
  
  if(x1==x2)
  {
      if(y1 > y2)
      {
        for( y = y2 ; y <= y1; y++) vGlcdPixel(x1, y, put);
      }
      if(y1 < y2)
      {
        for( y = y1 ; y <= y2 ; y++) vGlcdPixel(x1, y, put);
      }	
  }
  
  if( y1 < y2 )
  {
    if( (y2 - y1) < (x2 - x1) )
    {
      	for(x=0 ; x <= (x2-x1) ; x++)
        {
          y=((y2-y1)*x/(x2-x1))+y1; 
          
          vGlcdPixel(x+x1, y, put);
        } 
    }
    else
    {
      	for(y = 0; y <= (y2-y1) ; y++)
        {
          x=((x2-x1)*y/(y2-y1))+x1; 
          
          vGlcdPixel(x, y+y1, put);
        }
    }
  }
  if( y1 > y2 )
  {
    if( (y1-y2) < (x2-x1) )
    {
       	for(x=0; x <= (x2-x1) ;x++)
        {
          y= y1-((y1-y2)*x/(x2-x1)); 
          vGlcdPixel(x+x1, y, put);
        }	
    }
    else
    {
 	for( y=0 ; y <= (y1-y2) ; y++)
        {
          x=((x2-x1)*y/(y1-y2))+x1; 
          vGlcdPixel(x, y1-y, put);
        }
    }
  }
  //if(index){tmp=x0; x0=x1; x1=tmp; tmp=y0; y0=y1; y1=tmp;}
};
#if 0
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
// draw circle
void vGlcdCircle(uint8_t xcenter, uint8_t ycenter, uint8_t radius, uint8_t put)
{
  int32_t tswitch, y, x = 0;
  uint8_t d;

  d = ycenter - xcenter;
  y = radius;
  tswitch = 3 - 2 * radius;
  while (x <= y) 
  {
    vGlcdPixel(xcenter + x, ycenter + y, put);     vGlcdPixel(xcenter + x, ycenter - y, put);
    vGlcdPixel(xcenter - x, ycenter + y, put);     vGlcdPixel(xcenter - x, ycenter - y, put);
    vGlcdPixel(ycenter + y - d, ycenter + x, put); vGlcdPixel(ycenter + y - d, ycenter - x, put);
    vGlcdPixel(ycenter - y - d, ycenter + x, put); vGlcdPixel(ycenter - y - d, ycenter - x, put);

    if (tswitch < 0) tswitch += (4 * x + 6);
    else {
      tswitch += (4 * (x - y) + 10);
      y--;
    }
    x++;
  }
}
#endif

/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vRectangular(uint8_t x, uint8_t y, uint8_t width, uint8_t high, uint8_t put)
{
  uint8_t tmpCnt = 0;
  
  for(tmpCnt = 0; tmpCnt < width ; tmpCnt++ )
  {
     vGlcdPixel(x+tmpCnt, y, put);
     vGlcdPixel(x+tmpCnt, y+(high-1), put);
  }
  
  for(tmpCnt = 0; tmpCnt < high ; tmpCnt++ )
  {
     vGlcdPixel(x, y+tmpCnt, put);
     vGlcdPixel(x+(width-1), y+tmpCnt, put);
  }
}
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vRectangular_Full(uint8_t x, uint8_t y, uint8_t width, uint8_t high, uint8_t put)
{
  uint8_t tmpCnt_X, tmpCnt_Y;
  uint8_t data = 0;
  
  if(put) data = 0xFF;
    
  for(tmpCnt_Y =0; tmpCnt_Y < high; tmpCnt_Y++)
  {
    for(tmpCnt_X =0; tmpCnt_X < width; tmpCnt_X++)
    {
      vUC1608x_SetAdress(tmpCnt_X+x, tmpCnt_Y+y);
      vUC1608x_WData(data);
      GLCDRam[tmpCnt_Y+y][tmpCnt_X+x] = data;
    }
  }
    
}

#if 0
void vRectangular_Full(uint8_t x, uint8_t y, uint8_t width, uint8_t high, uint8_t put)
{
  uint8_t tmpCnt_X, tmpCnt_Y;
  
  for(tmpCnt_Y =0; tmpCnt_Y < high; tmpCnt_Y++)
  {
    for(tmpCnt_X =0; tmpCnt_X < width; tmpCnt_X++)
    {
      vGlcdPixel(x+tmpCnt_X, y+tmpCnt_Y, put);
    }
  }
    
}
#endif
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */ 
void GLCD_SRAM_ReWrite(void)
{
  ImageDisplayRam( 0, 0, 0, 0, 240, 16, GLCDRam[0]);
}

/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */ 
uint8_t ImageDisplay(uint8_t mode, uint8_t turn, uint8_t x, uint8_t Page, uint8_t x_IndexSize, uint8_t Page_IndexSize, const uint8_t *image)
{
  uint8_t xCnt = 0, pageCnt = 0;
  uint8_t tempRamData = 0;
  uint8_t tempData = 0;
  
 // xCnt = x; pageCnt = Page;
  for( pageCnt = 0 ; pageCnt < Page_IndexSize ; pageCnt ++)
  {
    for(xCnt = 0; xCnt < x_IndexSize ; xCnt++)
    {
      if(turn)tempData = 0xFF - *image;
      else tempData = *image;
      image++;
      
      tempRamData = GLCDRam[pageCnt+Page][xCnt+x];
      switch(mode)
      {
      case IMG_OVW: // 0
        tempRamData = tempData;
        break;
      case IMG_OR:  // 기존 이미지 유지, OR 연산.
        tempRamData = tempRamData|tempData;
        break;
      case IMG_Clr: // 이미지만 지움
        tempRamData = tempRamData & (~tempData);
        break;
      default:
        break;
      }

      vUC1608x_SetAdress(xCnt+x, pageCnt+Page);
      vUC1608x_WData(tempRamData);
      GLCDRam[pageCnt+Page][xCnt+x] = tempRamData;
    }
  }
 
  return (x_IndexSize+x);
}
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */ 
uint8_t ImageDisplayRam(uint8_t mode, uint8_t turn, uint8_t x, uint8_t Page, uint8_t x_IndexSize, uint8_t Page_IndexSize, uint8_t *image)
{
  uint8_t xCnt = 0, pageCnt = 0;
  uint8_t tempRamData = 0;
  uint8_t tempData = 0;
  
 // xCnt = x; pageCnt = Page;
  for( pageCnt = 0 ; pageCnt < Page_IndexSize ; pageCnt ++)
  {
    for(xCnt = 0; xCnt < x_IndexSize ; xCnt++)
    {
      if(turn)tempData = 0xFF - *image;
      else tempData = *image;
      image++;
      
      tempRamData = GLCDRam[pageCnt+Page][xCnt+x];
      switch(mode)
      {
      case IMG_OVW: // 0
        tempRamData = tempData;
        break;
      case IMG_OR:  // 기존 이미지 유지, OR 연산.
        tempRamData = tempRamData|tempData;
        break;
      case IMG_Clr: // 이미지만 지움
        tempRamData = tempRamData & (~tempData);
        break;
      default:
        break;
      }

      vUC1608x_SetAdress(xCnt+x, pageCnt+Page);
      vUC1608x_WData(tempRamData);
      GLCDRam[pageCnt+Page][xCnt+x] = tempRamData;
    }
  }
 
  return (x_IndexSize+x);
}

/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */ 
uint8_t uCharEng8x16(uint8_t mode, uint8_t turn, uint8_t x, uint8_t Page, uint8_t data)
{
  return ImageDisplay(mode, turn, x, Page, 8, 2, fontEng8x16[data]);
}
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */ 
uint8_t uCharEng5x7(uint8_t mode, uint8_t turn, uint8_t x, uint8_t Page, uint8_t data)
{
  return ImageDisplay(mode, turn, x, Page, 6, 1, fontEng5x7[data]);
}

/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */ 
uint8_t uGlcdEngString(uint8_t mode, uint8_t turn, uint8_t x, uint8_t Page, uint8_t Fontc, uint8_t *pStr)
{
  while(*pStr)
  {
    switch(Fontc)
    {
    case 8*16:
      if( x > 240 - 8)  return 0;
      x = uCharEng8x16(mode, turn, x, Page, *pStr);
      break;
    case 5*7:
      if( x > 240 - 6)  return 0;
      x = uCharEng5x7(mode, turn, x, Page, *pStr);
      break;
    default:
      break;
    }
    
    pStr++;
  }
  return x;
}


/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */ 
uint8_t uGlcdEngStringFlash(uint8_t mode, uint8_t turn, uint8_t x, uint8_t Page, uint8_t Fontc,const uint8_t *pStr)
{
  while(*pStr)
  {
    switch(Fontc)
    {
    case 8*16:
      if( x > 240 - 8)  return 0;
      x = uCharEng8x16(mode, turn, x, Page, *pStr);
      break;
    case 5*7:
      if( x > 240 - 5)  return 0;
      x = uCharEng5x7(mode, turn, x, Page, *pStr);
      break;
    default:
      break;
    }
    
    pStr++;
  }
  
  return x;
}


/******************************************************************************
*        GLCD Control End
******************************************************************************/
/* ****************************************************************************
* Name   : 
 * Input  : None
 * Output : None
 * Return : None
 *************************************************************************** */
void vUC1608X_Init(void)
{
  vUC1608x_GPIO_Init();
  
  UC1608X_RSTn (SET);
  SYS_DELAY(20000);
 
  vUC1608x_WCommand (nU1608X_RESET);
  SYS_DELAY(1000);
  
  vUC1608x_WCommand(nUC1608X_MR_TC);
  //SYS_DELAY(5);
  
  vUC1608x_WCommand(nUC1608X_PWCTRL);
  //SYS_DELAY(5);
  
  vUC1608x_WCommand(nUC1608X_START_LINE);
  //SYS_DELAY(5);
  
  vUC1608x_WCommand(nUC1608X_BIAS_RATIO);
  //SYS_DELAY(5);
  
  vUC1608x_WCommand(nUC1608X_G_PM);
  //SYS_DELAY(5);
  
  vUC1608x_WCommand(nUC1608X_G_PM_DATA);
  //SYS_DELAY(5);
  
  vUC1608x_WCommand(nUC1608X_RAM_ADD_CTRL);
  //SYS_DELAY(5);
  vUC1608x_WCommand(nUC1608X_MAP_CTRL);
  //SYS_DELAY(5);
  
  vUC1608x_WCommand(nUC1608X_PIXEL_ON);
  //SYS_DELAY(5);
  
  vUC1608x_WCommand(nUC1608X_INVERS_DISPLAY);
  //SYS_DELAY(5);
  
  vUC1608x_WCommand(nUC1608X_DISPLAY_ON);
  //SYS_DELAY(5);
  
  vGlcdAllClear();
  //vGlcdAllClear();
  
#ifdef HS_GLCD_DEBUG
  //printf("vUC1608X_Init() ... OK\n");
#endif
}

//****************************************************************************
void KmacLog(void)
{
  ImageDisplay(2, 0, (240-110)/2, (16-6)/2, 110, 6, KMAC_Log[0]);
}


//*****************************************************************************

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
