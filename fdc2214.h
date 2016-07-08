#define FDC_CONF 0x1A
#define FDC_ADDR_R 0x55
#define FDC_ADDR_W 0x54

unsigned char* fdcRead(unsigned char reg);
void fdcWrite(unsigned char input[]);
void fdcInit();
void fdcSleep();
void fdcReset();
void fdcMeas(unsigned int samples);
