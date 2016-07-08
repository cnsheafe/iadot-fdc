/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

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

#include "fdc2214.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "uart.h"          /* User funct/params, such as InitApp */
#include "timer.h"
#include "adclib.h"

void main(void){
    configureOscillator();
    configureInterrupts();
    uartInit();
    i2cInit();
    //timer3_init();
    //adcInit();
    unsigned char *capstat,*adcstat;
    char cmd;
    
    while(1){
        cmd = readChar();
        if(cmd == 'm'){
            fdcMeas(250);
        }
        else if(cmd == 'd'){
            delay(2);
            writeStr("delayed?\n");
        }
        else if(cmd == 'e'){
            capstat = fdcRead(FDC_CONF);
            writeStr("MSB:");writeVal(capstat[0]);writeChar(' ');
            writeStr("LSB:");writeVal(capstat[1]);writeChar('\n');
        }
        else if(cmd == 'r'){
            fdcReset();
        }
        else if(cmd == 'v'){
            capstat = adcRead(0);
            //capstat = adcRead(1);
            //writeStr("MSB:");writeVal(capstat[1]);writeChar(' ');
            //writeStr("LSB:");writeVal(capstat[0]);writeChar('\n');
        }
    }                          
}
