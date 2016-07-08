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
#include "i2clib.h"
#include "timer.h"


void i2cInit(){
    SSPCON1bits.SSPEN = 0;
    TRISCbits.RC3 = 1;//SCL set as clock output
    TRISCbits.RC4 = 1;//SDA set as data input
    SSPCON1bits.SSPM = 0b1000;//Master_Mode w/ clock Fosc/(4*(SSPADD+1))
    SSPSTATbits.SMP = 1;
    SSPADD = 9;
    SSPCON1bits.SSPEN = 1;
}

void i2cIdle(){
    while(SSPSTATbits.RW|(0x1F&SSPCON2));
}

void i2cStart(unsigned char input[],unsigned char len){
    i2cInit();
    SSPCON2bits.SEN = 1;//Start condition
    i2cIdle();
    for(int i = 0;i<len;i++){
        SSPBUF = input[i];
        i2cIdle();
    }
}





