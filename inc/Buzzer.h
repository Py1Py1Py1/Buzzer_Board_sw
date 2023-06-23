#ifndef BUZZER_H
#define BUZZER_H
#include <esp_system.h>

enum BuzzerPIN{
    Buzzer = 10,
};


void ini_Buzzer();

void setBuzzer(uint8_t);

void Buzzer_task(void);



#endif //BUZZER_H