/* 
 * File:   uart.h
 * Author: voldaltz
 *
 * Created on October 24, 2015, 5:09 PM
 */
void uartInit(void);
void writeChar(char c);
void writeStr(char* str);
char readChar(void);
void readStr(char* buffer);
void readOpt(char buffer[3]);
void writeVal(unsigned long val);

