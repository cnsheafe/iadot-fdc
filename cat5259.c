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

#include "i2clib.h"

#define DIGPOT_ADDR 0x50
#define WRITE_OP_CODE 0xA0
#define READ_OP_CODE 0x90

void writePot(unsigned char input[],unsigned char len){
    i2cStart(input,len);
    SSPCON2bits.PEN = 1;
    i2cIdle();
}
unsigned char readPot(unsigned char input[],unsigned char len){
    unsigned char buffer = 0;
    i2cStart(input,len);
    SSPCON2bits.RCEN = 1;
    i2cIdle();
    buffer = SSPBUF;
    SSPCON2bits.ACKDT = 0;
    SSPCON2bits.ACKEN = 1;
    i2cIdle();
    SSPCON2bits.PEN = 1;
    i2cIdle();
    return buffer;
}
