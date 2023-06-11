/**
  UART2 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    uart2.c

  @Summary
    This is the generated source file for the UART2 driver using Foundation Services Library

  @Description
    This source file provides APIs for driver for UART2. 

    Generation Information : 
        Product Revision  :  Foundation Services Library - pic24-dspic-pic32mm : v1.26
        Device            :  PIC32MM0256GPM064
    The generated drivers are tested against the following:
        Compiler          :  XC32 1.42
        MPLAB 	          :  MPLAB X 3.45
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "xc.h"
#include "uart2.h"
#include "../define_type.h"

/**
  Section: Data Type Definitions
*/

/** UART Driver Queue Status

  @Summary
    Defines the object required for the status of the queue.
*/

static uint8_t * volatile rxTail;
static uint8_t * rxHead;
static uint8_t * txTail;
static uint8_t * volatile txHead;
static bool volatile rxOverflowed;
extern uint32_t total_price,Price1;
extern const unsigned char ID_array[29];
extern const unsigned char  Saman_Data[96];
extern union SetParameter SystemParm;
extern bool price_received;
extern bool TakinTasks(uint8_t data);
extern uint8_t DataPack[200];

uint16_t update_crc(uint16_t crc_accum, unsigned char *data_blk_ptr, uint16_t data_blk_size,bool reflect_input,bool reflect_output);


const uint8_t * SepehrEft_1="TEJARAT_BANK020012345678";
const uint8_t * SepehrEft_2="000002                        ";

const uint8_t * InputType="\r\n Input psp/device type : ";
const uint8_t * OutputType="\r\n Output psp type : ";
const uint8_t * OutputBaudrate="\r\n Output baudrate : ";
const uint8_t * InputBaudrate="\r\n Input baudrate : ";
const uint8_t * TomanMode="\r\n Toman Mode : ";
const uint8_t * Active="Enable\r\n";
const uint8_t * DeActive="Disable\r\n";
const uint8_t * Psp[11]= {
                            "Parsian",
                            "Asan Pardakht",
                            "Saman Kish",
                            "Fanava",
                            "Pardakht Novin",
                            "Beh Pardakht",
                            "Sepehr Saderat",
                            "Pardakht Novin S800",
                            "!!!Not defined",
                            "!!!Not defined",
                            "Cas Scale",
};


/** UART Driver Queue Length

  @Summary
    Defines the length of the Transmit and Receive Buffers

*/

#define UART2_CONFIG_TX_BYTEQ_LENGTH (8+1)
#define UART2_CONFIG_RX_BYTEQ_LENGTH (8+1)

/** UART Driver Queue

  @Summary
    Defines the Transmit and Receive Buffers

*/

static uint8_t txQueue[UART2_CONFIG_TX_BYTEQ_LENGTH] ;
static uint8_t rxQueue[UART2_CONFIG_RX_BYTEQ_LENGTH] ;

void (*UART2_TxDefaultInterruptHandler)(void);
void (*UART2_RxDefaultInterruptHandler)(void);

/**
  Section: Driver Interface
*/
extern void UART3_Write( uint8_t byte);

void UART2_Initialize (void)
{
   uint32_t crystal=6000000;
   IEC1bits.U2TXIE = 0;
   IEC1bits.U2RXIE = 0;
  
   // STSEL 1; PDSEL 8N; RTSMD disabled; OVFDIS disabled; ACTIVE disabled; RXINV disabled; WAKE disabled; BRGH enabled; IREN disabled; ON enabled; SLPEN disabled; SIDL disabled; ABAUD disabled; LPBACK disabled; UEN TX_RX; CLKSEL SYSCLK; 
   U2MODE = (0x28008 & ~(1<<15));  // disabling UART
   // UTXISEL TX_ONE_CHAR; UTXINV disabled; ADDR 0; MASK 0; URXEN disabled; OERR disabled; URXISEL RX_ONE_CHAR; UTXBRK disabled; UTXEN disabled; ADDEN disabled; 
   U2STA = 0x0;
   // BaudRate = 9600; Frequency = 24000000 Hz; BRG 624; 
   if(SystemParm.Set_Parameter.Out_PosBaudRate<=0 || SystemParm.Set_Parameter.Out_PosBaudRate>115200)
     U2BRG =0x33;
   else
     U2BRG = crystal/SystemParm.Set_Parameter.Out_PosBaudRate;
   
   txHead = txQueue;
   txTail = txQueue;
   rxHead = rxQueue;
   rxTail = rxQueue;
   
   rxOverflowed = 0;

   UART2_SetTxInterruptHandler(UART2_Transmit_ISR);

   UART2_SetRxInterruptHandler(UART2_Receive_ISR);
   IEC1bits.U2RXIE = 1;
   
    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
   U2STASET = _U2STA_UTXEN_MASK;
   U2MODESET = _U2MODE_ON_MASK;  // enabling UART ON bit
   U2STASET = _U2STA_URXEN_MASK; 
}

/**
    Maintains the driver's transmitter state machine and implements its ISR
*/
void UART2_SetTxInterruptHandler(void* handler){
    UART2_TxDefaultInterruptHandler = handler;
}

void __attribute__ ((vector(_UART2_TX_VECTOR), interrupt(IPL1SOFT))) _UART2_TX ( void )
{
    (*UART2_TxDefaultInterruptHandler)();
}

