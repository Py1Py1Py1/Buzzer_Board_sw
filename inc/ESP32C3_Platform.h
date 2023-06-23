

#ifndef ESP32C3_PLAFORM_H
#define ESP32C3_PLAFORM_H


#define TIMEOUT 1000

/**         Includes         **/

#include <stdint.h>
#include <stdlib.h>


#include <stdbool.h>

#define WE_SUCCESS (0)
#define WE_FAIL (-1)

// //Use SDA1 and SCL1 on Arduino Due
// #if defined(__SAM3X8E__)
// #define Wire Wire1
// #endif

/**         Functions definition         **/



void I2CSetAddress(int address);
int I2CInit(int address);
int ReadReg(uint8_t RegAdr, int NumByteToRead, uint8_t *Data);
int WriteReg(uint8_t RegAdr, int NumByteToWrite, uint8_t *Data);


void WSEN_task(void);




#endif /* ESP32C3_PLAFORM_H */
