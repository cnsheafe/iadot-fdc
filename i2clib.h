/* 
 * File:   I2C.h
 * Author: voldaltz
 *
 * Created on February 2, 2016, 3:34 PM
 */

void i2cInit();
void i2cIdle();
void i2cStart(unsigned char input[],unsigned char len);