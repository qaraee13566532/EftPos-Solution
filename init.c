
#include <xc.h>
#include "HardwareProfile.h"
#include "MyMain.h"
#include "define_type.h"






extern void DelayUs(uint32_t DelaySet);
extern uint8_t PageWrite(uint32_t PageAddress);
extern void PageRead(uint32_t PageAddress);
extern void UART2_Initialize(void);
extern void UART3_Initialize(void);

extern uint32_t ReadBuffer[512];
extern uint16_t beez_time,CashCnt,ShledCnt;
extern uint8_t ErrorCalibrationTimout,CalibSettingState,Loadcell_Id,SwitchWeight;
extern const uint16_t  DivitionTable[13];

extern bool AutoSend,Run_Flag,Onoff_Flag,NoCalibrate,packServiced,sensitiveDataCompleted,isSensitiveScaleZero,
            EnableLedShow,OnOff_State,Power_Save_Flag,Count_Flag,LockMode,sendSensitive,sendNormal,price_received,
            En_Write_Flag,En_Setup_Flag,WeightCalculated,StartResolutionLog,startSave;   


extern union SetParameter SystemParm;
extern uint8_t  ResetCause  __attribute__ ((persistent));

void UpdateSetting(void)
{
    uint16_t LoopCnt;
    for(LoopCnt=0;LoopCnt<sizeof(struct SettingParameters)/4;LoopCnt++)
        ReadBuffer[LoopCnt]=SystemParm.Set_Parameter_Words[LoopCnt];
    PageWrite (NVM_SETTING_PAGE_ADDRESS);
}

void Load_Setting( void )
{
    uint16_t LoopCnt;
    PageRead(NVM_SETTING_PAGE_ADDRESS);
    for(LoopCnt=0;LoopCnt<sizeof(struct SettingParameters)/4;LoopCnt++)
        SystemParm.Set_Parameter_Words[LoopCnt]=ReadBuffer[LoopCnt];  
    if(SystemParm.Set_Parameter.IsConfig!=1)
    {
        SystemParm.Set_Parameter.IsConfig=1;
        SystemParm.Set_Parameter.Out_PosBaudRate=115200; 
        SystemParm.Set_Parameter.In_PosBaudRate=57600; 
        SystemParm.Set_Parameter.Out_EftPo_Type=0;     //for parsian output 
        SystemParm.Set_Parameter.In_EftPo_Type=10;     //for cas input 
        SystemParm.Set_Parameter.Toman_Mode=0;          
        UpdateSetting();
    }
    UART2_Initialize();
    UART3_Initialize();
}
    
void InitVariables(void)
{
    price_received=false;
}

void InitMainPrograms(void)
{
    InitVariables();
    Load_Setting();
}
