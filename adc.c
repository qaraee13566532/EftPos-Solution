
#include <xc.h>
#include "HardwareProfile.h"
#include "MyMain.h"
#include "define_type.h"


extern struct WStructure WeightStruc[2];
extern union CalParm CalibrationParametes;
extern union SetParameter SystemParm;
extern uint8_t  One_Gram,Fi_Ind,CalibProcess,disp[DISP_NO],Video_Ram_Backup[DISP_NO],ErrorCalibrationTimout,CalibSettingState,Loadcell_Id,SwitchWeight;

extern int32_t ADC_DATA;

extern int32_t  SavedOffset[2]  __attribute__ ((persistent));

extern bool    Power_Save_Flag,Onoff_Flag,NoCalibrate,WeightCalculated,
               ClFlag,OnOff_State,Run_Flag,EnableLedShow,AddToList,LockMode;
extern APP_DATA appData;
extern uint16_t Generic_Counter2,Drift_Counter,beez_time;
extern uint32_t in_data,Power_Save_Counter,Generic_Counter3,Plu_Unit_Price;
extern uint32_t ReadBuffer[512];

extern  const uint16_t  STABLE_TIME[3];
extern  double ZeroCheck;
extern  const uint16_t  DivitionTable[13];

extern uint32_t Change_To_long(uint8_t Buf[10]);
 
uint32_t mylabs (int32_t Number)
{
	if(Number>=0)
		return Number;
	else
		return (Number*(int32_t)-1);
}


uint32_t RealNumber( uint32_t Number,uint8_t Lid)
{
	uint32_t TempMul=1;
    uint8_t LoopCnt;
    LoopCnt=0;
	while(LoopCnt<(3-CalibrationParametes.CalibParameter[Lid].Point_Index))
	{
		TempMul*=10;
		LoopCnt++;
	}
	Number*=TempMul;
	return Number;
}



int32_t N_Round ( double Input ,uint8_t Lid)
{
    double halfDiv=0,fullDiv=0;
    uint32_t MaxCapacity=RealNumber(CalibrationParametes.CalibParameter[Lid].Max_weight_P2,Lid);
    int32_t val=Input;
//	if(MaxCapacity<(uint32_t)100000)
//	{
        if(WeightStruc[Lid].Divition==1)
        {
            return (int32_t) (Input+(double)0.5);
        }
        else
            Input = (int32_t) Input;
		if(One_Gram==0)
		{
/*			if(WeightStruc[Lid].Divition<5)
			{
				if((int32_t)Input>(int32_t)5000)
				{
					if(MaxCapacity<=30000)
						Input = ((Input+2)/5)*5;
					else
					if(MaxCapacity<=40000)
						Input = ((Input+5)/10)*10;
					else
						Input = (Input/20)*20;
					WeightStruc[Lid].W2_Flag=1;
					WeightStruc[Lid].W1_Flag=0;
				}
				else
				{
					if(Input>=0)
                        Input = ((Input+WeightStruc[Lid].Divition/2)/WeightStruc[Lid].Divition)*WeightStruc[Lid].Divition;
                    else
                        Input = ((Input-WeightStruc[Lid].Divition/2)/WeightStruc[Lid].Divition)*WeightStruc[Lid].Divition;
					WeightStruc[Lid].W1_Flag=1;
					WeightStruc[Lid].W2_Flag=0;
				}	
			}
			else
			{*/
         //   if(WeightStruc[Lid].Divition>1)
         //   {

				if((int32_t)Input<=(int32_t)(CalibrationParametes.CalibParameter[Lid].Max_weight_P1))
				{
                    WeightStruc[Lid].Divition=DivitionTable[CalibrationParametes.CalibParameter[Lid].Divition_Index_P1];
					WeightStruc[Lid].W1_Flag=1;
					WeightStruc[Lid].W2_Flag=0;
				}
				else
				{
                    WeightStruc[Lid].Divition=DivitionTable[CalibrationParametes.CalibParameter[Lid].Divition_Index_P2];
                    WeightStruc[Lid].W2_Flag=1;
                    WeightStruc[Lid].W1_Flag=0;
				}	
                val = ((val+WeightStruc[Lid].Divition/2)/WeightStruc[Lid].Divition)*WeightStruc[Lid].Divition;
                //Input=((Input+halfDiv)/fullDiv)*fullDiv;
                
		//	}				
        //    else
        //    {
           //     WeightStruc[Lid].W1_Flag=1;
           //     WeightStruc[Lid].W2_Flag=0;
           // }
		}
		else
		{
			WeightStruc[Lid].W1_Flag=1;
			WeightStruc[Lid].W2_Flag=0;
		}
/*	}
	else
	{
		Input = ((Input+WeightStruc[Lid].Divition/2)/WeightStruc[Lid].Divition)*WeightStruc[Lid].Divition;
		WeightStruc[Lid].W1_Flag=1;
		WeightStruc[Lid].W2_Flag=0;
	}	*/
	/*if(WeightStruc[Lid].Divition==1)
	{
		if(mylabs(Input)<2)
			Input=0;
	}*/	
	return val;
}




