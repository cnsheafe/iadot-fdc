/* 
 * File:   nrf24.h
 * Author: voldaltz
 *
 * Created on April 7, 2016, 1:25 PM
 */
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


void spiInit();
unsigned char *nrfRead(unsigned char reg);
void nrfWrite(unsigned char reg, unsigned char data);
void nrfTransmit(unsigned char *data,unsigned char len);
unsigned char* nrfReceive();
void nrfShowSettings();
unsigned char *nrfReadADDR(unsigned char reg);
void nrfWriteADDR(unsigned char reg,unsigned char *data);
