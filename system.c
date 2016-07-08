/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

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

#endif

#include "system.h"

/* Refer to the device datasheet for information about available
oscillator configurations. */
void configureOscillator(void)
{
    OSCCONbits.IRCF = 0b110;//4MHz
    OSCCONbits.SCS = 0;//Use primary OSC
    OSCTUNEbits.PLLEN = 0;
    OSCTUNEbits.TUN = 0b0000;
    
}

void configureInterrupts(void){
    RCONbits.IPEN = 1;//Enables interrupt priority
    
    /*Enable interrupts*/
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.SSPIE = 1;
    PIE1bits.RCIE = 1;
    PIE1bits.ADIE = 1;
    PIE2bits.TMR3IE = 1;
    PIE2bits.BCLIE = 1;
    INTCON2bits.INTEDG2 = 0;
    INTCON3bits.INT2IE = 1;
    INTCON3bits.INT2IP = 1;
    /*Interrupt priorities*/
    IPR1bits.SSPIP = 0;
    IPR1bits.RCIP = 0;
    IPR1bits.ADIP = 0;
    IPR2bits.TMR3IP = 1;
}