extern fir_t fir_ftr;

void fir_create(fir_t *context, float *filter_coeff, uint32_t filter_size, float gain, uint32_t sample_time)
{
    context->size = filter_size;
    context->gain = gain;
    free(context->tab);
    context->tab = (int32_t*)malloc(filter_size * sizeof(int32_t));
    context->coeff = filter_coeff;
    context->index = 0;
    context->filtered = 0;
    context->sample_time = sample_time;
    context->last_time = 0;
}

void fir_filter(fir_t *context, int32_t input)
{
    int32_t sum = 0;
    int16_t i=0;
    context->tab[context->index] = input;
    for (i = 0; i < context->size; i++)
    {
      sum += context->coeff[i] * context->tab[(i + context->index) % context->size];
    }
    context->index = (context->index + 1) % context->size;
    context->filtered = (int32_t)(sum * context->gain);
}



void Calculate_Weight(uint8_t Lid)
{
    uint8_t LoopCnt;
    uint8_t gfilter=15;
    double Res=0;
    static double filterdWeight[2][15];
    static double maxW,upmaxW;
    Res=WeightStruc[Lid].ADC_WITHOUT_OFFSET;
    Res= (Res*CalibrationParametes.CalibParameter[Lid].W_factor.factor);
    WeightStruc[Lid].Weight=Res;
    
    if(mylabs(WeightStruc[Lid].Weight-WeightStruc[Lid].FilterTempWeight)<2*WeightStruc[Lid].Divition)
    {
        LoopCnt=gfilter-1;
        do
        {
            filterdWeight[Lid][LoopCnt]=filterdWeight[Lid][LoopCnt-1];
            LoopCnt--;
        }while(LoopCnt>0);
        filterdWeight[Lid][0]=Res;
    
        maxW=0;
        for(LoopCnt=0;LoopCnt<gfilter;LoopCnt++)
            maxW+=filterdWeight[Lid][LoopCnt];
        maxW/=(double)gfilter;
        WeightStruc[Lid].Round_Weight=N_Round(maxW,Lid);
        
       /* maxW=filterdWeight[Lid][0];
        for(LoopCnt=0;LoopCnt<gfilter;LoopCnt++)
            if(maxW<filterdWeight[Lid][LoopCnt])
                maxW=filterdWeight[Lid][LoopCnt];
        
        if(labs(maxW-upmaxW)>0.9)
        {
            upmaxW=maxW;
            WeightStruc[Lid].Round_Weight=N_Round(maxW,Lid);
        }
        else
            WeightStruc[Lid].Round_Weight=N_Round(WeightStruc[Lid].Round_Weight,Lid);*/
        if(WeightStruc[Lid].Round_Weight==100)
            WeightStruc[Lid].Round_Weight=100;
    }
    else
    {
        WeightStruc[Lid].Round_Weight=N_Round(Res,Lid);
        WeightStruc[Lid].FilterTempWeight=Res;
        for(LoopCnt=0;LoopCnt<gfilter;LoopCnt++)
            filterdWeight[Lid][LoopCnt]=Res;
    }
 }
 
