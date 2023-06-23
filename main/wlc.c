#include <stdio.h>

#include "esp_now.h"
#include "main.h"
#include "esp_wifi.h"
#include "wlc.h"

#include "nvs_flash.h"
#include "string.h"

// define receiver mac adress
static uint8_t receiver_mac[6] = {0xf4,0x12,0xfa,0x17,0xbd,0x54};
                                

void wlc_init()
{
    // Initialisiere ESPNOW UND WIFI

     esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }

    CHECK(ret);


    CHECK(esp_netif_init());
    CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    CHECK(esp_wifi_init(&cfg));
    CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    CHECK(esp_wifi_start());
    CHECK(esp_wifi_set_channel(1,1));


    CHECK_M(esp_now_init(), "ESP-NOW Init fail");

    esp_now_peer_info_t peer = {0};
    peer.channel = 1;
    peer.ifidx = ESP_IF_WIFI_STA;
    peer.encrypt = false;
    memcpy(peer.peer_addr, receiver_mac,6);

    CHECK_M(esp_now_add_peer(&peer), "Add peer fail");

}




void transmit_data(can_data* data){
    CHECK(esp_now_send(receiver_mac, (uint8_t*) data, sizeof(can_data)));
}


void wlc_task(){

    while(1){

        transmit_data(&data);
        
        vTaskDelay(100/ portTICK_PERIOD_MS);

    }
 }