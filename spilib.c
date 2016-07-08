#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <stdio.h>
#include <stdlib.h>
#endif

#include "uart.h"
#include "spilib.h"

void spiInit(){
    TRISCbits.RC5 = 0;
    TRISCbits.RC3 = 0;
    SSPCON1bits.SSPEN = 0;
    SSPSTATbits.CKE = 1;
    SSPCON1bits.CKP = 0;
    SSPCON1bits.SSPM = 0b0000;
    SSPSTATbits.SMP = 0;
    SSPCON1bits.SSPEN = 1;
}
