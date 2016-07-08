#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#include "uart.h"

volatile char data = 0,en_read = 0,garb = 0,IRQ = 0;
volatile unsigned char ackflag = 0,data_rdy = 0,con_sen = 0,b_error = 0,adcflag=0,bflag=0;
volatile unsigned int count = 0,bfcount=0;

void interrupt high_priority i2cISR(void){
    if (PIR2bits.TMR3IF){
        count++;
        PIR2bits.TMR3IF = 0;
        TMR3 = 0x3CAF;
    }
    if (INTCON3bits.INT2IF){
        IRQ = 1;
        INTCON3bits.INT2IF = 0;
    }
}

void interrupt low_priority lowISR(void){
    if(PIR1bits.ADIF){
        adcflag = 1;
        PIR1bits.ADIF = 0;
    }
    
    if(PIR1bits.SSPIF){
        if(!SSPCON2bits.ACKSTAT){
            //writeChar('a');
            ackflag = 1;
        }
        if(SSPSTATbits.S){
            //writeChar('s');
        }
        if(SSPSTATbits.BF){
            bflag = 1;
            //bfcount++;
            //writeChar('f');
            //writeVal(bfcount);
            //writeChar('\n');
        }
        else{
            //writeChar('u');
        }
        PIR1bits.SSPIF = 0;
    }
    if(PIR2bits.BCLIF){
        writeChar('b');
        PIR2bits.BCLIF = 0;
    }
     
    else if(PIR1bits.RCIF & !(RCSTAbits.FERR|RCSTAbits.OERR)){
        data = RCREG;
        en_read = 1;
    }
    else if(RCSTAbits.FERR|RCSTAbits.OERR){
        data = RCREG;
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    else{
        garb = RCREG;
    }
}