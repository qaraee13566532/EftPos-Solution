#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "xc.h"
#include "uart3.h"
#include "../MyMain.h"
#include "../define_type.h"
#include "../HardwareProfile.h"

extern  uint32_t     Change_To_long(uint8_t Buf[10]);
extern  uint8_t      PageWrite(uint32_t PageNumber);
extern  uint8_t      DataPack[200],TempBuffer[10];

extern uint8_t  ResetCause  __attribute__ ((persistent));

extern bool price_received;

extern  union SetParameter SystemParm;
extern  const uint16_t  CrcTable[];
extern  uint16_t beez_time;
extern  uint32_t total_price;


static uint8_t * volatile rxTail;
static uint8_t *rxHead;
static uint8_t *txTail;
static uint8_t * volatile txHead;
static bool volatile rxOverflowed;


extern void SendPriceToEftPos(void);
extern void UpdateSetting(void);
extern void Rs232OutTasks(void );
extern void UART2_Initialize (void);
extern void UART2_Write(uint8_t byte);
extern void DelayUs(uint32_t DelaySet);
extern void GetConfiguration(uint8_t ProtocolFunction);
extern uint16_t update_crc(uint16_t crc_accum, unsigned char *data_blk_ptr, uint16_t data_blk_size,bool reflect_input,bool reflect_output);

/** UART Driver Queue Length

  @Summary
    Defines the length of the Transmit and Receive Buffers

*/

#define UART3_CONFIG_TX_BYTEQ_LENGTH (200+1)
#define UART3_CONFIG_RX_BYTEQ_LENGTH (200+1)

/** UART Driver Queue

  @Summary
    Defines the Transmit and Receive Buffers

*/

static uint8_t txQueue[UART3_CONFIG_TX_BYTEQ_LENGTH];
static uint8_t rxQueue[UART3_CONFIG_RX_BYTEQ_LENGTH];

void (*UART3_TxDefaultInterruptHandler)(void);
void (*UART3_RxDefaultInterruptHandler)(void);

/**
  Section: Driver Interface
*/

void UART3_Initialize(void)
{
    uint32_t crystal=6000000;
    IEC1bits.U3TXIE = 0;
    IEC1bits.U3RXIE = 0;

    // STSEL 1; PDSEL 8N; RTSMD disabled; OVFDIS disabled; ACTIVE disabled; RXINV disabled; WAKE disabled; BRGH enabled; IREN disabled; ON enabled; SLPEN disabled; SIDL disabled; ABAUD disabled; LPBACK disabled; UEN TX_RX; CLKSEL SYSCLK; 
    // Data Bits = 8; Parity = None; Stop Bits = 1;
    U3MODE = (0x28008 & ~(1<<15));  // disabling UART ON bit
    // UTXISEL TX_ONE_CHAR; UTXINV disabled; ADDR 0; MASK 0; URXEN disabled; OERR disabled; URXISEL RX_ONE_CHAR; UTXBRK disabled; UTXEN disabled; ADDEN disabled; 
    U3STA = 0x00;
    // U3TXREG 0; 
    U3TXREG = 0x00;
    // BaudRate = 115200; Frequency = 24000000 Hz; BRG 51; 
    if(SystemParm.Set_Parameter.In_PosBaudRate<=0 || SystemParm.Set_Parameter.In_PosBaudRate>115200)
        U3BRG =0x33;
    else
        U3BRG = crystal/SystemParm.Set_Parameter.In_PosBaudRate;

    
    txHead = txQueue;
    txTail = txQueue;
    rxHead = rxQueue;
    rxTail = rxQueue;
   
    rxOverflowed = 0;

    UART3_SetTxInterruptHandler(UART3_Transmit_ISR);

    UART3_SetRxInterruptHandler(UART3_Receive_ISR);

    IEC1bits.U3RXIE = 1;
    
    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    U3STASET = _U3STA_UTXEN_MASK;
    U3MODESET = _U3MODE_ON_MASK;   // enabling UART ON bit
    U3STASET = _U3STA_URXEN_MASK;
}

/**
    Maintains the driver's transmitter state machine and implements its ISR
*/

void UART3_SetTxInterruptHandler(void* handler)
{
    UART3_TxDefaultInterruptHandler = handler;
}

