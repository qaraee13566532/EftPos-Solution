
/**
  TMR1 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr1.c

  @Summary
    This is the generated source file for the TMR1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for TMR1. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.145.0
        Device            :  PIC32MM0256GPM064
    The generated drivers are tested against the following:
        Compiler          :  XC32 v2.20
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
#include <stdio.h>
#include "tmr1.h"
#include "../HardwareProfile.h"
#include "../MyMain.h"
#include "../define_type.h"

/**
 Section: File specific functions
*/
void (*TMR1_InterruptHandler)(void) = NULL;
void TMR1_CallBack(void);

extern uint8_t  GetAdcSample,keywptr,keyrptr,keybuff[KEY_MAX],key_no,Video_Ram_Backup[DISP_NO],key_read,
                key_n,key_np,Key_Column_Counter,key_zero_time,last_sample,key_cnt,BitPos,CountDown,LcdErrorTimeoutCounter,RtcErrorTimeoutCounter,DispPos,TcpSendCnt;

extern uint16_t CashCnt,LED_COUNTER,beez_time,CJ_CNT_ON,CJ_CNT_OFF,Drift_Counter,TimeDateCounter,ShledCnt;
extern APP_DATA appData;
extern uint32_t Power_Save_Counter,Generic_Counter3,Generic_Counter2,Plu_Unit_Price,Generic_Counter,NetworkTimeout,CalcPsTime,WorkingTime;
extern uint8_t  DisplayUpdateTime,disp[DISP_NO] __attribute__ ((persistent));  
extern union SetParameter SystemParm;
extern bool     Cash_Open,EnableLedShow,Run_Flag,OnOff_State;
extern struct WStructure WeightStruc[2];
extern uint8_t  ResetCause  __attribute__ ((persistent));


/**

extern void PowerMonitorService(void);
extern void Show_Leds( void );
extern void Write_Char(uint8_t  Ch,uint8_t  Pos);
extern void BlankDisp( void );
extern void Write_String(uint8_t  Message);

  @Description
    This defines the object required for the maintenance of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
*/

typedef struct _TMR_OBJ_STRUCT
{
    /* Timer Elapsed */
    volatile bool           timerElapsed;
    /*Software Counter value*/
    volatile uint8_t        count;

} TMR_OBJ;

static TMR_OBJ tmr1_obj;

/**
  Section: Driver Interface
*/

void TMR1_Initialize (void)
{
    // Period = 0.001 s; Frequency = 24000000 Hz; PR1 24000; 
    PR1 = 0x5DC0;
    //  TCKPS 1:1; TWDIS disabled; TCS PBCLK; TECS SOSC; SIDL disabled; TSYNC disabled; TWIP disabled; TGATE disabled; ON enabled; 
    T1CON = 0x8000;    

    if(TMR1_InterruptHandler == NULL)
    {
        TMR1_SetInterruptHandler(&TMR1_CallBack);
    }

    IFS0CLR = _IFS0_T1IF_MASK;
    IEC0SET = _IEC0_T1IE_MASK;

    tmr1_obj.timerElapsed = false;

}



void __attribute__ ((vector(_TIMER_1_VECTOR), interrupt(IPL1SOFT))) TMR1_ISR()
{
    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes
    if(TMR1_InterruptHandler) 
    { 
        TMR1_InterruptHandler(); 
    }

    //***User Area End

    tmr1_obj.count++;
    tmr1_obj.timerElapsed = true;
    IFS0CLR = _IFS0_T1IF_MASK;
}