/*

void Calculate_Weight(uint8_t Lid)
{
    uint8_t LoopCnt=Fi_Ind-1;
    double Res=0;
    static int32_t AdcArray[2][21];
    int32_t minadc,maxadc;
    do
    {
        AdcArray[Lid][LoopCnt]=AdcArray[Lid][LoopCnt-1];
        LoopCnt--;
    }while(LoopCnt>0);
    AdcArray[Lid][0]=WeightStruc[Lid].ADC_WITHOUT_OFFSET;
    maxadc=minadc=AdcArray[Lid][0];
    for(LoopCnt=0;LoopCnt<Fi_Ind;LoopCnt++)
    {
        if(maxadc<AdcArray[Lid][LoopCnt])
            maxadc=AdcArray[Lid][LoopCnt];
        if(minadc>AdcArray[Lid][LoopCnt])
            minadc=AdcArray[Lid][LoopCnt];
    }
    //Res=(maxadc+minadc)/2;
    Res=maxadc;
    //fir_filter(&fir_ftr,WeightStruc[Lid].ADC_WITHOUT_OFFSET);
    Res= (Res*CalibrationParametes.CalibParameter[Lid].W_factor.factor);
    WeightStruc[Lid].Weight=Res;
    WeightStruc[Lid].Round_Weight=N_Round(Res,Lid);
 }
 */


/*
void Calculate_Weight(uint8_t Lid)
{
    uint8_t LoopCnt=Fi_Ind-1;
    double Res=0;
    static int32_t AdcArray[2][21];
    do
    {
        AdcArray[Lid][LoopCnt]=AdcArray[Lid][LoopCnt-1];
        LoopCnt--;
    }while(LoopCnt>0);
    AdcArray[Lid][0]=WeightStruc[Lid].ADC_WITHOUT_OFFSET;
    for(LoopCnt=0;LoopCnt<Fi_Ind;LoopCnt++)
        Res+=AdcArray[Lid][LoopCnt];
    Res/=Fi_Ind;
    //WeightStruc[Lid].Weight= (Res*CalibrationParametes.CalibParameter[Lid].W_factor.factor);
    fir_filter(&fir_ftr,WeightStruc[Lid].ADC_WITHOUT_OFFSET);
 //   Res=fir_ftr.filtered;
    WeightStruc[Lid].Weight= ((double)fir_ftr.filtered*CalibrationParametes.CalibParameter[Lid].W_factor.factor);
 }
 */


 /*

void Calculate_Weight( uint8_t Lid  )
{
//	WeightStruc[Lid].Weight= ((double)WeightStruc[Lid].ADC_WITHOUT_OFFSET*CalibrationParametes.CalibParameter[Lid].W_factor.factor+(double)0.5);
	WeightStruc[Lid].Weight= ((double)WeightStruc[Lid].ADC_WITHOUT_OFFSET*CalibrationParametes.CalibParameter[Lid].W_factor.factor);
}
*/


uint8_t Get_Weight (uint8_t Lid)
{
	uint8_t Sig_Cnt;
	if(WeightStruc[Lid].data_completed )
	{
		WeightStruc[Lid].data_completed=0;
		WeightStruc[Lid].ADC_WITHOUT_OFFSET=WeightStruc[Lid].ADC_DATA-WeightStruc[Lid].offset;
        SavedOffset[Lid]=WeightStruc[Lid].offset;
		if(WeightStruc[Lid].Tare_Flag)
		{
            
			WeightStruc[Lid].Tare_Flag=0;
			if(WeightStruc[Lid].Stable_Flag && WeightStruc[Lid].Round_Weight>=0 )
			{
				WeightStruc[Lid].Tare_Weight=WeightStruc[Lid].Round_Weight;
                WeightStruc[Lid].Net_Flag =1;
			}
			if(WeightStruc[Lid].Round_Weight==0)
				WeightStruc[Lid].Net_Flag =0;
		}
        Calculate_Weight(Lid);
		return 1;
	}
	return 0;
}