void __attribute__ ((vector(_UART3_TX_VECTOR), interrupt(IPL1SOFT))) _UART3_TX ( void )
{
    (*UART3_TxDefaultInterruptHandler)();
}

void UART3_Transmit_ISR ( void )
{ 
    if(txHead == txTail)
    {
        IEC1bits.U3TXIE = 0;
        return;
    }

    IFS1CLR= 1 << _IFS1_U3TXIF_POSITION;

    while(!(U3STAbits.UTXBF == 1))
    {
        U3TXREG = *txHead++;

        if(txHead == (txQueue + UART3_CONFIG_TX_BYTEQ_LENGTH))
        {
            txHead = txQueue;
        }

        // Are we empty?
        if(txHead == txTail)
        {
            break;
        }
    }
}

void UART3_SetRxInterruptHandler(void* handler)
{
    UART3_RxDefaultInterruptHandler = handler;
}

void __attribute__ ((vector(_UART3_RX_VECTOR), interrupt(IPL1SOFT))) _UART3_RX( void )
{
    (*UART3_RxDefaultInterruptHandler)();
}

void UART3_Receive_ISR(void)
{

    while((U3STAbits.URXDA == 1))
    {
        *rxTail = U3RXREG;

        // Will the increment not result in a wrap and not result in a pure collision?
        // This is most often condition so check first
        if ( ( rxTail    != (rxQueue + UART3_CONFIG_RX_BYTEQ_LENGTH-1)) &&
             ((rxTail+1) != rxHead) )
        {
            rxTail++;
        } 
        else if ( (rxTail == (rxQueue + UART3_CONFIG_RX_BYTEQ_LENGTH-1)) &&
                  (rxHead !=  rxQueue) )
        {
            // Pure wrap no collision
            rxTail = rxQueue;
        } 
        else // must be collision
        {
            rxOverflowed = true;
        }

    }

    IFS1CLR= 1 << _IFS1_U3RXIF_POSITION;
}

void __attribute__ ((vector(_UART3_ERR_VECTOR), interrupt(IPL1SOFT))) _UART3_ERR( void )
{
    if ((U3STAbits.OERR == 1))
    {
        U3STACLR = _U3STA_OERR_MASK; 
    }
    
    IFS1CLR= 1 << _IFS1_U3EIF_POSITION;
}

/**
  Section: UART Driver Client Routines
*/

uint8_t UART3_Read( void)
{
    uint8_t data = 0;

    while (rxHead == rxTail )
    {
    }
    
    data = *rxHead;

    rxHead++;

    if (rxHead == (rxQueue + UART3_CONFIG_RX_BYTEQ_LENGTH))
    {
        rxHead = rxQueue;
    }
    return data;
}

void DelayUs(uint32_t DelaySet)
{
    uint32_t longp=6*DelaySet;
    while(longp--)
        Nop();
}  

void send_Novins800_Response_To_Samankish(uint8_t data)
{
    uint8_t index=0;
    for(index=0;index<6;index++)
       Write_Serial_TO_INPUT(0x00);
    Write_Serial_TO_INPUT(0x72);
    
    if(data!=00)
    {
        Write_Serial_TO_INPUT(0x06);
        Write_Serial_TO_INPUT(0xB1);
        Write_Serial_TO_INPUT(0x04);
        Write_Serial_TO_INPUT(0x88);
        Write_Serial_TO_INPUT(0x02);
        Write_Serial_TO_INPUT(0x01);
        Write_Serial_TO_INPUT(data);
    }
    else
    {
        Write_Serial_TO_INPUT(0x58);
        Write_Serial_TO_INPUT(0xB1);
        Write_Serial_TO_INPUT(0x56);
        Write_Serial_TO_INPUT(0x85);
        Write_Serial_TO_INPUT(0x06);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x31);

        Write_Serial_TO_INPUT(0x8C);
        Write_Serial_TO_INPUT(0x06);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x31);

        Write_Serial_TO_INPUT(0x8B);
        Write_Serial_TO_INPUT(0x0C);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x31);

        Write_Serial_TO_INPUT(0x8A);
        Write_Serial_TO_INPUT(0x10);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);

        Write_Serial_TO_INPUT(0x89);
        Write_Serial_TO_INPUT(0x08);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);

        Write_Serial_TO_INPUT(0x88);
        Write_Serial_TO_INPUT(0x02);
        Write_Serial_TO_INPUT(0x01);
        Write_Serial_TO_INPUT(data);
        
        Write_Serial_TO_INPUT(0x87);
        Write_Serial_TO_INPUT(0x13);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        Write_Serial_TO_INPUT(0x30);
        
        Write_Serial_TO_INPUT(0x86);
        Write_Serial_TO_INPUT(0x01);
        Write_Serial_TO_INPUT(0x31);
        
    }
}

