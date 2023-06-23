#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/twai.h"
#include "twai_can.h"
#include "main.h"


#define TX_GPIO_NUM             7
#define RX_GPIO_NUM             6
#define My_ID                   0x001
#define Sender_ID               0x010
#define buzzer_bit              0


//Initialisierung

void twai_init(){

//Initialisierung vom CAN-BUS

    static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();        //Baudrate von 1 Mbits 
    static const twai_filter_config_t f_config = {                                   //Filter: Es werden nur Nachrichten von 0x123 angenommen
        .acceptance_code = (Sender_ID << 21),                           
        .acceptance_mask = ~(TWAI_STD_ID_MASK << 21),
        .single_filter = true,
    };
    static const twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NORMAL); //TX = PIN 4, RX = PIN 5, Modus = Testmodus

//Install TWAI driver

    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        //printf("CAN Driver installed\n");
    } 
    else {
        printf("CAN Failed to install driver\n");
        return;
    }

//Start TWAI driver

    if (twai_start() == ESP_OK) {
        //printf("CAN Driver started\n");
    } else {
        printf("CAN Failed to start driver\n");
        return;
    }
}


//Dämpferwertesendefunktion
void can_transmit(can_data *data){
    twai_message_t msg = {0};
    msg.self = 1;
    msg.identifier = My_ID;
    msg.extd = 0;
    msg.rtr = 0;
    msg.data_length_code = 4;
    msg.data[0] = (uint8_t)(data->damper_fh);
    msg.data[1] = (uint8_t)(data->damper_fh >> 8);
    msg.data[2] = (uint8_t)data->damper_fr;
    msg.data[3] = (uint8_t)(data->damper_fr >> 8);
    
    twai_transmit(&msg, pdMS_TO_TICKS(100));
    // if (twai_transmit(&msg, pdMS_TO_TICKS(100)) == ESP_OK) {
    //         printf("Message queued for transmission\n");
            
    // } 
    // else {
    //         printf("Failed to queue message for transmission\n");
    //         printf("%ld %d %d\n", msg.identifier, msg.data[0], msg.data[1]);
    //     }  



 } 

// Buzzerstatusempfangsfunktion

 uint8_t can_get_buzzerstatus(){

    twai_message_t message_received = {0};
    
     if (twai_receive(&message_received, pdMS_TO_TICKS(1000)) == ESP_OK) {
        // printf("Buzzerstatus received\n");
    } else {
        // printf("Failed to receive Buzzerstatus\n");
    }

    //Process received message

    // printf("SenderID is %ld\n", message_received.identifier);

    return message_received.data[buzzer_bit];

 };


 void can_task(){

    while(1){
        can_transmit(&data);
        
        vTaskDelay(100/ portTICK_PERIOD_MS);

    }
 }