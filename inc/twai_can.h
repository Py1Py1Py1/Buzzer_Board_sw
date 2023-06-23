#ifndef   TWAI_CAN_H
#define   TWAI_CAN_H  


#include <esp_system.h>
#include "wlc.h"

// typedef struct {
//     uint16_t damper_fr;
//     uint16_t damper_fh;
// }can_senddata;

void twai_init();  // Initialisierungsfunktion
void can_transmit(can_data *data);
uint8_t can_get_buzzerstatus(void);
void can_task(void);

#endif // TWAI_CAN_H


