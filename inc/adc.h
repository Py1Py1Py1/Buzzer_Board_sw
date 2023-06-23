#ifndef ADC_H
#define ADC_H

#include <esp_system.h>



void adc_ini(void);

uint16_t get_adc(uint8_t);
void adc_task(void);

#endif