#ifndef EVENT_H
#define EVENT_H

#define EVENT_QUEUE_LENGTH 50

typedef uint16_t event;
uint16_t get_event();
void give_event(uint16_t event, byte nomore);
unsigned char del_event(uint16_t event);
void init_event();
void save_pause(uint16_t* qu,uint16_t* p_head,uint16_t* p_tail);
void get_pause(uint16_t* qu,uint16_t p_head,uint16_t p_tail);

typedef struct {
    uint16_t head;
    uint16_t tail;
    uint16_t queue[EVENT_QUEUE_LENGTH];
} EventQueue;

extern EventQueue events;

enum cntrl_event
{
  //status read
         // eventEquiType=0,    //Erp 기준 장비 분류
          eventEquiSN=1,        //장비 시리얼 넘버
          eventFwVer,           //fw 버전
          eventReserve0, 
          eventHwVer,           //hw 버전       
          eventSetpId,   
          eventTotalSqNum,      //장비에 저장 되어 있는 시퀀스 개수
          eventStSqId,          //시퀀스의 ID 확인
          eventSeqSelR,         //현재 로딩 되어있는 시퀀스 메모리 블록 Number
          eventTotalSN,         //전체 스텝 개수
          
          eventTotalTime,       //각 스텝의 소요 시간
          eventCrrtTotalTime,   //진행 중인 step과 step 진행 시간
          eventCrrtProTime,     //현재 프로세스 진행 시간
          eventStepTime,        //step의 시간을 확인
          eventCtAndPt,         //현재 실행 중인 step과 proces를 알려준다.
          eventXMotorVer,
          eventFwDownReset,
          eventZMotorVer,
          eventAirTempRes,
          eventEquiType,
          eventConfig,

          event_Developer_MAIN_VER,
          event_Developer_LLD_VER,

          eventUSBreset,
          eventUSBretry,
          eventUSBcontinue,

          event_MAIN_Date,
          event_LLD_Date,


             
    
 //Control Write
          eventSeqSels=0x0100,  // 선택 시퀀스 Loading
          eventSeqSelRes,
          eventCntinRun,        //선택한 스텝부터 시퀀스 끝까지 실행
          eventCntinRunRes,
          eventStepSigRun,      //선택한 스텝만 실행
          eventStepSigRunRes,
          eventStPsSigRun,      //선택한 스탭의 process만 실행
          eventStPsSigRunRes,
          eventSeqStop,         //진행 중인 Process 를 완료 하고 Stand-by 상태가 됨
          eventSeqStopRes,
          eventPause,           //일시정지
          eventPauseRes,
          eventDevReset,        //장비 sw 리셋
          eventDevResetRes,
          eventAppExit,
          eventAppExitResp,
          eventDevState,
          eventDevStateRep,
          eventDoorAlarm,
          eventDoorAlarmRes,
          eventProbeAlarm,
          eventProbeAlarmRes,
          eventFullSmpTime,
          eventFullSmpTimeRes,
          
          //
         // eventBarPr=0x0106,
         // eventLedPwr,    //LED on/off

          eventError=0x01FF,
          
          
         // Device Control(0x02)
          eventSeqSel = 0x0220,
          eventStartStepNum,
          eventXAxisHome,
          eventYAxisHome,
          eventZAxisHome,
          eventTrayHome,
          eventASPHome,
          eventPrimeFunc,
          eventDispFunc,
          eventDispAspFunc,
          eventAspFunc,
          eventShakerFunc,
          eventSmplTotalNum,
          eventSmplPrimeFunc,
          eventAutoCleanFunc,
          eventAutoCleanFuncEnd,
          eventWashingFunc,
          eventRinseFunc,
          
          
//Motor Setting Mode
 
          eventPosPag =0x0300,
          eventLLDPag ,
          eventDispPag,
          eventCamPag,
          eventEtcPag,

          
          hseTrayPage = 0x0400,
          hseTrayHorizPosSet,
          hseTrayHorizPosRead,
          hseTrayDAspPosSet,
          hseTrayDAspPosRead,
          hseTrayAlyPosSet,
          hseTrayAlyPosRead,
          hseTrayDryPosSet,
          hseTrayDryPosRead,
          hseTraySpeedSet,
          hseTraySpeedRead,
          hseTrayRunTimeMove,
          hseTrayHome,
          hseTrayAngUpPosSet,
          hseTrayAngUpPosRead,
          hseTrayAngDownPosSet,
          hseTrayAngDownPosRead,
          hseTrayAccelSet,
          hseTrayAccelRead,
          
          hseBarPage=0x0500,
          hseBarRead,
          hseBarFirstPosSet,
          hseBarFirstPosRead,
          hseBarLastPosSet,
          hseBarLastPosRead,
          hseBarDataStart,
          hseBarDataEnd,
          hseBarSqEnd,
          hseQrDataStart,
          hseQrDataReadEnd,
          hseQrTrayPosSet,
          hseQrTrayPosRead,
          hseBarRacGapSet,
          hseBarRacGapRead,
          hseBarNumbRead,
          hseQrLEDSet,
       
          hseAspHome,
          hseAspSpeedSet,
          hseAspSpeedRead,
          hseAspUpPosSet,
          hseAspUpPosRead,
          hseAspDownPosSet,
          hseAspDownPosRead,
          
          hseProbePage=0x0600,
          hseProbeBathYSet,
          hseProbeBathYRead,
          hseProbeBathZSet,
          hseProbeBathZRead,
          hseProbeSmplXSet,
          hseProbeSmplXRead,
          hseProbeSmplXEndSet,
          hseProbeSmplXEndRead,
          hseProbeSmplY1Set,
          hseProbeSmplY1Read,
          
          hseProbeSmplY2Set,
          hseProbeSmplY2Read,
          hseProbeSmplZSet,
          hseProbeSmplZRead,
          hseProbeTrayXSet,
          hseProbeTrayXRead,
          hseProbeTrayXEndSet,
          hseProbeTrayXEndRead,
          hseProbeTrayYSet,
          hseProbeTrayYRead,
          hseProbeTrayZSet,
          hseProbeTrayZRead,
          hseProbeBathInitSet,
          hseProbeBathInitRead,
          hseProbeStripGapSet,
          hseProbeStripGapRead,
          hseProbeSampleGapSet,
          hseProbeSampleGapRead,
          
          hseZaxiClldMoveOffsetSet,
          hseZaxiClldMoveOffsetRead,

          eventLLDFwDown = 0x6400,
          
          hseAspDispPage=0x0700,
          hseAspBathXSet,
          hseAspBathXRead,
          hseAspBathZSet,
          hseAspBathZRead,
          hseDispBathYSet,
          hseDispBathYRead,
          hseAspTrayXSet,
          hseAspTrayXRead,
          hseAspTrayZSet,
          hseAspTrayZRead,
          hseDispTrayYSet,
          hseDispTrayYRead,
          hseAspHomeSet,
          hseAspHomeRead,
          
          hseClldPage=0x0800,
          hseClldSensControl,
          hseClldSensControlResp,
          hseClldOutputVol,
          hseClldOutputVolResp,
          hseClldTestRun,
          hseClldTestRunResp,
          
          hseClldOffSet,
          hseClldOffSetResp,
          hseClldOffRead,
          hseClldOffReadResp,
          hseClldRaRbSet,
          hseClldRaRbSetResp,
          hseClldRaRbRead,
          hseClldRaRbReadResp,
          