void UART2_Transmit_ISR(void)
{ 
    if(txHead == txTail)
    {
        IEC1bits.U2TXIE = 0;
        return;
    }

    IFS1CLR= 1 << _IFS1_U2TXIF_POSITION;

    while(!(U2STAbits.UTXBF == 1))
    {
        U2TXREG = *txHead++;

        if(txHead == (txQueue + UART2_CONFIG_TX_BYTEQ_LENGTH))
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

void UART2_SetRxInterruptHandler(void* handler){
    UART2_RxDefaultInterruptHandler = handler;
}

void __attribute__ ((vector(_UART2_RX_VECTOR), interrupt(IPL1SOFT))) _UART2_RX( void )
{
    (*UART2_RxDefaultInterruptHandler)();
}

void UART2_Receive_ISR(void)
{
    while((U2STAbits.URXDA == 1))
    {
        *rxTail = U2RXREG;

        // Will the increment not result in a wrap and not result in a pure collision?
        // This is most often condition so check first
        if ( ( rxTail    != (rxQueue + UART2_CONFIG_RX_BYTEQ_LENGTH-1)) &&
             ((rxTail+1) != rxHead) )
        {
            rxTail++;
        } 
        else if ( (rxTail == (rxQueue + UART2_CONFIG_RX_BYTEQ_LENGTH-1)) &&
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

      IFS1CLR= 1 << _IFS1_U2RXIF_POSITION;
}

void __attribute__ ((vector(_UART2_ERR_VECTOR), interrupt(IPL1SOFT))) _UART2_ERR ( void )
{
    if ((U2STAbits.OERR == 1))
    {
        U2STACLR = _U2STA_OERR_MASK; 
    }

     IFS1CLR= 1 << _IFS1_U2EIF_POSITION;
}

void Write_Serial_Buffer(uint8_t data)
{
    UART2_Write(data);
}


void Rs232OutTasks(void )
{
    uint8_t data = 0;
    if (rxHead != rxTail )
    {
        data = *rxHead;
        rxHead++;
        if (rxHead == (rxQueue + UART2_CONFIG_RX_BYTEQ_LENGTH))
        {
            rxHead = rxQueue;
        }
        TakinTasks(data);
    }
}


uint8_t UART2_Read( void)
{
    uint8_t data = 0;

    while (rxHead == rxTail )
    {
    }
    
    data = *rxHead;

    rxHead++;

    if (rxHead == (rxQueue + UART2_CONFIG_RX_BYTEQ_LENGTH))
    {
        rxHead = rxQueue;
    }
    return data;
}

#define LB32   0x00000001 //32 B?T MASKELEME sol bit 32
#define LB64   0x0000000000000001 //64 B?T MASKELEME sol bit 64
#define L64_MASK    0x00000000ffffffff   //S?METR? sol bit simetri alma
#define H64_MASK    0xffffffff00000000	//S?METR? son hal

/* Initial Permutation tablosu */
static char IP[] = {
    58, 50, 42, 34, 26, 18, 10,  2, 
    60, 52, 44, 36, 28, 20, 12,  4, 
    62, 54, 46, 38, 30, 22, 14,  6, 
    64, 56, 48, 40, 32, 24, 16,  8, 
    57, 49, 41, 33, 25, 17,  9,  1, 
    59, 51, 43, 35, 27, 19, 11,  3, 
    61, 53, 45, 37, 29, 21, 13,  5, 
    63, 55, 47, 39, 31, 23, 15,  7
};

/* Inverse Initial Permutation tablosu */
static char PI[] = {
    40,  8, 48, 16, 56, 24, 64, 32, 
    39,  7, 47, 15, 55, 23, 63, 31, 
    38,  6, 46, 14, 54, 22, 62, 30, 
    37,  5, 45, 13, 53, 21, 61, 29, 
    36,  4, 44, 12, 52, 20, 60, 28, 
    35,  3, 43, 11, 51, 19, 59, 27, 
    34,  2, 42, 10, 50, 18, 58, 26, 
    33,  1, 41,  9, 49, 17, 57, 25
};

/*Expansion tablosu */
static char E[] = {
    32,  1,  2,  3,  4,  5,  
     4,  5,  6,  7,  8,  9,  
     8,  9, 10, 11, 12, 13, 
    12, 13, 14, 15, 16, 17, 
    16, 17, 18, 19, 20, 21, 
    20, 21, 22, 23, 24, 25, 
    24, 25, 26, 27, 28, 29, 
    28, 29, 30, 31, 32,  1
};

/* Post S-Box permutation */
static char P[] = {
    16,  7, 20, 21, 
    29, 12, 28, 17, 
     1, 15, 23, 26, 
     5, 18, 31, 10, 
     2,  8, 24, 14, 
    32, 27,  3,  9, 
    19, 13, 30,  6, 
    22, 11,  4, 25
};

/* The S-Box tablosu */
static char S[8][64] = {{
    /* S1 */
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,  
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,  
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0, 
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
},{
    /* S2 */
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,  
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,  
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15, 
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
},{
    /* S3 */
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,  
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,  
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
},{
    /* S4 */
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,  
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,  
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
},{
    /* S5 */
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9, 
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6, 
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14, 
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
},{
    /* S6 */
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
},{
    /* S7 */
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
},{
    /* S8 */
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
}};

/* Permuted Choice 1 tablosu */
static char PC1[] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

/* Permuted Choice 2 tablosu */
static char PC2[] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

/* Iteration Shift Array */
static char iteration_shift[] = {
 /* 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16 */
    1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1
};


uint64_t des(uint64_t input, uint64_t key,   char mode) {
    
  
    int i, j;
    
    /* 8 bit */
    char satir, sutun;
    
    /* 28 bits */
    uint32_t C                  = 0;
    uint32_t D                  = 0;
    
    /* 32 bit */
    uint32_t L                  = 0;
    uint32_t R                  = 0;
    uint32_t s_output           = 0;
    uint32_t f_function_res     = 0;
    uint32_t temp               = 0;
    
    /* 48 bit */
    uint64_t sub_key[16]        = {0};
    uint64_t s_input            = 0;
    
    /* 56 bit */
    uint64_t permuted_choice_1  = 0;
    uint64_t permuted_choice_2  = 0;
    
    /* 64 bit */
    uint64_t init_perm_res      = 0;
    uint64_t inv_init_perm_res  = 0;
    uint64_t pre_output         = 0;
    
    /* initial permutation */
    for (i = 0; i < 64; i++) {
       
        init_perm_res <<= 1;//girilen de?eri 1 bit yana kayd?rd?m.
        init_perm_res |= (input >> (64-IP[i])) & LB64; //Initial Permutation tablosunda bit kayd?rma yap?p or kap?s?na sokup 
        //|=i?lem ile tekrar or kap?s?na girdi. kontrol i?in
       
      
         
    }
    // printf ("ilk karistirma(girilen): %llx\n", init_perm_res);
    L = (uint32_t) (init_perm_res >> 32) & L64_MASK;// kar?st?rd?k ve 64 biti 32 bit 32 bit ay?rd?k.
      // printf ("SOL 32 Bit: %llx\n",L);
    R = (uint32_t) init_perm_res & L64_MASK;  // sag taraf durcak.
       // printf ("SAG 32 Bit: %llx\n",R);
    /* initial key kar??t?rma */
    for (i = 0; i < 56; i++) {
        
        permuted_choice_1 <<= 1;//
        permuted_choice_1 |= (key >> (64-PC1[i])) & LB64;//64 bitten 56 bite anahtar? d???r?p kontrollü yapt?k.
		 //printf ("ilk karistirma(anahtar): %llx\n", permuted_choice_1);
    }
    
    C = (uint32_t) ((permuted_choice_1 >> 28) & 0x000000000fffffff);//kar?san tablodaki bitler 28 sat?r saga kayd?.
    D = (uint32_t) (permuted_choice_1 & 0x000000000fffffff);//ve kontrol yapt?k.
   //  printf ("SOL 56 Bit: %llx\n",C);
   //  printf ("SAG 56 Bit: %llx\n",D);
    /* 16 tane anahtar olusturma*/
    for (i = 0; i< 16; i++) {
        
        
        // shift Ci and Di
        for (j = 0; j < iteration_shift[i]; j++) { //shift tablosundaki index kadar d?nd?r?p bit kayd?r?p XOR yapt?k.
            
            C = 0x0fffffff & (C << 1) | 0x00000001 & (C >> 27);
            D = 0x0fffffff & (D << 1) | 0x00000001 & (D >> 27);
            
        }
    //	printf ("%d:SOL 56 Bit(SHIFT): %llx\n",i,C);
     //	printf ("%d:SAG 56 Bit(SHIFT): %llx\n",i,D);
        /*uras? da, bir ?stte ?retilen 56 bitlik shifted key i?inden 48 bitlik anahtar ?retimi i?indir. 
		Asl?nda burada hem s?k??t?rma hem de kar??t?rma (permutasyon) yap?lm?? olur. 
		?rne?in, key?in ilk biti 14. s?rayla de?i?tirilir. ?kinci biti 17.s?rayla, gibi..*/
        permuted_choice_2 = 0;
        permuted_choice_2 = (((uint64_t) C) << 28) | (uint64_t) D ;//kar?san tablodaki bitler 28 sat?r saga kayd?.
       
        sub_key[i] = 0;
        //48 biti 56 bite c?kard?k.(s?k??t?rma)
        for (j = 0; j < 48; j++) {
            
            sub_key[i] <<= 1;//32+32
            sub_key[i] |= (permuted_choice_2 >> (56-PC2[j])) & LB64;//anahtar?n son hali 64 bit oldu.
            
        }
        
    }
     
    
    //16 kere d?nd?r?p 
    for (i = 0; i < 16; i++) {
        
      
        s_input = 0;
        
        for (j = 0; j< 48; j++) {
            
            s_input <<= 1;
            s_input |= (uint64_t) ((R >> (32-E[j])) & LB32);// kar?st?rd?g?m?z girilen de?eri 32 bit kayd?r?p xor a soktuk.
            
        }
        	
        /* 
        Burada, 48 bitlik geni?letilmi? d?z text (expanded RPT of 48 bit) ile 48 bitlik anahtar 
		(compressed key of 48 bit) aras?nda bitwise XOR i?lemi yap?l?yor. Sonu? olarak yine 48 bitlik bir 
		metin ortaya ??kacakt?r.
    
       if (mode == 'd') {
            // decryption
          //  s_input = s_input ^ sub_key[15-i];
            
       // } else {
            // encryption
           
            
       // }
        
        /* S-Box tablosu */
       /* Bir ?nceki i?lemle XOR?lanm?? metin (XORed RPT) ( text ile keyin birle?tirilmesi) S box?a verilir. 
	   Burada metin (48 bit), her biri 6 bitten olu?an 8 blo?a b?l?n?r. Her blok i?in ayr? bir Sbox tablosu bulunur. 
	   Bu sebeple de a?a??da 8 adet Sbox tablosu g?receksiniz. Sbox?lar 16 s?tun, 4 sat?rdan olu?ur. 0 ile 15 aras?nda de?er
	    al?r. Ve her SBox 4 bitlik ??k?? verir. B?t?n SBox?lar?n g?revi bitti?inde sonu? olarak (4*8) 32 bit d?necektir. 
		(Sbox RPT) Bu 6 bitlik verinin 1. ve 6. verisi sat?r, 2.3.4.5 bitleri ise Sbox ?zerindeki s?t?nlar? g?sterir. 
		Bunlar?n kesi?ti?i nokta da d?n?? de?erini olu?turacakt?r. B?ylece her sbox??n 4 bitlik ??k??? olacakt?r.*/
	
		  if (mode == 'd') {
            //decryption
            s_input = s_input ^ sub_key[15-i];
            
        	} else {
            // encryption
            s_input = s_input ^ sub_key[i]; //kar?st?r?lan anahtar ve key son halimizi ald?.
            
        	}
		 
		 
        for (j = 0; j < 8; j++) {
           
            
            satir = (char) ((s_input & (0x0000840000000000 >> 6*j)) >> 42-6*j);
            satir = (satir >> 4) | satir & 0x01;
            
            sutun = (char) ((s_input & (0x0000780000000000 >> 6*j)) >> 43-6*j);
            
            s_output <<= 4;
            s_output |= (uint32_t) (S[j][16*satir + sutun] & 0x0f);
            
        }
        
        f_function_res = 0;
        
        for (j = 0; j < 32; j++) {
            
            f_function_res <<= 1;
            f_function_res |= (s_output >> (32 - P[j])) & LB32;
            
        }
        
        temp = R;
        R = L ^ f_function_res;
        L = temp;
        //printf ("%d:SAG: %llx  SOL: %llx\n",j, R,L);
    }
    
    pre_output = (((uint64_t) R) << 32) | (uint64_t) L;
        
    /* inverse initial permutation */
    for (i = 0; i < 64; i++) {
        
        inv_init_perm_res <<= 1;
        inv_init_perm_res |= (pre_output >> (64-PI[i])) & LB64;
        
    }
   
    return inv_init_perm_res;
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

void Change_To_String_All(unsigned char Buf[10],unsigned long Number)
{
	unsigned char i,dig;
	i=0;
	do
	{
		dig=Number%10;
		Buf[i]=dig;
		Number/=10;
		i++;
	}while(i<10);
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


uint32_t MakeTexts(const uint8_t * StrData,uint32_t OffsetMem,uint32_t NumberAdded,bool Bypass)
{
    strcpy(DataPack+OffsetMem+2,StrData);
    OffsetMem+=strlen(StrData);
    if(Bypass==0)
    {
        OffsetMem+=Change_To_Str(DataPack+OffsetMem+2,NumberAdded);
        DataPack[OffsetMem+2]=',';OffsetMem++;
    }
    return OffsetMem;
}

void GetConfiguration(uint8_t ProtocolFunction)
{
    uint32_t TempVariable=0;
    uint16_t crci=0,LoopCnt;
    DataPack[0]=0xaa;//ProtocolFunction;
    DataPack[1]=0x55;//ProtocolFunction;
    TempVariable=0;
    TempVariable=MakeTexts(OutputType,TempVariable,0,1);
    TempVariable=MakeTexts(Psp[SystemParm.Set_Parameter.Out_EftPo_Type],TempVariable,0,1);
    TempVariable=MakeTexts(OutputBaudrate,TempVariable,SystemParm.Set_Parameter.Out_PosBaudRate,0);
    TempVariable=MakeTexts(InputType,TempVariable,0,1);
    TempVariable=MakeTexts(Psp[SystemParm.Set_Parameter.In_EftPo_Type],TempVariable,0,1);
    
    TempVariable=MakeTexts(InputBaudrate,TempVariable,SystemParm.Set_Parameter.In_PosBaudRate,0);
    TempVariable=MakeTexts(TomanMode,TempVariable,0,1);
    if(SystemParm.Set_Parameter.Toman_Mode)
        TempVariable=MakeTexts(Active,TempVariable,0,1);
    else
        TempVariable=MakeTexts(DeActive,TempVariable,0,1);
    DataPack[2]=TempVariable;
    DataPack[3]=ProtocolFunction;
    crci=update_crc(0,(unsigned char *)&DataPack[3],TempVariable,false,false);   
    DataPack[TempVariable+3]=(uint8_t)(crci>>8);
    DataPack[TempVariable+4]=(uint8_t)(crci&0xff);
    for(LoopCnt=0;LoopCnt<TempVariable+4;LoopCnt++)
    {
        UART2_Write(DataPack[LoopCnt]);
        UART3_Write(DataPack[LoopCnt]);
    }
}

void asan_pardakht(void)
{
	unsigned char Buf[10];
	int count,count1,count2;
	unsigned int checksum,chek;
    if(SystemParm.Set_Parameter.Toman_Mode>0)
    	Price1=total_price*10;
    else
        Price1=total_price;
	Change_To_String_All(Buf,Price1);
	count=9;
	while(Buf[count]==0)
	{
		count-=1;
	}  
	checksum=0;
	Write_Serial_Buffer('$');
	Write_Serial_Buffer('P');
	checksum=checksum^'P';
	Write_Serial_Buffer('C');
	checksum=checksum^'C';
	Write_Serial_Buffer('B');
	checksum=checksum^'B';
	Write_Serial_Buffer('U');
	checksum=checksum^'U';
	Write_Serial_Buffer('Y');
	checksum=checksum^'Y';
	Write_Serial_Buffer(',');
	checksum=checksum^',';
	for(count1=0;count1<count+1;count1++)
	{
		Write_Serial_Buffer('0'+Buf[count-count1]);
		checksum=checksum^('0'+Buf[count-count1]);
	}
	Write_Serial_Buffer(',');
	checksum=checksum^',';
	Write_Serial_Buffer('1');
	checksum=checksum^'1';
	Write_Serial_Buffer('0');
	checksum=checksum^'0';
	Write_Serial_Buffer(',');
	checksum=checksum^',';
	Write_Serial_Buffer('2');
	checksum=checksum^'2';
	Write_Serial_Buffer('0');
	checksum=checksum^'0';
	Write_Serial_Buffer('1');
	checksum=checksum^'1';
	Write_Serial_Buffer('7');
	checksum=checksum^'7';
	Write_Serial_Buffer('1');
	checksum=checksum^'1';
	Write_Serial_Buffer('1');
	checksum=checksum^'1';
	Write_Serial_Buffer('1');
	checksum=checksum^'1';
	Write_Serial_Buffer('3');
	checksum=checksum^'3';
	Write_Serial_Buffer('1');
	checksum=checksum^'1';
	Write_Serial_Buffer('2');
	checksum=checksum^'2';
	Write_Serial_Buffer('2');
	checksum=checksum^'2';
	Write_Serial_Buffer('3');
	checksum=checksum^'3';
	Write_Serial_Buffer('4');
	checksum=checksum^'4';
	Write_Serial_Buffer('1');
	checksum=checksum^'1';
	Write_Serial_Buffer(',');
	checksum=checksum^',';
	Write_Serial_Buffer('1');
	checksum=checksum^'1';
	Write_Serial_Buffer(',');
	checksum=checksum^',';
	Write_Serial_Buffer('0');
	checksum=checksum^'0';
	Write_Serial_Buffer(',');
	checksum=checksum^',';
	Write_Serial_Buffer('*');
	checksum=checksum&0xff;
	chek=checksum&0xf0;
	chek=chek>>4;
	if(chek<10)
		chek+=48;
	else 
		chek+=55;
	Write_Serial_Buffer(chek);
	chek=checksum&0x0f;
	if(chek<10)
		chek+=48;
	else 
		chek+=55;
	Write_Serial_Buffer(chek);
	Write_Serial_Buffer(0x0d);
	Write_Serial_Buffer(0x0a);
}

void saman_kish(void)
{
	unsigned char  j=0,i;
	unsigned long Price_b=0,Price_Length,Price=0,Packet_D_L;
	unsigned char  Price_Array[9];
    if(SystemParm.Set_Parameter.Toman_Mode>0)
    	Price=total_price*10;
    else
        Price=total_price;
	do
	{
		j++;		
		Price=(Price/10);				
	}while(Price>0);
	Price_Length=j;	
	for( i=j;i>0;i-- )
	{
		Price_b=total_price%10;
		Price_Array[i-1]=Price_b ^ 0x30;
		total_price/=10;				
	}	
	//****************************DATA ARRAY 
    Write_Serial_Buffer(0x02);//Packet_Data[0]=0x02; //constant
    Packet_D_L=108+(Price_Length*2);	
	Write_Serial_Buffer(Packet_D_L);	//Packet_Data[1]=Packet_D_L;
	Write_Serial_Buffer(0x00);//Packet_Data[2]=0x00;
	Write_Serial_Buffer(0x00);//Packet_Data[3]=0x00;
	Write_Serial_Buffer(0x00);//Packet_Data[4]=0x00;
	Write_Serial_Buffer(0x01);//Packet_Data[5]=0x01;//constant
	Write_Serial_Buffer(0x72);//Packet_Data[6]=0x72;//constant
	Write_Serial_Buffer((Price_Length*2)+102);//Packet_Data[7]=(Price_Length*2)+102;//length of packet data in 1 byte	from this byte until end of packet data
	Write_Serial_Buffer(0xB1);//Packet_Data[8]=0xB1;
	Write_Serial_Buffer(Price_Length+23);//Packet_Data[9]=Price_Length+23; //length of packet data in 1 byte	from this byte until B2 TAG	
	Write_Serial_Buffer(0x81);//Packet_Data[10]=0x81;	
	Write_Serial_Buffer(Price_Length);//Packet_Data[11]=Price_Length;
	for(i=0;i<Price_Length;i++){Write_Serial_Buffer(Price_Array[i]);}//{Packet_Data[12+i]=Price_Array[i];}	
	for(i=0;i<21;i++){Write_Serial_Buffer(Saman_Data[i]);}//{Packet_Data[(12+j)+i]=Saman_Data[i];}
	//****************B2 TAG
	Write_Serial_Buffer(0xB2);//Packet_Data[33+j]=0xB2;
	Write_Serial_Buffer(Price_Length+75);//Packet_Data[34+j]=Price_Length+75;//length of packet data in 1 byte	from this byte until end of packet data
	Write_Serial_Buffer(0xA1);//Packet_Data[35+j]=0xA1;
	Write_Serial_Buffer(Price_Length+23);//Packet_Data[36+j]=Price_Length+23;
	Write_Serial_Buffer(0x81);//Packet_Data[37+j]=0x81;
	Write_Serial_Buffer(Price_Length);//Packet_Data[38+j]=Price_Length;
	for(i=0;i<j;i++){Write_Serial_Buffer(Price_Array[i]);}//{Packet_Data[(39+j)+i]=Price_Array[i];}

	for(i=21;i<96;i++){Write_Serial_Buffer(Saman_Data[i]);}//{Packet_Data[(39+j)+i-21]=Saman_Data[i];}

}
/*
void pardakht_novin(void)
{
	unsigned char Buf[10]={0};
	int count,count1,count2;
	unsigned int checksum,chek;
    if(SystemParm.Set_Parameter.Toman_Mode>0)
    	Price1=total_price*10;
    else
        Price1=total_price;
	Change_To_String_All(Buf,Price1);
	count=9;
	while(Buf[count]==0)
	{
		count-=1;
	}  
	checksum=0;
	Write_Serial_Buffer('@');
	Write_Serial_Buffer('@');
	Write_Serial_Buffer('P');
	Write_Serial_Buffer('N');
	Write_Serial_Buffer('A');
	Write_Serial_Buffer('@');
	Write_Serial_Buffer('@');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('1');
	Write_Serial_Buffer('1');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0'+count+1);
	for(count1=0;count1<count+1;count1++)
	{
		Write_Serial_Buffer('0'+Buf[count-count1]);
	}
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('2');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	
	Write_Serial_Buffer('1');
	Write_Serial_Buffer('1');
}
*/
void pardakht_novin(void)
{
	unsigned char Buf[12],outBuf[70];
	int lenght,number,strCount,index;
    if(SystemParm.Set_Parameter.Toman_Mode>0)
    	Price1=total_price*10;
    else
        Price1=total_price;
	Change_To_String_All(Buf,Price1);
    strCount=10;
	while(Buf[strCount]==0)
	{
		strCount-=1;
	}  
    strCount++;
    lenght=strCount+35;
    outBuf[0]='0';
    outBuf[1]='0';
    outBuf[2]=lenght/10+'0';
    outBuf[3]=lenght%10+'0';
    outBuf[4]='R';
    outBuf[5]='Q';
    outBuf[6]='0';
    outBuf[7]=(lenght-5)/10+'0';
    outBuf[8]=(lenght-5)%10+'0';
    outBuf[9]='P';
    outBuf[10]='R';
    outBuf[11]='0';
    outBuf[12]='0';
    outBuf[13]='6';
    outBuf[14]='0';
    outBuf[15]='0';
    outBuf[16]='0';
    outBuf[17]='0';
    outBuf[18]='0';
    outBuf[19]='0';
    outBuf[20]='A';
    outBuf[21]='M';
    outBuf[22]='0';
    outBuf[23]=strCount/10+'0';
    outBuf[24]=strCount%10+'0';
    index=0;
    for(number=strCount-1;number>=0;number--,index++)
    {
        outBuf[25+index]=Buf[number]+'0';
    }
    outBuf[25+index]='C';
    outBuf[26+index]='U';
    outBuf[27+index]='0';
    outBuf[28+index]='0';
    outBuf[29+index]='3';
    outBuf[30+index]='3';
    outBuf[31+index]='6';
    outBuf[32+index]='4';
    outBuf[33+index]='P';
    outBuf[34+index]='D';
    outBuf[35+index]='0';
    outBuf[36+index]='0';
    outBuf[37+index]='1';
    outBuf[38+index]='1';
    for(number=0;number<39+index;number++)
        Write_Serial_Buffer(outBuf[number]);
}

void fanava(void)
{
	unsigned char Buf[11];
	int count,count1,count2;
	unsigned int checksum,chek;
    if(SystemParm.Set_Parameter.Toman_Mode>0)
    	Price1=total_price*10;
    else
        Price1=total_price;
	Change_To_String_All(Buf,Price1);
	count=12;
	while(Buf[count]==0)
	{
		count-=1;
	}  
	Write_Serial_Buffer(0x00);
	Write_Serial_Buffer(0x17);
	Write_Serial_Buffer(0x01);
	Write_Serial_Buffer(0x04);
	Write_Serial_Buffer(0x30);
	Write_Serial_Buffer(0x36);
	Write_Serial_Buffer(0x30);
	Write_Serial_Buffer(0x30);
	Write_Serial_Buffer(0x06);
	Write_Serial_Buffer(0x0c);
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0');
	Write_Serial_Buffer('0'+Buf[9]);
	Write_Serial_Buffer('0'+Buf[8]);
	Write_Serial_Buffer('0'+Buf[7]);
	Write_Serial_Buffer('0'+Buf[6]);
	Write_Serial_Buffer('0'+Buf[5]);
	Write_Serial_Buffer('0'+Buf[4]);
	Write_Serial_Buffer('0'+Buf[3]);
	Write_Serial_Buffer('0'+Buf[2]);
	Write_Serial_Buffer('0'+Buf[1]);
	Write_Serial_Buffer('0'+Buf[0]);
	Write_Serial_Buffer(0x0d);
	Write_Serial_Buffer(0x01);
	Write_Serial_Buffer(0x30);
	
}

/*
 
void parsian(void)
{
	unsigned char 	j,i,x;  
	unsigned long int 	Price_b=0,Price=0;  
	unsigned int  Price_Array[9];//={0x00};	
    uint64_t encrypt_resualt=0,in_des=0,key_des=0;

//    key_des=0x3132333435363738;
    in_des=0x54454a415241545f;
    key_des=0x3230303132333435;
//    in_des =0x5f544152414a4554;
    
    encrypt_resualt=des(in_des,key_des,'e');
    in_des=0x42414e4b30323030;
//    in_des=0x303032304b4e4142;
    
    in_des=encrypt_resualt^in_des;
    
    encrypt_resualt=des(in_des,key_des,'e');
    in_des=0x3132333435363738;
//    in_des=0x3837363534333231;
    in_des=encrypt_resualt^in_des;
    
    encrypt_resualt=des(in_des,key_des,'e');
    in_des=0x3030303031323334;
//    in_des=  0x3433323130303030;
    in_des=encrypt_resualt^in_des;
    
    encrypt_resualt=des(in_des,key_des,'e');
    in_des=0x3536373830303030;
//    in_des=0x3030303038373635;
    in_des=encrypt_resualt^in_des;
    
    encrypt_resualt=des(in_des,key_des,'e');
    in_des=0x3032202020202020;
//    in_des=  0x2020202020203230;
    in_des=encrypt_resualt^in_des;
    
    encrypt_resualt=des(in_des,key_des,'e');
    in_des=0x2020202020202020;
//    in_des=0x2020202020202020;
    in_des=encrypt_resualt^in_des;
    
    encrypt_resualt=des(in_des,key_des,'e');
    in_des=0x2020202020202020;
//    in_des=0x2020202020202020;
    in_des=encrypt_resualt^in_des;
    
    encrypt_resualt=des(in_des,key_des,'e');
    in_des=0x2020000000000000;
//    in_des=  0x0000000000002020;
    in_des=encrypt_resualt^in_des;
    
    encrypt_resualt=des(in_des,key_des,'e');

    


	for(j=0;j<9;j++)
		Price_Array[j]=0;
	j=0;
    if(SystemParm.Set_Parameter.Toman_Mode>0)
    	Price=total_price*10;
    else
        Price=total_price;
	do
	{
		j++;		
		Price=(Price/10);				
	}while(Price>0);	

	for( i=j;i>0;i-- )	
	{
		Price_b=total_price%10;
		Price_Array[i-1]=Price_b;
		total_price/=10;				
	}
	Write_Serial_Buffer(0x30);
	Write_Serial_Buffer(0x30);
	x=j+35;
	Write_Serial_Buffer((x/10)| 0x30);
	Write_Serial_Buffer((x%10)| 0x30);	 
	for(i=0;i<2;i++)	{Write_Serial_Buffer(ID_array[i]);}
	Write_Serial_Buffer(0x30);
	x=0;
	x=j+30;
	Write_Serial_Buffer((x/10)| 0x30);
	Write_Serial_Buffer((x%10)| 0x30);
	for(i=2;i<15;i++) {Write_Serial_Buffer(ID_array[i]);}
	Write_Serial_Buffer(0x30);
	Write_Serial_Buffer(0x30);
	Write_Serial_Buffer(( j | 0x30 ));	
	for(i=0;i<j ;i++){Write_Serial_Buffer(Price_Array[i]|0x30);}	
	for(i=15;i<29;i++){Write_Serial_Buffer(ID_array[i]);}
}
 */

void generateMAC(uint8_t data[],uint8_t macc[],uint16_t messageLenght,uint64_t keyValue)
{
    uint8_t index=0,dataindex=0,bcdIndex=0,loopNumber=0,innerLoop=0;
    uint64_t XOred=0,processValue=0,outValue=0;
    uint16_t num;
    uint8_t processBuf[8];
    
    num= (messageLenght % 8 == 0) ? messageLenght : (messageLenght + 8 - messageLenght % 8);
    loopNumber=num/8;
    for(index=0;index<loopNumber;index++)
    {
        for(innerLoop=0;innerLoop<8;innerLoop++)
            processBuf[innerLoop] = data[dataindex+innerLoop];
        dataindex+=8;
        processValue=0;
        for(bcdIndex=0;bcdIndex<8;bcdIndex++)
        {
            processValue|=processBuf[bcdIndex];
            if(bcdIndex!=7)
                processValue<<=8;
        }

        XOred = processValue ^ outValue;
        outValue=des(XOred,keyValue,'e');
    }
    for(index=8;index>0;index--)
    {
        macc[index-1]=outValue&0xff;
        outValue>>=8;
    }
}


void parsian(void)
{
	unsigned char 	j,i,x;  
	unsigned long int 	Price_b=0,Price=0;  
	unsigned int  Price_Array[9];
	for(j=0;j<9;j++)
		Price_Array[j]=0;
	j=0;
    if(SystemParm.Set_Parameter.Toman_Mode>0)
    	Price=total_price*10;
    else
        Price=total_price;
	do
	{
		j++;		
		Price=(Price/10);				
	}while(Price>0);	

	for( i=j;i>0;i-- )	
	{
		Price_b=total_price%10;
		Price_Array[i-1]=Price_b;
		total_price/=10;				
	}
	Write_Serial_Buffer(0x30);
	Write_Serial_Buffer(0x30);
	x=j+35;
	Write_Serial_Buffer((x/10)| 0x30);
	Write_Serial_Buffer((x%10)| 0x30);	 
	for(i=0;i<2;i++)	{Write_Serial_Buffer(ID_array[i]);}
	Write_Serial_Buffer(0x30);
	x=0;
	x=j+30;
	Write_Serial_Buffer((x/10)| 0x30);
	Write_Serial_Buffer((x%10)| 0x30);
	for(i=2;i<15;i++) {Write_Serial_Buffer(ID_array[i]);}
	Write_Serial_Buffer(0x30);
	Write_Serial_Buffer(0x30);
	Write_Serial_Buffer(( j | 0x30 ));	
	for(i=0;i<j ;i++){Write_Serial_Buffer(Price_Array[i]|0x30);}	
	for(i=15;i<29;i++){Write_Serial_Buffer(ID_array[i]);}
}

uint8_t reverse_byte(uint8_t data)
{
    uint8_t i=0,out=0;
    for(i=0;i<8;i++)
    {
        if(data&(1<<(7-i)))
            out|=(1<<i);
    }
    return out;
}

uint16_t reverse_word(uint16_t data)
{
    uint16_t i=0,out=0;
    for(i=0;i<16;i++)
    {
        if(data&(1<<(15-i)))
            out|=(1<<i);
    }
    return out;
}

uint16_t update_crc(uint16_t crc_accum, unsigned char *data_blk_ptr, uint16_t data_blk_size,bool reflect_input,bool reflect_output)
{
    uint16_t i, j;
    uint16_t crc_table[256] = { 0x0000,0x8005,0x800F,0x000A,0x801B,0x001E,0x0014,0x8011,0x8033,0x0036,0x003C,0x8039,0x0028,0x802D,0x8027,0x0022,
                                0x8063,0x0066,0x006C,0x8069,0x0078,0x807D,0x8077,0x0072,0x0050,0x8055,0x805F,0x005A,0x804B,0x004E,0x0044,0x8041,
                                0x80C3,0x00C6,0x00CC,0x80C9,0x00D8,0x80DD,0x80D7,0x00D2,0x00F0,0x80F5,0x80FF,0x00FA,0x80EB,0x00EE,0x00E4,0x80E1,
                                0x00A0,0x80A5,0x80AF,0x00AA,0x80BB,0x00BE,0x00B4,0x80B1,0x8093,0x0096,0x009C,0x8099,0x0088,0x808D,0x8087,0x0082,
                                0x8183,0x0186,0x018C,0x8189,0x0198,0x819D,0x8197,0x0192,0x01B0,0x81B5,0x81BF,0x01BA,0x81AB,0x01AE,0x01A4,0x81A1,
                                0x01E0,0x81E5,0x81EF,0x01EA,0x81FB,0x01FE,0x01F4,0x81F1,0x81D3,0x01D6,0x01DC,0x81D9,0x01C8,0x81CD,0x81C7,0x01C2,
                                0x0140,0x8145,0x814F,0x014A,0x815B,0x015E,0x0154,0x8151,0x8173,0x0176,0x017C,0x8179,0x0168,0x816D,0x8167,0x0162,
                                0x8123,0x0126,0x012C,0x8129,0x0138,0x813D,0x8137,0x0132,0x0110,0x8115,0x811F,0x011A,0x810B,0x010E,0x0104,0x8101,
                                0x8303,0x0306,0x030C,0x8309,0x0318,0x831D,0x8317,0x0312,0x0330,0x8335,0x833F,0x033A,0x832B,0x032E,0x0324,0x8321,
                                0x0360,0x8365,0x836F,0x036A,0x837B,0x037E,0x0374,0x8371,0x8353,0x0356,0x035C,0x8359,0x0348,0x834D,0x8347,0x0342,
                                0x03C0,0x83C5,0x83CF,0x03CA,0x83DB,0x03DE,0x03D4,0x83D1,0x83F3,0x03F6,0x03FC,0x83F9,0x03E8,0x83ED,0x83E7,0x03E2,
                                0x83A3,0x03A6,0x03AC,0x83A9,0x03B8,0x83BD,0x83B7,0x03B2,0x0390,0x8395,0x839F,0x039A,0x838B,0x038E,0x0384,0x8381,
                                0x0280,0x8285,0x828F,0x028A,0x829B,0x029E,0x0294,0x8291,0x82B3,0x02B6,0x02BC,0x82B9,0x02A8,0x82AD,0x82A7,0x02A2,
                                0x82E3,0x02E6,0x02EC,0x82E9,0x02F8,0x82FD,0x82F7,0x02F2,0x02D0,0x82D5,0x82DF,0x02DA,0x82CB,0x02CE,0x02C4,0x82C1,
                                0x8243,0x0246,0x024C,0x8249,0x0258,0x825D,0x8257,0x0252,0x0270,0x8275,0x827F,0x027A,0x826B,0x026E,0x0264,0x8261,
                                0x0220,0x8225,0x822F,0x022A,0x823B,0x023E,0x0234,0x8231,0x8213,0x0216,0x021C,0x8219,0x0208,0x820D,0x8207,0x0202
    };

    for(j = 0; j < data_blk_size; j++)
    {
        if(reflect_input==false)
            i = ((uint16_t)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
        else
            i = ((uint16_t)(crc_accum >> 8) ^ reverse_byte(data_blk_ptr[j])) & 0xFF;
        crc_accum = (crc_accum << 8) ^ crc_table[i];
    }

    if(reflect_output==true)
        return reverse_word(crc_accum);
    else
        return crc_accum;
}



void beh_pardakht(void)
{
	unsigned char 	j,i,x;  
	unsigned long int 	Price_b=0,Price=0;  
    uint32_t crc_main;
	uint8_t  Price_Array[32],OutPriceArray[9];
    uint8_t index=0,oindex=0;
    for(j=0;j<32;j++)
		Price_Array[j]=0;
    for(j=0;j<9;j++)
		OutPriceArray[j]=0;
    j=0;
    if(SystemParm.Set_Parameter.Toman_Mode>0)
    	Price=total_price*10;
    else
        Price=total_price;
    total_price=Price;
    do
	{
		j++;		
		Price=(Price/10);				
	}while(Price>0);
    for( i=j;i>0;i-- )	
	{
		Price_b=total_price%10;
		Price_Array[i-1]=Price_b;
		total_price/=10;				
	}

    for(i=j;i>0 ;i--)
    {
        if(index==0)
        {
            OutPriceArray[oindex]=Price_Array[i-1];
            index=1;
        }
        else
        {
            OutPriceArray[oindex]+=Price_Array[i-1]*16;
            oindex++;
            index=0;
        }
    }
	if(index==1)
        oindex++;
    
    Price_Array[0]=0xb0;
    Price_Array[1]=oindex;
    j=0;
    for(i=0;i<oindex;i++)
    {
        Price_Array[2+i]=OutPriceArray[oindex-i-1];
    }
    index=2+oindex;
    Price_Array[index]=0xb1;index++;
    Price_Array[index]=0x02;index++;
    Price_Array[index]=0x01;index++;
    Price_Array[index]=0x08;index++;
    

    Price_Array[index]=0xb9;index++;
    Price_Array[index]=0x06;index++;
    Price_Array[index]=0x31;index++;
    Price_Array[index]=0x32;index++;
    Price_Array[index]=0x33;index++;
    Price_Array[index]=0x34;index++;
    Price_Array[index]=0x35;index++;
    Price_Array[index]=0x36;index++;


    Price_Array[index]=0xA2;index++;
    Price_Array[index]=0x01;index++;
    Price_Array[index]=0x01;index++;

    Price_Array[index]=0xa4;index++;
    Price_Array[index]=0x04;index++;
    Price_Array[index]=0x35;index++;
    Price_Array[index]=0x32;index++;
    Price_Array[index]=0x36;index++;
    Price_Array[index]=0x33;index++;
    for(j=0;j<9;j++)
		OutPriceArray[j]=0;
    j=index;
    for(i=4;i>0;i--)
    {
        OutPriceArray[i-1]=index%10+'0';
        index/=10;
    }
   
    crc_main=update_crc(0,Price_Array,j,true,true);
    
    Write_Serial_Buffer(OutPriceArray[0]);
    Write_Serial_Buffer(OutPriceArray[1]);
    Write_Serial_Buffer(OutPriceArray[2]);
    Write_Serial_Buffer(OutPriceArray[3]);
    
    for(i=0;i<j;i++)
        Write_Serial_Buffer(Price_Array[i]);

    Write_Serial_Buffer(crc_main&0xff);
    Write_Serial_Buffer((crc_main&0xff00)>>8);
    
}


void sepehr_saderat(void)
{
    unsigned char 	j,i,x;  
	unsigned long int 	Price_b=0,Price=0;  
    uint64_t key;
    uint8_t  Price_Array[15],Mac[8],data[80],len=0;
    for( i=0;i<15;i++)	
        Price_Array[i]=0;
    for( i=0;i<80;i++)
        data[i]=0;
    for( i=0;i<8;i++)	
        Mac[i]=0;
    j=0;
    if(SystemParm.Set_Parameter.Toman_Mode>0)
    	Price=total_price*10;
    else
        Price=total_price;
    total_price=Price;
    for( i=12;i>0;i-- )	
	{
		Price_b=total_price%10;
		Price_Array[i-1]=Price_b+'0';
		total_price/=10;				
	}
    len=strlen(SepehrEft_1);
    for(i=0;i<strlen(SepehrEft_1);i++)
        data[i]=*(SepehrEft_1+i);
    for(i=0;i<12;i++)
        data[i+len]=Price_Array[i];
    len+=12;
    for(i=0;i<strlen(SepehrEft_2);i++)
        data[i+len]=*(SepehrEft_2+i);
    len+=strlen(SepehrEft_2);
    data[len]=0;
    data[len+1]=0;
    len+=2;
    key=0x3230303132333435;
    generateMAC(data,Mac,len,key);
    for(i=0;i<8;i++)
        data[len+i]=Mac[i];
    len+=8;
    for(i=0;i<len;i++)
        Write_Serial_Buffer(data[i]);
}

void SendPriceToEftPos(void)
{
    if(price_received)
    {
        switch(SystemParm.Set_Parameter.Out_EftPo_Type)
        {
            case 0 : // parsian
                parsian();
            break;
            case 1 : // asan pardakht
                asan_pardakht();
            break;
            case 2 : // saman kish
                saman_kish();
            break;
            case 3 : // fanava kart
                fanava();
            break;
            case 4 : // pardakh novin
                pardakht_novin();
            break;
            case 5 : // beh pardakht
                beh_pardakht();
            break;
            case 6 : // sepehr saderat
                sepehr_saderat();
            break;
            default:
            break;
        }
        price_received=false;
    }
}



void UART2_Write( uint8_t byte)
{
    while(UART2_IsTxReady() == 0)
    {
    }
    *txTail = byte;

    txTail++;
    
    if (txTail == (txQueue + UART2_CONFIG_TX_BYTEQ_LENGTH))
    {
        txTail = txQueue;
    }

    IEC1bits.U2TXIE = 1;
}

bool UART2_IsRxReady(void)
{    
    return !(rxHead == rxTail);
}

bool UART2_IsTxReady(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( UART2_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    return (size != 0);
}

bool UART2_IsTxDone(void)
{
    if(txTail == txHead)
    {
        return (bool)U2STAbits.TRMT;
    }
    
    return false;
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
static uint8_t UART2_RxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_rxTail = (uint8_t*)rxTail;
    
    if (snapshot_rxTail < rxHead) 
    {
        size = ( UART2_CONFIG_RX_BYTEQ_LENGTH - (rxHead-snapshot_rxTail));
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

/* !!! Deprecated API - This function may not be supported in a future release !!! */
uint8_t __attribute__((deprecated)) UART2_is_rx_ready(void)
{
    return UART2_RxDataAvailable();
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
static uint8_t UART2_TxDataAvailable(void)
{
    uint16_t size;
    uint8_t *snapshot_txHead = (uint8_t*)txHead;
    
    if (txTail < snapshot_txHead)
    {
        size = (snapshot_txHead - txTail - 1);
    }
    else
    {
        size = ( UART2_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1 );
    }
    
    if(size > 0xFF)
    {
        return 0xFF;
    }
    
    return size;
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
uint8_t __attribute__((deprecated)) UART2_is_tx_ready(void)
{
    return UART2_TxDataAvailable();
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART2_is_tx_done(void)
{
    return UART2_IsTxDone();
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
unsigned int __attribute__((deprecated)) UART2_ReadBuffer( uint8_t *buffer ,  unsigned int numbytes)
{
    unsigned int rx_count = UART2_RxDataAvailable();
    unsigned int i;
    
    if(numbytes < rx_count)
    {
        rx_count = numbytes;
    }
    
    for(i=0; i<rx_count; i++)
    {
        *buffer++ = UART2_Read();
    }
    
    return rx_count;    
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
unsigned int __attribute__((deprecated)) UART2_WriteBuffer( uint8_t *buffer , unsigned int numbytes )
{
    unsigned int tx_count = UART2_TxDataAvailable();
    unsigned int i;
    
    if(numbytes < tx_count)
    {
        tx_count = numbytes;
    }
    
    for(i=0; i<tx_count; i++)
    {
        UART2_Write(*buffer++);
    }
    
    return tx_count;  
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
UART2_TRANSFER_STATUS __attribute__((deprecated)) UART2_TransferStatusGet (void )
{
    UART2_TRANSFER_STATUS status = 0;
    uint8_t rx_count = UART2_RxDataAvailable();
    uint8_t tx_count = UART2_TxDataAvailable();
    
    switch(rx_count)
    {
        case 0:
            status |= UART2_TRANSFER_STATUS_RX_EMPTY;
            break;
        case UART2_CONFIG_RX_BYTEQ_LENGTH:
            status |= UART2_TRANSFER_STATUS_RX_FULL;
            break;
        default:
            status |= UART2_TRANSFER_STATUS_RX_DATA_PRESENT;
            break;
    }
    
    switch(tx_count)
    {
        case 0:
            status |= UART2_TRANSFER_STATUS_TX_FULL;
            break;
        case UART2_CONFIG_RX_BYTEQ_LENGTH:
            status |= UART2_TRANSFER_STATUS_TX_EMPTY;
            break;
        default:
            break;
    }

    return status;    
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
uint8_t __attribute__((deprecated)) UART2_Peek(uint16_t offset)
{
    uint8_t *peek = rxHead + offset;
    
    while(peek > (rxQueue + UART2_CONFIG_RX_BYTEQ_LENGTH))
    {
        peek -= UART2_CONFIG_RX_BYTEQ_LENGTH;
    }
    
    return *peek;
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART2_ReceiveBufferIsEmpty (void)
{
    return (UART2_RxDataAvailable() == 0);
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART2_TransmitBufferIsFull (void)
{
    return (UART2_TxDataAvailable() == 0);
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
UART2_STATUS __attribute__((deprecated)) UART2_StatusGet (void )
{
    return U2STA;
}

