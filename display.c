#include <xc.h>
#include <string.h>
#include "HardwareProfile.h"
#include "MyMain.h"
#include "define_type.h"


extern bool Kg_Flag,Lb_Flag,Bat_Flag,Send_Flag,Recive_Flag,Onoff_Flag,Func_Flag,
     Send_Led,Show_Back_Zero,Show_Decimal_Point,Show_Sign,key_pressed,OnOff_State,StartToTransmition,Clock_Source,PowerSaver,disp_mask,Power_Save_Flag,
     Save_Cal_Paramerets_Flag,Run_Flag,Mask_Weight,STCC,StartRelay,direct_vendor,Numeral_Vendor,
     ADDFIX_direct,ADDFIX_Numeral,Shift_Flag,ShowDp,Cash_Open,AddToList,Mem_Flag,Payback_Flag,SentFlag,Bat_Power_Good,Shut_Down,Bat_Blink,
     USBHostBusIsActived,HidReadEnableFlag, GetHidPacket, En_Write_Flag,Net_Send_Flag ,Net_Recive_Flag,En_Setup_Flag,Count_Flag,Run_Flag,OnOff_State;


extern void Write_String(uint8_t  Message);
extern uint8_t  disp[DISP_NO] __attribute__ ((persistent)); 

extern struct WStructure WeightStruc[2];
extern uint8_t          SwitchWeight,Video_Ram_Backup[DISP_NO],BatLowDetect,
					    Mask_Number,DispPos,DispData[6],Show_Front_Zero,Coustomer_Id,
                        Show_Error_flag,Show_Error_Value,CountDown,Sale_Row[MAX_CUSTOMER],Loadcell_Id,
                        Stop_Vendor[MAX_CUSTOMER],Normal_State,Sales_Modes[MAXROW][MAX_CUSTOMER];    
extern uint32_t         in_data,Total_Price[MAX_CUSTOMER];

extern const uint8_t    disp_char[10];
extern const uint8_t    Text_Convertion_Table[128];
extern uint16_t         beez_time,Generic_Counter,DcValHigh;
extern const uint8_t    POSI[3];

extern APP_DATA appData;
extern const uint8_t Text_Array[139][6];
extern union SetParameter SystemParm;
extern union CalParm CalibrationParametes;
extern bool EnableLedShow,Dc_Power_Good,Bat_Power_Good;


void Write_Char(uint8_t Ch,uint8_t Pos);
void BlankDisp( void );

extern uint8_t                     OffsetCalibration;
void ShowDemo ( void )
{
    uint8_t LoopCnt;
    bool IsSerialNoBlank;
    static uint8_t DemoState=0;
    switch(DemoState)
    {
        case SHOW_DEMO_STAT1 : 
            DispPos=DISP_WEIGHT;
            Write_String(_PAND);
            DispPos=DISP_UNIT_PRICE;
            Write_String(_SCALE);
        //    beez_time=BEEP_100ms;
            DemoState=SHOW_DEMO_STAT2;
//            WriteLcdMessages(PAND_MESSAGE,0,2,0,1,1);
//            ClearLines(0,160,1,1);
            Generic_Counter=1000;
        break;
        case SHOW_DEMO_STAT2 :
            if(Generic_Counter==0)
            {
           /*     if(((DcValHigh*100)/3870)>80)
                {
                    BlankDisp();
                    Write_Char('d',3);
                    Write_Char('c',2);
                    DispPos=DISP_UNIT_PRICE;
                    Write_String(_ERROR);
                    Generic_Counter=250;
                    WriteLcdMessages(KHATA_DAR_VOLTAGE_ADAPTOR ,0,2,0,1,1);
                    WriteLcdMessages(ADAPTOR_RA_TAVIZ_NEMAEID ,0,2,1,1,1);
                    appData.state = APP_STATE_DO_UNPLUG_DC;
                }
                else
                {*/
                    DispPos=DISP_WEIGHT;
                    Write_String(_SOFT);
                    DispPos=DISP_UNIT_PRICE;
                    Write_String(_VERSION);
                    disp[18]|=POINT;
                    DemoState=SHOW_DEMO_STAT3;
                    Generic_Counter=1000;
              //  }
            }
        break;
        case SHOW_DEMO_STAT3 :
            if(Generic_Counter==0)
            {
                IsSerialNoBlank=true;
                for(LoopCnt=0;LoopCnt<15;LoopCnt++)
                {
                    if(SystemParm.Set_Parameter.SerialNumber[LoopCnt]!=0x00)
                        IsSerialNoBlank=false;
                }
                if(SystemParm.Set_Parameter.SerialNumber[15]>0)
                    IsSerialNoBlank=false;
                if(IsSerialNoBlank)
                {
                    appData.state = APP_STATE_ENTER_SERIAL_NO;
                }
                else
                {
                    IsSerialNoBlank=false;
                    CountDown=10;
                    Generic_Counter=250;
                    appData.state = APP_STATE_COUNT_DOWN;
                }
            }
        break;
    }
}



