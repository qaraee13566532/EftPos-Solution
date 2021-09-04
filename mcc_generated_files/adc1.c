
/**
  ADC1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    adc1.c

  @Summary
    This is the generated header file for the ADC1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for ADC1.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.145.0
        Device            :  PIC32MM0256GPM064
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36b
        MPLAB 	          :  MPLAB X v5.25
*/

/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "system.h"
#include "adc1.h"
#include "../HardwareProfile.h"
#include "../MyMain.h"
#include "../define_type.h"
#include "usb/usb_hal_pic32mm.h"

/**
  Section: Data Type Definitions
*/

/* ADC Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintenance of the hardware instance.

  @Description
    This defines the object required for the maintenance of the hardware
    instance. This object exists once per hardware instance of the peripheral.

 */


extern bool Count_Flag,Count_Flag,Kg_Flag,Lb_Flag,Bat_Flag,Send_Flag,Recive_Flag,Onoff_Flag,Func_Flag,
     Send_Led,Show_Back_Zero,Show_Decimal_Point,Show_Sign,key_pressed,OnOff_State,StartToTransmition,Clock_Source,PowerSaver,disp_mask,Power_Save_Flag,
     Save_Cal_Paramerets_Flag,Run_Flag,Mask_Weight,STCC,StartRelay,direct_vendor,Numeral_Vendor,
     ADDFIX_direct,ADDFIX_Numeral,Shift_Flag,ShowDp,Cash_Open,AddToList,Mem_Flag,Payback_Flag,SentFlag,Dc_Power_Good,Bat_Power_Good,Shut_Down,Bat_Blink,
     USBHostBusIsActived,HidReadEnableFlag, GetHidPacket, En_Write_Flag,Net_Send_Flag ,Net_Recive_Flag,En_Setup_Flag;

typedef struct
{
	uint8_t intSample;
}

ADC_OBJECT;

static ADC_OBJECT adc1_obj;

/**
  Section: Driver Interface
*/

extern uint16_t BAT_VALUE,DC_VALUE,DcValHigh,BatCounter,
                OffCounter,ShutDownCounter,beez_time,Generic_Counter;
extern uint8_t BatLowDetect,Refresh_Set,DispPos;
extern APP_DATA appData;


void ADC1_Initialize (void)
{
    // ASAM enabled; DONE disabled; FORM Integer 16-bit; SAMP disabled; SSRC Internal counter ends sampling and starts conversion; SIDL disabled; MODE12 12-bit; ON enabled; 

   AD1CON1 = 0x807C;

    // CSCNA enabled; BUFM disabled; SMPI Generates interrupt after completion of every 2nd sample/conversion operation; OFFCAL disabled; VCFG AVDD/AVSS; BUFREGEN disabled; 

   AD1CON2 = 0xC04;

    // SAMC 1; EXTSAM disabled; ADRC PBCLK; ADCS 249999; 

   AD1CON3 = 0x3D18F;

    // CH0SA AN0; CH0NA AVSS; 

   AD1CHS = 0x00;

    // CSS9 disabled; CSS8 disabled; CSS7 disabled; CSS6 disabled; CSS5 disabled; CSS4 disabled; CSS3 disabled; CSS2 disabled; CSS15 disabled; CSS1 disabled; CSS14 disabled; CSS0 disabled; CSS13 disabled; CSS12 disabled; CSS11 disabled; CSS10 disabled; CSS30 disabled; CSS19 enabled; CSS18 enabled; CSS29 disabled; CSS17 disabled; CSS28 disabled; CSS16 disabled; CSS27 disabled; 

   AD1CSS = 0xC0000;

    // ASEN disabled; WM Legacy operation; ASINT No interrupt; CM Less Than mode; BGREQ disabled; LPEN Full power; 

   AD1CON5 = 0x00;

    // CHH9 disabled; CHH8 disabled; CHH7 disabled; CHH6 disabled; CHH5 disabled; CHH4 disabled; CHH3 disabled; CHH2 disabled; CHH1 disabled; CHH0 disabled; CHH11 disabled; CHH10 disabled; CHH13 disabled; CHH12 disabled; CHH15 disabled; CHH14 disabled; CHH17 disabled; CHH16 disabled; CHH19 disabled; CHH18 disabled; 

   AD1CHIT = 0x00;


   adc1_obj.intSample = AD1CON2bits.SMPI;
   
}

void ADC1_Start(void)
{
   AD1CON1SET = (1 << _AD1CON1_SAMP_POSITION);
}

void ADC1_Stop(void)
{
   AD1CON1CLR = (1 << _AD1CON1_SAMP_POSITION);
}

uint16_t ADC1_ConversionResultBufferGet(uint16_t *buffer)
{
    int count;
    uint16_t *ADC16Ptr;

    ADC16Ptr = (uint16_t *)&(ADC1BUF0);

    for(count=0;count<=adc1_obj.intSample;count++)
    {
        buffer[count] = (uint16_t)*ADC16Ptr;
        ADC16Ptr += 8;
    }
    return count;
}

uint16_t ADC1_ConversionResultGet(void)
{
    return ADC1BUF0;
}

bool ADC1_IsConversionComplete( void )
{
    return AD1CON1bits.DONE; //Wait for conversion to complete   
}

