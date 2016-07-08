/* 
 * File:   fdc1004.h
 * Author: voldaltz
 *
 * Created on April 19, 2016, 8:55 PM
 */
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


unsigned char* fdcRead(unsigned char reg);
void fdcWrite(unsigned char reg,unsigned char msb,unsigned char lsb);
void fdcInit();
void fdcMeas(unsigned char chx_reg,unsigned char *msb1,unsigned char *msb2,unsigned char *lsb);