void Load_Display_New( void )
{
	uint8_t dig_dsp,Check, LoopCnt;
	
	if(disp_mask==0)
	{
		LoopCnt=POSI[DispPos];
		do
		{
			dig_dsp=in_data%10;
			if(in_data || Show_Back_Zero)
				disp[LoopCnt]=disp_char[dig_dsp];
			else
			{
				if(Show_Front_Zero==0)
				{
					if((LoopCnt==0 || LoopCnt==16 || LoopCnt==23 )&& in_data==0)
						disp[LoopCnt] =disp_char[0]; 			
					else
						disp[LoopCnt] =DISPOFF; 			
				}
				else
				{
					if(LoopCnt<Show_Front_Zero+POSI[DispPos])
						disp[LoopCnt]=disp_char[dig_dsp];
					else
						disp[LoopCnt] =DISPOFF; 			
				}
					
			}
			if(LoopCnt==CalibrationParametes.CalibParameter[Loadcell_Id].Point_Index+POSI[DispPos] && Show_Decimal_Point)
				disp[LoopCnt] |= POINT;
			in_data/=10;

			LoopCnt++;
			if(Show_Sign && WeightStruc[Loadcell_Id].w_neg)
				disp[5]|= MINUS;
		}while(LoopCnt<Mask_Number+POSI[DispPos]);
	}
}

void Write_String ( uint8_t Message )
{
	uint8_t dcnt,dig_dsp,temp=0,LoopCnt;
	dcnt=POSI[DispPos];
	if(dcnt==23)
		dcnt=24;
	do
	{
		temp=Text_Array[Message][5-LoopCnt];
		disp[dcnt]=Text_Convertion_Table[temp];
		dcnt++;
		LoopCnt++;
	}while(LoopCnt<6);
}

void Write_Char(uint8_t Ch,uint8_t Pos)
{
	disp[Pos]=Text_Convertion_Table[Ch];
}

void Change_To_String(uint8_t Buf[15],uint32_t Number)
{
	uint8_t dig,LoopCnt;
	LoopCnt=0;
	do
	{
		dig=(uint8_t)(Number%10);
		Buf[LoopCnt]=dig;
		Number/=10;
		LoopCnt++;
	}while(LoopCnt<10);
}

uint8_t Change_To_Str(uint8_t Buf[10],uint32_t Number)
{
	uint8_t dig, LoopCnt,Len;
	LoopCnt=0;
	do
	{
		dig=Number%10;
		Buf[LoopCnt]=dig+'0';
		Number/=10;
		LoopCnt++;
	}while(Number>0);
    Len=LoopCnt;
    for(LoopCnt=0;LoopCnt<Len/2;LoopCnt++)
    {
        dig=Buf[LoopCnt];
        Buf[LoopCnt]=Buf[Len-LoopCnt-1];
        Buf[Len-LoopCnt-1]=dig;
    }
    return Len;    
}

uint32_t Change_To_long(uint8_t Buf[10])
{
	uint8_t PowIndex,LoopCnt,TempV,Len;
	uint32_t TempMul,Number;
	Number=0;
    LoopCnt=0;
    Len=strlen((const char *)Buf);
    for(LoopCnt=0;LoopCnt<Len/2;LoopCnt++)
    {
        TempV=Buf[LoopCnt];
        Buf[LoopCnt]=Buf[Len-LoopCnt-1];
        Buf[Len-LoopCnt-1]=TempV;
    }    
    LoopCnt=0;
	do
	{
		PowIndex=0;TempMul=1;
		while(PowIndex<LoopCnt)
		{
			TempMul*=10;
			PowIndex++;
		}
		Number+=((Buf[LoopCnt]-'0')*TempMul);
		LoopCnt++;
	}while(LoopCnt<Len);
	return Number;
}


void Show_Error( uint8_t Code )
{
    Show_Error_flag=1;
    Show_Error_Value=Code;
}

