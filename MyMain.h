// Loadcell---------------------------------------------------------------------
#define LOADCELL_A         0X0
#define LOADCELL_B         0X1



#define  TAKIN_ELECTRONIC_ARKA    0
#define  TOZIN_SADR               1
#define  PAND                     2



// DISPLAY CONSTANT ------------------------------------------------------------
#define DISPLAY_DATA_COMMAND         0X40
#define DISPLAY_ADDRESS_COMMAND      0XC0
#define DISPLAY_CONTROL_COMMAND      0X88
#define DISP_NO                      32
#define POINT                        0x80
#define DISPOFF                      0x00
#define _0                           0x3F
#define _1                           0x06
#define _2                           0x5B
#define _3                           0x4F
#define _4                           0x66
#define _5                           0x6D
#define _6                           0x7D
#define _7                           0x07
#define _8                           0x7F
#define _9                           0x6F
#define MINUS                        0x40
#define POINT                        0x80
#define DISPOFF                      0x00
#define DISP_WEIGHT                  0
#define DISP_TOTAL_PRICE             2
#define DISP_UNIT_PRICE              1

//KEYBOARD CONSTANT ------------------------------------------------------------
#define LINE1 		                 1
#define LINE2 		                 2
#define LINE3 		                 4
#define LINE4 		                 8
#define KEY_ZERO_LIMIT               2
#define KEY_MAX                      60
#define MENU           51
#define ACCUM          48
#define TARE           54  
#define ZERO           55   
#define ONOFF          53   
#define RESULT         52   
#define MUL            50
#define DIV            46
#define MMINUS         44
#define CUST1          28
#define CUST2          33
#define CUST3          34
#define KEY_SHFT       31
#define PAY_BACK       30
#define MEMORY         29
#define PRINT          49
#define KEY_1          37
#define KEY_2          41    
#define KEY_3          45
#define KEY_4          38
#define KEY_5          42
#define KEY_6          47
#define KEY_7          35
#define KEY_8          39
#define KEY_9          43
#define KEY_0          32
#define KEY_00         36
#define CLEAR          40
#define PLU_0          3
#define PLU_1          7
#define PLU_2          11
#define PLU_3          15
#define PLU_4          19
#define PLU_5          23
#define PLU_6          27
#define PLU_7          2
#define PLU_8          6
#define PLU_9          10
#define PLU_10         14
#define PLU_11         18
#define PLU_12         22
#define PLU_13         26
#define PLU_14         1
#define PLU_15         5
#define PLU_16         9
#define PLU_17         13
#define PLU_18         17
#define PLU_19         21
#define PLU_20         25
#define PLU_21         0
#define PLU_22         4
#define PLU_23         8
#define PLU_24         12
#define PLU_25         16
#define PLU_26         20
#define PLU_27         24
#define MAX_KEY_COLUMN 8
#define MAX_KEY_ROW    8

//BUZZER CONSTANT --------------------------------------------------------------
#define BEEP_50ms                        50
#define BEEP_100ms                       100
#define BEEP_300ms                       300
#define BEEP_500ms                       500

// INTERNAL ADC CONSTANT -------------------------------------------------------
#define BATTERY_VOLTAGE_CHANNEL      19
#define DC_VOLTAGE_CHANNEL           18
#define DC_MAX_VALUE                          83 //8.3 volte


// STATE CONSTANT --------------------------------------------------------------
#define CALIBRATION           1
#define NORMAL_OPERATION      2
#define COUNT_MODE            3
#define ACCUM_MODE            4
#define SETUP_LABLE_MODE      5
#define DATE_TIME_MODE        7
#define SETUP_DATE_TIME_MODE  8
#define MAKE_LABLE_MODE       10
#define SETUP_PRINT_MODE      11   
#define SCALE_LOCK            12   
#define SETUP_COUNT_MODE      13 
#define SETUP_FUNC_MODE       14 
#define SHOW_REPORTS          15  
#define SETUP_CALC_MODE       16
#define PAY_BACK_MODE         17
#define MEM_DATA_CODE         18
#define SHOW_ADC_INFO         19  
#define SCAN_KEY_CODE         20
#define SHOW_SUPPLY_INFO      21
#define SHOW_FULL_SEGMENT     22

  
// STABLE PROCEDURE CONSTANT ---------------------------------------------------
#define STABLE_TIME_HIGH      1500
#define STABLE_TIME_MIDDLE    1000
#define STABLE_TIME_LOW       500
#define STABLE_EXTENT_HIGH    3
#define STABLE_EXTENT_MIDDLE  2
#define STABLE_EXTENT_LOW     1

// DATE CONSTANT ---------------------------------------------------------------
#define DATEOFST              79

//CALIBRATION CONSTANT ---------------------------------------------------------
#define MAX_DIFF	          5
#define MAX_DIGREE            15

// NVM ADRESSES ----------------------------------------------------------------
#define NVM_CAPACITY_MEGA_BYTE                          4
#define MAX_ALL_PLU                                     15000
#define MAX_BARCODE_LENGHT                              24
#define MAX_KALA_INFO_LENGHT                            64
#define MAX_DETAIL_INFO_LENGHT                          32
#define MAX_CODE_LENGHT                                 3
#define MAX_PLU_BYTES                                   2
#define ONE_PAGE_BYTES                                  4096


#define NVM_CALIBRATION_PAGE_ADDRESS                    0x9D03F800
#define NVM_SETTING_PAGE_ADDRESS                        0x9D03F000


#define NVM_MAX_ADDRESS                                 0x400000
#define NVM_VARIABLE_SETTING_PAGE_ADDRESS               0X00003000
#define NVM_BARCODE_PAGE_ADDRESS                        0X00004000 




#define NVM_KALA_INFO_PAGE_ADDRESS                      ((NVM_BARCODE_PAGE_ADDRESS + (MAX_ALL_PLU * MAX_BARCODE_LENGHT/2) + ONE_PAGE_BYTES) & 0xFFFFF000)


#define NVM_DETAIL_SELL_PAGE_ADDRESS                    (((NVM_BARCODE_PAGE_ADDRESS + (MAX_ALL_PLU * MAX_BARCODE_LENGHT/2) + ONE_PAGE_BYTES) & 0xFFFFF000) + \
                                                        (((MAX_ALL_PLU * MAX_KALA_INFO_LENGHT) + ONE_PAGE_BYTES ) & 0xFFFFF000))


#define NVM_DETAIL_SELL_ROW                             (NVM_MAX_ADDRESS-(((NVM_BARCODE_PAGE_ADDRESS + (MAX_ALL_PLU * MAX_BARCODE_LENGHT/2) + ONE_PAGE_BYTES) & 0xFFFFF000) + \
                                                        (((MAX_ALL_PLU * MAX_KALA_INFO_LENGHT) + ONE_PAGE_BYTES ) & 0xFFFFF000))) / MAX_DETAIL_INFO_LENGHT

                             
#define PHYS_ADDR(x)  (x & 0x1FFFFFFF)
#define FLASH_UPPER_ADDRESS 0x1FFFFFFF                

// SERIAL CONSTANT -------------------------------------------------------------
#define SER_MAX_WRITE                 5000
#define SER_MAX_READ                  500
#define PRN_MAX_WRITE                 2000
#define PRN_MAX_READ                  500

