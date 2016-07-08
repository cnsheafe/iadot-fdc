
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
#include "i2clib.h"
#include "timer.h"

extern volatile unsigned char adcflag;
void adcInit(){
    TRISAbits.RA1 = 1;
    TRISAbits.RA2 = 1;
    PORTAbits.AN0 = 1;
    TRISAbits.RA3 = 1;
    ADCON0bits.CHS = 1;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.PCFG = 0;
    ADCON2bits.ADFM = 1;
    ADCON2bits.ACQT = 0b100;
    ADCON2bits.ADCS = 0b100;
    ADCON0bits.ADON = 1;
}
unsigned char* adcRead(unsigned char channel){
    unsigned char val[2];
    ADCON0bits.CHS = channel;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    adcflag = 0;
    val[0] = ADRESL;
    val[1] = ADRESH;
    //writeStr("StrainMSB:");writeVal(val[1]);writeChar(' ');
    //writeStr("StrainLSB:");writeVal(val[0]);writeChar('\n');
    return val;
}

