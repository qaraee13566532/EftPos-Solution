union Var_Bits
{
	struct
	{
		unsigned B0 :1;
		unsigned B1 :1;
		unsigned B2 :1;
		unsigned B3 :1;
		unsigned B4 :1;
		unsigned B5 :1;
		unsigned B6 :1;
		unsigned B7 :1;
		unsigned B8 :1;
		unsigned B9 :1;
		unsigned B10:1;
		unsigned B11:1;
		unsigned B12:1;
		unsigned B13:1;
		unsigned B14:1;
		unsigned B15:1;
	};
	uint16_t AllBits	;
};

union IntChar
{
	uint16_t  intno;
	uint8_t byteno[2];
};

union CalibFact
{
	float   factor;
	uint8_t byteno[4];
};

union LongChar
{
	float   factor;
	int32_t    longno; 
	uint8_t byteno[4]; 
}; 

struct Plu_Info_Structure 
{
    uint32_t  SellPrice;
    uint32_t  BuyPrice;
    uint32_t  PluCode;  
    float     PluCountCoeff;
    uint16_t  Mojodi_AvalDore;
    uint16_t  Name[NAME_LENGHT];
    uint8_t   BarcodeType;
    uint8_t   PluType;
    uint8_t   PluUnit;
    uint8_t   SaleOff;
    uint8_t   PluTax;
    uint8_t   PluStatusFlag;
};

union PluDataInfo
{
    struct   Plu_Info_Structure PluInfo;
    uint8_t  PluInfoBytes[sizeof(struct Plu_Info_Structure)];
    uint32_t PluInfoLong[sizeof(struct Plu_Info_Structure)/4];
};

struct Clients
{
    uint16_t LoginPass;
    uint8_t  Security_Setting;
    uint8_t  Security_Plusetup;
    uint8_t  Security_BarcodeSetup;
    uint8_t  Security_DataExport;
};

union ClientParm
{
    struct Clients ClientParameter;
    uint8_t ClientParameterBytes[sizeof(struct Clients)];
};

struct GlobalMemory
{
    uint64_t TotalPriceWeight[128];
    uint64_t TotalWeight[128];
    uint64_t TotalPriceNumeral[128];
    uint64_t TotalNumeral[128];
};

union GTempMemory
{
    struct GlobalMemory GlobalMem;
    uint32_t Temp4kBuffer[1024];
};

struct SettingVariables
{
    uint32_t  WorkingTime;
};

union SetVariable
{
    struct SettingVariables Set_Variable ;
    uint8_t Set_Variable_bytes[sizeof(struct SettingVariables)];
};

struct SettingParameters
{

    union     ClientParm   SystemClients[MAX_CLIENT];
    union     LongChar Bluetooth_PinCode_Address;

    uint32_t  LockDate;
    uint32_t  UnlockPass;
    uint32_t  FactorNo;
    uint32_t  nvm_barcod_page_add;
    uint32_t  nvm_plu_code_add;
    uint32_t  nvm_kala_info_page_add;
    uint32_t  nvm_detail_sell_page_add;
    uint32_t  SavedSellNumber;
    uint32_t  DateStartReport;
    uint32_t  DateEndReport;
    uint32_t  RemoteServerPort;
    uint32_t  QCTime;
    
    uint32_t  Reserv_2;
    uint32_t  Reserv_3;
    uint32_t  Reserv_4;
    uint32_t  Reserv_5;
    uint32_t  IsConfig;
    

    uint16_t  UpTitr[TITR_LENGHT];
    uint16_t  DownTitr[TITR_LENGHT];
    uint16_t  max_all_plu;
    uint16_t  ActiveCountMemory;
    uint16_t  ReportFromMem;
    uint16_t  ReportToMem;


    
    
    uint8_t   Power_Save_Time;
    uint8_t   Power_Save_En;
    uint8_t   Baudrate_Index;
    uint8_t   Rs232_Out_Index;

    uint8_t   Backlight_Index;
    uint8_t   Filter_Index;
    uint8_t   Stable_Time_Index;
    uint8_t   Stable_Extent_Index;

    uint8_t   DateType;
    uint8_t   Beep_Status;
    uint8_t   StoreNumber;
    uint8_t   ScaleNumber;

    uint8_t   BarSetLableItem;
    uint8_t   BarRowEnable;
    uint8_t   BarEndEnable;
    uint8_t   LockCheck;

    uint8_t   CurrentSavedDate;
    uint8_t   IsLock;
    uint8_t   IsPluLocked;
    uint8_t   IsKeyNumbersLocked;
    uint8_t   Net_Scale_Lock;   

    uint8_t   LoginUserID;
    uint8_t   CustPrint;
    uint8_t   SystemSecure;
    uint8_t   ReportDateType;

