
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
#include "adclib.h"

#define FDC_ADDR_R 0x55
#define FDC_ADDR_W 0x54
#define FDC_CONF 0x1A
#define FDC_IDRIVE_0 0x1E
#define FDC_SETTLE_0 0x10
#define FDC_CLK_DIV_0 0x14
#define FDC_RCOUNT_0 0x08
#define FDC_MUX 0x1B
#define FDC_RESET 0x1C
#define FDC_STATUS 0x18

extern volatile unsigned int count;
extern volatile char en_read;
unsigned char* fdcRead(unsigned char reg){
    static unsigned char val[2];
    unsigned char header[2],ack[2];
    header[0] = FDC_ADDR_W;
    header[1] = reg;
    ack[0] = 0;
    ack[1] = 1;
    i2cStart(header,2);
    SSPCON2bits.RSEN = 1;
    i2cIdle();
    SSPBUF = FDC_ADDR_R;
    i2cIdle();
    for(char i=0;i<2;i++){
        SSPCON2bits.RCEN = 1;
        i2cIdle();
        val[i] = SSPBUF;
        SSPCON2bits.ACKDT = ack[i];
        SSPCON2bits.ACKEN = 1;
        i2cIdle();
    }
    SSPCON2bits.PEN = 1;
    i2cIdle(); 
    return val;
}
void fdcWrite(unsigned char input[]){
    i2cStart(input,4);
    SSPCON2bits.PEN = 1;
    i2cIdle();
}
void fdcInit(){
    unsigned char cap_w[4];
    cap_w[0] = FDC_ADDR_W;
    cap_w[1] = FDC_IDRIVE_0;
    cap_w[2] = 0b11111000;
    cap_w[3] = 0b00000000;
    fdcWrite(cap_w);
    
    cap_w[0] = FDC_ADDR_W;
    cap_w[1] = FDC_SETTLE_0;
    cap_w[2] = 0;
    cap_w[3] = 20;
    fdcWrite(cap_w);
    
    cap_w[0] = FDC_ADDR_W;
    cap_w[1] = FDC_RCOUNT_0;
    cap_w[2] = 0x20;
    cap_w[3] = 0x00;
    fdcWrite(cap_w);
    
    cap_w[0] = FDC_ADDR_W;
    cap_w[1] = FDC_CLK_DIV_0;
    cap_w[2] = 0b00000000;
    cap_w[3] = 2;//FREF_CLKDIV
    fdcWrite(cap_w);
 
    cap_w[0] = FDC_ADDR_W;
    cap_w[1] = FDC_MUX;
    cap_w[2] = 0b00000010;
    cap_w[3] = 0b00001101;
    fdcWrite(cap_w);
    
    cap_w[0] = FDC_ADDR_W;
    cap_w[1] = FDC_CONF;
    cap_w[2] = 0b00010110;
    cap_w[3] = 0b10000101;
    fdcWrite(cap_w);
}
void fdcSleep(){
    unsigned char cap_w[4];
    cap_w[0] = FDC_ADDR_W;
    cap_w[1] = FDC_CONF;
    cap_w[2] = 0b00011110;
    cap_w[3] = 0b10100001;
    fdcWrite(cap_w);
}
void fdcReset(){
    unsigned char cap_w[4];
    cap_w[0] = FDC_ADDR_W;
    cap_w[1] = FDC_RESET;
    cap_w[2] = 0b10000000;
    cap_w[3] = 0b00000000;
    fdcWrite(cap_w);
}

void fdcMeas(unsigned int samples){
    unsigned char *buf,rdy=0;
    fdcInit();
    count = 0;
    T3CONbits.TMR3ON = 1;
    TMR3 = 0x3CAF;
    //for(int i=0;i<samples;i++){
    while(!en_read){
        while(!rdy){
            buf = fdcRead(FDC_STATUS);
            if(buf[1]&0x08){
                rdy = 1;
            }
        }
        rdy = 0;
        buf = fdcRead(0x00);
        writeStr("MSB1:");writeVal(buf[0]);writeChar(' ');
        writeStr("MSB2:");writeVal(buf[1]);writeChar('\n');
        buf = fdcRead(0x01);
        writeStr("LSB1:");writeVal(buf[0]);writeChar(' ');
        writeStr("LSB2:");writeVal(buf[1]);writeChar('\n');
        writeStr("Count:");writeVal(count);writeChar(' ');
        writeStr("TMR3:");writeVal(TMR3-15655);writeChar('\n');
        //buf = adcRead(0);
    }
    T3CONbits.TMR3ON = 0;
    fdcSleep();
}