// CHARACTER CODE UTF8 ---------------------------------------------------------
#define UTF8_AeBakola         0xD8A2
#define UTF8_Alef             0xD8A7
#define UTF8_Be   	          0xD8A8
#define UTF8_Pe    	          0xD9BE
#define UTF8_Te               0xD8AA
#define UTF8_Ce               0xD8AB
#define UTF8_Jim              0xD8AC
#define UTF8_Che              0xDA86
#define UTF8_He_Jimi          0xD8AD
#define UTF8_Khe              0xD8AE
#define UTF8_Dal              0xD8AF
#define UTF8_Zal              0xD8B0
#define UTF8_Re               0xD8B1
#define UTF8_Ze               0xD8B2
#define UTF8_Je               0xDA98
#define UTF8_Cin              0xD8B3
#define UTF8_Shin             0xD8B4
#define UTF8_Sad              0xD8B5
#define UTF8_Zad              0xD8B6
#define UTF8_Ta               0xD8B7
#define UTF8_Za               0xD8B8
#define UTF8_Ein              0xD8B9
#define UTF8_Ghein            0xD8BA
#define UTF8_Fe               0xD981
#define UTF8_Ghaf             0xD982
#define UTF8_Kaf              0xDAA9
#define UTF8_Gaf              0xDAAF
#define UTF8_Lam              0xD984
#define UTF8_Mim              0xD985
#define UTF8_Non              0xD986
#define UTF8_Vav              0xD988
#define UTF8_He               0xD987
#define UTF8_Ye               0xDB8C
#define UTF8_Hamze_Tanha      0x0621
#define UTF8_Hamze		      0x0626
#define UTF8_Parantez_baz     0x0029
#define UTF8_Parantez_basteh  0x0028
#define UTF8_Darsad           0x0025
#define UTF8_Zarb             0x002a
#define UTF8_Alamat_Kochektar 0x003e
#define UTF8_Alamat_Bozorgtar 0x003c
#define UTF8_Mosavi           0x003d
#define UTF8_Momayez          0x002f
#define UTF8_Alamat_Soal      0x061f
#define UTF8_Alamat_Taajob    0x0021
#define UTF8_Giomeh           0x0022
#define UTF8_Manfi            0x002d
#define UTF8_PereProcess      0x0023
#define UTF8_Daler            0x0024
#define UTF8_Fasele           0x0020
#define UTF8_Jam              0x002b
#define UTF8_0                0x0660
#define UTF8_1                0x0661
#define UTF8_2                0x0662
#define UTF8_3                0x0663
#define UTF8_4                0x0664
#define UTF8_5                0x0665
#define UTF8_6                0x0666
#define UTF8_7                0x0667
#define UTF8_8                0x0668
#define UTF8_9                0x0669

// CHARACTER CODE UNICODE ------------------------------------------------------
#define UniCode_AeBakola         0x0622
#define UniCode_Alef             0x0627
#define UniCode_Be   	         0x0628
#define UniCode_Pe    	         0x067e
#define UniCode_Te               0x062a
#define UniCode_Ce               0x062b
#define UniCode_Jim              0x062c
#define UniCode_Che              0x0686
#define UniCode_He_Jimi          0x062d
#define UniCode_Khe              0x062e
#define UniCode_Dal              0x062f
#define UniCode_Zal              0x0630
#define UniCode_Re               0x0631
#define UniCode_Ze               0x0632
#define UniCode_Je               0x0698
#define UniCode_Cin              0x0633
#define UniCode_Shin             0x0634
#define UniCode_Sad              0x0635
#define UniCode_Zad              0x0636
#define UniCode_Ta               0x0637
#define UniCode_Za               0x0638
#define UniCode_Ein              0x0639
#define UniCode_Ghein            0x063a
#define UniCode_Fe               0x0641
#define UniCode_Ghaf             0x0642
#define UniCode_Kaf              0x06a9
#define UniCode_Gaf              0x06af
#define UniCode_Lam              0x0644
#define UniCode_Mim              0x0645
#define UniCode_Non              0x0646
#define UniCode_Vav              0x0648
#define UniCode_He               0x0647
#define UniCode_Ye               0x06cc
#define UniCode_0                0x0660
#define UniCode_1                0x0661
#define UniCode_2                0x0662
#define UniCode_3                0x0663
#define UniCode_4                0x0664
#define UniCode_5                0x0665
#define UniCode_6                0x0666
#define UniCode_7                0x0667
#define UniCode_8                0x0668
#define UniCode_9                0x0669
#define UniCode_Hamze_Tanha      0x0621
#define UniCode_Hamze		     0x0626
#define UniCode_Parantez_baz     0x0029
#define UniCode_Parantez_basteh  0x0028
#define UniCode_Darsad           0x0025
#define UniCode_Zarb             0x002a
#define UniCode_Alamat_Kochektar 0x003e
#define UniCode_Alamat_Bozorgtar 0x003c
#define UniCode_Mosavi           0x003d
#define UniCode_Momayez          0x002f
#define UniCode_Alamat_Soal      0x061f
#define UniCode_Alamat_Taajob    0x0021
#define UniCode_Giomeh           0x0022
#define UniCode_Manfi            0x002d
#define UniCode_PereProcess      0x0023
#define UniCode_Daler            0x0024
#define UniCode_Fasele           0x0020
#define UniCode_Jam              0x002b

// PRINTER CODE ----------------------------------------------------------------
#define Printer_0                           0x80
#define Printer_1                           0x81
#define Printer_2                           0x82
#define Printer_3                           0x83
#define Printer_4                           0x84
#define Printer_5                           0x85
#define Printer_6                           0x86
#define Printer_7                           0x87
#define Printer_8                           0x88
#define Printer_9                           0x89
#define Printer_VirGol                      0x8A
#define Printer_Manfi                       0x8B
#define Printer_Alamat_Soal                 0x8C
#define Printer_AeBakola                    0x8D
#define Printer_Hamze_Bozorg_Tanha          0xbc
#define Printer_Hamze_Bozorg_Chasban        0xff
#define Printer_Hamze_Kochak	            0x8e
#define Printer_Hamze_Tanha                 0x8f
#define Printer_Alef_Tanha                  0x90
#define Printer_Alef_Chasban                0x91
#define Printer_Be_Bozorg                   0x92
#define Printer_Be_Kochak                   0x93
#define Printer_Pe_Bozorg                   0x94
#define Printer_Pe_Kochak                   0x95
#define Printer_Te_Bozorg                   0x96
#define Printer_Te_Kochak                   0x97
#define Printer_Ce_Bozorg                   0x98
#define Printer_Ce_Kochak                   0x99
#define Printer_Jime_Bozorg                 0x9A
#define Printer_Jime_Kochak                 0x9B
#define Printer_Che_Bozorg                  0x9C
#define Printer_Che_Kochak                  0x9D
#define Printer_He_Bozorg                   0x9E
#define Printer_He_Kochak                   0x9F
#define Printer_Khe_Bozorg                  0xA0
#define Printer_Khe_Kochak                  0xA1
#define Printer_Dal                         0xA2
#define Printer_Zal                         0xA3
#define Printer_Re                          0xA4
#define Printer_Ze                          0xA5
#define Printer_Je                          0xA6
#define Printer_Cin_Bozorg                  0xA7
#define Printer_Cin_Kochak                  0xA8
#define Printer_Shin_Bozorg                 0xA9
#define Printer_Shin_Kochak                 0xAA
#define Printer_Sad_Bozorg                  0xAB
#define Printer_Sad_Kochak                  0xAC
#define Printer_Zad_Bozorg                  0xAD
#define Printer_Zad_Kochak                  0xAE
#define Printer_Ta                          0xAF
#define Printer_Za                          0xE0
#define Printer_Ein_Bozorg_Gheire_Chasban   0xE1
#define Printer_Ein_Bozorg_Chasban          0xE2
#define Printer_Ein_Kochak_Vasat_Chasban    0xE3
#define Printer_Ein_Kochak_Chasban          0xE4
#define Printer_Ghein_Bozorg_Gheire_Chasban 0xE5
#define Printer_Ghein_Bozorg_Chasban        0xE6
#define Printer_Ghein_Kochak_Vasat_Chasban  0xE7
#define Printer_Ghein_Kochak_Chasban        0xE8
#define Printer_Fe_Bozorg                   0xE9
#define Printer_Fe_Kochak                   0xEA
#define Printer_Ghaf_Bozorg                 0xEB
#define Printer_Ghaf_Kochak                 0xEC
#define Printer_Kaf_Bozorg                  0xED
#define Printer_Kaf_Kochak                  0xEE
#define Printer_Gaf_Bozorg                  0xEF
#define Printer_Gaf_Kochak                  0xF0
#define Printer_Lam_Bozorg                  0xF1
#define Printer_La_Bozorg                   0xF2
#define Printer_Lam_Kochak                  0xF3
#define Printer_Mim_Bozorg                  0xF4
#define Printer_Mim_Kochak                  0xF5
#define Printer_Non_Bozorg                  0xF6
#define Printer_Non_Kochak                  0xF7 
#define Printer_Vav                         0xF8
#define Printer_He_Tanha                    0xF9
#define Printer_He_Chasban                  0xFA
#define Printer_He_Aval                     0xFB
#define Printer_Ye_Bozorg_Chasban           0xFC
#define Printer_Ye_Bozorg_Tanha             0xFD
#define Printer_Ye_Kochak                   0xFE
#define Printer_Parantez_baz                0x29
#define Printer_Parantez_basteh             0x28
#define Printer_Darsad                      0x25
#define Printer_Zarb                        0x2a
#define Printer_Alamat_Kochektar            0x3e
#define Printer_Alamat_Bozorgtar            0x3c
#define Printer_Mosavi                      0x3d
#define Printer_Momayez                     0x2f
#define Printer_Alamat_Taajob               0x21
#define Printer_Giomeh                      0x22
#define Printer_PereProcess                 0x23
#define Printer_Daler                       0x24
#define Printer_Fasele                      0x20
#define Printer_Jam                         0x2b

