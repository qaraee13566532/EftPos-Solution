
#include <xc.h>
#include "HardwareProfile.h"
#include "MyMain.h"
#include "define_type.h"






extern void DelayUs(uint32_t DelaySet);

extern uint32_t ReadBuffer[512];
extern union CalParm CalibrationParametes;
extern APP_DATA appData;
extern uint16_t beez_time,CashCnt,ShledCnt;
extern uint8_t ErrorCalibrationTimout,CalibSettingState,Loadcell_Id,SwitchWeight;
extern const uint16_t  DivitionTable[13];

extern bool AutoSend,Run_Flag,Onoff_Flag,NoCalibrate,
            EnableLedShow,OnOff_State,Power_Save_Flag,Count_Flag,
            En_Write_Flag,En_Setup_Flag,WeightCalculated,StartResolutionLog;   

extern struct WStructure WeightStruc[2];

extern union SetParameter SystemParm;

extern int32_t Offset;

extern uint8_t ADC_Ch_SW,ResLogCounter;
extern uint8_t  ResetCause  __attribute__ ((persistent));


uint8_t WriteDefaultCalibSetting ( void )
{
    uint16_t LoopCnt,RetValue;
   /* for(LoopCnt=0;LoopCnt<512;LoopCnt++)
        ReadBuffer[LoopCnt]=0x00;
    for(LoopCnt=0;LoopCnt<sizeof(struct CalibrationParameters)/4;LoopCnt++)
        CalibrationParametes.CalibParameter_Words[LoopCnt]=0;
    CalibrationParametes.CalibParameter.Calibration_Weight_Number=DEFAULT_WEIGHT_CALIBRATION;
    CalibrationParametes.CalibParameter.Divition_Index=DEFUALT_DIVITION;
    CalibrationParametes.CalibParameter.Max_weight=MAXWEIGHT_1;
    CalibrationParametes.CalibParameter.Point_Index=DEFUALT_POIN_INDEX;
    CalibrationParametes.CalibParameter.W_factor.factor=DEFUALT_FACTOR;
    CalibrationParametes.CalibParameter.Zero_Atod=DEFUALT_ZERO;
    CalibrationParametes.CalibParameter.IsCalibrated=1;
    for(LoopCnt=0;LoopCnt<sizeof(struct CalibrationParameters)/4;LoopCnt++)
        ReadBuffer[LoopCnt]=CalibrationParametes.CalibParameter_Words[LoopCnt]; 
    RetValue=PageWrite(NVM_CALIBRATION_PAGE_ADDRESS);*/
    return RetValue;
}

uint8_t LoadCalibrationParameters(void)
{
    uint8_t LoopCnt;
    PageRead(NVM_CALIBRATION_PAGE_ADDRESS);
    for(LoopCnt=0;LoopCnt<sizeof(struct CalibrationParameters)/2;LoopCnt++)
        CalibrationParametes.CalibParameter_Words[LoopCnt]=ReadBuffer[LoopCnt]; 
    WeightStruc[0].Divition=DivitionTable[CalibrationParametes.CalibParameter[0].Divition_Index_P1];
    WeightStruc[0].offset=CalibrationParametes.CalibParameter[0].Zero_Atod;
    WeightStruc[1].Divition=DivitionTable[CalibrationParametes.CalibParameter[1].Divition_Index_P1];
    WeightStruc[1].offset=CalibrationParametes.CalibParameter[1].Zero_Atod;
    
    Loadcell_Id=SystemParm.Set_Parameter.DefualtChannel;

    if(CalibrationParametes.CalibParameter[0].IsCalibrated!=1 && CalibrationParametes.CalibParameter[1].IsCalibrated!=1)
        return 0;
    else
        return 1;
}

void UpdateSetting(void)
{
    uint16_t LoopCnt;
    for(LoopCnt=0;LoopCnt<sizeof(struct SettingParameters)/4;LoopCnt++)
        ReadBuffer[LoopCnt]=SystemParm.Set_Parameter_Words[LoopCnt];
    PageWrite (NVM_SETTING_PAGE_ADDRESS);
}