          hseClldSWSSet,
          hseClldSWSResp,
          hseClldSWSRead,
          hseClldSWSReadResp,
          hseClldSWSWrite,
          hseClldSWSWriteResp,
          hseClldTestStop,
          hseClldTestStopResp,
          hseClldVol,
          hseClldValueSet,
          hseClldValueRead,

        
          hsePlldPage=0x0900,
          hsePlldQcStart,
          hsePlldQcStartResp,
          hsePlldJudgResult,
          hsePlldJudgResultResp,
          hsePlldStreamStart,
          hsePlldStreamStartResp,
          hsePlldStreamData,
          hsePlldStreamDataResp,
          hsePlldJudgSlope,
          hsePlldJudgResp,
          hsePlldMinPramSet,
          hsePlldMinPramSetResp,
          hsePlldMinPramRead,
          hsePlldMinPramReadResp,
          hsePlldMaxPramSet,
          hsePlldMaxPramSetResp,
          hsePlldMaxPramRead,
          hsePlldMaxPramReadResp,
          hsePlldSlopPramSet,
          hsePlldSlopPramSetResp,
          hsePlldSlopPramRead,
          hsePlldSlopPramReadResp,
          hsePlldBeginIndexSet,
          hsePlldBeginIndexRead,
          hsePlldEndIndexSet,
          hsePlldEndIndexRead,
          
          
          hseLLDPage=0x0A00,
          hseLLDQcStart,
          hseLLDQcStartResp,
          hseLLDJudgDataInxRult,
          
          hseDWPage=0x0B00,
          hseP15DispSet,
          hseP15DispRead,
          hseP15DispMove,
          hseP15DispPause,
          
          hseWSSetPage=0x0C00,
          hseP7DispSet,
          hseP7DispRead,
          hseP8DispSet,
          hseP8DispRead,
          hseP7DispMove,
          hseP8DispMove,

          
          hseReagSetPage=0x0D00,
          hseP1DispSet,
          hseP1DispRead,
          hseP2DispSet,
          hseP2DispRead,
          hseP3DispSet,
          hseP3DispRead,
          hseP4DispSet,
          hseP4DispRead,
          hseP5DispSet,
          hseP5DispRead,
          hseP6DispSet,
          hseP6DispRead,
          
          hseP1DispMove,
          hseP1DispPause,
          hseP2DispMove,
          hseP2DispPause,
          hseP3DispMove,
          hseP3DispPause,
          hseP4DispMove,
          hseP4DispPause,
          hseP5DispMove,
          hseP5DispPause,
          hseP6DispMove,
          hseP6DispPause,
          
          hseDSSetPage=0x0E00,
          hseSygeSet,
          hseSygeRead,
          hseSygeMove,
          hseSygePause,
          hseAirGapSet,
          hseAirGapRead,
          hseSygeSpeedSet,
          hseSygeSpeedRead,
          
          hseEtcSetPage=0x0F00,
          hseDrySet,
          hseDryOff,
          hseAspSet,
          hseAspRead,
          hseFanOn,
          hseFanOff,
          hseStausLedOn,
          hseStausLedOff,
          hseDoorOn,
          hseDoorOff,
          hseXAixsJogMoveSet,
          hseXAixsJogMoveRead,
          hseYAixsJogMoveSet,
          hseYAixsJogMoveRead,
          hseZAixsJogMoveSet,
          hseZAixsJogMoveRead,
          hseAspJogMoveSet,
          
          
          eventSqInMode=0x1001,
          eventSqTotalStep,
          eventSqSave,
          eventSqId,
          eventSqStepNum,
          eventSqPrNum,       
          eventSqPrParamByte,
          eventSqPrParamWord,
          eventSqPrParamDword,
          eventSampStripMatch,
          eventSeqStart,
          eventSampStripMatchRun,
          eventManualErrorSampBit1,
          eventManualErrorSampBit2,
          eventSqManualStart,
          eventSqCcdBlockerCheck,
          eventSqCcdBlockerFlagOn,
          eventSqOpenCcdBlockerWindow, 
          eventSqCcdBlockerStart,
          eventSqCcdBlockerBeepControl,
          
          
          
          hseCamLedPramWrite=0x1101,
          hseCamLedPramWriteResp,
          hseCamLedPramRead,
          hseCamLedPramReadResp,
          hseCamLedOnOff,
          hseCamLedOnOffResp,
          hseCamAnlyXPosSet,
          hseCamAnlyXPosSetResp,
          hseCamAnlyXPosRead,
          hseCamAnlyXPosReadResp,
          hseCamAnlyXEndPosSet,
          hseCamAnlyXEndPosSetResp,
          hseCamAnlyXEndPosRead,
          hseCamAnlyXEndPosReadResp,
          hseCamStripGo,
          hseCamStripGoResp,
          hseCamStripWaitGo,
          hseCamStripWaitGoResp,
          
          
          
          
          //barcode
          eventBarScanInit=0xA000,
          eventBarScan,
          eventBarScanOper,
          eventBarScanResult,
          eventBarScanOk,
          eventBarScanFail,
          eventBarScanEnd,
          eventBarScanReOper,
          eventBarScanCheck,
         
          //sample hlld
          eventSmpInit,
          eventSmpZinit,
          eventSmpRackMove,
          eventSmpAbsMoveCheck,
          eventSmpMoveZCheck,
          
          eventSmpProbeSpeedSet,
          eventSmpClldMinPosMoveZ,
          
          eventSmpSWSInitCheck,
          eventSmpCll,
          eventSmpCllCheck,
          eventSmpSWSCheck,
          eventSmpSygAspOper,
          eventSmpProbeZHome,
          eventSmpStripMove,
          eventSmpStripDspMove,
          eventSmpSygDspOper,
          eventSmpPlldJude,
          
          
          eventSmpClldVolInit,
          eventSmpClldRageSpeedSet,
          eventSmpClldRagePosMoveZ,

          

          eventSmpStripMoveXYCheck,
       
          eventSmpStripMoveZCheck,
          eventSmpStripHome,
          eventSmpNext,
          eventSmpEnd,
          eventSmpFinalEnd,
          eventSmpPass,
          
          eventSmpQcStreamInit,
          eventSmpQcStreamGo,
      
          
          //sample_prime
          eventSmpPrimeInit,
          eventSmpPrimeBathMove,
          eventSmpPrimeAixsCheck,
          eventSmpPrimeStripMoveZ,
          eventSmpPrimeStripMoveZCheck,
          eventSmpPrimeSygeSet,
          eventSmpPrimePumpOn,
          eventSmpPrimeStructInit,
          eventSmpPrimeSygeOper,
          eventSmpPrimeProveSet,
          eventSmpPrimeProveMove,
          eventSmpPrimeBathSuck,
          eventSmpPrimeBathInit,
          eventSmpPrimeEnd,
          eventSmpPrimeFinalEnd,
          eventSmpPrimeProbePressCheck,
          eventSmpPrimeProbePressResult,
          
          
          //prime
          eventPrimeIinit,
          eventPrimeBathPsn,
          eventPrimeDspAspOut,
          eventPrimeDspAspEnd,
          eventPrimeEnd,
          eventPrimeAixsCheck,
          eventPrimeZixsCheck,
          
          //dispense
          eventDspIinit,
          eventDspSkAngle,
          eventDspStripMove,
          eventDspOper,
          eventDspEnd,
          eventDspCheck,
          eventDspAixsCheck,
          eventDspByStripPass,
          
          //dispense&asp
          eventDspAspIinit,
          eventDspAspSkAngle,
          eventDspAspStripMove,
          eventDspAspOut,
          eventDspAspOper,
          eventDspAspEnd,
          eventDspAspCheck,
          eventDspAspAixsCheck,
          eventDspAspFinalEnd,
          eventDspAspPanelCheck,
          eventDspAspStripPackCheck,
          eventDspAspStripPackAixsCheck,
          eventDspAspStripPackAspOut,
          eventDspAspStripPackAspOper,
          eventDspAspStripPackAspEnd,
          
