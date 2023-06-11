#include "MyMain.h"
#include "define_type.h"
#include "app.h"


bool Count_Flag,Count_Flag,Kg_Flag,Lb_Flag,Bat_Flag,Send_Flag,Recive_Flag,Onoff_Flag,Func_Flag,WeightCalculated,sendSensitive,sendNormal,startSave,
     Send_Led,Show_Back_Zero,Show_Decimal_Point,Show_Sign,key_pressed,OnOff_State,StartToTransmition,Clock_Source,PowerSaver,disp_mask,Power_Save_Flag,
     Save_Cal_Paramerets_Flag,Run_Flag,Mask_Weight,STCC,StartRelay,direct_vendor,Numeral_Vendor,StartResolutionLog,LockMode,sensitiveDataCompleted,isSensitiveScaleZero,
     ADDFIX_direct,ADDFIX_Numeral,Shift_Flag,ShowDp,Cash_Open,AddToList,Mem_Flag,Payback_Flag,SentFlag,Dc_Power_Good,Bat_Power_Good,Shut_Down,Bat_Blink,
     USBHostBusIsActived,HidReadEnableFlag, GetHidPacket, price_received,
     En_Write_Flag,Net_Send_Flag ,Net_Recive_Flag,En_Setup_Flag,AutoSend,ClFlag,ErrorCalibrationTimout,packServiced,requestZero;

double ZeroCheck;


union   IntChar  EE_DATA_INT,CRC;

union SetParameter SystemParm;

int32_t    ADC_DATA,indata;


uint64_t        Total_1,Total_2,Total_3,Total_4,Total_5,Total_6,Total_7,Total_8;

uint32_t        in_data,Power_Save_Counter,CalcPsTime,Generic_Counter3,
				Tare_Weight_1,Tare_Weight_2,Pre_Weight_1,Pre_Weight_2,Current_Time,Current_Date,Total_Add,Count_Add,
				Pass,EmptyAddressBarcodeWrite,TempDateTimeLong,LoadFactorDate,total_price,Price1,
                TempDateStartReport,TempDateEndReport,ADC_Ch_SW,
				TempRowDate,NetworkTimeout,WorkingTime,
				Mem_Code,Plu_Unit_Price,Total_Price[MAX_CUSTOMER],
				Price,Sale_Weights[MAXROW][MAX_CUSTOMER],
				Sale_Prices[MAXROW][MAX_CUSTOMER],Sale_Unit_Prices[MAXROW][MAX_CUSTOMER],
				Calc_Value1,Calc_Value2,Calc_Result,Sales_PluCode[MAXROW][MAX_CUSTOMER],Colpy=0;

uint16_t        CashCnt,EE_ADRESS,beez_time,Generic_Counter,Generic_Counter2,Drift_Counter,Sale_Plues[MAXROW][MAX_CUSTOMER],
				SerOutputWrp,SerOutputRrp,SerInputWrp,SerInputRrp,Sale_Num,kpfc=0,GetDateTimeAppFuncState,
				BAT_VALUE,DC_VALUE,THERMAL_HEAD_VALUE,PrnInputWrp,PrnInputRrp,PrnOutputWrp,PrnOutputRrp,DcValHigh,
				CJ_CNT_OFF,CJ_CNT_ON,Plu_Number,BarCodePlu_Number,BarCodeEmptyPlu_Number,Input[MAX_MESSAGE],GlobalMenuState,
				OffCounter,BatCounter,ShutDownCounter,InputUnicode,NewSaleAdd,LED_COUNTER,DownRange,UpRange,TimeDateCounter,ShledCnt;

int16_t         datesyear,datemyear;

int32_t         minadc,maxadc;