//NETWORK CONSTANT -------------------------------------------------------------
#define RESERVE_1               0X00
#define RESERVE_2               0X00
#define RESERVE_3               0X00
#define SCALE_ENCRYPTION_KEY    0X49


// LED MESSAGES ----------------------------------------------------------------
#define _DELETE					0					
#define _REPORT					1
#define _PRINT					2	
#define _PAGE_ONE				3	
#define _PAGE_TWO				4
#define _VERSION				5
#define _PAY					6
#define _WAIT					7
#define _DONE					8	
#define _ERROR					9
#define _KEY					10
#define _PAYBACK				11	
#define _TOTAL					12
#define _SAVED					13
#define _NO						14
#define _OVER					15
#define _DISCOUNT				16
#define _SHIFT					17
#define _CUST1					18
#define _CUST2					19
#define _CUST3					20
#define _CALC					21
#define _ADCEX					22
#define _BIT_RES				23
#define _DC_VOLTAGE				24	
#define _BAT_VOLTAGE			25	
#define _MEM					26
#define _PRICE					27
#define _CODE					28
#define _NAME					29
#define _BLANK					30
#define _TITR					31
#define _UP						32
#define _DOWN					33
#define _ENTER					34
#define _PLU					35
#define _FILTER					36		
#define _MINIMUM				37
#define _AVEREGE				38	
#define _MAXIMUM				39
#define _BARCODE_TEXT			40
#define _BARCODE_ROW			41	
#define _BARCODE				42
#define _SCALE					43
#define _STORE					44
#define _BUZZER					45
#define _SINGLE_DISPLAY			46
#define _RELAY					47
#define _STABLE_TIME			48	
#define _ZERO_START				49
#define _DEVISION				50
#define _POINT					51
#define _FULL					52
#define _NOLOAD					53
#define _ADDLOAD				54	
#define _PROCESS				55
#define _COUNT					56
#define _UNIT					57
#define _KILOGRAM				58
#define _POUND					59	
#define _NET					60
#define _GROSS					61
#define _POWERSAVE				62
#define _ONE_MIN				63
#define _THREE_MIN				64
#define _FIVE_MIN				65	
#define _OFF					66
#define _BAUDRATE				67
#define _LIGHT					68
#define _ZERO_TRACK_WEIGHT		69
#define _ZERO_SCOP				70
#define _STABLE_EXENT			71	
#define _ZERO_TRACK_TIME		72	
#define _DATE_TYPE				73
#define _DATE_SHAMSI			74
#define _DATE_MILADI			75
#define _DATE_TRANSFER			76
#define _ON						77
#define _DATE					78
#define _TIME					79	
#define _RESET					80	
#define _CONTINIUE_TRANSMITION	81	
#define _STABLE_TRANSMITION		82	
#define _PC_CONTROL				83	
#define _SYNC_BAUDRATE			84	
#define _RECIPT_PRINTER			85	
#define _LABLE_PRINTER			86
#define _SELL_TRANSFER			87	
#define _NET_MODE	     		88
#define _SCALE_LOCK	     		89
#define _START   	     		90
#define _PAND   	     		91
#define _ON_GRAM   	     		92
#define _STOP   	     		93
#define _AUTO   	     		94
#define _MANUAL   	     		95
#define _UNLOCK   	     		96
#define _BTLOW   	     		97
#define _ADDMODE   	     		98
#define _CONTINIUE          	99	
#define _MEM2                	100	
#define _ADD                	101	
#define _CLEAR                	102	
#define _MODE                	103
#define _PCS					104
#define _LOAD					105
#define _WEIGHT					106
#define _SAVE                   107
#define _PASS                   108
#define _SUPPLY                 109
#define _SOFT                   110
#define _SYSTEM                 111
#define _LOCKED                 112
#define _UNLOCKED               113
#define _SETUP                  114
#define _SCAN                   115
#define _FACTOR                 116
#define _ROW                    117
#define _TYPE                   118
#define _TAX                    119
#define _MAKE                   120
#define _SELL                   121
#define _TRANSFER               122
#define _CLIENT                 123
#define _RETURN                 124
#define _CASH                   125
#define _NUMBER                 126
#define _DETAIL                 127
#define _SPEC                   128
#define _DATE_TIME              129
#define _DISPLAY                130
#define _STABLE                 131
#define _PORT                   132
#define _SERVICE                133
#define _SERIALNO               134
#define _UPGRADE                135
#define _ID                     136
#define _MAYADIN                137
#define _WORKING                138



// LCD CONSTANT ----------------------------------------------------------------
#define	ModeSet				0x38
#define	ModeSetP1			0x64	
#define	ModeSetP2			0x65	
#define Display_on 			0xaf
#define Display_off 		0xae
#define Regulator			0x26
#define Contrast_level		0x81
#define PowerControll_on1	0x2c
#define PowerControll_on2	0x2e
#define PowerControll_on3	0x2f
#define	LcdBias_9			0x54
#define	InterOsc_on			0xab
#define	EntireDisp_on		0xa5
#define	EntireDisp_off		0xa4
#define BoostLevel_5		0x66
#define	Duty_set			0x48
#define	Duty_setP			0x40
#define Start_columnlsb		0x00
#define Start_columnmsb		0x10
#define Start_page			0xb0
#define	StartLine_set		0x40
#define Set_Initial_COM0	0x44
#define Set_Initial_COM0P	0x12
#define Entire_Displa_ON	0xA5
#define Entire_Displa_OFF	0xA4
#define FrcPwm_set			0x92
#define NLineInversion_on	0x4c
#define NLineInversion_onP	0x1f
#define NLineInversion_off	0xe4	
#define ReverseDisp_on		0xa7
#define ReverseDisp_off		0xa6
#define AdcSelect			0xa0
#define ComScanDirection	0xc8
#define Contrast_levelP     0x0b


// GENERAL CONSTANT ------------------------------------------------------------
#define OFF                          0
#define ON                           1
#define NAME_LENGHT                  20 
#define TITR_LENGHT                  30
#define PLU_NAME                     2  
#define UP_TITR                      1  
#define DOWN_TITR                    0  
#define MAX_TOTAL_PRICE              4000000000
#define MAX_TOTAL_WN                 4000000000
#define MAX_DISCOUNT                 4000000000
#define MAX_CUSTOMER                 3
#define MAXROW                       50
#define MAX_PRINTER_CHAR             32
#define MAX_PLU                      56
#define false                        0
#define true                         1

#define BLINK_TIME_WAIT              500
#define BLINK_TIME                   300
#define MAXWEIGHT_1                  30000
#define MAXWEIGHT_2                  50000
#define DEFUALT_DIVITION             5
#define DEFUALT_POIN_INDEX           3
#define DEFUALT_FACTOR               0.2
#define DEFUALT_ZERO                 0
#define bool	                     _Bool
#define BLANK_CODE                   0x80
#define EFT_POS                      3
#define UNIT_PRICE_MAX               9999999
#define TOTAL_PRICE_MAX              99999999
#define NO_FILTER                    0
#define MAX_MESSAGE                  50
#define DEFAULT_WEIGHT_CALIBRATION   20000



// CALIBRATION CONSTANT --------------------------------------------------------
#define CALIB_BLANK                  0
#define CALIB_SHOWDEMO               1
#define CALIB_DIVITION               2
#define CALIB_POINT                  3
#define CALIB_FULL                   4
#define CALIB_NOLOAD                 5
#define CALIB_ADDLOAD                6
#define CALIB_DOCALIBRATE            7
#define CALIB_SHOW_SAVED             8
#define CALIB_DO_REGISTER            9
#define CALIB_SHOW_ERROR             10
#define CALIBRATE_LOCKPASS           116500



// SHOW DEMO CONSTANT ----------------------------------------------------------
#define SHOW_DEMO_STAT1              0
#define SHOW_DEMO_STAT2             1
#define SHOW_DEMO_STAT3              2

// SERIAL FLASH CONSTANT -------------------------------------------------------
#define FLASH_PAGE_WRITE          0x02
#define FLASH_FAST_READ           0x0B
#define FLASH_JEDECID             0xBF0226BF
#define FLASH_ERASE               0xC7
#define FLASH_WEN                 0x06
#define FLASH_BLKUP               0x42
#define FLASH_EQIO                0x38

// SETUP TIME & DATE -----------------------------------------------------------
#define SETUP_TIME_DATE_SHOW_DEMO  0
#define SETUP_TIME_SET             1
#define SETUP_DATE_SET             2
#define SETUP_TIME_DATE_SHOW_ERROR 3

// SHOW TIME & DATE ------------------------------------------------------------
#define SHOW_TIME_DATE_DEMO    0
#define SHOW_TIME              1
#define SHOW_DATE              2

// SETUP LABLE CONSTANT---------------------------------------------------------
#define SETUP_LABLE_SHOW_DEMO    0
#define SETUP_LABLE_PROCESS      1
#define SHOW_DATE                2

//NORMAL MODE CONSTANT ---------------------------------------------------------
#define NORMAL_MODE_OPERATE            0
#define NORMAL_MODE_SHOW_ERROR         1
#define NORMAL_MODE_DELAY              2
#define DISCUNT_DELAY                  650
#define RELAY_DELAY                    500
#define SELL_CLEAR_DELAY               1000
#define CASH_DELAY                     300
#define NORMAL_MODE_CASH_OPEN          3
#define SHOW_CUST_DELAY                500

// INTERNAL ADC CONSTANT -------------------------------------------------------
#define MaxInternalAdcSample           15
#define MAX_SAMPLE_TIME                300
#define DC_FAIL_LEVEL                  1750 // 8v 
#define MIN_ACCEPTABLE_DC              1950 // 9v 
#define MIN_ACCEPTABLE_BAT             1150 //0.344
#define BAT_FAIL_SURE_CNT              2*MAX_SAMPLE_TIME
#define BAT_SHUTDOWN_CNT               2*MAX_SAMPLE_TIME
#define BAT_SHUTDOWN_BUZZER_CNT        100
#define BAT_BLINK_INDICATOR            MAX_SAMPLE_TIME

// CHANE NOTEIFICATION PORT SETTING CONSTANT -----------------------------------
#define SYS_PORT_C_ANSEL_NEW        0xe008
#define SYS_PORT_C_TRIS_NEW         0xf00e
#define SYS_PORT_C_LAT_NEW          0x10
#define SYS_PORT_C_ODC_NEW          0x0
#define SYS_PORT_C_CNPU_NEW         0x0
#define SYS_PORT_C_CNPD_NEW         0xf018
#define SYS_PORT_C_CNEN_NEW         0x2

// CHANE NOTEIFICATION PORT SETTING CONSTANT -----------------------------------
#define SYS_PORT_C_ANSEL_UPDC        0xe008
#define SYS_PORT_C_TRIS_UPDC         0xf008
#define SYS_PORT_C_LAT_UPDC          0x10
#define SYS_PORT_C_ODC_UPDC          0x0
#define SYS_PORT_C_CNPU_UPDC         0x0
#define SYS_PORT_C_CNPD_UPDC         0xf018
#define SYS_PORT_C_CNEN_UPDC         0x0

// USB MSD CONSTANT ------------------------------------------------------------
#define ERROR_NOT_OPENED            0
#define ERROR_NOT_WRITE             1
#define SUCCESSFULLY_WRITE          2

// BARCODE CONSTANT ------------------------------------------------------------
#define MaxReadSize                 0xFFE0ul
#define MaxSqiSector                MaxReadSize/32
#define MaxPages                    (MAX_ALL_PLU*(MAX_BARCODE_LENGHT/2)/MaxReadSize) +1
#define ReadInitCode                0x004A0000ul | MaxReadSize

//COUNT MODE CONSTANTS ---------------------------------------------------------
#define COUNT_SHOW_DEMO             0
#define COUNT_WAIT                  1
#define COUNT_LOAD_LAST_PLU_INFO    2
#define COUNT_START                 3
#define COUNT_SETUP_ENTER_PCS       4
#define COUNT_SETUP_LOAD_WEIGHT     5
#define COUNT_SETUP_ENTER_PLU       6
#define COUNT_SETUP_DO_CALIBRATE    7
#define COUNT_SHOW_RESUALT          8
#define COUNT_SETUP_READY_TO_SAVE   9
#define COUNT_SETUP_SHOW_ERROR      10
#define COUNT_ZERO_ERROR_VALUE      11
#define COUNT_ZERO_ERROR_STABLE     12
#define COUNT_DELAY                 500

//LCD FARSI CODE CONSTANT ------------------------------------------------------

#define Lcd_0                           0x81
#define Lcd_1                           0x82
#define Lcd_2                           0x83
#define Lcd_3                           0x84
#define Lcd_4                           0x85
#define Lcd_5                           0x86
#define Lcd_6                           0x87
#define Lcd_7                           0x88
#define Lcd_8                           0x89
#define Lcd_9                           0x8A
#define Lcd_VirGol                      0x2C
#define Lcd_Manfi                       0x2D
#define Lcd_Alamat_Soal                 0x7F
#define Lcd_AeBakola                    0x9B
#define Lcd_Hamze_Bozorg_Tanha          0x9C
#define Lcd_Hamze_Bozorg_Chasban        0x9D
#define Lcd_Hamze_Kochak	            0x9E
#define Lcd_Hamze_Tanha                 0x80
#define Lcd_Alef_Tanha                  0xA0
#define Lcd_Alef_Chasban                0xA1
#define Lcd_Be_Bozorg                   0xA2
#define Lcd_Be_Kochak                   0xA4
#define Lcd_Pe_Bozorg                   0x8B
#define Lcd_Pe_Kochak                   0x8D
#define Lcd_Te_Bozorg                   0xA6
#define Lcd_Te_Kochak                   0xA8
#define Lcd_Ce_Bozorg                   0xAA
#define Lcd_Ce_Kochak                   0xAC
#define Lcd_Jime_Bozorg                 0xAE
#define Lcd_Jime_Kochak                 0xB0
#define Lcd_Che_Bozorg                  0x8F
#define Lcd_Che_Kochak                  0x91
#define Lcd_He_Bozorg                   0xB2
#define Lcd_He_Kochak                   0xB4
#define Lcd_Khe_Bozorg                  0xB6
#define Lcd_Khe_Kochak                  0xB8
#define Lcd_Dal                         0xBA
#define Lcd_Zal                         0xBC
#define Lcd_Re                          0xBE
#define Lcd_Ze                          0xC0
#define Lcd_Je                          0x93
#define Lcd_Cin_Bozorg                  0xC2
#define Lcd_Cin_Kochak                  0xC4
#define Lcd_Shin_Bozorg                 0xC6
#define Lcd_Shin_Kochak                 0xC8 
#define Lcd_Sad_Bozorg                  0xCA 
#define Lcd_Sad_Kochak                  0xCC
#define Lcd_Zad_Bozorg                  0xCE
#define Lcd_Zad_Kochak                  0xD0
#define Lcd_Ta                          0xD2
#define Lcd_Za                          0xD4
#define Lcd_Ein_Bozorg_Gheire_Chasban   0xD6
#define Lcd_Ein_Bozorg_Chasban          0xD7
#define Lcd_Ein_Kochak_Vasat_Chasban    0xD9
#define Lcd_Ein_Kochak_Chasban          0xD8
#define Lcd_Ghein_Bozorg_Gheire_Chasban 0xDA
#define Lcd_Ghein_Bozorg_Chasban        0xDB
#define Lcd_Ghein_Kochak_Vasat_Chasban  0xDD
#define Lcd_Ghein_Kochak_Chasban        0xDC
#define Lcd_Fe_Bozorg                   0xDE
#define Lcd_Fe_Kochak                   0xE0
#define Lcd_Ghaf_Bozorg                 0xE2
#define Lcd_Ghaf_Kochak                 0xE4
#define Lcd_Kaf_Bozorg                  0xE6
#define Lcd_Kaf_Kochak                  0xE8
#define Lcd_Gaf_Bozorg                  0x95
#define Lcd_Gaf_Kochak                  0x97
#define Lcd_Lam_Bozorg                  0xEA
#define Lcd_Lam_kochak_Chasban          0xEC
#define Lcd_La_Bozorg                   0xFF
#define Lcd_Lam_Kochak                  0xEC
#define Lcd_Mim_Bozorg                  0xEE
#define Lcd_Mim_Kochak                  0xF0
#define Lcd_Non_Bozorg                  0xF2
#define Lcd_Non_Kochak                  0xF4 
#define Lcd_Vav                         0xFA
#define Lcd_He_Tanha                    0xF6
#define Lcd_He_Chasban                  0xF7
#define Lcd_He_Aval                     0xF8
#define Lcd_He_VASAT                    0xF9
#define Lcd_Ye_Bozorg_Chasban           0xFD
#define Lcd_Ye_Bozorg_Tanha             0xFC
#define Lcd_Ye_Kochak                   0xFE
#define Lcd_Parantez_baz                0x29
#define Lcd_Parantez_basteh             0x28
#define Lcd_Darsad                      0x25
#define Lcd_Zarb                        0x2a
#define Lcd_Alamat_Kochektar            0x3c
#define Lcd_Alamat_Bozorgtar            0x3e
#define Lcd_Mosavi                      0x3d
#define Lcd_Momayez                     0x2f
#define Lcd_Alamat_Taajob               0x21
#define Lcd_Giomeh                      0x22
#define Lcd_PereProcess                 0x23
#define Lcd_Daler                       0x24
#define Lcd_Fasele                      0x20
#define Lcd_Jam                         0x2b
#define MAX_LCD_MESSAGE                 50

//CLIENT CONSTANT --------------------------------------------------------------
#define MAX_CLIENT                      100
#define ADMIN_BUFF_INDEX                0
#define ADMIN_DEFAULT_PASS              1111
#define SHOW_PASS_MESSAGE               0
#define DO_CHECK_PASS                   1
#define WAIT_SHOW_MESSAGE               2
#define WAIT_SHOW_WELCOM                3
#define WAIT_TO_EXIT                    4
#define MAXPASS                         9999
#define ADMINISTRATOR                   1
#define ERROR_SHOW_DELAY                1000
#define WELCOM_DELAY                    1200

// LCD MESSAGES ID CONSTANT ----------------------------------------------------

#define PAND_MESSAGE                      0
#define ENTER_PASSWORD                    1
#define RAMZ_ESHTEBAH_AST                 2
#define CLIENT_ID                         3
#define WELCOME                           4
#define ADMIN_ID_MSG                      5
#define ATOD_SHOW                         6
#define SHOW_ATOD_RES                     7
#define SHOW_ATOD_NUM                     8
#define TAEIN_DEGHAT                      9
#define DEGHAT_MOSAVI                     10
#define TAEIN_ASHAR                       11
#define ASHAR_MOSAVI                      12
#define TAEIN_ZARFIAT                     13
#define ZARFIAT_MOSAVI                    14
#define TAEIN_SEFR                        15
#define TAEIN_VAZN_KALIBRE                16
#define VAZN_KALIBRE_MOSAVI               17
#define LOTFAN_MONTAZER_BEMANID           18
#define DARSADE_ANJAME_KALIBRE            19
#define NAMAYESHE_VAZNE_KALIBREH_SHODE    20
#define KALIBRASION                       21
#define TANZIMAT_ZAKHIREH_SHOD            22
#define VAZNE_MOSAVI                      23
#define KILO                              24
#define GERAM                             25
#define NEMAYESHE_KODE_KELIDHA            26
#define KODE_KELID_MOSAVI                 27
#define NAMAYESHE_VOLTAGE_TAGHZIEH        28
#define TAGHZIEH_BARGH_MOSAVI             29
#define TAGHZIEH_BATRI_MOSAVI             30
#define NEMAYESHE_NEMAYESHGARHA           31
#define DEGHATE_NAMAYESH_ROYE             32
#define YEK_GRAM_TANZIM_SHOD              33
#define DEGHATE_NEMITAVANAD_ROYE          34
#define YEK_GRAM_TANZIM_SHAVAD            35
#define REMZ_GHOFLE_NARMAFZARI             36
#define TARIKHE_GOFL_SYSTEM                37
#define REMZ_GHEIRE_FAAL_SHODAN_GHOFL      38
#define GHOFLE_NARMAFZARI_FAAL_AST         39
#define TARIKH_ESHTEBAH_AST                40
#define GHOFLE_NARMAFZARI_FAAL_SHOD        41
#define GHOFLE_NARMAFZARI_GHEIRE_FAAL_SHOD 42
#define SYSTEM_TAVASOTE_GHOFLE             43
#define NARMAFZARI_SHODEH_AST              44

#define TANZIMATE_KALA                        45
#define TANZIME_KALAYE_BARKODI                46
#define TANZIME_KALAYE_DASTI                  47
#define TANZIME_NAME_KALA                     48
#define TANZIME_GHEYMATE_KALA                 49
#define TANZIME_KODE_KALA                     50
#define SHOMARE_HAFEZE_RA_ENTEKHAB_KONID      51
#define SHOMARE_HAFEZE                        52
#define BARCODE_KALA_RA_ESKAN_KONID           53
#define KHATA                                 54
#define BARCODE_GHABLAN_TARIF_SHODE           55
#define HAFEZE_KHALI_MOJOD_NIST               56

#define GHEYMATE_KALA_RA_VARED_KONID          57
#define GHEYMATE_KALA                         58
#define KODE_KALA_RA_VARED_KONID              59
#define KODE_KALA                             60
#define NAME_KALA_RA_VARED_KONID              61
#define NAME_KALA                             62
#define AYA_TAGHIRAT_ZAKHIREH_SHAVAD          63
#define TAGHIRAT_ZAKHIREH_SHOD                64
#define BARGOZARI_AZ_FLASH_MEMORY             65
#define ZAKHIREH_ROYE_FLASH_MEMORY            66

#define ENTEKHABE_BAZE_HAFEZE                 67
#define AZ_HAFEZE                             68
#define TA_HAFEZE                             69

#define FLASH_MEMORY_MOTASEL_NIST             70
#define FILE_MOJOD_NIST                       71
#define BAZE_HAFEZE_SAHIH_NIST                72
#define FILE_SAHIH_NIST                       73

#define SABAD_FOROSH_KHALI_AST                74
#define KALAYE_HAZF_SHODE                     75
#define SHOMARE_FACTOR_RA_VARED_KONID         76
#define FACTOR                                77
#define IN_FACTOR_MOJOD_NIST                  78
#define FACTOR_HAZF_SHOD                      79
#define KALA_HAZF_SHOD                        80
#define NOE_KALA_TEDADI_NIST                  81
#define RADIFE_KALA                           82
#define RADIFE_KALA_MOJOD_NIST                83
#define AYA_KALA_HAZF_SHAVAD                  84
#define NOE_KALA_VAZNI_NIST                   85
#define FOROSH_BA_GHEYMATE_DASTI              86

#define NOE_FOROSHE_KALA                      87
#define DARSADE_TAKHFIFE_KALA                 88
#define MALIAT_ARZESHE_AFZODE                 89
#define VAZNI                                 90
#define TEDADI                                91
#define TAKHFIF                               92
#define MALIAT                                93


#define GOZARESHAT                            94
#define TAEIN_TARIKHE_GOZARESH                95
#define ROZE_JARI                             96
#define AZ_TARIKH_TA_TARIKH                   97
#define KOLI                                  98
#define TARIKH_SHOROE_GOZARESH                99
#define TARIKH_PAYANE_GOZARESH                100
#define TAEIN_NOE_GOZARESH                    101
#define BAR_ASASE_KARBARE_KHAS                102
#define KOLIE_KARBARAN                        103
#define CODE_KARBAR                           104
#define GOZARESH_MOSHAKHASAT_KALA             105
#define AZ_HAFEZE_TA_HAFEZE                   106
#define BE_TAFKIK                             107
#define GOZARESHE_FOROSHE_KARBARAN            108
#define GOZARESHE_FOROSHE_KOLI                109
#define GOZARESHE_FOROSHE_DASTI               110
#define GOZARESHE_FOROSHE_HAFEZE              111
#define VAZNI_KOL                             112
#define TEDADI_KOL                            113
#define GOZARESH_RIZE_FOROSH                  114
#define GOZARESH_MARJOEI_KALA                 115
#define HZFE_KOLI_FOROSH                      116
#define HAZF_FACTORE_KHAS                     117
#define ENTEGHALE_FOROSH_BE_FLASH             118
#define ZAMAN_ESHTEBAH_AST                    119
#define KHATA_DAR_TARIKH_GOZARESH             120
#define CODE_KARBAR_RA_VARED_KONID            121
#define CODE_KARBAR_MOJAZ_NIST                122
#define DASTRESI_GHIRE_MOJAZ                  123
#define HAFEZEH_PORSHODE_AST                  124
#define FOROSHI_SABT_NASHODE                  125
#define AYA_MOTMAEN_HASTID                    126
#define ETELAATE_FOROSH_HAZF_SHOD             127
#define TARIKH_FACTOR                         128
#define GOZARESHE_SANDOGH                     129

#define TANZIMAT_SYSTEM                       130
#define TARIKH_VA_ZAMAN                       131
#define BARCODE                               132
#define TANZIMATE_GHABZ                       133
#define MOHAFEZE_SAFHE_NMAYESH                134
#define KARBAR                                135
#define FILTERING_VAZN                        136
#define SOBATE_VAZN                           137
#define BOGHE_SAFHE_KELID                     138
#define PORTE_SAKHT_AFZARI                    139
#define TAEIN_NOE_TARIKH                      140
#define SHAMSI                                141
#define MILADI                                142
#define TANZIME_TAIKH                         143
#define TANZIME_SAAT                          144
#define TANZIME_SAAT_WB                       145
#define TAEIN_TARIK_WB                        146
#define TAEIN_NOE_TARIKH_WB                   147
#define TANZIMATE_BARCODE_WB                  148

#define BARCODE_SATR_GHABZ                    149
#define BARCODE_ENTEHAYE_GHABZ                150
#define BARCODE_LABLE_PRINTER                 151
#define BARCODE_SATR_GHABZ_WB                 152
#define BARCODE_ENTEHAYE_GHABZ_WB             153
#define BARCODE_LABLE_PRINTER_WB              154

#define VAZEIAT                               155
#define NOE_BARCODE                           156
#define ERTEFAE_BARCODE                       157
#define MOSHAKHASATE_BARCODE                  158
#define NEMAYESHE_ARGHAME_BARCODE             159

#define VAZEIAT_WB                            160
#define NOE_BARCODE_WB                        161
#define ERTEFAE_BARCODE_WB                    162
#define MOSHAKHASATE_BARCODE_WB               163
#define NEMAYESHE_ARGHAME_BARCODE_WB          164

#define FAAL                                  165
#define GHEIRE_FAAL                           166
#define BAR_UPC_A                             167
#define BAR_UPC_E                             168
#define BAR_EAN13                             169
#define BAR_EAN8                              170
#define BAR_CODE39                            171
#define BAR_I25                               172
#define BAR_CODEBAR                           173
#define BAR_CODE93                            174
#define BAR_CODE128                           175
#define BAR_CODE11                            176
#define BAR_MSI                               177
#define ERTEFA                                178

#define MOSHAKHASE_1                          179
#define MOSHAKHASE_2                          180
#define MOSHAKHASE_3                          181
#define MOSHAKHASE_4                          182
#define MOSHAKHASE_5                          183
#define MOSHAKHASE_6                          184
#define MOSHAKHASE_7                          185
#define MOSHAKHASE_8                          186
#define MOSHAKHASE_9                          187
#define MOSHAKHASE_10                         188

#define VAZNE_KALA                            189
#define CODE_KALA                             190
#define ADADE_SABET                           191
#define NOE_KALA                              192
#define CODE_FOROSH                           193
#define GHEYMATE_VAHED                        194
#define TARIKH_JARI                           195
#define ZAMANE_JARI                           196
#define MALIATE_KALA                          197
#define TAKHFIF_KALA                          198
#define KODE_FOROSHANDE                       199
#define STORE_KALA                            200
#define SCALE_KALA                            201
#define SHOMARE_MOSHTARI                      202
#define SHOMARE_SATR                          203
#define SHOMARE_FACTOR                        204
#define JAME_KOL                              205
#define RADIFE_KOL                            206


#define TITRE_BALAYE_GHABZ                    207
#define TITRE_PAEINE_GHABZ                    208
#define TANZIME_AMALKERDE_CHAP                209

#define TITRE_BALAYE_GHABZ_WB                 210
#define TITRE_PAEINE_GHABZ_WB                 211
#define TANZIME_AMALKERDE_CHAP_WB             212


#define HAR_SE_MOSHTARI                       213
#define TAK_MOSHTARI                          214
#define NEMAYESHE_TARIKH_SAAT                 215
#define TANZIME_NORE_NEMAYESHGAR              216
#define TANZIME_ZAMANE_AMALKARD               217
#define NEMAYESHE_TARIKH_SAAT_WB              218
#define TANZIME_NORE_NEMAYESHGAR_WB           219
#define TANZIME_ZAMANE_AMALKARD_WB            220

#define PAS_AZ_YEK_DAGHIGHE                   221
#define PAS_AZ_SE_DAGHIGHE                    222
#define PAS_AZ_PANJ_DAGHIGHE                  223
#define SATHE_NOR                             224
#define EIJADE_KARBARE_JADID                  225
#define HAZFE_KARBAR                          226
#define HAZFE_KOLI_KARBARAN                   227
#define TAGHIRE_RAMZ                          228

#define RAMZE_VORODE_KARBAR                   229
#define TEKRARE_RAMZ                          230
#define RAMZ_MOJOD_AST                        231















#define TANZIMATE_SAHIH_NIST                  232
#define SABT_MAGHDOR_NEMIBASHAD               233
#define KARBAR_TARIF_NASHODE                  234
#define HAZF_MODIR_MAGHDOR_NIST               235


#define ZAEIF                                 236
#define MOTAVASET                             237
#define GHAVI                                 238

#define TAGHIRATE_KAM                         239
#define TAGHIRATE_MOTAVAST                    240
#define TAGHIRATE_BALA                        241

#define TANZIMATE_PORT                        242
#define PORTE_SERIAL                          243
#define PORTE_SHABAKE                         244
#define PORTE_USB                             245
#define TANZIME_PORTE_SERIAL                  246
#define TANZIME_AMALKARD_SERIAL               247
#define TANZIME_SORAT                         248
#define KHOROJI_PORTE_SERIAL                  249
#define ETESAL_BE_PRINTER                     250
#define ETESAL_BE_RAYANEH                     251
#define ETESAL_BE_LEIBEL_PRINTER              252
#define ETESAL_BE_EFTPOS                      253
#define ETESAL_BE_BARCODE_SCANNER             254
#define ERSALE_VAZN                           255
#define ERSALE_VAZNE_SABET                    256
#define ERSALE_VAZNE_VA_TARIKH                257
#define ERSAL_VAZNE_SABET_VA_TARIKH           258
#define ERSALE_GHADIMI_3B                     259
#define ERSALE_GHADIMI_5B                     260
#define ERSALE_GHADIMI_11B                    261
#define ERSALE_ETELAATE_FOROSH                262
#define SOATE_ENTEGHALE_ETELAAT               263
#define TANZIME_PORTE_SHABAKE                 264
#define TANZIME_AMALKARD_SHABAKE              265
#define NAHVE_IP_GIRI                         266
#define IP_DASTGAH                            267
#define SUBNET_IP                             268
#define GATEWAY_IP                            269
#define DNS_IP                                270
#define IP_SERVER                             271
#define SERVER_PORT                           272

#define KHOROJI_PORTE_SHABAKE                 273
#define NAHVE_IP_GIRI_WB                      274
#define IP_DASTGAH_WB                         275
#define SUBNET_IP_WB                          276
#define GATEWAY_IP_WB                         277
#define DNS_IP_WB                             278
#define IP_SERVER_WB                          279
#define SERVER_PORT_WB                        280

#define DASTI                                 281
#define OTOMATIC                              282









#define TANZIMATE_USB_DAR_IN                  283
#define NOSKHE_MAGHDOR_NEMIBASHAD             284

#define MODE_SHOMARESHE_GHATAAT               285
#define TANZIME_TEDAD_GHATAAT                 286
#define TEDADE_GHATEH_RA_VARED_NAMEID         287
#define GHATAAT_RA_BARGOZARI_NAMAEID          288
#define SABTE_ZARIBE_GHATAAT                  289
#define TEDADE_GHATEH                         290


#define MASRAFE_BEHINEH_BATRY                 291
#define TAEIN_ZAMANE_FAAL_SHODAN              292
#define ZAMANE_FAAL_SHODAN                    293
#define ZAMAN_DAGHIGHEH                       294
#define ZAMANE_FAAL_SHODAN_WB                 295
#define VAZEIAT_BEHINEH_SAZI_WB               296
#define VAZEIAT_BEHINEH_SAZI                  297
#define BAR_ROYE_KAFE_ZIAD_AST                298
#define VAZNE_KAFE_SABET_NIST                 299
#define VAZNE_KAFE_MANFI_AST                  300

#define KHATA_GHEYMATE_FOROSHE_KALA           301
#define MOJAZ_NIST                            302
#define VAZN_GHEIRE_MOJAZ_AST                 303

#define KHATA_KALAYE_FOROKHTEH                304
#define SHODE_BISH_AZ_HADE_MOJAZF             305

#define KHATA_JAME_FOROSHE_KALA               306
#define KELID_ADAD_GHOFL_SHODE                307
#define KELID_HAFEZEH_GHOFL_SHODE             308

#define HAFEZEH_MOJOD_NIST                    309
#define MOHASEBEH_MAGHDOOR_NIST               310

#define MASHINE_HESAB                         311
#define MOHASEBE_MABAGHI_POL                  312


#define KHATA_MABLAGHE_PARDAKHTIN             313
#define KAMTAR_AZ_FACTOR_AST                  314

#define KHATA_VAZNE_GHATAAT_KAMTAR            315
#define AZ_DEGHATE_DASTGAH_AST                316

#define GHEYMATE_VAHEDE_SEFR_AST              317

#define RAMZE_SERVICE_MODE                    318
#define SERVICE_MODE                          319
#define RISETE_TANZIMATE_KARKHANEH            320
#define RISETE_MOSHAKHASATE_KALAHA            321
#define RISETE_FOROSH                         322
#define RISETE_KOLI                           323
#define TAEIN_TEDADE_HAFEZE                   324
#define TAEIN_TEDADE_HAFEZE_WB                325
#define TEDADE_HAFEZAH                        326
#define TANZIMAT_EMAL_SHOD                    327

#define VORODE_SERIAL_DASTGAH                 328
#define BE_ROZ_RESANI_NARMAFZAR               329
#define SERIAL_RA_ESKAN_KONID                 330
#define SERIAL_SABT_SHOD                      331
#define SERIAL_DASTGAH                        332

#define KHATA_TEDADE_HAFEZE_BISH              333
#define AZ_128_ADDA_AST                       334

#define IP_SAHIH_NIST                         335
#define ESHKAL_DAR_ETESALE_SHABAKE            336
#define ETESAL_FAAL_NIST                      337
#define KHATA_HALATE_IP_DEHI                  338
#define RA_DASTI_NEMAEID                      339
#define AMALIAT_NAMOVAFAGH_BOD                340
#define SHOMARE_PORT                          341

#define SHENASE_TARAZO                        342
#define SHENASE_TARAZO_WB                     343
#define SHOMARE_GHORFE                        344
#define SHOMARE_TARAZO                        345
#define SHOMARE_GHORFE_WB                     346
#define SHOMARE_TARAZO_WB                     347
#define GHORFEH                               348
#define TARAZO                                349
#define KHATA_DAR_VOLTAGE_ADAPTOR             350
#define ADAPTOR_RA_TAVIZ_NEMAEID              351
#define GOZARESH_MAYADIN                      352
#define FOROSHE_KOL                           353
#define GOZARESH_MAYADIN_ALAMAT               354
#define VAZNE_KOL                             355
#define TEDADE_KOL                            356
#define MEGHDARE_KHOROJE_KAGHAZ               357
#define MEGHDARE_KHOROJE_KAGHAZ_WB            358
#define KHOROJE_KAGHAZ_KHAT                   359
#define MOSHAHEDE_ZAMANE_KARKARD              360
#define ZAMANE_KARKARDE_SYSTEM                361
#define ZAMAN_SAAT                            362
#define TARIKH_CONTROL_KEIFI                  363
#define TenMinuts                             1000*60*10