void Check_Stable( uint8_t Lid )
{
    if(CalibrationParametes.CalibParameter[Lid].Stable_Extent_Index==0 || LockMode==true)
	{
        WeightStruc[Lid].Stable_Flag=1;
        return ;
    }
	if(mylabs(WeightStruc[Lid].Temp_Weight-WeightStruc[Lid].View_Weight) >= ((CalibrationParametes.CalibParameter[Lid].Stable_Extent_Index)*WeightStruc[Lid].Divition))
	{
		WeightStruc[Lid].Temp_Weight=WeightStruc[Lid].View_Weight;
		Generic_Counter2=0;
		WeightStruc[Lid].Stable_Flag=0;
	}
	else
	{
		if(Generic_Counter2>=((CalibrationParametes.CalibParameter[Lid].Stable_Time_Index+1)*SAMPLE_TIME_MILI_SECONT))
			if(OnOff_State && Run_Flag)
			{
                WeightStruc[Lid].Stable_Flag=1;
                if(CalibrationParametes.CalibParameter[Lid].Lock_Functionality>0)
                    LockMode=true;
            }
	}
}	
   
void FilterWeight(uint8_t Lid)
{
/*	uint8_t LoopCnt,Sig_Cnt;
    double average=0;
    if(CalibrationParametes.CalibParameter[Lid].Filter_Index)
    {
        Fi_Ind=CalibrationParametes.CalibParameter[Lid].Filter_Index;
        LoopCnt=Fi_Ind-1;
        if(mylabs(WeightStruc[Lid].Weight-WeightStruc[Lid].FilterTempWeight)<4*WeightStruc[Lid].Divition)
        {
            do
            {
                WeightStruc[Lid].signal[LoopCnt]=WeightStruc[Lid].signal[LoopCnt-1];
                LoopCnt--;
            }while(LoopCnt>0);
            WeightStruc[Lid].signal[0]=WeightStruc[Lid].Weight;
        //    WeightStruc[Lid].Round_Weight=0;
            for(LoopCnt=0;LoopCnt<Fi_Ind;LoopCnt++)
            //    WeightStruc[Lid].Round_Weight+=WeightStruc[Lid].signal[LoopCnt];
                average+=WeightStruc[Lid].signal[LoopCnt];
       //     WeightStruc[Lid].Round_Weight/=Fi_Ind;
            average/=Fi_Ind;
            if(appData.state!=APP_STATE_CALIBRATION)
                WeightStruc[Lid].Round_Weight=N_Round(average,Lid);
            else
                WeightStruc[Lid].Round_Weight=WeightStruc[Lid].Weight;
            WeightStruc[Lid].FilterTempWeight=WeightStruc[Lid].Weight;
        }
        else
        {
            if(appData.state!=APP_STATE_CALIBRATION)
                WeightStruc[Lid].Round_Weight=N_Round(WeightStruc[Lid].Weight,Lid);
            else
                WeightStruc[Lid].Round_Weight=WeightStruc[Lid].Weight;
            WeightStruc[Lid].FilterTempWeight=WeightStruc[Lid].Weight;
            for(LoopCnt=0;LoopCnt<Fi_Ind;LoopCnt++)
                WeightStruc[Lid].signal[LoopCnt]=WeightStruc[Lid].Weight;
        }
    }
    else
    {
        if(appData.state!=APP_STATE_CALIBRATION)
            WeightStruc[Lid].Round_Weight=N_Round(WeightStruc[Lid].Weight,Lid);
        else
            WeightStruc[Lid].Round_Weight=WeightStruc[Lid].Weight;
    }*/
}

void ShowZero( uint8_t Lid )
{
	if(OnOff_State && Run_Flag)
	{
		if(mylabs(WeightStruc[Lid].Round_Weight)<WeightStruc[Lid].Divition )
			WeightStruc[Lid].Zero_Flag=1;
		else
			WeightStruc[Lid].Zero_Flag=0;
	}
}