void NovinResponse_To_SamanKishProtocol_1_Response(uint8_t data)
{
    static uint8_t InputPriceStatre=0,PriceCent=0,Len;
    if(InputPriceStatre==0 && data=='R')
        InputPriceStatre=1;
    else
    if(InputPriceStatre==1 && data=='S')
        InputPriceStatre=2;
    else
    if(InputPriceStatre==2 && data=='0')
        InputPriceStatre=3;
    else
    if(InputPriceStatre==3 && data=='0')
        InputPriceStatre=4;
    else
    if(InputPriceStatre==4 && data=='2')
        InputPriceStatre=5;
    else
    if(InputPriceStatre==5)
    {
        InputPriceStatre=6;
        TempBuffer[0]=data;
    }
    else
    if(InputPriceStatre==6)
    {
        InputPriceStatre=7;
        TempBuffer[1]=data;
    }
    else
    if(InputPriceStatre==7)
    {
        InputPriceStatre=0;
        if(TempBuffer[0]=='5' && TempBuffer[1]=='5')
            send_Novins800_Response_To_Samankish(55);
        else
        if(TempBuffer[0]=='1' && TempBuffer[1]=='2')
            send_Novins800_Response_To_Samankish(63);
        else
        if(TempBuffer[0]=='5' && TempBuffer[1]=='0')
            send_Novins800_Response_To_Samankish(5);
        else
        if(TempBuffer[0]=='5' && TempBuffer[1]=='4')
            send_Novins800_Response_To_Samankish(78);
        else
        if(TempBuffer[0]=='5' && TempBuffer[1]=='1')
            send_Novins800_Response_To_Samankish(51);
        else
        if(TempBuffer[0]=='5' && TempBuffer[1]=='6')
            send_Novins800_Response_To_Samankish(78);
        else
        if(TempBuffer[0]=='5' && TempBuffer[1]=='8')
            send_Novins800_Response_To_Samankish(57);
        else
        if(TempBuffer[0]=='6' && TempBuffer[1]=='1')
            send_Novins800_Response_To_Samankish(61);
        else
        if(TempBuffer[0]=='6' && TempBuffer[1]=='5')
            send_Novins800_Response_To_Samankish(75);
        else
        if(TempBuffer[0]=='9' && TempBuffer[1]=='9')
            send_Novins800_Response_To_Samankish(3);
        else
        if(TempBuffer[0]=='0' && TempBuffer[1]=='0')
            send_Novins800_Response_To_Samankish(0);
        else
            send_Novins800_Response_To_Samankish(84);
    }
    else
        InputPriceStatre=0;     
}