           //probe disp
          eventProbeDispIinit,
          eventProbeDispSkAngle,
          eventProbeDispStripMove,
          eventProbeDispProveMove,
          eventProbeDispOper,
          eventProbeDispEnd,
          eventProbeDispCheck,
          eventProbeDispAixsCheck,
          eventProbeDispZMoveCheck,
          eventProbeDispProbeHome,
          eventProbeDispFinalEnd,

          //aspiraion
          eventAspIinit,
          eventAspSkAngle,
          eventAspStripMove,
          eventAspAixsCheck,
          eventAspOut,
          eventAspOper,
          eventAspOperEnd,
          eventAspCheck,
          eventAspEnd,
          eventAspCon,
          
          //incubation
          eventSkOper,
          eventSkEnd,
           //RollBack
          eventRbOper,
          eventRbEnd,
          //dry
          eventdryOper,
          eventdryEnd,
          
          //etc function
          eventBathDspOper,
          eventBathDspEnd,
          eventBathAspOper,
          eventBathAspEnd,
          

          eventAllstHome,
          eventAllPumpCall,
          eventAllPumpStop,
          eventSystickTimer,
          
          //Auto_Clean
          eventAutoCleanInit,
          eventAutoCleanZCheck,
          eventAutoCleanBathMove,
          eventAutoCleanAixsCheck,
          eventAutoCleanAspOut,
          eventAutoCleanEnd,
          eventAutoCleanNext,
          eventAutoCleanPass,
          
          //clld
          eventClldVarResCtrl,
          
          eventBathFull,
          eventBathOk,
          eventDoorOpen,
          eventDoorClose,
          
          
           eventXhomeCk=0xA100,
           eventXAbsPosCk,
           eventHomeXAbsPosCk,
           eventYhomeCk,
           eventYAbsPosCk,
           eventHomeYAbsPosCk,
           eventZhomeCk,
           eventZAbsPosCk,
           eventHomeZAbsPosCk,
           eventAllhomeCk,
           eventAllhomeEnd,
           
           eventSkrHome,
           eventSkrHomeChk,//shaker
           eventSkAbsrHome,
           eventSkrRun,
           eventSkrRunUpAngle,
           eventSkrRunDownAngle,
           eventSkrRunChk,//shaker
           eventSkrRollBack,
           
           eventAspTimeOut,
           eventSmpCllVolCheck,
 
     
     
           
           eventBarTri=0xA200,
           eventBarReadComp,
           eventBarInitChek,
           eventQcTriger,
           
           eventSygeSendCheck,
            
          hseXaxiHome=0xA301, //x_motor
          hseXaxiSpeedSet,
          hseXaxiSpeedRead,
          hseXaxiAccelSet,
          hseXaxiAccelRead,
          hseXaxiJogMove,
          hseXaxiStripWidthSet,
          hseXaxiStripWidthRead,
          hseXaxiBathPosSet,
          hseXaxiBathPosRead,  
          hseXaxiDspPosSet,
          hseXaxiDspPosRead,
          hseXaxiAspPosSet,
          hseXaxiAspPosRead,//0x15
          hseXaxiReserve0,    
          
          hseXaxiReserve1, //0x0310
          hseXaxiSamplePosSet,
          hseXaxiSamplePosRead,
          hseXaxiBarPosSet,
          hseXaxiBarPosRead,
          hseXaxiCamPosSet,
          hseXaxiCamPosRead,
          hseXaxiReserve2,
          hseXaxiReserve3,
          hseXaxiSampleWidthSet,
          hseXaxiSampleWidthRead,
          hseXaxiClCurrentSet,
          hseXaxiClCurrentRead,
          hseXaxiReserve4,
          hseXaxiReserve5,
          hseXaxiReserve6,
          
          hseXaxiReserve7,//0x3020
          hseYaxiHome,
          hseYaxiSpeedSet,
          hseYaxiSpeedRead,
          hseYaxiAccelSet,
          hseYaxiAccelRead,
          hseYaxiJogMove,
          hseYaxiStripWidthSet,
          hseYaxiStripWidthRead,
          hseYaxiBathPosSet,
          hseYaxiBathPosRead,  
          hseYaxiDspPosSet,
          hseYaxiDspPosRead,
          hseYaxiAspPosSet,
          hseYaxiAspPosRead,
          hseYaxiBathSamplePosSet,
          
          hseYaxiBathSamplePosRead,
          hseYaxiSamplePosSet,
          hseYaxiSamplePosRead,
          hseYaxiClCurrentSet,
          hseYaxiClCurrentRead,
          
          hseZaxiHome = 0xA400,
          hseZaxiSpeedSet,
          hseZaxiSpeedRead,
          hseZaxiAccelSet,
          hseZaxiAccelRead,
          hseZaxiJogMove,
          hseZaxiReserve0,
          hseZaxiReserve1,
          hseZaxiBathPosSet,
          hseZaxiBathPosRead,  
          hseZaxiDspPosSet,
          hseZaxiDspPosRead,
          hseZaxiAspPosSet,
          hseZaxiAspPosRead,
          hseZaxiBathSamplePosSet,
          
          hseZaxiBathSamplePosRead,
          hseZaxiSamplePosSet,
          hseZaxiSamplePosRead,
          hseZaxiClCurrentSet,
          hseZaxiClCurrentRead,
          
          eventSpuOn=0xB000,
          eventSpuInit,
          eventZHomeInit,
          eventZHomeInitCheck,
          eventShakeHomeInit,
          eventAspHomeInit,
          eventMemoryInit,
          eventSygInit,
          eventPumpInit,
          eventLldInit,
          eventSpuStnby,
          eventTimer500ms,
          eventTimer100ms,
          eventTimer1s,
          
          eventAspQcDelay,
          eventSWSVarResCtrl,
          eventSWSVarCheck,
          eventSWSVarEnd,
          eventSWSVarFail,
          eventSqFullSqAly,
          eventSqNext,
          eventSqSamplNumMach,
          eventSqManualCheck,

          
          eventSpuResetAsp,
          eventSpuResetZ,
          eventSpuResetZCheck,
          eventSpuResetShak,
          eventSpuResetSyg,
          eventSpuResetPump,
          eventSpuResetXY,
          eventSpuResetEnd,
          eventAspRollBackEnd,
          eventBarPr,
          eventPauseInfinRoutin,
          eventShakeInit,
	  _eventEnd,