void UpdateCalibrationParameters(void)
{
    uint16_t LoopCnt;
    for(LoopCnt=0;LoopCnt<sizeof(struct CalibrationParameters)/2;LoopCnt++)
        ReadBuffer[LoopCnt]=CalibrationParametes.CalibParameter_Words[LoopCnt] ;
    PageWrite (NVM_CALIBRATION_PAGE_ADDRESS);
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
        SystemParm.Set_Parameter.BarRowHeight=100;
        SystemParm.Set_Parameter.BarEndHeight=100;
        SystemParm.Set_Parameter.BarLableHeight=100;
        SystemParm.Set_Parameter.Baudrate_Index=3;
        SystemParm.Set_Parameter.Backlight_Index=5;
        SystemParm.Set_Parameter.StoreNumber=1;
        SystemParm.Set_Parameter.ScaleNumber=1;
        SystemParm.Set_Parameter.Power_Save_En=1;
        SystemParm.Set_Parameter.Power_Save_Time=1;
        SystemParm.Set_Parameter.ScreenSaverShowTimeDate=1;
        SystemParm.Set_Parameter.Stable_Extent_Index=1;
        SystemParm.Set_Parameter.Stable_Time_Index=20;
        SystemParm.Set_Parameter.FreeLinePrint=2;
        SystemParm.Set_Parameter.SelectedManufacture=0;
        SystemParm.Set_Parameter.DualSyncLoadcell=0;
        SystemParm.Set_Parameter.DefualtChannel=0;
        SystemParm.Set_Parameter.ViewFormat=0;
        SystemParm.Set_Parameter.AutoSend=0;
        SystemParm.Set_Parameter.AutoSendNewOld=0;


        SystemParm.Set_Parameter.max_all_plu =MAX_ALL_PLU ;
        SystemParm.Set_Parameter.nvm_barcod_page_add=NVM_BARCODE_PAGE_ADDRESS;
        SystemParm.Set_Parameter.nvm_kala_info_page_add = NVM_KALA_INFO_PAGE_ADDRESS ;
        SystemParm.Set_Parameter.nvm_detail_sell_page_add = NVM_DETAIL_SELL_PAGE_ADDRESS ;
        SystemParm.Set_Parameter.SystemSecure = true;
        SystemParm.Set_Parameter.Rs232_Out_Index=EFT_POS;
        SystemParm.Set_Parameter.Filter_Index=9;
        SystemParm.Set_Parameter.SystemClients[ADMIN_BUFF_INDEX].ClientParameter.LoginPass=ADMIN_DEFAULT_PASS;
        SystemParm.Set_Parameter.LoginUserID = ADMINISTRATOR;
        SystemParm.Set_Parameter.RemoteServerPort=DEFAULT_SERVER_PORT;
        UpdateSetting();
    }
    if(LoadCalibrationParameters()==0)
    {
        Show_Dash();
        appData.state=APP_STATE_CALIBRATION_ERROR;
        NoCalibrate=true;
    }
    else
        appData.state=APP_STATE_COUNT_DOWN;
}


    
    
void InitVariables(void)
{
    uint8_t Cnt=0;
    ErrorCalibrationTimout=0;
    Run_Flag=true;
    OnOff_State=1;
    Onoff_Flag=0;
    EnableLedShow=true;
    Power_Save_Flag=0;
    ShledCnt=0;
    WeightCalculated=false;
    StartResolutionLog=false;
    ResLogCounter=0;
    
    for(;Cnt<2;Cnt++)
    {
        WeightStruc[Cnt].W1_Flag=0;
        WeightStruc[Cnt].Tare_Flag=0;
        WeightStruc[Cnt].Drift_Flag=0;
        WeightStruc[Cnt].Net_Flag=0;
        WeightStruc[Cnt].Over_Flag=0;
        WeightStruc[Cnt].Under_Flag=0;
        WeightStruc[Cnt].Stable_Flag=0;
        WeightStruc[Cnt].W1_Flag=0;
        WeightStruc[Cnt].W2_Flag=0;
        WeightStruc[Cnt].Zero_Flag=0;
        WeightStruc[Cnt].data_completed=0;
        WeightStruc[Cnt].neg_sign=0;
        WeightStruc[Cnt].w_neg=0;
    }

    CalibSettingState=0;
    NoCalibrate=false;
    CashCnt=0;
    ADC_Ch_SW=0;
    Loadcell_Id=0;
    SwitchWeight=0;
}


void CountDownService( void  )
{
    static uint8_t CountDownState=0,CountDownCounter_1=0,CountDownCounter_2=0;
    static bool done_1=0,done_2=0;
    
    switch(CountDownState)
    {
        case 0: 
            if(SystemParm.Set_Parameter.DualSyncLoadcell)
            {
                if(Weight_Tasks(0))
                {
                    Adjust_Zero(1,0,1,0);
                    CountDownCounter_1++;
                    if(CountDownCounter_1>30)
                    {
                        ResetCause=0xA3;
                        CountDownCounter_1=0;
                        done_1=true;
                    }
                }
                if(Weight_Tasks(1))
                {
                    Adjust_Zero(1,0,1,1);
                    CountDownCounter_2++;
                    if(CountDownCounter_2>30)
                    {
                        ResetCause=0xA3;
                        CountDownCounter_2=0;
                        done_2=true;
                    }
                }
                if(done_1 && done_2)
                {
                    appData.state=APP_STATE_NORMAL_MODE;
                    CountDownState=0;
                    CountDownCounter_1=0;
                    CountDownCounter_2=0;
                    done_1=false;
                    done_2=false;

                }
            }
            else
            {
                if(Weight_Tasks(Loadcell_Id))
                {
                    Adjust_Zero(1,0,1,Loadcell_Id);
                    CountDownCounter_1++;
                    if(CountDownCounter_1>30)
                    {
                        ResetCause=0xA3;
                        CountDownCounter_1=0;
                        appData.state=APP_STATE_NORMAL_MODE;
                        CountDownState=0;
                    }
                }
            }
        break;
    }
}


void InitMainPrograms(void)
{
    InitVariables();
    Load_Setting();
    if(ResetCause!=0xa3)
    {
        Show_Dash();
        beez_time=80;
    }
    if(SystemParm.Set_Parameter.DualSyncLoadcell==0)
        Channel_Selection=(bool)SystemParm.Set_Parameter.DefualtChannel;
    Display_Clock_1=0;
    AutoSend=false;
}
