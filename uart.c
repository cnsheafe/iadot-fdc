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

extern volatile char data,en_read,garb;

void uartInit(void){
    
    TRISCbits.RC7 = 1;
    TRISCbits.RC6 = 1;
    RCSTAbits.SPEN = 1;     //Enable Serial port
    
    TXSTAbits.SYNC = 0;   
    TXSTAbits.BRGH = 1;
    BRG16 = 0;
    SPBRGH = 0;
    SPBRG = 12;//BAUDRATE of 19.2k @ 4MHz FOSC
    
    PIE1bits.RCIE = 1;
    PIE1bits.TXIE = 0;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
    
}


void writeChar(char c){
    while(!TRMT);
    TXREG = c;
}


void writeStr(char* str){
    for(char i = 0;str[i]!='\0';i++){
        writeChar(str[i]);
    }
}

char readChar(void){
    while(!en_read);
    en_read = 0;
    return data;    
}

void readStr(char* buffer){//Unstable.Don't use.
    char i = 0;
    do{
        i++;
        buffer[i-1] = readChar();
    }while(buffer[i]!='\n');
}

void readOpt(char buffer[3]){
    for (char i = 0; i<3 ;i++){
        buffer[i] = readChar();
    }
}
void writeVal(unsigned long val){
    char buffer[20];
    ultoa(buffer,val,10);
    writeStr(buffer);
}