  //0xC ~ 0xD 영역은 SW가 독립적으로 카메라 튜닝데이터를 저장하는 영역이라 사용 불가.
    //@Save  
          hseTrayHorizPosSave       = 0xE000, // hseTrayHorizPosSet = 0x0401
          hseTrayDAspPosSave        = 0xE001, // hseTrayDAspPosSet  = 0x0403 
          hseTrayAlyPosSave         = 0xE002, // hseTrayAlyPosSet = 0x0405
          hseTrayDryPosSave         = 0xE003, // hseTrayDryPosSet = 0x0407
          hseTraySpeedSave          = 0xE004, // hseTraySpeedSet = 0x0409
          hseTrayAngUpPosSave       = 0xE005, // hseTrayAngUpPosSet = 0x040D
          hseTrayAngDownPosSave     = 0xE006, // hseTrayAngDownPosSet = 0x040F
          hseTrayAccelSave          = 0xE007, // hseTrayAccelSet =0x0411
          hseBarFirstPosSave        = 0xE008, //hseBarFirstPosSet = 0x0502
          hseBarLastPosSave         = 0xE009, //hseBarLastPosSet = 0x0504
          hseQrTrayPosSave          = 0xE00A, //hseQrTrayPosSet = 0x050B
          hseBarRacGapSave          = 0xE00B, //hseBarRacGapSet = 0x050D
          hseAspSpeedSave           = 0xE00C, //hseAspSpeedSet = 0x0512
          hseAspUpPosSave           = 0xE00D, //hseAspUpPosSet = 0x0514
          hseAspDownPosSave         = 0xE00E, //hseAspDownPosSet = 0x0516                  
          hseBarMirrorSave          = 0xE00F, //hseBarMirrorSet = 0x0519
          hseProbeBathYSave         = 0xE010, //hseProbeBathYSet = 0x0601
          hseProbeBathZSave         = 0xE011, //hseProbeBathZSet = 0x0603
          hseProbeSmplXSave         = 0xE012, //hseProbeSmplXSet = 0x0605
          hseProbeSmplXEndSave      = 0xE013, //hseProbeSmplXEndSet = 0x0607
          hseProbeSmplY1Save        = 0xE014, //hseProbeSmplY1Set = 0x0609
          hseProbeSmplY2Save        = 0xE015, //hseProbeSmplY2Set = 0x060B
          hseProbeSmplZSave         = 0xE016, //hseProbeSmplZSet =0x060D
          hseProbeTrayXSave         = 0xE017, //hseProbeTrayXSet =0x060F
          hseProbeTrayXEndSave      = 0xE018, //hseProbeTrayXEndSet = 0x0611
          hseProbeTrayYSave         = 0xE019, //hseProbeTrayYSet = 0x0613
          hseProbeTrayZSave         = 0xE01A, //hseProbeTrayZSet = 0x0615
          hseProbeBathInitSave      = 0xE01B, //hseProbeBathInitSet = 0x0617
          hseProbeStripGapSave      = 0xE01C, //hseProbeStripGapSet = 0x0619
          hseProbeSampleGapSave     = 0xE01D, //hseProbeSampleGapSet = 0x061B
          hseAspBathXSave           = 0xE01E, //hseAspBathXSet = 0x0701
          hseAspBathZSave           = 0xE01F, //hseAspBathZSet = 0x0703
          hseDispBathYSave          = 0xE020, //hseDispBathYSet = 0x0705
          hseAspTrayXSave           = 0xE021, //hseAspTrayXSet = 0x0707
          hseAspTrayZSave           = 0xE022, //hseAspTrayZSet = 0x0709
          hseDispTrayYSave          = 0xE023, //hseDispTrayYSet = 0x070B
          hseAspHomeSave            = 0xE024, //hseAspHomeSet = 0x070D
          hsePlldMinPramSave        = 0xE025, //hsePlldMinPramSet = 0x090B
          hsePlldMaxPramSave        = 0xE026,  //hsePlldMaxPramSet = 0x090F
          hsePlldSlopPramSave       = 0xE027, //hsePlldSlopPramSet = 0x0913
          hseP15DispSave            = 0xE028, //hseP15DispSet = 0x0B01
          hseP7DispSave             = 0xE029, //hseP7DispSet = 0x0C01
          hseP8DispSave             = 0xE02A, //hseP8DispSet = 0x0C03
          hseP1DispSave             = 0xE02B, //hseP1DispSet = 0x0D01
          hseP2DispSave             = 0xE02C, //hseP2DispSet = 0x0D03
          hseP3DispSave             = 0xE02D, //hseP3DispSet = 0x0D05
          hseP4DispSave             = 0xE02E, //hseP4DispSet = 0x0D07
          hseP5DispSave             = 0xE02F, //hseP5DispSet = 0x0D09 
          hseP6DispSave             = 0xE030, //hseP6DispSet = 0x0D0B
          hseSygeSave               = 0xE031, //hseSygeSet = 0x0E01
          hseAirGapSave             = 0xE032, //hseAirGapSet = 0x0E05
          hseCamAnlyXPosSave        = 0xE033, //hseCamAnlyXPosSet = 0x1107
          hseCamAnlyXEndPosSave     = 0xE034, //hseCamAnlyXEndPosSet  = 0x110B
          hseXaxiDspPosSave         = 0xE035, //hseXaxiDspPosSet = 0xA30B
          hseXaxiSamplePosSave      = 0xE036, //hseXaxiSamplePosSet = 0xA311
          hseXaxiCamPosSave         = 0xE037, //hseXaxiCamPosSet = 0xA315
          hseXaxiClCurrentSave      = 0xE038, //hseXaxiClCurrentSet = 0xA31B
          hseYaxiBathPosSave        = 0xE039, //hseYaxiBathPosSet = 0xA329
          hseYaxiDspPosSave         = 0xE03A, //hseYaxiDspPosSet = 0xA2EF
          hseYaxiBathSamplePosSave  = 0xE03B, //hseYaxiBathSamplePosSet = 0xA32F
          hseYaxiSamplePosSave      = 0xE03C, //hseYaxiSamplePosSet = 0xA331
          hseZaxiBathPosSave        = 0xE03D, //hseZaxiBathPosSet = 0xA408
          hseZaxiDspPosSave         = 0xE03E, //hseZaxiDspPosSet = 0xA40A
          hseZaxiSamplePosSave      = 0xE03F, //hseZaxiSamplePosSet = 0xA410
          hseSygeSpeedSave          = 0xE040, //hseSygeSpeedSet = 0x0E07
          
          // 향후 추가가능성
          hsePlldBeginIndexSave     = 0xE041, //hsePlldBeginIndexSet = 0x0917
          hsePlldEndIndexSave       = 0xE042, //hsePlldEndIndexSet = 0x0919
          ///////////////////



         // 개발용 프로토콜
          TEST_MCU_FW_DATE_CHECK = 0xF000,  
          TEST_LLD_FW_DATE_CHECK = 0xF001, 
          TEST_AGING_MODE   = 0xF002,     
};

#endif