    uint8_t   ReportClientType;
    uint8_t   ClientReportId;
    uint8_t   ReportMemoryType;
    uint8_t   ReportSpecMemType;
    
    uint8_t   DateTimeState;
    uint8_t   BarSetEndItem;
    uint8_t   Reserve_b0;
    uint8_t   DisplayProtectState;
    
    uint8_t   ClientState;
    uint8_t   FilterState;
    uint8_t   SobatState;
    uint8_t   PortState;

    uint8_t   BarSubState;
    uint8_t   BarSetRowItem;
    uint8_t   BarLableEnable;
    uint8_t   BarRowTextEnable;
    
    uint8_t   BarEndTextEnable;
    uint8_t   BarLableTextEnable;
    uint8_t   BarRowType;
    uint8_t   BarEndType;
    
    uint8_t   BarLableType;
    uint8_t   BarRowHeight;
    uint8_t   BarEndHeight;
    uint8_t   BarLableHeight;
    
    uint8_t   BarRowSpec[10];
    uint8_t   BarRowNum[10];
    
    uint8_t   BarEndSpec[10];
    uint8_t   BarEndNum[10];

    uint8_t   BarLableSpec[10];
    uint8_t   BarLableNum[10];
    
    uint8_t   PrintChoice;
    uint8_t   ProtectChoice;
    uint8_t   ScreenSaverShowTimeDate;
    uint8_t   ScreenSaverTime_Index;
    
    uint8_t   ClientChoice;
    uint8_t   Port_Status;
    uint8_t   PortSelSetting;
    uint8_t   PowerSaveChoiceState;
    
    uint8_t   SerialNumber[16];
    
    uint8_t   EnableToUpgarde;
    uint8_t   DhcpClient;
    uint8_t   Net_PortSelSetting;
    uint8_t   Net_PortSelectFunc;

    uint8_t   FreeLinePrint;
    uint8_t   SelectedManufacture;
    uint8_t   DualSyncLoadcell;
    uint8_t   DefualtChannel;
    
    uint8_t   ViewFormat;
    uint8_t   AutoSend;
    uint8_t   AutoSendNewOld;
    uint8_t   Reserve_4_Byte;
    
};

            
union SetParameter
{
    struct SettingParameters Set_Parameter ;
    uint32_t Set_Parameter_Words[sizeof(struct SettingParameters)/4];
};

struct CalibrationParameters
{
    uint32_t Max_weight_P1;
    uint32_t Max_weight_P2;
    int32_t  Zero_Atod;
    uint32_t Calibration_Weight_Number;
    union    LongChar W_factor;
    uint8_t  Divition_Index_P1;
    uint8_t  Divition_Index_P2;
    uint8_t  Point_Index;
    uint8_t  IsCalibrated;
    uint8_t  Zero_Percentage;
    uint8_t  Filter_Index;
    uint8_t  Stable_Time_Index;
    uint8_t  Stable_Extent_Index;

};

union CalParm
{ 
    struct CalibrationParameters CalibParameter[2];
    uint32_t CalibParameter_Words[sizeof(struct CalibrationParameters)/2];
};


struct WStructure
{
    int32_t   ADC_DATA;
    int32_t   Weight;
    int32_t   Round_Weight;
    int32_t   Pre_Weight;
    int32_t   offset;
    int32_t   ADC_WITHOUT_OFFSET;
    int32_t   Temp_Weight;
    int32_t   FilterTempWeight;
    int32_t   View_Weight;
    int32_t   minAdc;
    int32_t   maxAdc;
    uint16_t  Divition;
    uint16_t  Drift_Counter;
    
    uint32_t  Tare_Weight;
    
    int32_t signal[MAX_DIGREE ];
    
    bool Zero_Flag;
    bool Net_Flag;
    bool Stable_Flag;
    bool W1_Flag;
    bool W2_Flag;
    bool w_neg;
    bool neg_sign;
    bool Tare_Flag;
    bool Drift_Flag;
    bool Over_Flag;
    bool Under_Flag;
    bool data_completed;
};


union PluBarCode
{
    uint32_t PluBarcodeData[MAX_BARCODE_LENGHT/8];
    uint8_t  PluBarcodeDataBytes[MAX_BARCODE_LENGHT/2];
};


struct SellDetail
{
    uint32_t  SellPrice;
    uint32_t  PluCode;  
    uint32_t  DateTime; 
    uint32_t  Weight_Number;

    uint16_t  FactorNo;
    uint16_t  PluNumber;
    uint16_t  StoreScale;
      
    uint8_t   PluType;
    uint8_t   PluUnit;
    uint8_t   SaleOff;
    uint8_t   PluTax;
    uint8_t   SellType;
    uint8_t   SellRow;
    