uint8_t Adjust_Zero ( uint8_t  Start,uint8_t ZeroTrack,uint8_t bypass,uint8_t Lid )
{
    if(bypass)
    {
        WeightStruc[Lid].offset=WeightStruc[Lid].ADC_DATA;
        WeightStruc[Lid].Tare_Weight=0;
        WeightStruc[Lid].Net_Flag=0;
        return 0;
    }
    else
    {
        
        ZeroCheck=CalibrationParametes.CalibParameter[Lid].Zero_Percentage*RealNumber(CalibrationParametes.CalibParameter[Lid].Max_weight_P2,Lid);
        ZeroCheck/=100;
        ZeroCheck/=CalibrationParametes.CalibParameter[Lid].W_factor.factor;
        if(WeightStruc[Lid].ADC_DATA-CalibrationParametes.CalibParameter[Lid].Zero_Atod<ZeroCheck)
        {
            if(WeightStruc[Lid].Stable_Flag || Start)
            {
                if(WeightStruc[Lid].Tare_Weight==0 || ZeroTrack==1)
                {
                    WeightStruc[Lid].offset=WeightStruc[Lid].ADC_DATA;
                    WeightStruc[Lid].Tare_Weight=0;
                    WeightStruc[Lid].Net_Flag=0;
                           // beez_time=50;
                    return 0;
                }
            }
            else
                return 1;
        }
        else
            return 2;
    }
}

/*
void Del_Drift( uint8_t Lid )
{
	if(mylabs(WeightStruc[Lid].Weight)<=WeightStruc[Lid].Divition && WeightStruc[Lid].Weight!=0)
		WeightStruc[Lid].Drift_Flag=1;
	else
	{
		WeightStruc[Lid].Drift_Flag=0;
		WeightStruc[Lid].Drift_Counter=0;
	}
	if(WeightStruc[Lid].Drift_Flag && WeightStruc[Lid].Drift_Counter>= 2000)
		Adjust_Zero(0,1,0,Lid);	
}
*/

 
void Del_Drift( uint8_t Lid )
{
    double Margin;
    Margin = CalibrationParametes.CalibParameter[Lid].W_factor.factor;
    
    Margin=(int)((WeightStruc[Lid].Divition/(Margin))*0.7);
	if(mylabs(WeightStruc[Lid].ADC_WITHOUT_OFFSET)<=Margin && WeightStruc[Lid].ADC_WITHOUT_OFFSET!=0 && WeightStruc[Lid].Net_Flag==false)
		WeightStruc[Lid].Drift_Flag=1;
	else
	{
		WeightStruc[Lid].Drift_Flag=0;
		WeightStruc[Lid].Drift_Counter=0;
	}
	if(WeightStruc[Lid].Drift_Flag && WeightStruc[Lid].Drift_Counter>= 500)
	{
        Adjust_Zero(0,1,0,Lid);	
        WeightStruc[Lid].Drift_Flag=0;
        Drift_Counter=0;

    }
}