void Show_Dash( void )
{
	uint8_t Dis_Blank,LoopCnt;
	Dis_Blank=32;
    LoopCnt=0;
	do
	{
		disp[LoopCnt]=0x40;
		LoopCnt++;
	}
	while(LoopCnt<Dis_Blank);
}
void BlankDisp( void )
{
	uint8_t Dis_Blank,LoopCnt;
	Dis_Blank=32;
    LoopCnt=0;
	do
	{
		disp[LoopCnt]=0;
		LoopCnt++;
	}
	while(LoopCnt<Dis_Blank);
}

void View_Error( void )
{
	static uint8_t Show_Error_Stat=0;
    uint8_t LoopCnt;
    if(OnOff_State==false ||  BatLowDetect==true)
        return;
	
    switch(Show_Error_Stat)
    {
        case 0:
            for(LoopCnt=0;LoopCnt<DISP_NO;LoopCnt++)
                Video_Ram_Backup[LoopCnt]=disp[LoopCnt];
            BlankDisp();
            DispPos=DISP_WEIGHT;
            Write_String(_ERROR);
            
            Write_Char('n',22);
            Write_Char('o',21);
            Write_Char('=',20);
            Write_Char(' ',19);
            Write_Char('0'+Show_Error_Value/10,18);
            Write_Char('0'+Show_Error_Value%10,17);
            Show_Error_Stat=1;
            Generic_Counter=50;
        break;
        case 1:
        case 2:
            if(Generic_Counter==0)
            {
        //        beez_time=BEEP_50ms;
                Show_Error_Stat++;
                if(Show_Error_Stat<3)
                    Generic_Counter=150;
                else
                    Generic_Counter=1000;
            }
            break;
        case 3:
            if(Generic_Counter==0)
            {
                Show_Error_Stat=0;
                for(LoopCnt=0;LoopCnt<DISP_NO;LoopCnt++)
                    disp[LoopCnt]=Video_Ram_Backup[LoopCnt];
                Show_Error_flag=0;
            }
        break;
   	}
}



void Start_Bus(void)
{
    uint32_t ClkDelay=0;
	if(!Clock_Source)
		Display_Clock_1=1;
	else
		Display_Clock_2=1;
    for(ClkDelay=0;ClkDelay<280;ClkDelay++)
        Nop();
    Display_Data=1;
    for(ClkDelay=0;ClkDelay<280;ClkDelay++)
        Nop();
	Display_Data=0;
    for(ClkDelay=0;ClkDelay<280;ClkDelay++)
        Nop();
}



void Stop_Bus(void)
{
    uint32_t ClkDelay=0;
	if(!Clock_Source)
		Display_Clock_1=1;
	else
		Display_Clock_2=1;
    for(ClkDelay=0;ClkDelay<280;ClkDelay++)
        Nop();
	Display_Data=0;
    for(ClkDelay=0;ClkDelay<280;ClkDelay++)
        Nop();
	Display_Data=1;
    for(ClkDelay=0;ClkDelay<280;ClkDelay++)
        Nop();
}



void TM1640_write(uint8_t Data )
{
	uint8_t LoopCnt;
    uint32_t ClkDelay=0;
    for(LoopCnt=0;LoopCnt<8;LoopCnt++)
	{
        for(ClkDelay=0;ClkDelay<400;ClkDelay++)
            Nop();
		if(!Clock_Source)
			Display_Clock_1=0;
		else
			Display_Clock_2=0;
        for(ClkDelay=0;ClkDelay<400;ClkDelay++)
            Nop();
		if(Data&0x01)
			Display_Data=1;
		else
			Display_Data=0;
		Data>>=1;
        for(ClkDelay=0;ClkDelay<400;ClkDelay++)
            Nop();
		if(!Clock_Source)
			Display_Clock_1=1;
		else
			Display_Clock_2=1;
        for(ClkDelay=0;ClkDelay<400;ClkDelay++)
            Nop();
	}
}

void TM1640_clear(void)
{
	uint8_t i;
    Start_Bus();
	TM1640_write(0xC0);
	for (i = 0; i < 16; ++i)
		TM1640_write(0x00);
	Stop_Bus();
}