    uint8_t   SellStatus;
    uint8_t   ClientId;
    uint8_t   reserve_1;
    uint8_t   reserve_2;
};

union SellDetailData
{
    struct   SellDetail SellData;
    uint8_t  SellDataBytes[sizeof(struct SellDetail)];
    uint32_t SellDataLong[sizeof(struct SellDetail)/4];
    
};

typedef enum
{
	/* Application's state machine's initial state. */
    APP_MENU_NULL_SETTING=0,
            
            
    APP_MENU_PLU_SETTING_NUMERAL, 
    APP_MENU_PLU_SETTING_BARCODE,
            
    APP_MENU_PLU_SETTING_ENTER_MEMORY,
            
    APP_MENU_PLU_SETTING_SCAN_BARCODE,            
            
      
    APP_MENU_PLU_SETTING_CHOICE_PRICE,
    APP_MENU_PLU_SETTING_CHOICE_NAME,
    APP_MENU_PLU_SETTING_CHOICE_CODE,
    APP_MENU_PLU_SETTING_CHOICE_PLU_TYPE,
    APP_MENU_PLU_SETTING_CHOICE_DISCOUNT,
    APP_MENU_PLU_SETTING_CHOICE_TAX,
            
            
            
    APP_MENU_PLU_SETTING_ENTER_PRICE,
    APP_MENU_PLU_SETTING_ENTER_NAME,
    APP_MENU_PLU_SETTING_ENTER_CODE,
    APP_MENU_PLU_SETTING_ENTER_PLU_TYPE,
    APP_MENU_PLU_SETTING_ENTER_DISCOUNT,
    APP_MENU_PLU_SETTING_ENTER_TAX,
            
    APP_MENU_PLU_SETTING_DOSE_SAVE,            

	APP_MENU_PLU_SETTING_CHOICE_LOAD_INFO,
	APP_MENU_PLU_SETTING_CHOICE_EXPORT_INFO,
    APP_MENU_PLU_SETTING_ENTER_LOAD_INFO,
	APP_MENU_PLU_SETTING_ENTER_EXPORT_INFO,
            

    APP_MENU_REPORT_CHOICE_DATE_SETTING,
    APP_MENU_REPORT_CHOICE_CLIENT_SETTING,
    APP_MENU_REPORT_CHOICE_TOTAL_MAYADIN,
    APP_MENU_REPORT_CHOICE_TOTAL_REPORT,
    APP_MENU_REPORT_CHOICE_CASH_TRANSFER,
    APP_MENU_REPORT_CHOICE_MEMORY_REPORT,
    APP_MENU_REPORT_CHOICE_NUMERAL_SETTING,
    APP_MENU_REPORT_CHOICE_DETAIL_REPORT,
    APP_MENU_REPORT_CHOICE_PLU_SPEC_REPORT,
    APP_MENU_REPORT_CHOICE_RETURN_REPORT,
    APP_MENU_REPORT_CHOICE_TOTAL_DELETE,
    APP_MENU_REPORT_CHOICE_FACTOR_DELETE,
    APP_MENU_REPORT_CHOICE_SELL_TRANSFER,

    APP_MENU_REPORT_ENTER_DATE_SETTING,
    APP_MENU_REPORT_ENTER_CLIENT_SETTING,
    APP_MENU_REPORT_ENTER_TOTAL_MAYADIN,
    APP_MENU_REPORT_ENTER_TOTAL_REPORT,
    APP_MENU_REPORT_ENTER_CASH_TRANSFER,        
    APP_MENU_REPORT_ENTER_MEMORY_REPORT,
    APP_MENU_REPORT_ENTER_NUMERAL_SETTING,
    APP_MENU_REPORT_ENTER_DETAIL_REPORT,
    APP_MENU_REPORT_ENTER_PLU_SPEC_REPORT,
    APP_MENU_REPORT_ENTER_RETURN_REPORT,
    APP_MENU_REPORT_ENTER_TOTAL_DELETE,
    APP_MENU_REPORT_ENTER_FACTOR_DELETE,
    APP_MENU_REPORT_ENTER_SELL_TRANSFER,
            
    APP_GET_DATE_TIME,
    APP_STATE_GLOBAL_MAKE_REPORT,
    APP_MENU_REPORT_PRINT_MEM_ITEMS_SALE,
    APP_MENU_REPORT_PRINT_MEM_ITEMS_RETURN,
            
            
    APP_STATE_OFFSET_CALIBRATION,
            
            
            
            
            
    APP_STATE_SET_CHOICE_DATE_TIME,
    APP_STATE_SET_CHOICE_BARCOD,
    APP_STATE_SET_CHOICE_PRINT,
    APP_STATE_SET_CHOICE_DISPLAY_PROTECT,
    APP_STATE_SET_CHOICE_CLIENT,
    APP_STATE_SET_CHOICE_FILTER,
    APP_STATE_SET_CHOICE_SOBAT,
    APP_STATE_SET_CHOICE_BEEP,
    APP_STATE_SET_CHOICE_PORTS,
    APP_STATE_SET_CHOICE_POWER_SAVE,
    APP_STATE_SET_SOFT_UPGRADE,
    APP_STATE_SET_SCALE_ID,
    APP_STATE_SET_CHOICE_WORKING_TIME,
            
    APP_STATE_SET_ENTER_DATE_TIME,
    APP_STATE_SET_ENTER_BARCOD,
    APP_STATE_SET_ENTER_PRINT,
    APP_STATE_SET_ENTER_DISPLAY_PROTECT,
    APP_STATE_SET_ENTER_CLIENT,
    APP_STATE_SET_ENTER_FILTER,
    APP_STATE_SET_ENTER_SOBAT,
    APP_STATE_SET_ENTER_BEEP,
    APP_STATE_SET_ENTER_PORTS,
    APP_STATE_SET_ENTER_POWER_SAVE,
    APP_STATE_SET_ENTER_SCALE_ID,        
            
   APP_STATE_SET_GET_UPTITR,         
   APP_STATE_SET_GET_DOWN,         
   APP_CALCULATOR_SERVEICE,         
   APP_SHOW_TIME_DATE_STATE,         
   APP_PAYBACK_TASK_STATE,         
   APP_STATE_SERVICE_MODE,
            
            
            
            
            
            
            
            
            
    

            
	APP_STATE_KALA_RETURN,
	APP_STATE_INIT,
    APP_STATE_OPEN_HOST_LAYER,
    APP_STATE_WAIT_FOR_HOST_ENABLE,
    APP_STATE_HOST_ENABLE_DONE,
    APP_STATE_WAIT_FOR_DEVICE_ATTACH,
    APP_STATE_DEVICE_ATTACHED,
    APP_STATE_READ_HID,
    APP_STATE_DEVICE_DETACHED,
    APP_STATE_CHANGE_DEVICE_PARAMETERS,
    APP_USART_STATE_DRIVER_OPEN,
    APP_USART_STATE_CHECK_FOR_STRING_TO_SEND,
    APP_USART_STATE_DRIVER_WRITE,
	APP_STATE_BUS_ENABLE,
    APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE,
    APP_STATE_DEVICE_CONNECTED,
    APP_STATE_MOUNT_DISK,
    APP_STATE_UNMOUNT_DISK,
    APP_STATE_OPEN_FILE,
    APP_STATE_WRITE_TO_FILE,
    APP_STATE_CLOSE_FILE,
    APP_STATE_IDLE,
    APP_STATE_ERROR,
    APP_STATE_SERVICE_TASKS,
    APP_STATE_SHOW_DEMO,
	APP_STATE_COUNT_DOWN,
	APP_STATE_ENTER_SERIAL_NO,
	APP_STATE_NORMAL_MODE,
    APP_STATE_CALIBRATION,
    APP_STATE_CALIBRATION_SETTING,
    APP_STATE_CALIBRATION_ERROR,
    APP_STATE_SCALE_LOCK,
    APP_STATE_SHOW_ADC_INFO,
    APP_STATE_SCAN_KEY_CODE,
    APP_STATE_SHOW_SUPPLY_INFO,
    APP_STATE_SHOW_FULL_SEGMENT,
    APP_STATE_ONE_GRAM,
    APP_STATESHOW_SERIALNO,
    APP_STATE_COUNT_MODE,
    APP_STATE_LOGIN_CLIENTS,
    APP_STATE_DO_NOTHING,
    APP_MENU_SETTING_EXIT_MENU,
    APP_STATE_DO_UNPLUG_DC,
    APP_STATE_QC_TIME,

	/* TODO: Define states used by the application state machine. */

} APP_STATES;


typedef struct
{
    /* The application's current state */
    APP_STATES state;
    
    /* USART Application task state */
    APP_STATES usartTaskState;
     
    /* Number of bytes written by the USART write*/
    uint32_t nBytesWritten;
    
    /* Size of the buffer to be written */
    uint32_t stringSize;
    
    /* Buffer used for USART writing */
    uint8_t string[64];
    
    /* Holds the current offset in the string buffer */
    uint16_t currentOffset;
    
    /* Flag used to determine if data is to be written to USART */
    _Bool stringReady;
    
    /* Flag used to select CAPSLOCK sequence */
    _Bool capsLockPressed;

    /* Application data buffer */
    uint32_t nBytesRead;

    

} APP_DATA;



