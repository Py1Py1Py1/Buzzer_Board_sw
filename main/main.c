#include <stdio.h>
#include "main.h"

#include "adc.h"
#include "wlc.h"
#include "Buzzer.h"
#include "twai_can.h"
#include "WSEN_ISDS.h"
#include "WSEN_HIDS.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "driver/i2c.h"


esp_err_t err;

volatile can_data data = {0};


void app_main(void)
{


    // //print mac adress
    // uint8_t mac[6] = {0};
    // esp_efuse_mac_get_default(mac);
    // printf("mac: %x:%x:%x:%x:%x:%x\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);


    // Init Funktions

    wlc_init();
    adc_ini();
    ini_Buzzer();
    twai_init();

    CHECK(WE_I2CInit(0x6B));
    CHECK(WSEN_HIDS_init());
    CHECK(WSEN_ISDS_init());

    // // call Tasks
    xTaskCreate(WSEN_task, "WSEN_task", 2048, NULL, 10, NULL);
    xTaskCreate(adc_task, "adc_task", 2048, NULL, 10, NULL);
    xTaskCreate(Buzzer_task, "Buzzer_task", 2048, NULL, 10, NULL);
    xTaskCreate(can_task, "can_task", 2048, NULL, 10, NULL);
    xTaskCreate(wlc_task, "wlc_task", 2048, NULL, 10, NULL);



    // // Round counter while Loop
    // uint16_t Round = 0;


//     while(1){


        
// Debug prints

//         printf("Round:%d", Round);       
         
//         printf("temp: %.2f°C\n", data.temp_degre);
//         printf("humm: %.2f%%\n", data.humm_per);
//         printf("\n");
//         printf("acc_x :  %dmg\n", data.acc_X);
//         printf("acc_y :  %dmg\n", data.acc_Y);
//         printf("acc_z :  %dmg\n", data.acc_Z);
//         printf("\n");
//         printf("gyro_X : %ldmdps\n", data.gyro_X);
//         printf("gyro_Y : %ldmdps\n", data.gyro_Y);
//         printf("gyro_Z : %ldmdps\n", data.gyro_Z);
//         printf("\n");
//         printf("damper_fr: %fV\n", (float) data.damper_fr * 2.5 / 4095);
//         printf("damper_fh: %fV\n", (float) data.damper_fh * 2.5 / 4095);
        
//         Round++;
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
        
//     }

}
