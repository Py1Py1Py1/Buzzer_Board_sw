#ifndef WLC_H
#define WLC_H

#include <esp_system.h>
#include "main.h"


void wlc_init();

void transmit_data(can_data*);

void wlc_task(void);

#endif //WLC_H