void TM1640_setup(uint8_t intensity)
{
	Start_Bus();
	TM1640_write(0x40);
	Stop_Bus();
	Start_Bus();
	TM1640_write(0x88 | (intensity & 7));
	Stop_Bus();
	Start_Bus();
	TM1640_write(0x88 | (intensity & 7));
	Stop_Bus();
	Start_Bus();
	TM1640_write(0x88 | (intensity & 7));
	Stop_Bus();
	Start_Bus();
	TM1640_write(0x88 | (intensity & 7));
	Stop_Bus();
	Start_Bus();
	TM1640_write(0x88 | (intensity & 7));
	Stop_Bus();
	TM1640_clear();
}



void IncBuf( uint8_t Buf[6],uint8_t Dig)
{
	if(Buf[Dig]<9)
		Buf[Dig]++;
	else
		Buf[Dig]=0;
}


void Show_Leds( void )
{
    disp[6]=0X00;
	disp[7]=0X00;
	if(EnableLedShow)
    {
        if(Power_Save_Flag==0 && Onoff_Flag==0)
        {
            if(SystemParm.Set_Parameter.ViewFormat==0)
            {
                if(WeightStruc[Loadcell_Id].Zero_Flag) 
                    disp[6]|=0x1;	
                if(WeightStruc[Loadcell_Id].Net_Flag) 
                    disp[6]|=0x2;	
                if(WeightStruc[Loadcell_Id].Stable_Flag) 
                    disp[6]|=0x4;	
                if(WeightStruc[Loadcell_Id].W1_Flag) 
                    disp[6]|=0x10;	
                if(WeightStruc[Loadcell_Id].W2_Flag) 
                    disp[6]|=0x20;	
            }
            else
            if(SystemParm.Set_Parameter.ViewFormat==1)
            {
                if(WeightStruc[0].Zero_Flag && WeightStruc[1].Zero_Flag) 
                    disp[6]|=0x1;	
                if(WeightStruc[0].Net_Flag && WeightStruc[1].Net_Flag) 
                    disp[6]|=0x2;	
                if(WeightStruc[0].Stable_Flag && WeightStruc[1].Stable_Flag) 
                if(WeightStruc[Loadcell_Id].Stable_Flag) 
                    disp[6]|=0x4;	
            }
            else
            if(SystemParm.Set_Parameter.ViewFormat==2)
            {
                if(WeightStruc[SwitchWeight].Zero_Flag) 
                    disp[6]|=0x1;	
                if(WeightStruc[SwitchWeight].Net_Flag) 
                    disp[6]|=0x2;	
                if(WeightStruc[SwitchWeight].Stable_Flag) 
                    disp[6]|=0x4;	
                if(WeightStruc[SwitchWeight].W1_Flag) 
                    disp[6]|=0x10;	
                if(WeightStruc[SwitchWeight].W2_Flag) 
                    disp[6]|=0x20;	
            }
            if(Dc_Power_Good) 
                disp[6]|=0x8;	
        }		
    }
}

 void InitDisplay(uint8_t LightLevel,bool Mode)
 {
    uint8_t LoopCnt;
    LoopCnt=0;
	if(Mode==true)
    {
        do
        {
            Write_Char(0,LoopCnt);
            LoopCnt++;
        }while(LoopCnt<32);
    }
    Clock_Source=0;
	Display_Clock_1=1;
	Display_Clock_2=0;
	Display_Data=1;
	TM1640_setup(LightLevel);
	Clock_Source=1;
	Display_Clock_1=0;
	Display_Clock_2=1;
	Display_Data=1;
	TM1640_setup(LightLevel);
}
 
 void Display_Total(void )
{
	uint8_t LoopCnt=0,SellCent=0,i;
    for(LoopCnt=0;LoopCnt<Sale_Row[Coustomer_Id];LoopCnt++)
    {
        if((Sales_Modes[LoopCnt][Coustomer_Id]&0xf0)==0)
            SellCent++;
    }
	BlankDisp();
	DispPos=DISP_WEIGHT;
    Write_String(_TOTAL);	
    DispPos=DISP_UNIT_PRICE;
    Write_Char('N',22);
    Write_Char('O',21);
    disp[21]|=POINT;
    in_data=SellCent;	
    Show_Front_Zero=0;
    Mask_Number=3;
    Load_Display_New();
    DispPos=DISP_TOTAL_PRICE;
    in_data=Total_Price[Coustomer_Id];	
    Show_Front_Zero=0;
    Mask_Number=8;
    Load_Display_New();
	AddToList=1;
}
 
 void ShowCust( void )
{
	BlankDisp();
	DispPos=DISP_WEIGHT;
	Write_String(_CUST1+Coustomer_Id);
	Write_Char(Coustomer_Id+'1',31);
    Normal_State=NORMAL_MODE_DELAY;
    Generic_Counter=SHOW_CUST_DELAY;
}

