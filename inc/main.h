#ifndef MAIN_H
#define MAIN_H

#include <esp_system.h>

extern esp_err_t err;

typedef struct {
    uint16_t damper_fr;
    uint16_t damper_fh;
    float temp_degre; 
    float humm_per;
    int16_t acc_X;
    int16_t acc_Y;
    int16_t acc_Z;
    int32_t gyro_X;
    int32_t gyro_Y;
    int32_t gyro_Z;

}can_data;

extern volatile can_data data;



#define CHECK(X) if ((err = X)!= ESP_OK) printf("ERR %x (%s) in %s::%d \n", err, esp_err_to_name(err), __FILE__, __LINE__);
#define  CHECK_M(X,Y) if ((err =X) != ESP_OK) printf("ERR %x (%s) in %s::%d: %s\n", err, esp_err_to_name(err), __FILE__, __LINE__, Y);

#endif //MAIN_H