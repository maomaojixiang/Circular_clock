#ifndef __SHT30_H
#define __SHT30_H	
#include "sys.h"
#include "iic_init.h"

extern u8 sht30_data_buffer[6];
extern u16 SHT30_temperature;
extern u16 SHT30_humidity;

void SHT_Init(void);
int sht30_data_process(void);
#endif
