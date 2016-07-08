
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


#define M_MSB_1 0x00
#define M_LSB_1 0x01
#define M_MSB_2 0x02
#define M_LSB_2 0x03
#define M_MSB_3 0x04
#define M_LSB_3 0x05
#define M_MSB_4 0x06
#define M_LSB_4 0x07

#define CONF_1 0x08
#define CONF_2 0x09
#define CONF_3 0x0A
#define CONF_4 0x0B

#define FDC_CONF 0x0C
#define OFFSET_1 0x0D
#define OFFSET_2 0x0E
#define OFFSET_3 0x0F
#define OFFSET_4 0x10

#define GAIN_1 0x11
#define GAIN_2 0x12
#define GAIN_3 0x13
#define GAIN_4 0x14

#define TI_ID 0xFE
#define DEV_ID 0xFF

#define FDC_W 0b10100000
#define FDC_R 0b10100001

unsigned char* fdcRead(unsigned char reg){
    static unsigned char val[2];
    unsigned char header[2],ack[2];
    header[0] = FDC_W;
    header[1] = reg;
    ack[0] = 0;
    ack[1] = 1;
    i2cStart(header,2);
    SSPCON2bits.RSEN = 1;
    i2cIdle();
    SSPBUF = FDC_R;
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
        
void fdcWrite(unsigned char reg,unsigned char msb,unsigned char lsb){
    unsigned char input[4];
    input[0] = FDC_W; input[1] = reg; input[2] = msb; input[3] = lsb;
    i2cStart(input,4);
    SSPCON2bits.PEN = 1;
    i2cIdle();
}

void fdcInit(){
    //fdcWrite(CONF_1,0b00010011,0b01000000);
    fdcWrite(CONF_1,0b00010000,0);
    fdcWrite(CONF_2,0b00110000,0);
    fdcWrite(CONF_3,0b01010000,0);
    fdcWrite(CONF_4,0b01110000,0);
    fdcWrite(FDC_CONF,0b00001100,0b10000000);
    fdcWrite(OFFSET_1,0b11011001,0);
    fdcWrite(OFFSET_2,0b11011001,0);
    fdcWrite(OFFSET_3,0b11011001,0);
    fdcWrite(OFFSET_4,0b11011001,0);
}
void fdcMeas(unsigned char chx_reg,unsigned char *msb1,unsigned char *msb2,unsigned char *lsb){
    unsigned char done = 0,*buf;
    unsigned char addr,offset;
    offset = chx_reg/2;
    addr = 1<<(7-offset);
    fdcWrite(FDC_CONF,0b00001100,addr);
    
    while(!done){
        addr = 1<<(3-offset);
        buf = fdcRead(FDC_CONF);
        if(buf[1]&(addr)){
            buf = fdcRead(chx_reg);
            *msb1 = buf[0];
            *msb2 = buf[1];
            buf = fdcRead(chx_reg+1);
            *lsb = buf[0];
            done = 1;
        }
    }
}