void ADC1_ChannelSelect( ADC1_CHANNEL channel )
{
    AD1CHS = channel;
}


/**
  End of File
*/


void SleepActivate(void)
{
    IEC0=0;
    IEC1=0;
    IEC2=0;
    IEC3=0;
    IFS0=0;
    IFS1=0;
    IFS2=0;
    IFS3=0;
 //   UART2_Disable();
   // UART3_Disable();   
    SYSTEM_RegUnlock();
    LATA=0X00;
    LATB=0X00;
    LATC=0X00;
    LATD=0X00;
    ADC1_Stop();
    AD1CON1=0X0;
    AD1CON2=0X0;
    AD1CON3=0X0;
    AD1CON5=0X0;

 
    UPS_CTRL=0;
    PMD1=0XFFFFFFFFul;
    PMD2=0XFFFFFFFFul;
    PMD3=0XFFFFFFFFul;
    PMD4=0XFFFFFFFFul;
    PMD5=0XFFFFFFFFul;
    PMD6=0XFFFFFFFFul;
    PMD7=0XFFFFFFFFul;
    
    RCON|=0X8;
    SYSTEM_RegLock();
    Sleep();
}




void PowerMonitorService(void)
{
    static uint8_t AdcInputSelect=0;
    static uint16_t BatSample[MaxInternalAdcSample],DcSample[MaxInternalAdcSample],ThSample[MaxInternalAdcSample];
    uint16_t SampleCnt=0;
    uint32_t Resault;
    uint8_t  BuzTime=BAT_SHUTDOWN_BUZZER_CNT;
    uint8_t i=0;
    


    for(SampleCnt=MaxInternalAdcSample-1;SampleCnt>0;SampleCnt--)
        BatSample[SampleCnt]=BatSample[SampleCnt-1];
    BatSample[0]=ADC1BUF19;
    Resault=0;
    for(SampleCnt=0;SampleCnt<MaxInternalAdcSample;SampleCnt++)
        Resault+=BatSample[SampleCnt];
    BAT_VALUE=Resault/MaxInternalAdcSample;

    if(Dc_Power_Good==0)
        DC_VALUE=ADC1BUF18;
    else
    {
        for(SampleCnt=MaxInternalAdcSample-1;SampleCnt>0;SampleCnt--)
            DcSample[SampleCnt]=DcSample[SampleCnt-1];
        DcSample[0]=ADC1BUF18;
        DcValHigh=DcSample[0];
        Resault=0;
        for(SampleCnt=0;SampleCnt<MaxInternalAdcSample;SampleCnt++)
            Resault+=DcSample[SampleCnt];
        DC_VALUE=Resault/MaxInternalAdcSample;                    
    }            

    if(DC_VALUE<=DC_FAIL_LEVEL)
    {
        Dc_Power_Good=0;
    }
    else
        if(DC_VALUE>=MIN_ACCEPTABLE_DC)
            Dc_Power_Good=1;
    if(BAT_VALUE<=MIN_ACCEPTABLE_BAT)
    {
        Bat_Power_Good=0;
    }
    else
        Bat_Power_Good=1;
    
    if(Dc_Power_Good==0)
    {
        BatCounter++;
        if(BatCounter>BAT_BLINK_INDICATOR)
        {
            if(Bat_Blink)
                Bat_Blink=0;
            else
                Bat_Blink=1;
            BatCounter=0;
        }
        if(Bat_Power_Good==0)
        {
            if(BatLowDetect==0)
            {
                OffCounter++;
                if(OffCounter==BAT_FAIL_SURE_CNT)
                {
                    BatLowDetect=1;
                    OffCounter=0;
                    Refresh_Set=1;
                }
            }
        }
        else
        {	
            OffCounter=0;
        }

        if(BatLowDetect)
        {
            ShutDownCounter++;
            if(!(ShutDownCounter%BuzTime) && Shut_Down==0)
                beez_time=BEEP_100ms;
            if(ShutDownCounter>BAT_SHUTDOWN_CNT && Shut_Down==0)
            {
                ShutDownCounter=0;
                Shut_Down=1;
                OffCounter=0;
                SleepActivate();
            }
        }
    }
    else
    {
        OffCounter=0;
        BatCounter=0;
        BatLowDetect=0;
        ShutDownCounter=0;
        Bat_Blink=0;
    }
    /*if(appData.state != APP_STATE_DO_UNPLUG_DC)
    {
        if(((DC_VALUE*100)/3870)>DC_MAX_VALUE)
        {
            BlankDisp();
            Write_Char('d',3);
            Write_Char('c',2);
            DispPos=DISP_UNIT_PRICE;
            Write_String(_ERROR);
            Generic_Counter=250;
            appData.state = APP_STATE_DO_UNPLUG_DC;
        }
    }*/

}

void __attribute__ ((weak)) ADC1_CallBack(void)
{
    PowerMonitorService();

}

void ADC1_Tasks ( void )
{
    NopDelay();
	if(IFS1bits.AD1IF)
	{
		// ADC1 callback function 
		ADC1_CallBack();
        // clear the ADC interrupt flag
		IFS1CLR= 1 << _IFS1_AD1IF_POSITION;
	}
}

