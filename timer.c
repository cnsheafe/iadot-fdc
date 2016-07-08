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
#include "timer.h"
extern volatile unsigned int count;
/*

*/
void timer3_init(){
    T3CON = 0b10000000;
}

void delay(float sec){
    T3CONbits.TMR3ON = 1;
    unsigned char done = 0;
    volatile unsigned int t1=count,t2=count;
    TMR3 = 0xFFF5;
    while(!done){
        t2 = count;
        if((t2-t1)*0.0001>=sec){
            done = 1;
            T3CONbits.TMR3ON = 0;
        }
    }
}

unsigned int intRNG(unsigned int seed){
    unsigned char msb,lsb,x,y,f;
    unsigned int val;
    msb = (seed & 0xFF00)>>8;
    lsb = (seed & 0x00FF);
    y = ((msb & 0x80)>>7)^((lsb & 0x40)>>6);
    x = ((msb & 0x10)>>4)^((lsb & 0x08)>>3);
    f = x^y;
    val = (timer_val<<1)|f;
    return val;
}