void Delay_1640()
{
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
}

void Start_1640()		
{

	Display_Clock_1=0;
	Display_Data=1; 
	Display_Clock_1=1;	
	Delay_1640();
	
	Display_Data=0;
	Delay_1640();
	Display_Clock_1=0;
	Delay_1640();

}
//**************************************************************
void Stop_1640()			 //1640??
{

	Display_Clock_1=0;
	Display_Data=0;
	Display_Clock_1=1;
	Delay_1640();
	Display_Data=1;
	Delay_1640();

}


/*------????T1640----------*/
void Write_1640(uint8_t vdata1)
{

  	uint8_t i = 0;
 	Display_Data=0;
	Display_Clock_1=0;
	Delay_1640();
  	for (i=0; i<8; i++)  //????8???????????????
 	{

 		Display_Clock_1=0;
		Delay_1640();
 	 	if (vdata1&0x1)
 	   	{

 	    	Display_Data=1;
			
 	   	
}
 	 	else
 	   	{

 	    	Display_Data=0;
 	   	
}
		Delay_1640();
 	 	Display_Clock_1=1;
		Delay_1640();
 	 	vdata1>>=1;
    
}
	Display_Data=0;
	Display_Clock_1=0;
 
}


/*------?????--------*/
void Send_1640()
{

  	uint8_t j=0;
	Nop();Nop();Nop();Nop();Nop();
	
	Start_1640();
	Delay_1640();
  	Write_1640(0x40);    //??????????????????
	Delay_1640();
	Stop_1640();

	Start_1640();
	Delay_1640();
  	Write_1640(0xc0);    //??????00H???? 
  	Nop();Nop();Nop();Nop();Nop();
  	for (j= 0; j<16; ++j)
	{

	 	Write_1640(disp[j]);
	
}
	Stop_1640();

	Start_1640();
	Delay_1640();
  	Write_1640(0x8f); 	//?????????????????
	Delay_1640();
	Stop_1640();
	

}



void Delay_1640_2()
{

	uint8_t i;
	for(i=0;i<150;i++) Nop();

}

void Start_1640_2()		  //1640_2??
{

	Display_Clock_2=0;
	Display_Data=1; 
	Display_Clock_2=1;	
	Delay_1640_2();
	
	Display_Data=0;
	Delay_1640_2();
	Display_Clock_2=0;
	Delay_1640_2();

}
//**************************************************************
void Stop_1640_2()			 //1640_2??
{

	Display_Clock_2=0;
	Display_Data=0;
	Display_Clock_2=1;
	Delay_1640_2();
	Display_Data=1;
	Delay_1640_2();

}


/*------????T1640_2----------*/
void Write_1640_2(uint8_t vdata1)
{

  	uint8_t i = 0;
 	Display_Data=0;
	Display_Clock_2=0;
	Delay_1640_2();
  	for (i=0; i<8; i++)  //????8???????????????
 	{

 		Display_Clock_2=0;
		Delay_1640_2();
 	 	if (vdata1&0x1)
 	   	{

 	    	Display_Data=1;
			
 	   	
}
 	 	else
 	   	{

 	    	Display_Data=0;
 	   	
}
		Delay_1640_2();
 	 	Display_Clock_2=1;
		Delay_1640_2();
 	 	vdata1>>=1;
    
}
	Display_Data=0;
	Display_Clock_2=0;
 
}


/*------?????--------*/
void Send_1640_2()
{

  	uint8_t j=0;
	Nop();Nop();Nop();Nop();Nop();
	
	Start_1640_2();
	Delay_1640_2();
  	Write_1640_2(0x40);    //??????????????????
	Delay_1640_2();
	Stop_1640_2();

	Start_1640_2();
	Delay_1640_2();
  	Write_1640_2(0xc0);    //??????00H???? 
  	Nop();Nop();Nop();Nop();Nop();
  	for (j= 16; j<32; ++j)
	{

	 	Write_1640_2(disp[j]);
	
}
	Stop_1640_2();

	Start_1640_2();
	Delay_1640_2();
  	Write_1640_2(0x8f); 	//?????????????????
	Delay_1640_2();
	Stop_1640_2();
	

}

void UpdateDisplayData(void)
{
    if(BatLowDetect)
    {
        BlankDisp();
        DispPos=DISP_WEIGHT;
        Write_String(_BTLOW);
    }	
    Send_1640();
}