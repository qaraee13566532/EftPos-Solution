#include "mcc_generated_files/system.h"
#include "variables.h"
#include "HardwareProfile.h"
#include <math.h>
#include <xc.h>

extern void WordWriteExample();
extern void Pc_Command_Services(void);

void DelayUs(uint32_t DelaySet)
{
    uint32_t longp=6*DelaySet;
    while(longp--)
        Nop();
}          

void NopDelay(void)
{
    Nop();
    Nop();
}
    

void Write_Serial_Buffer(uint8_t Data)
{
    UART3_Write(Data);
}


Print_NewLine(void)
{
    Write_Serial_Buffer(10);
    Write_Serial_Buffer(13);
}



void Print_Space( unsigned char Number)
{
	do
	{
		Write_Serial_Buffer(' ');
		Number--;
	}while(Number>0);

}

uint32_t   ChangeNumberDot(uint32_t  DotNumber)
{
	if(CalibrationParametes.CalibParameter[Loadcell_Id].Point_Index==0)
		DotNumber/=1000;
	if(CalibrationParametes.CalibParameter[Loadcell_Id].Point_Index==1)
		DotNumber/=100;
	if(CalibrationParametes.CalibParameter[Loadcell_Id].Point_Index==2)
		DotNumber/=10;
	return DotNumber;
}




void Send_Weight(int32_t ShowValue,bool DateTransmit,bool PrnUnit,bool ShowBack,bool ShDp,uint8_t ShowNormalWeight,bool GramUnit)
{
	uint8_t LoopCnt,Buf[10],EnableShow=0;
    if(ShowValue<0)
        Change_To_String(Buf,ChangeNumberDot(ShowValue*-1));
    else
        Change_To_String(Buf,ChangeNumberDot(ShowValue));
    
                
    if(ShowNormalWeight==1)
    {
        Print_NewLine();
        Write_Serial_Buffer('Z');
        Write_Serial_Buffer('E');
        Write_Serial_Buffer('R');
        Write_Serial_Buffer('O');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('A');
        Write_Serial_Buffer('D');
        Write_Serial_Buffer('C');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('C');
        Write_Serial_Buffer('A');
        Write_Serial_Buffer('L');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('=');
        Write_Serial_Buffer(' ');
    }
    if(ShowNormalWeight==2)
    {
        Print_NewLine();
        Write_Serial_Buffer('A');
        Write_Serial_Buffer('D');
        Write_Serial_Buffer('C');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('=');
        Write_Serial_Buffer(' ');
    }
                
    if(ShowNormalWeight==3)
    {
        Print_NewLine();
        Write_Serial_Buffer('M');
        Write_Serial_Buffer('A');
        Write_Serial_Buffer('X');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('A');
        Write_Serial_Buffer('D');
        Write_Serial_Buffer('C');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('=');
        Write_Serial_Buffer(' ');
    }
    if(ShowNormalWeight==4)
    {
        Print_NewLine();
        Write_Serial_Buffer('M');
        Write_Serial_Buffer('I');
        Write_Serial_Buffer('N');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('A');
        Write_Serial_Buffer('D');
        Write_Serial_Buffer('C');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('=');
        Write_Serial_Buffer(' ');
    }

    if(ShowNormalWeight==5)
    {
        Print_NewLine();
        Write_Serial_Buffer('D');
        Write_Serial_Buffer('I');
        Write_Serial_Buffer('F');
        Write_Serial_Buffer('F');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('=');
        Write_Serial_Buffer(' ');
    }
    if(ShowNormalWeight==6)
    {
        Print_NewLine();
        Write_Serial_Buffer('R');
        Write_Serial_Buffer('E');
        Write_Serial_Buffer('S');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('=');
        Write_Serial_Buffer(' ');
    }
    if(ShowNormalWeight==7)
    {
        Print_NewLine();
        Write_Serial_Buffer('B');
        Write_Serial_Buffer('o');
        Write_Serial_Buffer('a');
        Write_Serial_Buffer('r');
        Write_Serial_Buffer('d');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('v');
        Write_Serial_Buffer('e');
        Write_Serial_Buffer('r');
        Write_Serial_Buffer('s');
        Write_Serial_Buffer('i');
        Write_Serial_Buffer('o');
        Write_Serial_Buffer('n');
        Write_Serial_Buffer(' ');
        Write_Serial_Buffer('=');
    }

    
    if(Buf[9])
        EnableShow=1;
    if(ShowValue>=0 && !WeightStruc[Loadcell_Id].Over_Flag)
    {
        if(ShowBack)
            Write_Serial_Buffer('0'+Buf[9]);
        else
        {
            if(EnableShow)
                Write_Serial_Buffer('0'+Buf[9]);
        }        
    }
    else
        Write_Serial_Buffer('-');
    LoopCnt=9;
    do
    {
        if(Buf[LoopCnt-1])
            EnableShow=1;
        if(GramUnit==0)
        {
            if(LoopCnt==4 && CalibrationParametes.CalibParameter[Loadcell_Id].Point_Index==3 && ShowBack==0)
                EnableShow=1;
            if(LoopCnt==3 && CalibrationParametes.CalibParameter[Loadcell_Id].Point_Index==2 && ShowBack==0)
                EnableShow=1;
            if(LoopCnt==2 && CalibrationParametes.CalibParameter[Loadcell_Id].Point_Index==1 && ShowBack==0)
                EnableShow=1;
        }
        if(ShowNormalWeight==7)
        {    
            if(LoopCnt==3 && ShDp )
                Write_Serial_Buffer('.');
        }
        else
        if(LoopCnt==CalibrationParametes.CalibParameter[Loadcell_Id].Point_Index && ShDp )
            Write_Serial_Buffer('.');
            
        if(WeightStruc[Loadcell_Id].Over_Flag)
            Write_Serial_Buffer('-');
        else
        {
            if(ShowBack)
                Write_Serial_Buffer('0'+Buf[LoopCnt-1]);
            else
            {
                if(EnableShow)
                    Write_Serial_Buffer('0'+Buf[LoopCnt-1]);
            }
        }
        LoopCnt--;
    }
    while(LoopCnt>0);
    if(GramUnit==1 && ShowValue==0)
        Write_Serial_Buffer('0');
	if(PrnUnit)
	{
        Write_Serial_Buffer('k');
        Write_Serial_Buffer('g');
		Print_Space(1);
	}
	if(DateTransmit==true)
	{
//        GetDate();
		Print_NewLine();
		Print_Space(2);
//		Send_Date_Time();
	}    
}



void ShowAdcResolution(void )
{
	static int32_t minadc,maxadc,Counter=0;
    if(Counter=0)
    {
        minadc=ADC_DATA;
        maxadc=minadc;
    }
    if(ADC_DATA>maxadc)
        maxadc=ADC_DATA;
    if(ADC_DATA<minadc)
        minadc=ADC_DATA;
    in_data=(24-(log(maxadc-minadc)/log(2)))*10;
    in_data*=10;
    Counter++;
    
}

void Global_Services(void)
{
    ADC1_Tasks();
    Pc_Command_Services();
}


int main(void)
{
    SYSTEM_Initialize();
    InitMainPrograms();
    for(;;)
    {
        Global_Services();
    }
}

