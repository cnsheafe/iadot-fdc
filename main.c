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

#include "fdc1004.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "uart.h"          /* User funct/params, such as InitApp */
#include "timer.h"
#include "adclib.h"
#include "nrf24.h"

extern volatile char en_read;
void main(void){
    configureOscillator();
    configureInterrupts();
    uartInit();
    i2cInit();
    timer3_init();
    ADCON1bits.PCFG = 0b1111;
    TRISBbits.RB0 = 0;
    PORTBbits.RB0 = 1;
    TRISBbits.RB1 = 0;
    //PORTBbits.RB1 = 1;
    LATBbits.LB1 = 1;
    TRISBbits.RB2 = 1;
    unsigned char packetlen = 3;
    nrfWrite(nrfCONFIG,0b01101011);
    nrfWrite(nrfRX_PW_0,packetlen);
    unsigned char *capstat,tdata[16],addr[4],lsb,packet[3];
    unsigned char msb1,msb2;
    char cmd;
    fdcInit();
    addr[0] = M_MSB_1;
    addr[1] = M_MSB_2;
    addr[2] = M_MSB_3;
    addr[3] = M_MSB_4;
    
    while(1){
        cmd = readChar();
        if(cmd == 'm'){
            while(!en_read){//press q to stop
                    fdcMeas(addr[0],&msb1,&msb2,&lsb);
                    writeVal(msb1);writeChar(' ');writeVal(msb2);
                    writeChar(' ');writeVal(lsb);writeChar('\n');
                }
            packet[0] = lsb;
            packet[1] = msb2;
            packet[2] = msb1;
            en_read = 0;
        }
        
        else if(cmd == 'e'){
            capstat = fdcRead(FDC_CONF);
            writeStr("MSB:");writeVal(capstat[0]);writeChar(' ');
            writeStr("LSB:");writeVal(capstat[1]);writeChar('\n');
        }
        else if(cmd == 'r'){
            //while(!en_read){
            capstat = nrfRead(nrfSTAT);
            if(capstat[1]&0x40){
                nrfWrite(nrfSTAT,0x40);
            }
            nrfReceiveMode();
            }
           // en_read = 0;
        //}
        
        else if(cmd == 's'){
            nrfShowSettings();
        }
        
        else if(cmd == 't'){
            unsigned int randvals = intRand(TMR3);
            packet[0] = (randvals & 0xFF00)>>8;
            packet[1] = 
            nrfWrite(nrfCONFIG,0b01101010);
            nrfTransmit(packet,packetlen);
            capstat = nrfRead(nrfSTAT);
            if(capstat[1]&0x10){
                nrfWrite(nrfSTAT,0x10);
            }
            if(capstat[1]&0x20){
                nrfWrite(nrfSTAT,0x20);
            }
        }
    }
}                          