void InputDevice_SamanKishPosProtocol_1(uint8_t data)
{
    static uint8_t InputPriceStatre=0,PriceCent=0,Len;
    uint8_t SettingPartState=0,index;
    uint32_t TempVariable=0,crci;
  /*  if(InputPriceStatre==0 && data==0x72)
        InputPriceStatre=1;
    else
    if(InputPriceStatre==1)
        InputPriceStatre=2;
    else*/
    if(InputPriceStatre==0 && data==0xB1)
        InputPriceStatre=1;
    else
    if(InputPriceStatre==1)
        InputPriceStatre=2;
    else
    if(InputPriceStatre==2 && data==0x81)
        InputPriceStatre=3;
    else
    if(InputPriceStatre==3)
    {
        Len=data;
        InputPriceStatre=4;
        PriceCent=0;
        for(index=0;index<10;index++)
           TempBuffer[index]=0; 
    }
    else
    if(InputPriceStatre==4)
    {
        if(Len>0)
        {
            TempBuffer[PriceCent]=data;
            PriceCent++;
            TempBuffer[PriceCent]=0x00;
            Len--;
        }
        else
        {
            InputPriceStatre=0;
            if(price_received==false)
            {
                total_price=Change_To_long(TempBuffer);
                price_received=1;
            }
        }
    }
    else
        InputPriceStatre=0;    
}
void InputDevice_CasScale(uint8_t data)
{
    static uint8_t InputPriceStatre=0,PriceCent=0;
    uint8_t Len,SettingPartState=0,index;
    uint32_t TempVariable=0,crci;
    if(InputPriceStatre==0 && data=='<')
        InputPriceStatre=1;
    else
    if(InputPriceStatre==1 && data=='a')
        InputPriceStatre=2;
    else
    if(InputPriceStatre==2 && data=='m')
        InputPriceStatre=3;
    else
    if(InputPriceStatre==3 && data=='o')
        InputPriceStatre=4;
    else
    if(InputPriceStatre==4 && data=='u')
        InputPriceStatre=5;
    else
    if(InputPriceStatre==5 && data=='n')
        InputPriceStatre=6;
    else
    if(InputPriceStatre==6 && data=='t')
        InputPriceStatre=7;
    else
    if(InputPriceStatre==7 && data=='>')
    {
        InputPriceStatre=8;
        PriceCent=0;
        for(index=0;index<10;index++)
           TempBuffer[index]=0; 
    }
    else
    if(InputPriceStatre==8)
    {
        if(data>='0' && data<='9')
        {

            TempBuffer[PriceCent]=data;
            PriceCent++;
            TempBuffer[PriceCent]=0x00;
        }
        else
        {
            if(data=='<')
                InputPriceStatre=9;
             else
                InputPriceStatre=0;
        }
    }
    else
    if(InputPriceStatre==9 && data=='/')
        InputPriceStatre=10;
    else
    if(InputPriceStatre==10 && data=='a')
        InputPriceStatre=11;
    else
    if(InputPriceStatre==11 && data=='m')
        InputPriceStatre=12;
    else
    if(InputPriceStatre==12 && data=='o')
        InputPriceStatre=13;
    else
    if(InputPriceStatre==13 && data=='u')
        InputPriceStatre=14;
    else
    if(InputPriceStatre==14 && data=='n')
        InputPriceStatre=15;
    else
    if(InputPriceStatre==15 && data=='t')
        InputPriceStatre=16;
    else
    if(InputPriceStatre==16 && data=='>')
    {
        InputPriceStatre=0;
        price_received=1;
        total_price=Change_To_long(TempBuffer);
    }
    else
        InputPriceStatre=0;
   
}