void TMR1_Period16BitSet( uint16_t value )
{
    /* Update the counter values */
    PR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Period16BitGet( void )
{
    return( PR1 );
}

void TMR1_Counter16BitSet ( uint16_t value )
{
    /* Update the counter values */
    TMR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Counter16BitGet( void )
{
    return( TMR1 );
}


void SelectColumn(uint8_t Column)
{
    
    switch(Column)
    {
        case 0 : 
           LATCbits.LATC0=1; 
        break;
        case 1 : 
           LATCbits.LATC1=1; 
        break;
        case 2 : 
           LATCbits.LATC2=1; 
        break;
        case 3 : 
           LATCbits.LATC11=1; 
        break;
        case 4 : 
           LATAbits.LATA3=1; 
        break;
        case 5 : 
           LATAbits.LATA8=1; 
        break;
        case 6 : 
           LATBbits.LATB4=1; 
        break;
        case 7 : 
           LATDbits.LATD4=1; 
        break;
            
    }
    
}

void DeSelectColumn(uint8_t Column)
{
    
    switch(Column)
    {
        case 0 : 
           LATCbits.LATC0=0; 
        break;
        case 1 : 
           LATCbits.LATC1=0; 
        break;
        case 2 : 
           LATCbits.LATC2=0; 
        break;
        case 3 : 
           LATCbits.LATC11=0; 
        break;
        case 4 : 
           LATAbits.LATA3=0; 
        break;
        case 5 : 
           LATAbits.LATA8=0; 
        break;
        case 6 : 
           LATBbits.LATB4=0; 
        break;
        case 7 : 
           LATDbits.LATD4=0; 
        break;
            
    }
    
}


void __attribute__ ((weak)) TMR1_CallBack(void)
{
    if(beez_time)
    {
        BUZZER=1;
        beez_time-=1 ;

    }
    else
        BUZZER=0;
    
    if(ResetCause==0xA3)
    {
        if(ShledCnt<=3000)
            ShledCnt++;
        if(ShledCnt>=3000)
        {
            if(Run_Flag)
            {
                if(OnOff_State==0)
                    disp[0]|=POINT;
                Show_Leds();
            }
        }
    }
    else
    {
        if(Run_Flag)
        {
            if(OnOff_State==0)
                disp[0]|=POINT;
            Show_Leds();
        }
    }
    

    Generic_Counter2++;
    if(Generic_Counter2>30000) Generic_Counter2=0;
    DisplayUpdateTime++;
    if(DisplayUpdateTime==20)
    {
        UpdateDisplayData();
        DisplayUpdateTime=0;
    }
    
    if(Cash_Open)
    {
        if(CashCnt++>CASH_DELAY)
        {
            CashCnt=0;
            Cash_Open=0;
            CASH=0;
        }
    }
    if(WeightStruc[0].Drift_Flag)
        WeightStruc[0].Drift_Counter++;
    else
        WeightStruc[0].Drift_Counter=0;
    if(WeightStruc[1].Drift_Flag)
        WeightStruc[1].Drift_Counter++;
    else
        WeightStruc[1].Drift_Counter=0;
}
      
/*
void __attribute__ ((weak)) TMR1_CallBack(void)
{
    uint8_t TempCnt;

    if(GetAdcSample++>MAX_SAMPLE_TIME)
    {
        GetAdcSample=0;
        PowerMonitorService();
    }

    if(Shut_Down==0)
    {
        if(LED_COUNTER)
        {
            LED_COUNTER--;
            if(LED_COUNTER==0)
            {
                CASH=0;
            }
        }	
        if(keywptr!=keyrptr)
        {
            key_no=keybuff[keyrptr];
            keyrptr=keyrptr+1;
            if(keyrptr>=KEY_MAX)keyrptr=0;
            if(!Power_Save_Flag && appData.state!=APP_SHOW_TIME_DATE_STATE)
                key_pressed=1;
            beez_time=BEEP_50ms;
            Power_Save_Counter=0;
            Generic_Counter3=0;
            if(appData.state==APP_SHOW_TIME_DATE_STATE)
            {
                appData.state=APP_STATE_NORMAL_MODE;       
                AddToList=0;
                Plu_Unit_Price=0;
            }	
            if(Power_Save_Flag)
            {
                Power_Save_Flag=0;
                Onoff_Flag=0;
                OnOff_State=1;
                for(TempCnt=0;TempCnt<DISP_NO;TempCnt++)
                    disp[TempCnt]=Video_Ram_Backup[TempCnt];
                LCD_LIGHT=1;
                Generic_Counter2=0;
            }
        }
        if(Run_Flag)
        {
            if(OnOff_State==0)
                disp[0]|=POINT;
            Show_Leds();
        }

        if(beez_time && OnOff_State)
        {
            if(SystemParm.Set_Parameter.Beep_Status==0)
                BUZZER=1;
            beez_time-=1 ;
        }
        else
            BUZZER=0;

        key_read=0;
        if(Key_Row1)
            key_read|=0x1;
        if(Key_Row2)
            key_read|=0x2;
        if(Key_Row3)
            key_read|=0x4;
        if(Key_Row4)
            key_read|=0x8;
        if(Key_Row5)
            key_read|=0x10;
        if(Key_Row6)
            key_read|=0x20;
        if(Key_Row7)
            key_read|=0x40;
        if(Key_Row8)
            key_read|=0x80;

        if(key_read)
        {
            switch (key_read)
            {
                case 0x01: key_n = 0x00 ; break ;
                case 0x02: key_n = 0x01 ; break ;
                case 0x04: key_n = 0x02 ; break ;
                case 0x08: key_n = 0x03 ; break ;
                case 0x10: key_n = 0x04 ; break ;
                case 0x20: key_n = 0x05 ; break ;
                case 0x40: key_n = 0x06 ; break ;
                case 0x60: key_n = 0x07 ; break ;
            }
            key_np=key_n;
            key_n = key_n+Key_Column_Counter*8 ;
            if (key_zero_time==KEY_ZERO_LIMIT)
            {
                if (key_n == last_sample)
                {
                    key_cnt=key_cnt+1;
                    if(key_cnt==4)
                    {
                        PowerSaver=0;
                        if(Cash_Open && key_n!=CLEAR)
                            Cash_Open=0;

                        keybuff[keywptr]=key_n;
                        keywptr=keywptr+1;
                        if(keywptr>=KEY_MAX)
                            keywptr=0;
                        if(keywptr==keyrptr)
                        {
                            if(keywptr==0)keywptr=KEY_MAX-1;
                            else keywptr=keywptr-1;
                        }
                        key_zero_time = 0 ;
                        last_sample   = 0 ;
                        key_cnt = 0;
                    }
                }
                else 
                    last_sample = key_n ;
            }
            else 
                key_zero_time = 0 ;
        }
        else
        {
            if((key_n&0xfc)==(Key_Column_Counter*8))
                if(key_zero_time!=KEY_ZERO_LIMIT) 
                    key_zero_time = key_zero_time+1 ;
        }

        DeSelectColumn(Key_Column_Counter);
        Key_Column_Counter++;  
        if(Key_Column_Counter==MAX_KEY_COLUMN)
            Key_Column_Counter=0;
        SelectColumn(Key_Column_Counter);
        if(Generic_Counter>0) Generic_Counter--;
        
        Generic_Counter2++;
        if(Generic_Counter2>30000) Generic_Counter2=0;

        if(NetworkTimeout>0) NetworkTimeout--;

          
        if(!CAL_JMP )
        {
            if(CJ_CNT_ON<1000)
                CJ_CNT_ON++;
            if(CJ_CNT_OFF>=100)
            {
                if(CJ_CNT_ON>=1000)
                {
                    Save_Cal_Paramerets_Flag=1;
                    CJ_CNT_ON=0;
                    CJ_CNT_OFF=0;
                }
            }
            else
                CJ_CNT_OFF=0;	

        }
        else
        {
            if(CJ_CNT_OFF<100)
                CJ_CNT_OFF++;
            CJ_CNT_ON=0;
        }
        if(CountDown>0)
        {
            if(Generic_Counter==0)
            {
                Generic_Counter=250;

                beez_time=BEEP_50ms;
                CountDown--;
                if(CountDown!=0)
                {
                    for(TempCnt=0;TempCnt<32;TempCnt++)
                        if(TempCnt!=6 && TempCnt!=7)
                            Write_Char(CountDown+'0',TempCnt);
                }
            }
        }

        if(SystemParm.Set_Parameter.Power_Save_En && Run_Flag && OnOff_State)
        {
            if(Power_Save_Flag==0)
            {
                if(Dc_Power_Good==0)
                {
                    if(Power_Save_Counter++>CalcPsTime)
                    {
                        Power_Save_Flag=1;
                        Power_Save_Counter=0;
                        for(TempCnt=0;TempCnt<DISP_NO;TempCnt++)
                        {
                            if(appData.state!=APP_SHOW_TIME_DATE_STATE)
                                Video_Ram_Backup[TempCnt]=disp[TempCnt];
                            else
                                Video_Ram_Backup[TempCnt]=0;
                        }
                        Onoff_Flag=1;
                        OnOff_State=0;
                        Generic_Counter2=0;
                        BlankDisp();
                        LCD_LIGHT=0;
                    }
                }
                else
                {
                    Power_Save_Counter=0;
                    Power_Save_Flag=0;
                    LCD_LIGHT=1;
                }
            }
        }
        if(Drift_Flag)
            Drift_Counter++;
        else
            Drift_Counter=0;
        if(LcdErrorTimeoutCounter>0)
            LcdErrorTimeoutCounter--;
        else
            LcdErrorTimeoutCounter=250;
        
        if(RtcErrorTimeoutCounter>0)
            RtcErrorTimeoutCounter--;
        else
            RtcErrorTimeoutCounter=250;

        if(appData.state==APP_STATE_NORMAL_MODE)
        {
            Generic_Counter3++;
            if(SystemParm.Set_Parameter.ScreenSaverShowTimeDate)
            {
                if(Generic_Counter3>=TimeDateCounter)
                {
                    Generic_Counter=0;
                    appData.state=APP_SHOW_TIME_DATE_STATE;
                    EnableLedShow=0;
                    DispPos=DISP_TOTAL_PRICE;
                    Write_String(_BLANK);
                    disp[23]=0;
                }
            }
        }
        else
            Generic_Counter3=0;
        if(TcpSendCnt>0)
            TcpSendCnt--;
        else
            Net_Send_Flag=0;
        WorkingTime++;
    }   
}
*/
void  TMR1_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.T1IE = false;
    TMR1_InterruptHandler = InterruptHandler; 
    IEC0bits.T1IE = true;
}

void TMR1_Start( void )
{
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;

    /*Enable the interrupt*/
    IEC0SET = _IEC0_T1IE_MASK;

    /* Start the Timer */
    T1CONSET = _T1CON_ON_MASK;
}

void TMR1_Stop( void )
{
    /* Stop the Timer */
    T1CONCLR = _T1CON_ON_MASK;

    /*Disable the interrupt*/
    IEC0CLR = _IEC0_T1IE_MASK;
}

bool TMR1_GetElapsedThenClear(void)
{
    bool status;
    
    status = tmr1_obj.timerElapsed;

    if(status == true)
    {
        tmr1_obj.timerElapsed = false;
    }
    return status;
}

int TMR1_SoftwareCounterGet(void)
{
    return tmr1_obj.count;
}

void TMR1_SoftwareCounterClear(void)
{
    tmr1_obj.count = 0; 
}

/**
 End of File
*/