uint8_t Weight_Tasks( uint8_t Lid )
{
	uint8_t LoopCnt;
    int32_t TempCalc1,TempCalc2;
    if(Get_Weight(Lid))
	{
		Check_Stable(Lid);
     //   WeightStruc[Lid].Round_Weight=WeightStruc[Lid].Weight;
    //    FilterWeight(Lid);
		ShowZero(Lid);
		Del_Drift(Lid);
        
        
        if(CalibrationParametes.CalibParameter[Lid].Filter_Index==0)
            LockMode=false;
        if(LockMode==false)
            WeightStruc[Lid].View_Weight=WeightStruc[Lid].Round_Weight-WeightStruc[Lid].Tare_Weight;
        else
        {
            TempCalc1=WeightStruc[Lid].View_Weight;
            TempCalc2=WeightStruc[Lid].Round_Weight;
            TempCalc1=TempCalc1-TempCalc2;
            TempCalc1=TempCalc1+WeightStruc[Lid].Tare_Weight;
            if(TempCalc1<0)
                TempCalc1*=-1;
            if(TempCalc1>(uint16_t)(CalibrationParametes.CalibParameter[Lid].Number_Of_Locked_Divisions*WeightStruc[Lid].Divition))
            {
                LockMode=false;
                Generic_Counter2=0;
                WeightStruc[Lid].Temp_Weight=0;
                WeightStruc[Lid].Stable_Flag=0;
            }
        }
        
        
        
    	//WeightStruc[Lid].View_Weight=WeightStruc[Lid].Round_Weight-WeightStruc[Lid].Tare_Weight;

        if(WeightStruc[0].Round_Weight>(int32_t)(RealNumber(CalibrationParametes.CalibParameter[0].Max_weight_P2,0)+9*DivitionTable[CalibrationParametes.CalibParameter[0].Divition_Index_P2]) )
            WeightStruc[0].Over_Flag=true;
        else
            WeightStruc[0].Over_Flag=false;
        if(WeightStruc[1].Round_Weight>(int32_t)(RealNumber(CalibrationParametes.CalibParameter[1].Max_weight_P2,0)+9*DivitionTable[CalibrationParametes.CalibParameter[1].Divition_Index_P2]) )
            WeightStruc[1].Over_Flag=true;
        else
            WeightStruc[1].Over_Flag=false;
                                                                                    
        if(WeightStruc[0].Round_Weight<(int32_t)(20*DivitionTable[CalibrationParametes.CalibParameter[0].Divition_Index_P1]) *-1)
            WeightStruc[0].Under_Flag=true;
        else
            WeightStruc[0].Under_Flag=false;
        
        if(WeightStruc[1].Round_Weight<(int32_t)(20*DivitionTable[CalibrationParametes.CalibParameter[1].Divition_Index_P1])*-1 )
            WeightStruc[1].Under_Flag=true;
        else
            WeightStruc[1].Under_Flag=false;

		if(WeightStruc[Lid].View_Weight<0)
			WeightStruc[Lid].w_neg=1;
		else
			WeightStruc[Lid].w_neg=0;
        
        if(SystemParm.Set_Parameter.ViewFormat==2)
        {
            if(WeightStruc[0].View_Weight==0 && WeightStruc[1].View_Weight>0 )
                SwitchWeight=1;
            if(WeightStruc[0].View_Weight>0 && WeightStruc[1].View_Weight==0 )
                SwitchWeight=0;
        }
        WeightCalculated=true;

            
        /*    
		if(mylabs((WeightStruc[Lid].Round_Weight-WeightStruc[Lid].Pre_Weight))>(uint32_t)MAX_DIFF*WeightStruc[Lid].Divition)
		{
			WeightStruc[Lid].Pre_Weight=WeightStruc[Lid].Round_Weight;
			Power_Save_Counter=0;
            Generic_Counter3=0;
            if(appData.state==APP_SHOW_TIME_DATE_STATE)
            {
                appData.state=APP_STATE_NORMAL_MODE;
                EnableLedShow=1;
            }

			if( Power_Save_Flag)
			{
				beez_time=BEEP_100ms;
				Power_Save_Flag=0;
				Onoff_Flag=0;
				OnOff_State=1;
				for(LoopCnt=0;LoopCnt<DISP_NO;LoopCnt++)
					disp[LoopCnt]=Video_Ram_Backup[LoopCnt];
				Generic_Counter2=0;
			}
			if(AddToList)
			{
				beez_time=BEEP_100ms;
				AddToList=0;
				Plu_Unit_Price=0;
			}
		}*/
		return 1;
	}
	else
		return 0;
}
void WriteErrorMessage(void)
{
    UART3_Write(13);
    UART3_Write(10);
    UART3_Write(13);
    UART3_Write(10);
    UART3_Write('n');
    UART3_Write('o');
    UART3_Write(' ');
    UART3_Write('c');
    UART3_Write('a');
    UART3_Write('l');
    UART3_Write('i');
    UART3_Write('b');
    UART3_Write('r');
    UART3_Write('a');
    UART3_Write('t');
    UART3_Write('i');
    UART3_Write('o');
    UART3_Write('n');
    UART3_Write(' ');
    UART3_Write('!');
    UART3_Write('!');
    UART3_Write('!');
    UART3_Write(13);
    UART3_Write(10);
    UART3_Write(13);
    UART3_Write(10);
}