bool TakinTasks(uint8_t data)
{
    static uint8_t TakinCalibState=0,LoopCnt=0,crclow=0,crchigh=0,ProtocolAddress=0,ProtocolDataLenght=0,ProtocolFunction=0,TempCnt;
    uint8_t Len,SettingPartState=0;
    uint32_t TempVariable=0,crci;
    switch(SystemParm.Set_Parameter.In_EftPo_Type)
    {
        case 19:
            InputDevice_CasScale(data);
        break;
        case 2:
            InputDevice_SamanKishPosProtocol_1(data);
        break;
    }
    switch(SystemParm.Set_Parameter.Out_EftPo_Type)
    {
        case 6:
            NovinResponse_To_SamanKishProtocol_1_Response(data);
        break;
    }
    if(TakinCalibState==0 && data==0xaa)
    {
        TakinCalibState=1;
    }
    else
    if(TakinCalibState==1 && data==0x55)
    {
        TakinCalibState=2;
    }
    else
    if(TakinCalibState==2)
    {
        ProtocolDataLenght=data;
        TakinCalibState=3;
    }    
    else
    if(TakinCalibState==3)
    {
        ProtocolFunction=data;
        DataPack[0]=ProtocolFunction;
        if(ProtocolDataLenght<=1)
            TakinCalibState=5;
        else
            TakinCalibState=4;
        LoopCnt=1;
    }
    else
    if(TakinCalibState==4)
    {
        DataPack[LoopCnt]=data;
        LoopCnt++;
        if(LoopCnt==ProtocolDataLenght)
            TakinCalibState=5;
    }
    else
    if(TakinCalibState==5)
    {
        TakinCalibState=6;
        crchigh=data;

    }
    else
    if(TakinCalibState==6)
    {
        TakinCalibState=0;
        crclow=data;
        TempVariable=crchigh;
        TempVariable<<=8;
        TempVariable|=crclow;
        crci=update_crc(0,DataPack,ProtocolDataLenght,false,false);
        if(ProtocolFunction==0x1)
           crci= TempVariable;
        if(TempVariable==crci)
        {
            switch(ProtocolFunction)
            {
                case 0x02:
                    GetConfiguration(ProtocolFunction);
                break;
                case 0x01:
                    LoopCnt=0;
                    SettingPartState=0;
                    TempCnt=0;   
                    do
                    {
                        data=DataPack[LoopCnt];
                        if(data!=',')
                        {
                            if(data>='0' && data<='9')
                            {
                                TempBuffer[TempCnt]=data;
                                TempCnt++;
                                TempBuffer[TempCnt]=0x00;
                            }
                        }
                        if(data==',' || LoopCnt==ProtocolDataLenght+1)
                        {
                            switch(SettingPartState)
                            {
                                case 0:
                                    SettingPartState=1;
                                     SystemParm.Set_Parameter.Out_EftPo_Type=Change_To_long(TempBuffer);
                                break;
                                case 1:
                                    SettingPartState=2;
                                     SystemParm.Set_Parameter.Out_PosBaudRate=Change_To_long(TempBuffer);
                                break;
                                case 2:
                                    SettingPartState=3;
                                     SystemParm.Set_Parameter.Toman_Mode=Change_To_long(TempBuffer);
                                break;
                                case 3:
                                    SettingPartState=4;
                                     SystemParm.Set_Parameter.In_EftPo_Type=Change_To_long(TempBuffer);
                                break;
                                case 4:
                                    SettingPartState=0;
                                     SystemParm.Set_Parameter.In_PosBaudRate=Change_To_long(TempBuffer);
                                break;
                            }
                            for(TempCnt=0;TempCnt<10;TempCnt++)
                                TempBuffer[TempCnt]=0;
                            TempCnt=0;
                        }
                        LoopCnt++;

                    }while(LoopCnt<ProtocolDataLenght+2);
                    GetConfiguration(ProtocolFunction);
                    __builtin_disable_interrupts();
                    DelayUs(100);
                    UpdateSetting();
                    DelayUs(100);
                    __builtin_enable_interrupts();
                break;

            }
        }
        else
        {
            TakinCalibState=0;
        }
    }
    else
    {
        TakinCalibState=0;
    }    
}

void Write_Serial_TO_INPUT(uint8_t data)
{
    UART3_Write(data);
}



void Rs232InTasks(void )
{
    uint8_t data = 0;
    if (rxHead != rxTail )
    {
        data = *rxHead;
        rxHead++;
        if (rxHead == (rxQueue + UART3_CONFIG_RX_BYTEQ_LENGTH))
        {
            rxHead = rxQueue;
        }
        TakinTasks(data);
    }
}


void Pc_Command_Services(void)
{
    Rs232InTasks();
    Rs232OutTasks();
    SendPriceToEftPos();
}

void UART3_Write( uint8_t byte)
{
    while(UART3_IsTxReady() == 0)
    {
    }

    *txTail = byte;

    txTail++;
    
    if (txTail == (txQueue + UART3_CONFIG_TX_BYTEQ_LENGTH))
    {
        txTail = txQueue;
    }

    IEC1bits.U3TXIE = 1;
}

bool UART3_IsRxReady(void)
{    
    return !(rxHead == rxTail);
}