uint8_t 	    Mask_Number,DispPos,DispData[6],EE_DATA_BYTE,Coder_Step,TempCodeBuf,OffsetCalibration,
				key_read,key_n,key_zero_time,last_sample,key_cnt,keybuff[KEY_MAX],St_Auto_Rel,Loadcell_Id,
				keywptr,keyrptr,Key_Column_Counter,key_no,Video_Ram_Backup[DISP_NO],CalibSettingState,
				Calib_Unit_Index,Fi_Ind,Show_Error_flag,Show_Error_Value,TempCnt,DisplayUpdateTime,
				Show_Front_Zero,Prog_State,Calc_Operate,Normal_State,RetNormal_State,ResLogCounter,
				Divition_Index,Calculate_State,GetAdcSample,WriteMessageBuffer[MAX_MESSAGE],
				Year,Month,Day,Hour,Min,Second,HidByteCounter,CurserByte,CurserPos,TempBuffer[10],
				IAC,KeyState,KeyCnt[56],key_np,FontId,ExitReturnState,GetDateTimeAppReturnState,
				Ser_Write_Buf[SER_MAX_WRITE],Ser_Read_Buf[SER_MAX_READ],serbuf,TestFlag,
				Prn_Write_Buf[PRN_MAX_WRITE],Prn_Read_Buf[PRN_MAX_READ],prnbuf,DispStep,UniCnt,PrnCnt,
				Digit_Index,StringLenght,ReportMode,MsgTableSelect,TcpSendCnt,DataPack[200],sensitiveScaleDataPack[50],sPack[50],
				Sync_Baud_Flag,CountDown,RetKeCode,PrnToBuff[MAX_PRINTER_CHAR],
				SetupCnt,Mem_Index,BatLowDetect,Recive_Counter,Refresh_Set,ErrorCof,One_Gram,PrnPos,
				OutPut[MAX_MESSAGE],PackState,Encryption_Key,SetBluetoothPincodeFlg,Set,GXPos,LcdErrorTimeoutCounter,RtcErrorTimeoutCounter,
                Sales_PluTax[MAXROW][MAX_CUSTOMER],TempDay,TempMonth,TempYear,TempHour,TempMin,TempSecond,Point_Index,CalibProcess;

bool            TheFirstEnterToMenu,DoScanFlag,FindBarcodeFlag,DateReturnStatus,
                FindEmptyLocation,ModeSelect,ShowCurser,SelectDateTime,MayadinTotal,
                DateAccept,UserAccept,FirstWriteHeader,EnableLedShow,NetWorkSendApp,NetDeviceConnected,NoCalibrate;
uint32_t ReadBuffer[512];

uint8_t  WriteBuffer[256];
uint8_t  *PByteBuffer;
uint32_t *PLongBuffer;
uint16_t *PIntBuffer;
const uint8_t * FontAddress;

uint8_t  BarcodeTempReciveCode[MAX_BARCODE_LENGHT];



uint8_t  ResetCause  __attribute__ ((persistent));

int32_t  SavedOffset[2]  __attribute__ ((persistent));


uint8_t  disp[DISP_NO] __attribute__ ((persistent));  


int8_t mld[13];
int8_t shm[13]; 
int8_t mm[13];
int8_t ss[13];

uint32_t FlashData[4];


int8_t datesmon,datemmon,datesday,datemday;

