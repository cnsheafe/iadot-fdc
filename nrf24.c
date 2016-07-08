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
#include "timer.h"
#include "nrf24.h"

/*SPI Commands*/
#define R_PX 0b01100001
#define T_PX 0b10100000
#define FL_R 0b11100010
#define FL_T 0b11100001

/*Register Map*/
#define nrfCONFIG 0
#define nrfEN_AA 1
#define nrfEN_RX 2
#define nrfSET_AW 3
#define nrfSET_RETR 4
#define nrfRF_CH 5
#define nrfRF_SETUP 6
#define nrfSTAT 7
#define nrfOBS_TX 8
#define nrfRPD 9
#define nrfRX_ADDR_0 0x0A
#define nrfRX_ADDR_1 0x0B
#define nrfRX_ADDR_2 0x0C
#define nrfRX_ADDR_3 0x0D
#define nrfRX_ADDR_4 0x0E
#define nrfRX_ADDR_5 0x0F
#define nrfTX_ADDR 0x10
#define nrfRX_PW_0 0x11
#define nrfRX_PW_1 0x12
#define nrfRX_PW_2 0x13
#define nrfRX_PW_3 0x14
#define nrfRX_PW_4 0x15
#define nrfRX_PW_5 0x16
#define nrfFIFO_STAT 0x17



extern volatile unsigned char bflag;
extern volatile char en_read,IRQ;

void spiInit(){
    TRISCbits.RC5 = 0;
    TRISCbits.RC3 = 0;
    SSPCON1bits.SSPEN = 0;
    SSPSTATbits.CKE = 1;
    SSPCON1bits.CKP = 0;
    SSPCON1bits.SSPM = 0b0000;
    SSPSTATbits.SMP = 0;
    SSPCON1bits.SSPEN = 1;
    delay(0.0001);
}

unsigned char *nrfRead(unsigned char reg){
    unsigned char val[2];
    spiInit();
    PORTBbits.RB0 = 0;
    for(char i=0;i<2;i++){
        SSPBUF = reg;
        delay(0.00001);
        while(!bflag);
        bflag = 0;
        val[i] = SSPBUF;
    }
    PORTBbits.RB0 = 1;
    return val;
}

void nrfWrite(unsigned char reg,unsigned char data){
    spiInit();
    unsigned char tmp;
    /*Begin write*/
    PORTBbits.RB0 = 0;
    SSPBUF = (0b00100000)|reg;
    while(!bflag);
    tmp = SSPBUF;
    bflag = 0;
    SSPBUF = data;
    while(!bflag);
    bflag = 0;
    PORTBbits.RB0 = 1;
}

void nrfTransmit(unsigned char *data,unsigned char len){
    nrfWrite(nrfSTAT,0b00011110);
    unsigned char tmp;
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB1 = 1;
    SSPBUF = T_PX;
    while(!bflag);
    tmp = SSPBUF;
    bflag = 0;
    for(int i=0;i<len;i++){
        SSPBUF = data[i];
        writeVal(data[i]);writeChar('\n');
        while(!bflag);
        bflag = 0;
    }
    PORTBbits.RB0 = 1;
}

unsigned char* nrfReceive(){
    unsigned char *len = nrfRead(nrfRX_PW_0);
    unsigned char val[32];
    spiInit();
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 1;
    SSPBUF = R_PX;
    delay(0.0001);
    while(!bflag);
    val[0] = SSPBUF;
    bflag = 0;
    
    for(char i=0;i<len[1];i++){
        SSPBUF = R_PX;
        delay(0.0001);
        while(!bflag);
        val[i] = SSPBUF;
        bflag = 0;
    }
    
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 0;
    return val;
}

void nrfReceiveMode(){
    spiInit();
    unsigned char* status,val[20],*len=nrfRead(nrfRX_PW_0);
    
    nrfWrite(nrfCONFIG,0b00111011);
    while(!IRQ);
    IRQ = 0;
    
        PORTBbits.RB0 = 0;
        SSPBUF = R_PX;
        delay(0.0001);
        while(!bflag);
        val[0] = status;
        bflag = 0;
        
        for(char i=0;i<len[1];i++){
            SSPBUF = R_PX;
            delay(0.0001);
            while(!bflag);
            val[i] = SSPBUF;
            writeVal(val[i]);writeChar('\n');
            bflag = 0;
        }
    PORTBbits.RB0 = 1;
en_read = 0;
}


void nrfShowSettings(){
    unsigned char *val;
    for(int i =0;i<18;i++){
        if((i == 0x0A)||(i==0x0B)||(i==0x10)){
            val = nrfReadADDR(i);
            writeVal(i);writeChar(':');
            for(char k=1;k<6;k++){
                writeVal(val[k]);writeChar(' ');
            }
            writeChar('\n');
        }
        else{
            val = nrfRead(i);
            writeVal(i);writeChar(':');writeVal(val[1]);
            writeChar('\n');
        }
    }
}

unsigned char *nrfReadADDR(unsigned char reg){
    unsigned char val[5];
    /*Only for registers 0x0A,0x0B,0x10*/
    spiInit();
    PORTBbits.RB0 = 0;
    for(char i=0;i<6;i++){
        SSPBUF = reg;
        delay(0.0001);
        while(!bflag);
        bflag = 0;
        val[i] = SSPBUF;
    }
    PORTBbits.RB0 = 1;
    return val;
}

void nrfWriteADDR(unsigned char reg,unsigned char *data){
    spiInit();
    unsigned char tmp;
    /*Begin write*/
    PORTBbits.RB0 = 0;
    SSPBUF = (0b00100000)|reg;
    while(!bflag);
    tmp = SSPBUF;
    bflag = 0;
    for(char i=0;i<5;i++){
        SSPBUF = data[i];
        while(!bflag);
        bflag = 0;
    }
    PORTBbits.RB0 = 1;
}
