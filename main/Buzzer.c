#include "Buzzer.h"
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "twai_can.h"



void ini_Buzzer(){

    gpio_config_t iniBUZZER = {


        .pin_bit_mask =   1ULL << Buzzer,

        .pull_down_en = 0,
        .pull_up_en = 0,
        .intr_type = 0,
        .mode = GPIO_MODE_OUTPUT,
    };

    gpio_config(&iniBUZZER);

    gpio_set_level(Buzzer, 0);
}



void setBuzzer(uint8_t val){

    gpio_set_level(Buzzer, val);
    // printf("Buzzer val:%d! \n", val);
    // vTaskDelay(time / portTICK_PERIOD_MS);
    // gpio_set_level(Buzzer, 0);

}

void Buzzer_task(){

    while(1){

        setBuzzer(can_get_buzzerstatus());
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}