const uint8_t BAR_ROW_ITEM_BUF[]   ={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
const uint8_t BAR_END_ITEM_BUF[]   ={2,6,7,10,11,12,15,16,17};
const uint8_t BAR_LABLE_ITEM_BUF[] ={0,1,2,3,5,6,7,8,9,10,11,12};
const uint8_t MAXBARLENGHT[] ={sizeof(BAR_ROW_ITEM_BUF),sizeof(BAR_END_ITEM_BUF),sizeof(BAR_LABLE_ITEM_BUF)};
const uint8_t Xposi[18]={100,100,140,100,105,105,90,90,115,115,100,115,115,120,115,120,100,100};
const uint8_t BAR_TYPE_LIMIT[] ={12,12,12,8,11,22,14,16,25,20,16};


const unsigned char ID_array[29]={'R','Q'
	,'P','R','0','0','6','0','0','0','0','0','0','A','M'
	,'C','U','0','0','3','3','6','4','P','D','0','0','1','1'};	

const unsigned char  Saman_Data[96]={0x82,0x0A,0xD4,0xE4,0xC7,0xD3,0xE5,0x3A,0x31,0x30,0x30,0x0A  //????? : 100                           
													 ,0x83,0x01,0x01,0x84,0x01,0x03,0x85,0x01,0x00  //?????? ???? ??
													 ,0x82,0x01,0x30,0x83,0x01,0x30,0x84,0x01,0x30,0x85,0x01
	                         ,0x30,0x86,0x01,0x30,0x87,0x01,0x30,0x88,0x01,0x30  //????? ????? ?? ?? 0 ?? ??? ????? ??? ???
													 ,0xA2,0x30,0xA1,0x0C,0x81,0x05,0xD4,0xE4,0xC7,0xD3,0xE5,0x82,0x03,0x31,0x30,0x30 //????? : 100 
                           ,0xA1,0x11,0x81,0x06,0x44,0x6C,0x6C,0x56,0x65,0x72
                           ,0x82,0x07,0x32,0x2E,0x31,0x2E,0x30,0x2E,0x31
                           ,0xA1,0x0D,0x81,0x06,0x50,0x72,0x67,0x56,0x65,0x72
                           ,0x82,0x03,0x31,0x2E,0x30
                           ,0x31,0xC1,0xE7,0x04};

float low_pass_coeff[] = {
    
    
  0.007231119,
  0.009833168,
  0.012771189,
  0.016005623,
  0.019482260,
  0.023133397,
  0.026879696,
  0.030632679,
  0.034297764,
  0.037777714,
  0.040976339,
  0.043802294,
  0.046172761,
  0.048016870,
  0.049278658,
  0.049919435,
  0.049919435,
  0.049278658,
  0.04801687,
  0.046172761,
  0.043802294,
  0.040976339,
  0.037777714,
  0.034297764,
  0.030632679,
  0.026879696,
  0.023133397,
  0.01948226,
  0.016005623,
  0.012771189,
  0.009833168,
  0.007231119
};


const uint8_t disp_char[10]      = {_0,_1,_2,_3,_4,_5,_6,_7,_8,_9};
const uint8_t Text_Convertion_Table[128]=  {
                                   
                                       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                       0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,0x40,   0,   0,
                                    0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,   0,   0,   0,0x48,   0,   0xD3,
                                       0,0x77,0x7f,0x39,0x3f,0x79,0x71,0x3d,0x76,0x06,0x1e,0x76,0x38,0x55,0x37,0x3f,
                                    0x73,0x67,0x50,0x6d,0x78,0x3e,0x3e,0x6a,0x76,0x6e,0x5b,   0,   0,   0,   0,0x08,
                                       0,0x5f,0x7c,0x58,0x5e,0x7b,0x71,0x6f,0x74,0x04,0x1e,0x76,0x06,0x55,0x54,0x5c,
                                    0x73,0x67,0x50,0x6d,0x78,0x1c,0x1c,0x6a,0x76,0x6e,0x5b,   0,   0,   0,   0,   0 };


const uint8_t ScanCodeConvertor[255]=  {
                                            0,  0,  0,  0, 65, 66, 67, 68, 69, 70,
                                           71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                                           81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
                                            1,  2,  3,  4,  5,  6,  7,  8,  9,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                            0,  0,  0,  0,  0
                                        };




const uint16_t  DivitionTable[13]   = {1,2,5,10,20,25,50,100,200,500,1000,2000,5000};

const uint8_t   Zero_Scope[4]={2,4,10,20};
const uint8_t   Zero_Start[4]={2,4,10,20};

const double     Zero_Track[7]={0.5,1,1.5,2,2.5,3,5};

const uint16_t  STABLE_EXTENT[3]={STABLE_EXTENT_LOW,STABLE_EXTENT_MIDDLE,STABLE_EXTENT_HIGH};
const uint16_t  STABLE_TIME[3]={STABLE_TIME_LOW,STABLE_TIME_MIDDLE,STABLE_TIME_HIGH};
 
const uint32_t  PS_Counter[3]={60000,180000,300000};
const uint16_t  Zero_Track_Time[9]={500,1000,1500,2500,3000,3500,4000,4500,5000};
const uint8_t   POSI[3]={0,16,23};
uint8_t   ImportFilePath[100];
uint8_t   ExportFilePath[100];
const uint32_t SerialBuadRate[8]={1200,2400,4800,9600,19200,38400,57600,115200};


const uint8_t   key_map[56]={
									21,14,7 ,0 ,22,15,8 ,1 ,23,16,
									9 ,2 ,24,17,10,3 ,25,18,11, 4,
									26,19,12, 5,27,20,13, 6,28,29,
									30,31,0 ,33,34, 7, 0, 1, 4, 8,
									40,2 , 5, 9,44, 3,46, 6,48,49,
									50,51,52,53,54,55
							  };


const uint16_t Farsi_KeyCode_To_Unicode_Conv_Array[112]={ 
													0x06AF,0x0637,0x062F,0x0627,0x0644,0x0638,0x0631,0x0628,0x0645,0x0639,
													0x0632,0x067E,0x0646,0x063A,0x0633,0x062A,0x0648,0x0641,0x0634,0x062C,
													0x0647,0x0642,0x0635,0x0686,0x06CC,0x06A9,0x0636,0x062E,0x0000,0x0000,
													0x0000,0x0000,0x0660,0x0000,0x0000,0x0667,0x0000,0x0661,0x0664,0x0668,
													0x0000,0x0662,0x0665,0X0669,0x002D,0x0663,0x002F,0x0666,0x002B,0x0000,
													0x002A,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0630,0x0622,
													0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0698,0x0000,0x0000,0x0000,
													0x0000,0x062B,0x0000,0x0000,0x0000,0x0000,0x0621,0x0000,0x0000,0x0000,
													0x0626,0x0000,0x0000,0X062D,0x0000,0x0000,0x0000,0x0000,0x003a,0x0000,
													0x0000,0x0028,0x005c,0X061F,0x003C,0x0029,0x0020,0x0021,0x003E,0x0025,
													0x002D,0x0022,0x002F,0X003D,0x002B,0x0020,0x002A,0x0000,0x0000,0x0000,
													0x0000,0x0000
												 };

const uint8_t English_KeyCode_To_Unicode_Conv_Array[112]={  'v','o','h','a','w','p','i','b','x','q',
                                                            'j','c','y','r','k','d','z','s','l','e',
                                                            0  ,'t','m','f',0  ,'u','n','g',0  ,0  ,
                                                            0  ,0  ,'0',0  ,0  ,'7',0  ,'1','4','8',
                                                            0  ,'2','5','9','-','3','/','6','+',0  ,
                                                            '*',0  ,0  ,0  ,0  ,0  ,
                                                            'V','O','H','A','W','P','I','B','X','Q',
                                                            'J','C','Y','R','K','D','Z','S','L','E',
                                                            0  ,'T','M','F',0  ,'U','N','G',0  ,0  ,
                                                            0,0  ,':',0  ,0  ,'(',0x5c,'?','<',')',
                                                            ' ' ,'!','>','%','-','"','/','=','+',0  ,
                                                            '*',0  ,0  ,0  ,0  ,0  };


const uint8_t Text_Array[139][6]= {
										{'d','E','L','E','t','E'},
										{'r','E','P','O','r','t'},
										{'P','r','i','n','t',' '},
										{'P','A','g','E','-','1'},
										{'P','A','g','E','-','2'},
										{'n','S','V','1','0','0'},
										{' ',' ','P','A','Y',' '},
										{' ','V','A','I','t',' '},
										{' ','d','o','n','E',' '},
										{'E','r','r','o','r',' '},
										{' ','K','E','y',' ',' '},
										{'P','-','b','A','C',' '},
										{'t','o','t','A','L',' '},
										{'S','A','V','E','d',' '},
										{'N','O',' ',' ',' ',' '},
										{' ','O','V','E','r',' '},
										{'d','i','s','c','n','t'},
										{'S','h','I','f','t',' '},
										{'C','U','S','t','1',' '},
										{'C','U','S','t','2',' '},
										{'C','U','S','t','3',' '},
										{' ','C','A','L','C',' '},
										{' ','A','d','C',' ',' '},
										{'b','-','r','E','S',' '},
										{' ',' ','d','C',' ',' '},
										{' ','b','A','t',' ',' '},
										{'m','E','m','o','r','y'},
										{'P','r','I','C','E',' '},
										{'C','o','d','E',' ',' '},
										{'n','A','m','E',' ',' '},
										{' ',' ',' ',' ',' ',' '},
										{' ','t','I','t','r',' '},
										{' ',' ','U','P',' ',' '},
										{' ','d','o','U','n',' '},
										{'E','n','t','E','r',' '},
										{' ','P','L','U',' ',' '},
										{'F','I','L','t','E','r'},
										{' ','m','i','n',' ',' '},
										{' ','A','V','E',' ',' '},
										{' ','m','A','H',' ',' '},
										{'b','A','r','-','t','H'},
										{'b','A','r','-','r','o'},
										{'b','A','r','C','O','d'},
										{'S','C','A','L','E',' '},
										{'S','t','O','r','E',' '},
										{'b','U','Z','Z','E','r'},
										{'1','-','d','s','P',' '},
										{'r','E','L','A','y',' '},
										{'S','t','b','-','t','I'},
										{'Z','r','o','-','S','t'},
										{'d',' ',' ',' ',' ',' '},
										{'P',' ',' ',' ',' ',' '},
										{' ','F','U','L','L',' '},
										{'n','o','L','o','A','d'},
										{'A','d','L','o','A','d'},
										{'P','=',' ',' ',' ',' '},
										{'C','o','u','n','t',' '},
										{' ','U','n','i','t',' '},
										{' ',' ','k','g',' ',' '},
										{' ',' ','l','b',' ',' '},
										{' ','n','E','t',' ',' '},
										{'g','r','o','s','s',' '},
										{'P','S','-','t','i','m'},
										{'1',' ','m','i','n',' '},
										{'3',' ','m','i','n',' '},
										{'5',' ','m','i','n',' '},
										{' ','o','F','F',' ',' '},
										{'b','A','u','d','-','s'},
										{'L','I','g','h','t',' '},
										{'Z','r','o','-','t','v'},
										{'Z','r','o','-','S','c'},
										{'S','t','b','-','E','c'},
										{'Z','r','o','-','t','t'},
										{'d','A','t','E','-','t'},
										{' ','S','h','m',' ',' '},
										{' ','m','I','L',' ',' '},
										{'d','A','t','-','t','r'},
										{' ',' ','o','n',' ',' '},
										{' ','d','A','t','E',' '},
										{' ','t','i','m','E',' '},
										{'r','E','S','E','t',' '},
										{'C','n','t','-','t','r'}, 
										{'S','t','b','-','t','r'},
										{'R','s','-','2','3','2'},
										{'S','y','n','c','-','b'},
										{'P','r','n','-','r','c'},
										{'P','r','n','-','L','A'},
										{'S','E','L','L','-','t'},
										{'n','E','t','-','m',' '},
										{' ','L','o','C','k',' '},
										{'S','t','A','r','t',' '},
										{' ','P','A','n','d',' '},
										{' ','1','-','g','r',' '},
										{'S','t','o','P',' ',' '},
										{' ','A','U','t','o',' '},
										{' ','m','n','L',' ',' '},
										{'U','n','L','o','C',' '},
										{'b','A','t','-','L','o'},
										{'A','d','d','-','m','d'},
										{' ','C','n','t',' ',' '},
										{'m','E','m','=',' ',' '},
										{' ','A','d','d',' ',' '},
										{'C','L','E','A','r',' '},
										{'m','o','d','E',' ',' '},
										{' ','P','C','S',' ',' '},
										{' ','L','o','A','d',' '},
										{'V','E','I','g','H','t'},
										{'S','A','V','E',' ','?'},
										{' ','P','A','S','S',' '},
										{'S','U','P','P','L','y'},
										{'S','O','F','t','-','V'},
										{'S','Y','S','t','E','m'},
										{'L','O','C','H','E','d'},
										{'U','-','L','O','C','H'},
										{'S','E','t','U','P',' '},
										{' ','S','C','A','n',' '},
										{'F','A','C','-','n','o'},
										{' ','r','O','W',' ',' '},
										{' ','t','y','p','E',' '},
										{' ','t','A','X',' ',' '},
										{' ','m','A','k','E',' '},
										{' ','S','E','L','L',' '},
										{'t','r','A','n','S',' '},
										{'C','L','I','E','n','t'},  
										{'r','E','t','u','r','n'},
										{' ','C','A','S','H',' '},
										{'n','U','m','b','E','r'},
										{'d','E','t','A','I','L'},
										{'S','P','E','C',' ',' '},
										{'d','A','t','-','t','I'},
										{'d','I','S','P','L','y'},
										{'s','t','A','b','L','E'},
										{' ','P','o','r','t',' '},
										{'S','E','r','V','I','C'},
										{'S','E','r','I','A','L'},
										{'P','g','r','A','d','E'},
										{' ',' ','I','d',' ',' '},
										{'A','y','A','d','i','n'},
										{' ','w','o','r','k',' '}
                                        
                                        
};

const uint8_t  Gray_Parameters[]=
{
	0x00,0x00,0x00,0x00, 0x04,0x04,0x04,0x04, 0x08,0x08,0x08,0x08, 0x0d,0x0d,0x0d,0x0d,
	0x10,0x10,0x10,0x10, 0x14,0x14,0x14,0x14, 0x18,0x18,0x18,0x18, 0x1b,0x1b,0x1b,0x1b,
	0x1e,0x1e,0x1e,0x1e, 0x21,0x21,0x21,0x21, 0x24,0x24,0x24,0x24, 0x28,0x28,0x28,0x28, 
	0x2d,0x2d,0x2d,0x2d, 0x30,0x30,0x30,0x30, 0x34,0x34,0x34,0x34, 0x38,0x38,0x38,0x38,
	0x3d,0x3d,0x3d,0x3d, 
};
/*
PORTS_CHANNEL                           PortID[14]={  PORT_CHANNEL_E,PORT_CHANNEL_E,PORT_CHANNEL_B,PORT_CHANNEL_B,PORT_CHANNEL_B,
                                                      PORT_CHANNEL_B,PORT_CHANNEL_B,PORT_CHANNEL_B,PORT_CHANNEL_B,PORT_CHANNEL_B,
                                                      PORT_CHANNEL_A,PORT_CHANNEL_A,PORT_CHANNEL_B,PORT_CHANNEL_B
                                                   };
PORTS_BIT_POS                           BitPos[14]={  8,9,5,4,3,2,1,0,6,7,9,10,8,9 };
*/
const double ResAdc[50]=
{
    200,
    190,
    184.150375,
    180,
    176.7807191,
    174.150375,
    171.9264508,
    170,
    168.30075,
    166.7807191,
    165.4056838,
    164.150375,
    162.9956028,
    161.9264508,
    160.931094,
    160,
    159.1253716,
    158.30075,
    157.5207249,
    156.7807191,
    156.0768258,
    155.4056838,
    154.7643804,
    154.150375,
    153.5614381,
    152.9956028,
    152.451125,
    151.9264508,
    151.42019,
    150.931094,
    150.4580369,
    150,
    149.5560588,
    149.1253716,
    148.7071698,
    148.30075,
    147.9054663,
    147.5207249,
    147.1459778,
    146.7807191,
    146.42448,
    146.0768258,
    145.7373525,
    145.4056838,
    145.081469,
    144.7643804,
    144.4541115,
    144.150375,
    143.8529016,
    143.5614381
};

const uint8_t * EftPos_Commandns[]={"<cpml><command><purchase><amount>","</amount><print>yes</print></purchase></command></cpml>"};




    const uint16_t  CrcTable[]  = {
    0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
    0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
    0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };