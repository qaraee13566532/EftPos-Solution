#define Display_Clock_2                 LATAbits.LATA10  
#define Display_Clock_1                 LATBbits.LATB13
#define Display_Data                    LATBbits.LATB15

// ADC 
#define Adc_Clock                       LATBbits.LATB8
#define Adc_Data                        PORTBbits.RB9
#define Channel_Selection               LATCbits.LATC13

// KEYBOARD ---------------------------------------------------
#define Key_Row1                        PORTCbits.RC15
#define Key_Row2                        PORTCbits.RC14
#define Key_Row3                        PORTCbits.RC12
#define Key_Row4                        PORTCbits.RC5
#define Key_Row5                        PORTCbits.RC4
#define Key_Row6                        PORTCbits.RC3
#define Key_Row7                        PORTDbits.RD3
#define Key_Row8                        PORTDbits.RD2


// BUZZER -----------------------------------------------------
#define BUZZER                          LATDbits.LATD0

// UPS CONTROL ------------------------------------------------
#define UPS_CTRL                        LATAbits.LATA0

// CASH DRAWER ------------------------------------------------
#define CASH                            LATAbits.LATA1

// CALIBRATION ---------------------------------------------
#define CAL_JMP                         PORTAbits.RA6
#define ONOFF_KEY                       PORTBbits.RB0


// LCD --------------------------------------------------------
#define LCD_LIGHT                       LATAbits.LATA12
#define LCD_READ                        LATCbits.LATC6
#define LCD_WRITE                       LATCbits.LATC7
#define LCD_CS                          LATBbits.LATB14
#define LCD_RESET                       LATAbits.LATA13
#define LCD_A0                          LATCbits.LATC8
#define LCD_DATA_0                      LATDbits.LATD1
#define LCD_DATA_1                      LATAbits.LATA5
#define LCD_DATA_2                      LATCbits.LATC9
#define LCD_DATA_3                      LATAbits.LATA15
#define LCD_DATA_4                      LATAbits.LATA14
#define LCD_DATA_5                      LATCbits.LATC10
#define LCD_DATA_6                      LATBbits.LATB6
#define LCD_DATA_7                      LATAbits.LATA7

// ONOFF KEY -------------------------------------------------
#define OnoffKey                        LATBbits.LATB0
// RELAY -----------------------------------------------------
#define Relay_1                         LATCbits.LATC0
#define Relay_2                         LATCbits.LATC1
#define Relay_3                         LATCbits.LATC2
#define Relay_4                         LATCbits.LATC11
#define Relay_5                         LATAbits.LATA3
#define Relay_6                         LATAbits.LATA8
#define Relay_7                         LATBbits.LATB4
#define Relay_8                         LATDbits.LATD4



// KEY COLUMN -----------------------------------------------------
#define KeyColumn_0                       LATCbits.LATC0
#define KeyColumn_1                       LATCbits.LATC1
#define KeyColumn_2                       LATCbits.LATC2
#define KeyColumn_3                       LATCbits.LATC11
#define KeyColumn_4                       LATAbits.LATA3
#define KeyColumn_5                       LATAbits.LATA8
#define KeyColumn_6                       LATBbits.LATB4
#define KeyColumn_7                       LATDbits.LATD4