bool UART3_IsTxReady(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( UART3_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    return (size != 0);
}

bool UART3_IsTxDone(void)
{
    if(txTail == txHead)
    {
        return (bool)U3STAbits.TRMT;
    }
    
    return false;
}


/*******************************************************************************

  !!! Deprecated API !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

static uint8_t UART3_RxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_rxTail = (uint8_t*)rxTail;
    
    if (snapshot_rxTail < rxHead) 
    {
        size = ( UART3_CONFIG_RX_BYTEQ_LENGTH - (rxHead-snapshot_rxTail));
    }
    else
    {
        size = ( (snapshot_rxTail - rxHead));
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

static uint8_t UART3_TxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( UART3_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

unsigned int __attribute__((deprecated)) UART3_ReadBuffer( uint8_t *buffer ,  unsigned int numbytes)
{
    unsigned int rx_count = UART3_RxDataAvailable();
    unsigned int i;
    
    if(numbytes < rx_count)
    {
        rx_count = numbytes;
    }
    
    for(i=0; i<rx_count; i++)
    {
        *buffer++ = UART3_Read();
    }
    
    return rx_count;    
}

unsigned int __attribute__((deprecated)) UART3_WriteBuffer( uint8_t *buffer , unsigned int numbytes )
{
    unsigned int tx_count = UART3_TxDataAvailable();
    unsigned int i;
    
    if(numbytes < tx_count)
    {
        tx_count = numbytes;
    }
    
    for(i=0; i<tx_count; i++)
    {
        UART3_Write(*buffer++);
    }
    
    return tx_count;  
}

UART3_TRANSFER_STATUS __attribute__((deprecated)) UART3_TransferStatusGet (void )
{
    UART3_TRANSFER_STATUS status = 0;
    uint8_t rx_count = UART3_RxDataAvailable();
    uint8_t tx_count = UART3_TxDataAvailable();
    
    switch(rx_count)
    {
        case 0:
            status |= UART3_TRANSFER_STATUS_RX_EMPTY;
            break;
        case UART3_CONFIG_RX_BYTEQ_LENGTH:
            status |= UART3_TRANSFER_STATUS_RX_FULL;
            break;
        default:
            status |= UART3_TRANSFER_STATUS_RX_DATA_PRESENT;
            break;
    }
    
    switch(tx_count)
    {
        case 0:
            status |= UART3_TRANSFER_STATUS_TX_FULL;
            break;
        case UART3_CONFIG_RX_BYTEQ_LENGTH:
            status |= UART3_TRANSFER_STATUS_TX_EMPTY;
            break;
        default:
            break;
    }

    return status;    
}

uint8_t __attribute__((deprecated)) UART3_Peek(uint16_t offset)
{
    uint8_t *peek = rxHead + offset;
    
    while(peek > (rxQueue + UART3_CONFIG_RX_BYTEQ_LENGTH))
    {
        peek -= UART3_CONFIG_RX_BYTEQ_LENGTH;
    }
    
    return *peek;
}

bool __attribute__((deprecated)) UART3_ReceiveBufferIsEmpty (void)
{
    return (UART3_RxDataAvailable() == 0);
}

bool __attribute__((deprecated)) UART3_TransmitBufferIsFull(void)
{
    return (UART3_TxDataAvailable() == 0);
}

uint32_t __attribute__((deprecated)) UART3_StatusGet (void)
{
    return U3STA;
}

unsigned int __attribute__((deprecated)) UART3_TransmitBufferSizeGet(void)
{
    if(UART3_TxDataAvailable() != 0)
    { 
        if(txHead > txTail)
        {
            return(txHead - txTail);
        }
        else
        {
            return(UART3_CONFIG_TX_BYTEQ_LENGTH - (txTail - txHead));
        }
    }
    return 0;
}

unsigned int __attribute__((deprecated)) UART3_ReceiveBufferSizeGet(void)
{
    if(UART3_RxDataAvailable() != 0)
    {
        if(rxHead > rxTail)
        {
            return(rxHead - rxTail);
        }
        else
        {
            return(UART3_CONFIG_RX_BYTEQ_LENGTH - (rxTail - rxHead));
        } 
    }
    return 0;
}

void __attribute__((deprecated)) UART3_Enable(void)
{
    U3STASET = _U3STA_UTXEN_MASK;
    U3STASET = _U3STA_URXEN_MASK;
    U3MODESET = _U3MODE_ON_MASK;
}

void __attribute__((deprecated)) UART3_Disable(void)
{
    U3STACLR = _U3STA_UTXEN_MASK;
    U3STACLR = _U3STA_URXEN_MASK;
    U3MODECLR = _U3MODE_ON_MASK;
}