#define NET_TIME_OUT_SERVER_CONNECTION        3000


//SHOW ADC CONSTANT ------------------------------------------------------------
#define SHOW_ADC_DEMO                   0
#define SHOW_ADC_TASK                   1
#define SHOW_ADC_ERROR_SHOW             2
#define CHANNEL_A             0
#define CHANNEL_B             1
#define SAMPLE_TIME_MILI_SECONT         100



//SHOW SCAN CODE CONSTANT ------------------------------------------------------
#define SHOW_SCAN                       0
#define SCAN_DO_TASK                    1
#define SCAN_CODE_EXIT                  2
#define SHOW_ONOFF_DELAY                500

// SUPPLY INFO CONSTANT --------------------------------------------------------
#define SHOW_SUPPLY_DEMO                0
#define SUPPLY_DO_TASKS                 1

// FULL SEGMENT CONSTANT -------------------------------------------------------
#define FULL_SEG_DEMO                   0
#define FULL_SEG_DO_TASKS               1

// ONE GRAM CONSTANT -----------------------------------------------------------
#define ONE_GRAM_DEMO                   0
#define ONE_GRAM_WAIT                   1
#define ONE_GRAM_DELAY                  2000

// LOCK CONSTANT ---------------------------------------------------------------
#define LOCKPASS                        120010
#define LOCK_SHOW_DEMO                  0
#define ENTER_PASSWORD                  1
#define ENTER_DATE                      2
#define UNLOCK_PASSWORD                 3
#define LOCK_SHOW_LAST                  4
#define SHOW_LOCK_ERROR                 5
#define END_WAIT                        6
#define JMPTOUNLOCK                     7
#define PINCODE_LAST_DELAY              1500

// MENU SETTING ----------------------------------------------------------------
#define FELESH_RAST                     0x1C
#define FELESH_CHAP                     0x11
#define FELESH_BALA                     0x1D
#define FELESH_PAEIN                    0x1E
#define RIGHT_SIDE                       0
#define LEFT_SIDE                        1
#define UP_SIDE                          2
#define DOWN_SIDE                        3
#define ENTER_SIDE                       4
#define EXIT_SIDE                        5


#define Dc_Coeff                          4.581632
#define Bat_Coeff                         9.442528


#define NUMERAL_CHOICE_PLU_SETTING_INIT         0
#define NUMERAL_CHOICE_PLU_DO_TASKS             1

#define BARCODE_CHOICE_PLU_SETTING_INIT         0
#define BARCODE_CHOICE_PLU_DO_TASKS             1

#define NUMERAL_ENTER_MEMORY_INIT               0
#define NUMERAL_ENTER_MEMORY_DO_TASKS           1


#define BARCODE_ENTER_MEMORY_INIT               0
#define BARCODE_ENTER_MEMORY_DO_TASKS           1
#define READY_TO_SCAN_BARCODE                   2
#define CHECK_SCAN_BARCODE                      3
#define BARCODE_ENTER_MEMORY_MAKE_DELAY         4 
#define BARCODE_ENTER_MEMORY_RETURN             6 
#define BARCODE_ENTER_MEMORY_RETURN_DELAY       1200 



#define PLU_SETTING_PRICE_INIT                  0
#define PLU_SETTING_PRICE_DO_TASKS              1
#define READY_TO_GET_PRICE                      2

#define PLU_SETTING_CODE_INIT                   0
#define PLU_SETTING_CODE_DO_TASKS               1

#define PLU_SETTING_NAME_INIT                   0
#define PLU_SETTING_NAME_DO_TASKS               1

// USB FILE DATA CONSTANT ------------------------------------------------------

#define  LOAD_FILE_INIT              0
#define  FILE_OPENED                 1
#define  LOAD_ERROR_STATE            2
#define  READ_HEADER_START_ADDRESS   3
#define  READ_HEADER_LENGHT          4
#define  READ_TO_START_DATA          5
#define  READ_MAIN_DATA              6
#define  LOAD_PLU_TYPE               7
#define  LOAD_ROW_DATA_PROCESS       8
#define  LOAD_BULK_DATA_PROCESS      9


        
#define FILE_PLU_NUMBER              0
#define FILE_PLU_NAME                1
#define FILE_PLU_SELL_PRICE          2
#define FILE_PLU_BUY_PRICE           3
#define FILE_PLU_TYPE                4
#define FILE_PLU_CODE                5
#define FILE_PLU_SALE_OFF            6
#define FILE_PLU_TAX                 7
#define FILE_PLU_QUANTITY            8
#define FILE_PLU_UNIT                9
#define FILE_PLU_BARCODE             10
#define FILE_PLU_BAR_TYPE            11

#define  DELIMITED                   0x0009


// EXPORT FILE CONSTANT---------------------------------------------------------
#define  EXPORT_INFO_INIT                 0
#define  EXPORT_INFO_INIT_DO_TASKS        1
#define  EXPORT_INFO_ERROR                2
#define  EXPORT_INFO_ERROR_SHOW_DELAY     1500
#define  EXPORT_FILE_INIT                 0
#define  EXPORT_FILE_BIGIN                1
#define  EXPORT_FILE_WRITE_HEADER         2
#define  EXPORT_ERROR_STATE               3
#define  EXPORT_FILE_WRITE_DATA           4
#define  EXPORT_QUIT                      5
#define  UNICODE_HEADER                   0XFEFF
#define  EXPORT_FILE_GET_UP_DOWN_VAL      6
#define  EXPORT_ERROR_MEMORY              7
#define  EXPORT_SHOW_DELAY                350

// IMPORT FLE CONSTANT----------------------------------------------------------


#define LOAD_INFO_INIT 0
#define LOAD_INFO_INIT_DO_TASKS 1
#define LOAD_INFO_ERROR 2
#define LOAD_INFO_ERROR_SHOW_DELAY   1500


#define ERASE_CALIB     0
#define ERASE_CONFIG    1
#define ERASE_REPORT    2      
#define ERASE_ALL       4

#define DATE_UP   1
#define DATE_EQUL 0
#define DATE_DOWN 2

#define REPORT_TOTAL         0
#define REPORT_MEM           1
#define REPORT_NUMERAL       2
#define REPORT_MEM_VAZNI     3
#define REPORT_MEM_TEDADI    4
#define REPORT_MEM_BETAFKIK  5
#define REPORT_MEM_KOLI      6
#define REPORT_RETURN        7
#define REPORT_CASH_TOTAL        8
#define REPORT_MEMTOMEM_SALE     9
#define REPORT_MEMTOMEM_RETURN   10
#define REPORT_MEMTOMEM_MAYADIN   11








#define RETURN_KALA_INIT    0
#define RETURN_SHOW_INIT    1
#define RETURN_ERROR_SHOW   2
#define LOAD_FACTOR_DATA    3  
#define ENTER_FACTOR_NO     4  
#define ENTER_FAC_DO_TASKS  5  
#define SHOW_HAZF_SHOD      6 
#define CONFIRM_TO_DELETE   7  
#define MAX_FACTOR                50000 
#define RETURN_SHOW_ERROR_DELAY   1200 
#define SHOW_HAZF_SHOD_DELAY      1200 

#define DEFAULT_SERVER_PORT        16000
#define DEFAULT_SERVER_IP          "192.168.1.1"




