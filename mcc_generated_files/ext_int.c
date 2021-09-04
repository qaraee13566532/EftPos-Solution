
#include <xc.h>
#include "ext_int.h"
#include "../HardwareProfile.h"
#include "../MyMain.h"
#include "../define_type.h"
#include <math.h>



extern void Send_Weight(int32_t ShowValue,bool DateTransmit,bool PrnUnit,bool ShowBack,bool ShDp,uint8_t ShowNormalWeight,bool GramUnit);

extern struct WStructure WeightStruc[2];
extern union CalParm CalibrationParametes;

extern uint8_t ADC_Ch_SW,Loadcell_Id,ResLogCounter;

extern int32_t ADC_DATA,indata;

extern union SetParameter SystemParm;
extern bool StartResolutionLog;

extern uint16_t beez_time;



void ATOD_CLOCK(void)
{
    DelayUs(1);
	Adc_Clock=0;
    DelayUs(1);
	Adc_Clock=1;
    DelayUs(1);
}

 void __attribute__ ((weak)) EX_INT2_CallBack(void)
{
    static uint8_t channelcnt=0;
    uint8_t  Adc_Count;
    int32_t Adc_Out_Data=0;
    bool neg_sign=0;
    EX_INT2_InterruptDisable();

    Adc_Out_Data=0;
    
        
    for(Adc_Count=0;Adc_Count<24;++Adc_Count)     
    {
        ATOD_CLOCK();
        Adc_Out_Data*=2;
        if(Adc_Data)
            Adc_Out_Data+=1;
    }
    if(Adc_Out_Data!=(int32_t)(0xffffff) && Adc_Out_Data!=(int32_t)(0x0) && Adc_Out_Data<(int32_t)(0xffffff) && Adc_Out_Data>(int32_t)(0))
    {
        ADC_DATA=Adc_Out_Data-(int32_t)0x00800000;
        if(ADC_DATA<0)
            neg_sign=1;
    }
       
    if(SystemParm.Set_Parameter.DualSyncLoadcell)
    {
        if(ADC_Ch_SW==0)
        {
            ADC_Ch_SW=1;
            Channel_Selection=CHANNEL_B;
            WeightStruc[0].ADC_DATA=ADC_DATA;
            WeightStruc[0].data_completed=1;
            WeightStruc[0].neg_sign=neg_sign;
        }
        else
        {
            ADC_Ch_SW=0;
            Channel_Selection=CHANNEL_A;
            WeightStruc[1].ADC_DATA=ADC_DATA;
            WeightStruc[1].data_completed=1;
            WeightStruc[1].neg_sign=neg_sign;
        }
    }
    else
    {
        WeightStruc[Loadcell_Id].ADC_DATA=ADC_DATA;
        WeightStruc[Loadcell_Id].data_completed=1;
        WeightStruc[Loadcell_Id].data_completed=1;
        WeightStruc[Loadcell_Id].neg_sign=neg_sign;
        if(StartResolutionLog)
        {
            if(WeightStruc[Loadcell_Id].ADC_DATA>WeightStruc[Loadcell_Id].maxAdc)
                WeightStruc[Loadcell_Id].maxAdc=WeightStruc[Loadcell_Id].ADC_DATA;
            if(WeightStruc[Loadcell_Id].ADC_DATA<WeightStruc[Loadcell_Id].minAdc)
                WeightStruc[Loadcell_Id].minAdc=WeightStruc[Loadcell_Id].ADC_DATA;
            ResLogCounter++;
            if(ResLogCounter>=50)
            {
                ResLogCounter=0;
                indata=(24-(log(labs(WeightStruc[Loadcell_Id].maxAdc-WeightStruc[Loadcell_Id].minAdc))/log(2)))*10;
                Send_Weight(CalibrationParametes.CalibParameter[Loadcell_Id].Zero_Atod,0,0,0,0,1,0);
                Send_Weight(WeightStruc[Loadcell_Id].ADC_DATA,0,0,0,0,2,0);
                Send_Weight(WeightStruc[Loadcell_Id].maxAdc,0,0,0,0,3,0);
                Send_Weight(WeightStruc[Loadcell_Id].minAdc,0,0,0,0,4,0);
                Send_Weight(labs(WeightStruc[Loadcell_Id].maxAdc-WeightStruc[Loadcell_Id].minAdc),0,0,0,0,5,0);
                Send_Weight(indata,0,0,0,0,6,0);
                Send_Weight(1001,0,0,0,1,7,0);
                StartResolutionLog=false;
                if(indata<170)
                    beez_time=BEEP_500ms;
                else
                    beez_time=BEEP_50ms;
            }
        }
    }

    EX_INT2_InterruptEnable();

}
 
/**
  Interrupt Handler for EX_INT2 - INT2
*/
void __attribute__ ( ( vector ( _EXTERNAL_2_VECTOR ), interrupt ( IPL1SOFT ) ) ) _EXTERNAL_2_ISR (void)
{
    //***User Area Begin->code: External 2***
    
	// EX_INT2 callback function 
     EX_INT2_CallBack();
    
	//***User Area End->code: External 2***
     EX_INT2_InterruptFlagClear();
}

 void __attribute__ ((weak)) EX_INT4_CallBack(void)
{
    // Add your custom callback code here
}

 
/**
  Interrupt Handler for EX_INT4 - INT4
*/
void __attribute__ ( ( vector ( _EXTERNAL_4_VECTOR ), interrupt ( IPL1SOFT ) ) ) _EXTERNAL_4_ISR (void)
{
    //***User Area Begin->code: External 4***

	// EX_INT4 callback function 
	EX_INT4_CallBack();
    
    //***User Area End->code: External 4***
   // EX_INT4_InterruptFlagClear();
}
/**
    Section: External Interrupt Initializers
 */
/**
    void EXT_INT_Initialize(void)

    Initializer for the following external interrupts
    INT2
*/
void EXT_INT_Initialize(void)
{
    /*******
     * INT2
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT2_InterruptFlagClear();   
    EX_INT2_NegativeEdgeSet();
    EX_INT2_InterruptEnable();
}