void LoadCalibParameterAndWrite ( void )
{
    uint16_t LoopCnt;
    for(LoopCnt=0;LoopCnt<sizeof(struct CalibrationParameters)/2;LoopCnt++)
        ReadBuffer[LoopCnt]=CalibrationParametes.CalibParameter_Words[LoopCnt]; 
}

uint8_t  CalibrateService( uint8_t Lid )
{
    if(Weight_Tasks(Lid))
    {
        if(mylabs(WeightStruc[Lid].Round_Weight-RealNumber(CalibrationParametes.CalibParameter[Lid].Calibration_Weight_Number,Lid))<=(DivitionTable[CalibrationParametes.CalibParameter[Lid].Divition_Index_P1]) || CalibProcess>100)
        {
            if(CalibProcess>=100)
            {
                CalibProcess=0;
                CalibrationParametes.CalibParameter[Lid].IsCalibrated=1;
                CalibrationParametes.CalibParameter[Lid].Zero_Atod=WeightStruc[Lid].offset;
                LoadCalibParameterAndWrite();
                if(PageWrite(NVM_CALIBRATION_PAGE_ADDRESS)!=3)
                {
                    UART3_Write('W');
                    UART3_Write('r');
                    UART3_Write('i');
                    UART3_Write('t');
                    UART3_Write('e');
                    UART3_Write(' ');
                    UART3_Write('E');
                    UART3_Write('r');
                    UART3_Write('r');
                    UART3_Write('o');
                    UART3_Write('r');
                    UART3_Write(' ');
                    UART3_Write('!');
                    UART3_Write('!');
                    UART3_Write('!');
                    UART3_Write(13);
                    UART3_Write(10); 
                    appData.state=APP_STATE_NORMAL_MODE;
                    return 0;
                }
                else
                {
                    UART3_Write('D');
                    UART3_Write('o');
                    UART3_Write('n');
                    UART3_Write('e');
                    UART3_Write(' ');
                    UART3_Write('.');
                    UART3_Write('.');
                    UART3_Write('.');
                    UART3_Write(13);
                    UART3_Write(10);
                    SendTakinFuncrions(0x01,0x23,0x30);
                    appData.state=APP_STATE_NORMAL_MODE;
                    NoCalibrate=false;
                    LockMode=false;
                    return 2;
                }
            }
            else
            {
                CalibProcess+=2;
                ErrorCalibrationTimout=0;
            }
        }
        else
        {
            CalibrationParametes.CalibParameter[Lid].W_factor.factor=((double)RealNumber(CalibrationParametes.CalibParameter[Lid].Calibration_Weight_Number,Lid))/((double)WeightStruc[Lid].ADC_WITHOUT_OFFSET);
            CalibProcess=0;
            ErrorCalibrationTimout++;
        }
        if(CalibProcess<99)
        {
            UART3_Write(CalibProcess/10+'0');
            UART3_Write(CalibProcess%10+'0');
        }
        else
        {
            UART3_Write('1');
            UART3_Write(( CalibProcess%100)/10+'0');
            UART3_Write(( CalibProcess%100)%10+'0');
        }
        UART3_Write('%');
        UART3_Write(13);
        UART3_Write(10);
        if(ErrorCalibrationTimout>=100)
        {
            appData.state=APP_STATE_CALIBRATION_ERROR;
            UART3_Write('F');
            UART3_Write('a');
            UART3_Write('i');
            UART3_Write('l');
            UART3_Write(' ');
            UART3_Write('!');
            UART3_Write('!');
            UART3_Write('!');
            UART3_Write(13);
            UART3_Write(10);
            return 1;
        }
    }
}
   