/*
         // eventEquiType=0,    //Erp 기준 장비 분류
          eventEquiSN = 0x0001, //장비 시리얼 넘버
          eventFwVer = 0x0002, //fw 버전
          eventReserve0 = 0x0003,
          eventHwVer = 0x0004, //hw 버전
          eventSetpId = 0x0005,
          eventTotalSqNum = 0x0006, //장비에 저장 되어 있는 시퀀스 개수
          eventStSqId = 0x0007, //시퀀스의 ID 확인
          eventSeqSelR = 0x0008, //현재 로딩 되어있는 시퀀스 메모리 블록 Number
          eventTotalSN = 0x0009, //전체 스텝 개수

          eventTotalTime = 0x000A, //각 스텝의 소요 시간
          eventCrrtTotalTime = 0x000B, //진행 중인 step과 step 진행 시간
          eventCrrtProTime = 0x000C, //현재 프로세스 진행 시간
          eventStepTime = 0x000D, //step의 시간을 확인
          eventCtAndPt = 0x000E, //현재 실행 중인 step과 proces를 알려준다.
          eventXMotorVer = 0x000F,
          eventFwDownReset = 0x0010,
          eventZMotorVer = 0x0011,
          eventAirTempRes = 0x0012,
          eventEquiType = 0x0013,
          eventConfig = 0x0014,

          event_Developer_MAIN_VER = 0x0015,
          event_Developer_LLD_VER = 0x0016,

          eventUSBreset = 0x0017,
          eventUSBretry = 0x0018,
          eventUSBcontinue = 0x0019,

 //Control Write
          eventSeqSels = 0x0100, // 선택 시퀀스 Loading
          eventSeqSelRes = 0x0101,
          eventCntinRun = 0x0102, //선택한 스텝부터 시퀀스 끝까지 실행
          eventCntinRunRes = 0x0103,
          eventStepSigRun = 0x0104, //선택한 스텝만 실행
          eventStepSigRunRes = 0x0105,
          eventStPsSigRun = 0x0106, //선택한 스탭의 process만 실행
          eventStPsSigRunRes = 0x0107,
          eventSeqStop = 0x0108, //진행 중인 Process 를 완료 하고 Stand-by 상태가 됨
          eventSeqStopRes = 0x0109,
          eventPause = 0x010A, //일시정지
          eventPauseRes = 0x010B,
          eventDevReset = 0x010C, //장비 sw 리셋
          eventDevResetRes = 0x010D,
          eventAppExit = 0x010E,
          eventAppExitResp = 0x010F,
          eventDevState = 0x0110,
          eventDevStateRep = 0x0111,
          eventDoorAlarm = 0x0112,
          eventDoorAlarmRes = 0x0113,
          eventProbeAlarm = 0x0114,
          eventProbeAlarmRes = 0x0115,
          eventFullSmpTime = 0x0116,
          eventFullSmpTimeRes = 0x0117,

          eventError = 0x01FF,

         // Device Control(0x02)
          eventSeqSel = 0x0220,
          eventStartStepNum = 0x0221,
          eventXAxisHome = 0x0222,
          eventYAxisHome = 0x0223,
          eventZAxisHome = 0x0224,
          eventTrayHome = 0x0225,
          eventASPHome = 0x0226,
          eventPrimeFunc = 0x0227,
          eventDispFunc = 0x0228,
          eventDispAspFunc = 0x0229,
          eventAspFunc = 0x022A,
          eventShakerFunc = 0x022B,
          eventSmplTotalNum = 0x022C,
          eventSmplPrimeFunc = 0x022D,
          eventAutoCleanFunc = 0x022E,
          eventAutoCleanFuncEnd = 0x022F,
          eventWashingFunc = 0x0230,
          eventRinseFunc = 0x0231,

//Motor Setting Mode
 
          eventPosPag = 0x0300,
          eventLLDPag = 0x0301,
          eventDispPag = 0x0302,
          eventCamPag = 0x0303,
          eventEtcPag = 0x0304,

          hseTrayPage = 0x0400,
          hseTrayHorizPosSet = 0x0401,
          hseTrayHorizPosRead = 0x0402,
          hseTrayDAspPosSet = 0x0403,
          hseTrayDAspPosRead = 0x0404,
          hseTrayAlyPosSet = 0x0405,
          hseTrayAlyPosRead = 0x0406,
          hseTrayDryPosSet = 0x0407,
          hseTrayDryPosRead = 0x0408,
          hseTraySpeedSet = 0x0409,
          hseTraySpeedRead = 0x040A,
          hseTrayRunTimeMove = 0x040B,
          hseTrayHome = 0x040C,
          hseTrayAngUpPosSet = 0x040D,
          hseTrayAngUpPosRead = 0x040E,
          hseTrayAngDownPosSet = 0x040F,
          hseTrayAngDownPosRead = 0x0410,
          hseTrayAccelSet = 0x0411,
          hseTrayAccelRead = 0x0412,
          
          hseBarPage = 0x0500,
          hseBarRead = 0x0501,
          hseBarFirstPosSet = 0x0502,
          hseBarFirstPosRead = 0x0503,
          hseBarLastPosSet = 0x0504,
          hseBarLastPosRead = 0x0505,
          hseBarDataStart = 0x0506,
          hseBarDataEnd = 0x0507,
          hseBarSqEnd = 0x0508,
          hseQrDataStart = 0x0509,
          hseQrDataReadEnd = 0x050A,
          hseQrTrayPosSet = 0x050B,
          hseQrTrayPosRead = 0x050C,
          hseBarRacGapSet = 0x050D,
          hseBarRacGapRead = 0x050E,
          hseBarNumbRead = 0x050F,
          hseQrLEDSet = 0x0510,
       
          hseAspHome = 0x0511,
          hseAspSpeedSet = 0x0512,
          hseAspSpeedRead = 0x0513,
          hseAspUpPosSet = 0x0514,
          hseAspUpPosRead = 0x0515,
          hseAspDownPosSet = 0x0516,
          hseAspDownPosRead = 0x0517,
          
          hseBarReadPass = 0x0518,

          hseProbePage = 0x0600,
          hseProbeBathYSet = 0x0601,
          hseProbeBathYRead = 0x0602,
          hseProbeBathZSet = 0x0603,
          hseProbeBathZRead = 0x0604,
          hseProbeSmplXSet = 0x0605,
          hseProbeSmplXRead = 0x0606,
          hseProbeSmplXEndSet = 0x0607,
          hseProbeSmplXEndRead = 0x0608,
          hseProbeSmplY1Set = 0x0609,
          hseProbeSmplY1Read = 0x060A,
          
          hseProbeSmplY2Set = 0x060B,
          hseProbeSmplY2Read = 0x060C,
          hseProbeSmplZSet = 0x060D,
          hseProbeSmplZRead = 0x060E,
          hseProbeTrayXSet = 0x060F,
          hseProbeTrayXRead = 0x0610,
          hseProbeTrayXEndSet = 0x0611,
          hseProbeTrayXEndRead = 0x0612,
          hseProbeTrayYSet = 0x0613,
          hseProbeTrayYRead = 0x0614,
          hseProbeTrayZSet = 0x0615,
          hseProbeTrayZRead = 0x0616,
          hseProbeBathInitSet = 0x0617,
          hseProbeBathInitRead = 0x0618,
          hseProbeStripGapSet = 0x0619,
          hseProbeStripGapRead = 0x061A,
          hseProbeSampleGapSet = 0x061B,
          hseProbeSampleGapRead = 0x061C,
          hseZaxiClldMoveOffsetSet=0x061D,
          hseZaxiClldMoveOffsetRead=0x061E,

          eventLLDFwDown = 0x6400,
          
          hseAspDispPage = 0x0700,
          hseAspBathXSet = 0x0701,
          hseAspBathXRead = 0x0702,
          hseAspBathZSet = 0x0703,
          hseAspBathZRead = 0x0704,
          hseDispBathYSet = 0x0705,
          hseDispBathYRead = 0x0706,
          hseAspTrayXSet = 0x0707,
          hseAspTrayXRead = 0x0708,
          hseAspTrayZSet = 0x0709,
          hseAspTrayZRead = 0x070A,
          hseDispTrayYSet = 0x070B,
          hseDispTrayYRead = 0x070C,
          hseAspHomeSet = 0x070D,
          hseAspHomeRead = 0x070E,
          
          hseClldPage = 0x0800,
          hseClldSensControl = 0x0801,
          hseClldSensControlResp = 0x0802,
          hseClldOutputVol = 0x0803,
          hseClldOutputVolResp = 0x0804,
          hseClldTestRun = 0x0805,
          hseClldTestRunResp = 0x0806,
          
          hseClldOffSet = 0x0807,
          hseClldOffSetResp = 0x0808,
          hseClldOffRead = 0x0809,
          hseClldOffReadResp = 0x080A,
          hseClldRaRbSet = 0x080B,
          hseClldRaRbSetResp = 0x080C,
          hseClldRaRbRead = 0x080D,
          hseClldRaRbReadResp = 0x080E,
          
          hseClldSWSSet = 0x080F,
          hseClldSWSResp = 0x0810,
          hseClldSWSRead = 0x0811,
          hseClldSWSReadResp = 0x0812,
          hseClldSWSWrite = 0x0813,
          hseClldSWSWriteResp = 0x0814,
          hseClldTestStop = 0x0815,
          hseClldTestStopResp = 0x0816,
          hseClldVol = 0x0817,
          hseClldValueSet = 0x0818,
          hseClldValueRead = 0x0819,

        
          hsePlldPage = 0x0900,
          hsePlldQcStart = 0x0901,
          hsePlldQcStartResp = 0x0902,
          hsePlldJudgResult = 0x0903,
          hsePlldJudgResultResp = 0x0904,
          hsePlldStreamStart = 0x0905,
          hsePlldStreamStartResp = 0x0906,
          hsePlldStreamData = 0x0907,
          hsePlldStreamDataResp = 0x0908,
          hsePlldJudgSlope = 0x0909,
          hsePlldJudgResp = 0x090A,
          hsePlldMinPramSet = 0x090B,
          hsePlldMinPramSetResp = 0x090C,
          hsePlldMinPramRead = 0x090D,
          hsePlldMinPramReadResp = 0x090E,
          hsePlldMaxPramSet = 0x090F,
          hsePlldMaxPramSetResp = 0x0910,
          hsePlldMaxPramRead = 0x0911,
          hsePlldMaxPramReadResp = 0x0912,
          hsePlldSlopPramSet = 0x0913,
          hsePlldSlopPramSetResp = 0x0914,
          hsePlldSlopPramRead = 0x0915,
          hsePlldSlopPramReadResp = 0x0916,
          hsePlldBeginIndexSet = 0x0917,
          hsePlldBeginIndexRead = 0x0918,
          hsePlldEndIndexSet = 0x0919,
          hsePlldEndIndexRead = 0x091A,
          
          
          hseLLDPage = 0x0A00,
          hseLLDQcStart = 0x0A01,
          hseLLDQcStartResp = 0x0A02,
          hseLLDJudgDataInxRult = 0x0A03,
          
          hseDWPage = 0x0B00,
          hseP15DispSet = 0x0B01,
          hseP15DispRead = 0x0B02,
          hseP15DispMove = 0x0B03,
          hseP15DispPause = 0x0B04,
          
          hseWSSetPage = 0x0C00,
          hseP7DispSet = 0x0C01,
          hseP7DispRead = 0x0C02,
          hseP8DispSet = 0x0C03,
          hseP8DispRead = 0x0C04,
          hseP7DispMove = 0x0C05,
          hseP8DispMove = 0x0C06,

          
          hseReagSetPage = 0x0D00,
          hseP1DispSet = 0x0D01,
          hseP1DispRead = 0x0D02,
          hseP2DispSet = 0x0D03,
          hseP2DispRead = 0x0D04,
          hseP3DispSet = 0x0D05,
          hseP3DispRead = 0x0D06,
          hseP4DispSet = 0x0D07,
          hseP4DispRead = 0x0D08,
          hseP5DispSet = 0x0D09,
          hseP5DispRead = 0x0D0A,
          hseP6DispSet = 0x0D0B,
          hseP6DispRead = 0x0D0C,
          
          hseP1DispMove = 0x0D0D,
          hseP1DispPause = 0x0D0E,
          hseP2DispMove = 0x0D0F,
          hseP2DispPause = 0x0D10,
          hseP3DispMove = 0x0D11,
          hseP3DispPause = 0x0D12,
          hseP4DispMove = 0x0D13,
          hseP4DispPause = 0x0D14,
          hseP5DispMove = 0x0D15,
          hseP5DispPause = 0x0D16,
          hseP6DispMove = 0x0D17,
          hseP6DispPause = 0x0D18,
          
          hseDSSetPage = 0x0E00,
          hseSygeSet = 0x0E01,
          hseSygeRead = 0x0E02,
          hseSygeMove = 0x0E03,
          hseSygePause = 0x0E04,
          hseAirGapSet = 0x0E05,
          hseAirGapRead = 0x0E06,
          hseSygeSpeedSet = 0x0E07,
          hseSygeSpeedRead = 0x0E08,
          
          hseEtcSetPage = 0x0F00,
          hseDrySet = 0x0F01,
          hseDryOff = 0x0F02,
          hseAspSet = 0x0F03,
          hseAspRead = 0x0F04,
          hseFanOn = 0x0F05,
          hseFanOff = 0x0F06,
          hseStausLedOn = 0x0F07,
          hseStausLedOff = 0x0F08,
          hseDoorOn = 0x0F09,
          hseDoorOff = 0x0F0A,
          hseXAixsJogMoveSet = 0x0F0B,
          hseXAixsJogMoveRead = 0x0F0C,
          hseYAixsJogMoveSet = 0x0F0D,
          hseYAixsJogMoveRead = 0x0F0E,
          hseZAixsJogMoveSet = 0x0F0F,
          hseZAixsJogMoveRead = 0x0F10,
          hseAspJogMoveSet = 0x0F11,
          
          
          eventSqInMode = 0x1001,
          eventSqTotalStep = 0x1002,
          eventSqSave = 0x1003,
          eventSqId = 0x1004,
          eventSqStepNum = 0x1005,
          eventSqPrNum = 0x1006,
          eventSqPrParamByte = 0x1007,
          eventSqPrParamWord = 0x1008,
          eventSqPrParamDword = 0x1009,
          eventSampStripMatch = 0x100A,
          eventSeqStart = 0x100B,
          eventSampStripMatchRun = 0x100C,
          eventManualErrorSampBit1 = 0x100D,
          eventManualErrorSampBit2 = 0x100E,
          eventSqManualStart = 0x100F,
          eventSqCcdBlockerCheck = 0x1010,
          eventSqCcdBlockerFlagOn = 0x1011,
          eventOpenCcdBlockerWindow = 0x1012,
          eventSqCcdBlockerStart = 0x1013,
          eventSqCcdBlockerBeepControl = 0x1014,
          
        
          hseCamLedPramWrite = 0x1101,
          hseCamLedPramWriteResp = 0x1102,
          hseCamLedPramRead = 0x1103,
          hseCamLedPramReadResp = 0x1104,
          hseCamLedOnOff = 0x1105,
          hseCamLedOnOffResp = 0x1106,
          hseCamAnlyXPosSet = 0x1107,
          hseCamAnlyXPosSetResp = 0x1108,
          hseCamAnlyXPosRead = 0x1109,
          hseCamAnlyXPosReadResp = 0x110A,
          hseCamAnlyXEndPosSet = 0x110B,
          hseCamAnlyXEndPosSetResp = 0x110C,
          hseCamAnlyXEndPosRead = 0x110D,
          hseCamAnlyXEndPosReadResp = 0x110E,
          hseCamStripGo = 0x110F,
          hseCamStripGoResp = 0x1110,
          hseCamStripWaitGo = 0x1111,
          hseCamStripWaitGoResp = 0x1112,
          
          
          
          
          //barcode
          eventBarScanInit = 0xA000,
          eventBarScan = 0xA001,
          eventBarScanOper = 0xA002,
          eventBarScanResult = 0xA003,
          eventBarScanOk = 0xA004,
          eventBarScanFail = 0xA005,
          eventBarScanEnd = 0xA006,
          eventBarScanReOper = 0xA007,
          eventBarScanCheck = 0xA008,
         
          //sample hlld
          eventSmpInit = 0xA009,
          eventSmpZinit = 0xA00A,
          eventSmpRackMove = 0xA00B,
          eventSmpAbsMoveCheck = 0xA00C,
          eventSmpMoveZCheck = 0xA00D,
          
          eventSmpProbeSpeedSet = 0xA00E,
          eventSmpClldMinPosMoveZ = 0xA00F,
          
          eventSmpSWSInitCheck = 0xA010,
          eventSmpCll = 0xA011,
          eventSmpCllCheck = 0xA012,
          eventSmpSWSCheck = 0xA013,
          eventSmpSygAspOper = 0xA014,
          eventSmpProbeZHome = 0xA015,
          eventSmpStripMove = 0xA016,
          eventSmpStripDspMove = 0xA017,
          eventSmpSygDspOper = 0xA018,
          eventSmpPlldJude = 0xA019,
          
          
          eventSmpClldVolInit = 0xA01A,
          eventSmpClldRageSpeedSet = 0xA01B,
          eventSmpClldRagePosMoveZ = 0xA01C,

          eventSmpStripMoveXYCheck = 0xA01D,
       
          eventSmpStripMoveZCheck = 0xA01E,
          eventSmpStripHome = 0xA01F,
          eventSmpNext = 0xA020,
          eventSmpEnd = 0xA021,
          eventSmpFinalEnd = 0xA022,
          eventSmpPass = 0xA023,
          
          eventSmpQcStreamInit = 0xA024,
          eventSmpQcStreamGo = 0xA025,
      
          
          //sample_prime
          eventSmpPrimeInit = 0xA026,
          eventSmpPrimeBathMove = 0xA027,
          eventSmpPrimeAixsCheck = 0xA028,
          eventSmpPrimeStripMoveZ = 0xA029,
          eventSmpPrimeStripMoveZCheck = 0xA02A,
          eventSmpPrimeSygeSet = 0xA02B,
          eventSmpPrimePumpOn = 0xA02C,
          eventSmpPrimeStructInit = 0xA02D,
          eventSmpPrimeSygeOper = 0xA02E,
          eventSmpPrimeProveSet = 0xA02F,
          eventSmpPrimeProveMove = 0xA030,
          eventSmpPrimeBathSuck = 0xA031,
          eventSmpPrimeBathInit = 0xA032,
          eventSmpPrimeEnd = 0xA033,
          eventSmpPrimeFinalEnd = 0xA034,
          eventSmpPrimeProbePressCheck = 0xA035,
          eventSmpPrimeProbePressResult = 0xA036,
          
          
          //prime
          eventPrimeIinit = 0xA037,
          eventPrimeBathPsn = 0xA038,
          eventPrimeDspAspOut = 0xA039,
          eventPrimeDspAspEnd = 0xA03A,
          eventPrimeEnd = 0xA03B,
          eventPrimeAixsCheck = 0xA03C,
          eventPrimeZixsCheck = 0xA03D,
          
          //dispense
          eventDspIinit = 0xA03E,
          eventDspSkAngle = 0xA03F,
          eventDspStripMove = 0xA040,
          eventDspOper = 0xA041,
          eventDspEnd = 0xA042,
          eventDspCheck = 0xA043,
          eventDspAixsCheck = 0xA044,
          eventDspByStripPass = 0xA045,
          
          //dispense&asp
          eventDspAspIinit = 0xA046,
          eventDspAspSkAngle = 0xA047,
          eventDspAspStripMove = 0xA048,
          eventDspAspOut = 0xA049,
          eventDspAspOper = 0xA04A,
          eventDspAspEnd = 0xA04B,
          eventDspAspCheck = 0xA04C,
          eventDspAspAixsCheck = 0xA04D,
          eventDspAspFinalEnd = 0xA04E,
          eventDspAspPanelCheck = 0xA04F,
          eventDspAspStripPackCheck = 0xA050,
          eventDspAspStripPackAixsCheck = 0xA051,
          eventDspAspStripPackAspOut = 0xA052,
          eventDspAspStripPackAspOper = 0xA053,
          eventDspAspStripPackAspEnd = 0xA054,
          
           //probe disp
          eventProbeDispIinit = 0xA055,
          eventProbeDispSkAngle = 0xA056,
          eventProbeDispStripMove = 0xA057,
          eventProbeDispProveMove = 0xA058,
          eventProbeDispOper = 0xA059,
          eventProbeDispEnd = 0xA05A,
          eventProbeDispCheck = 0xA05B,
          eventProbeDispAixsCheck = 0xA05C,
          eventProbeDispZMoveCheck = 0xA05D,
          eventProbeDispProbeHome = 0xA05E,
          eventProbeDispFinalEnd = 0xA05F,


          //aspiraion
          eventAspIinit = 0xA060,
          eventAspSkAngle = 0xA061,
          eventAspStripMove = 0xA062,
          eventAspAixsCheck = 0xA063,
          eventAspOut = 0xA064,
          eventAspOper = 0xA065,
          eventAspOperEnd = 0xA066,
          eventAspCheck = 0xA067,
          eventAspEnd = 0xA068,
          eventAspCon = 0xA069,
          
          //incubation
          eventSkOper = 0xA06A,
          eventSkEnd = 0xA06B,
           //RollBack
          eventRbOper = 0xA06C,
          eventRbEnd = 0xA06D,
          //dry
          eventdryOper = 0xA06E,
          eventdryEnd = 0xA06F,
          
          //etc function
          eventBathDspOper = 0xA070,
          eventBathDspEnd = 0xA071,
          eventBathAspOper = 0xA072,
          eventBathAspEnd = 0xA073,

          eventAllstHome = 0xA074,
          eventAllPumpCall = 0xA075,
          eventAllPumpStop = 0xA076,
          eventSystickTimer = 0xA077,
          
          //Auto_Clean
          eventAutoCleanInit = 0xA078,
          eventAutoCleanZCheck = 0xA079,
          eventAutoCleanBathMove = 0xA07A,
          eventAutoCleanAixsCheck = 0xA07B,
          eventAutoCleanAspOut = 0xA07C,
          eventAutoCleanEnd = 0xA07D,
          eventAutoCleanNext = 0xA07E,
          eventAutoCleanPass = 0xA07F,
          
          //clld
          eventClldVarResCtrl = 0xA080,
          
          eventBathFull = 0xA081,
          eventBathOk = 0xA082,
          eventDoorOpen = 0xA083,
          eventDoorClose = 0xA084,
           eventXhomeCk = 0xA100,
           eventXAbsPosCk = 0xA101,
           eventHomeXAbsPosCk = 0xA102,
           eventYhomeCk = 0xA103,
           eventYAbsPosCk = 0xA104,
           eventHomeYAbsPosCk = 0xA105,
           eventZhomeCk = 0xA106,
           eventZAbsPosCk = 0xA107,
           eventHomeZAbsPosCk = 0xA108,
           eventAllhomeCk = 0xA109,
           eventAllhomeEnd = 0xA10A,
           
           eventSkrHome = 0xA10B,
           eventSkrHomeChk = 0xA10C, //shaker
           eventSkAbsrHome = 0xA10D,
           eventSkrRun = 0xA10E,
           eventSkrRunUpAngle = 0xA10F,
           eventSkrRunDownAngle = 0xA110,
           eventSkrRunChk = 0xA111, //shaker
           eventSkrRollBack = 0xA112,
           
           eventAspTimeOut = 0xA113,
           eventSmpCllVolCheck = 0xA114,
 
           eventBarTri = 0xA200,
           eventBarReadComp = 0xA201,
           eventBarInitChek = 0xA202,
           eventQcTriger = 0xA203,
           
           eventSygeSendCheck = 0xA204,
            
          hseXaxiHome = 0xA301, //x_motor
          hseXaxiSpeedSet = 0xA302,
          hseXaxiSpeedRead = 0xA303,
          hseXaxiAccelSet = 0xA304,
          hseXaxiAccelRead = 0xA305,
          hseXaxiJogMove = 0xA306,
          hseXaxiStripWidthSet = 0xA307,
          hseXaxiStripWidthRead = 0xA308,
          hseXaxiBathPosSet = 0xA309,
          hseXaxiBathPosRead = 0xA30A,
          hseXaxiDspPosSet = 0xA30B,
          hseXaxiDspPosRead = 0xA30C,
          hseXaxiAspPosSet = 0xA30D,
          hseXaxiAspPosRead = 0xA30E, //0x15
          hseXaxiReserve0 = 0xA30F,
          
          hseXaxiReserve1 = 0xA310, //0x0310
          hseXaxiSamplePosSet = 0xA311,
          hseXaxiSamplePosRead = 0xA312,
          hseXaxiBarPosSet = 0xA313,
          hseXaxiBarPosRead = 0xA314,
          hseXaxiCamPosSet = 0xA315,
          hseXaxiCamPosRead = 0xA316,
          hseXaxiReserve2 = 0xA317,
          hseXaxiReserve3 = 0xA318,
          hseXaxiSampleWidthSet = 0xA319,
          hseXaxiSampleWidthRead = 0xA31A,
          hseXaxiClCurrentSet = 0xA31B,
          hseXaxiClCurrentRead = 0xA31C,
          hseXaxiReserve4 = 0xA31D,
          hseXaxiReserve5 = 0xA31E,
          hseXaxiReserve6 = 0xA31F,
          
          hseXaxiReserve7 = 0xA320, //0x3020
          hseYaxiHome = 0xA321,
          hseYaxiSpeedSet = 0xA322,
          hseYaxiSpeedRead = 0xA323,
          hseYaxiAccelSet = 0xA324,
          hseYaxiAccelRead = 0xA325,
          hseYaxiJogMove = 0xA326,
          hseYaxiStripWidthSet = 0xA327,
          hseYaxiStripWidthRead = 0xA328,
          hseYaxiBathPosSet = 0xA329,
          hseYaxiBathPosRead = 0xA32A,
          hseYaxiDspPosSet = 0xA32B,
          hseYaxiDspPosRead = 0xA32C,
          hseYaxiAspPosSet = 0xA32D,
          hseYaxiAspPosRead = 0xA32E,
          hseYaxiBathSamplePosSet = 0xA32F,
          
          hseYaxiBathSamplePosRead = 0xA330,
          hseYaxiSamplePosSet = 0xA331,
          hseYaxiSamplePosRead = 0xA332,
          hseYaxiClCurrentSet = 0xA333,
          hseYaxiClCurrentRead = 0xA334,
          
          hseZaxiHome = 0xA400,
          hseZaxiSpeedSet = 0xA401,
          hseZaxiSpeedRead = 0xA402,
          hseZaxiAccelSet = 0xA403,
          hseZaxiAccelRead = 0xA404,
          hseZaxiJogMove = 0xA405,
          hseZaxiReserve0 = 0xA406,
          hseZaxiReserve1 = 0xA407,
          hseZaxiBathPosSet = 0xA408,
          hseZaxiBathPosRead = 0xA409,
          hseZaxiDspPosSet = 0xA40A,
          hseZaxiDspPosRead = 0xA40B,
          hseZaxiAspPosSet = 0xA40C,
          hseZaxiAspPosRead = 0xA40D,
          hseZaxiBathSamplePosSet = 0xA40E,
          
          hseZaxiBathSamplePosRead = 0xA40F,
          hseZaxiSamplePosSet = 0xA410,
          hseZaxiSamplePosRead = 0xA411,
          hseZaxiClCurrentSet = 0xA412,
          hseZaxiClCurrentRead = 0xA413,
          
          eventSpuOn = 0xB000,
          eventSpuInit = 0xB001,
          eventZHomeInit = 0xB002,
          eventZHomeInitCheck = 0xB003,
          eventShakeHomeInit = 0xB004,
          eventAspHomeInit = 0xB005,
          eventMemoryInit = 0xB006,
          eventSygInit = 0xB007,
          eventPumpInit = 0xB008,
          eventLldInit = 0xB009,
          eventSpuStnby = 0xB00A,
          eventTimer500ms = 0xB00B,
          eventTimer100ms = 0xB00C,
          eventTimer1s = 0xB00D,
          
          eventAspQcDelay = 0xB00E,
          eventSWSVarResCtrl = 0xB00F,
          eventSWSVarCheck = 0xB010,
          eventSWSVarEnd = 0xB011,
          eventSWSVarFail = 0xB012,
          eventSqFullSqAly = 0xB013,
          eventSqNext = 0xB014,
          eventSqSamplNumMach = 0xB015,
          eventSqManualCheck = 0xB016,

          
          eventSpuResetAsp = 0xB017,
          eventSpuResetZ = 0xB018,
          eventSpuResetZCheck = 0xB019,
          eventSpuResetShak = 0xB01A,
          eventSpuResetSyg = 0xB01B,
          eventSpuResetPump = 0xB01C,
          eventSpuResetXY = 0xB01D,
          eventSpuResetEnd = 0xB01E,
          eventAspRollBackEnd = 0xB01F,
          eventBarPr = 0xB020,
          eventPauseInfinRoutin = 0xB021,
          eventShakeInit = 0xB022,
          _eventEnd = 0xB023,

  //0xC ~ 0xD 영역은 SW가 독립적으로 카메라 튜닝데이터를 저장하는 영역이라 사용 불가.
    //@Save  
          hseTrayHorizPosSave = 0xE000,
          hseTrayDAspPosSave = 0xE001,
          hseTrayAlyPosSave = 0xE002,
          hseTrayDryPosSave = 0xE003,
          hseTraySpeedSave = 0xE004,
          hseTrayAngUpPosSave = 0xE005,
          hseTrayAngDownPosSave = 0xE006,
          hseTrayAccelSave = 0xE007,
          hseBarFirstPosSave = 0xE008,
          hseBarLastPosSave = 0xE009,
          hseQrTrayPosSave = 0xE00A,
          hseBarRacGapSave = 0xE00B,
          hseAspSpeedSave = 0xE00C,
          hseAspUpPosSave = 0xE00D,
          hseAspDownPosSave = 0xE00E,
          hseBarMirrorSave = 0xE00F,
          hseProbeBathYSave = 0xE010,
          hseProbeBathZSave = 0xE011,
          hseProbeSmplXSave = 0xE012,
          hseProbeSmplXEndSave = 0xE013,
          hseProbeSmplY1Save = 0xE014,
          hseProbeSmplY2Save = 0xE015,
          hseProbeSmplZSave = 0xE016,
          hseProbeTrayXSave = 0xE017,
          hseProbeTrayXEndSave = 0xE018,
          hseProbeTrayYSave = 0xE019,
          hseProbeTrayZSave = 0xE01A,
          hseProbeBathInitSave = 0xE01B,
          hseProbeStripGapSave = 0xE01C,
          hseProbeSampleGapSave = 0xE01D,
          hseAspBathXSave = 0xE01E,
          hseAspBathZSave = 0xE01F,
          hseDispBathYSave = 0xE020,
          hseAspTrayXSave = 0xE021,
          hseAspTrayZSave = 0xE022,
          hseDispTrayYSave = 0xE023,
          hseAspHomeSave = 0xE024,
          hsePlldMinPramSave = 0xE025,
          hsePlldMaxPramSave = 0xE026,
          hsePlldSlopPramSave = 0xE027,
          hseP15DispSave = 0xE028,
          hseP7DispSave = 0xE029,
          hseP8DispSave = 0xE02A,
          hseP1DispSave = 0xE02B,
          hseP2DispSave = 0xE02C,
          hseP3DispSave = 0xE02D,
          hseP4DispSave = 0xE02E,
          hseP5DispSave = 0xE02F,
          hseP6DispSave = 0xE030,
          hseSygeSave = 0xE031,
          hseAirGapSave = 0xE032,
          hseCamAnlyXPosSave = 0xE033,
          hseCamAnlyXEndPosSave = 0xE034,
          hseXaxiDspPosSave = 0xE035,
          hseXaxiSamplePosSave = 0xE036,
          hseXaxiCamPosSave = 0xE037,
          hseXaxiClCurrentSave = 0xE038,
          hseYaxiBathPosSave = 0xE039,
          hseYaxiDspPosSave = 0xE03A,
          hseYaxiBathSamplePosSave = 0xE03B,
          hseYaxiSamplePosSave = 0xE03C,
          hseZaxiBathPosSave = 0xE03D,
          hseZaxiDspPosSave = 0xE03E,
          hseZaxiSamplePosSave = 0xE03F,
          hseSygeSpeedSave = 0xE040,
          
          // 향후 추가가능성
          hsePlldBeginIndexSave = 0xE041,
          hsePlldEndIndexSave = 0xE042,
          ///////////////////

         // 개발용 프로토콜
          TEST_MCU_FW_DATE_CHECK = 0xF000,
          TEST_LLD_FW_DATE_CHECK = 0xF001,
          TEST_AGING_MODE = 0xF002,
*/