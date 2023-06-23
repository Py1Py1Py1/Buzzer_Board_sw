#include <stdio.h>
#include "main.h"


#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>

#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_cali.h"


#include "adc.h"

// static int voltage[2][10];
// adc_cali_handle_t cali_handle;




void adc_ini(void)
{   
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_1, ADC_ATTEN_DB_11);

    // //CHECK_M((TAG, "calibration scheme version is %s", "Curve Fitting");
    // adc_cali_curve_fitting_config_t cali_config = {
    //     .unit_id = 0,
    //     .atten = ADC_ATTEN_DB_11,
    //     .bitwidth = ADC_BITWIDTH_DEFAULT,
    // };
    
    // CHECK(adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle));
    
}



uint16_t get_adc(uint8_t chann){


    uint16_t adc_raw = 0;
    

    while(!adc_raw){
        if(chann == 0){
            // CHECK(adc_cali_raw_to_voltage(cali_handle, adc_raw, &voltage[0][0]));
            adc_raw = adc1_get_raw(ADC1_CHANNEL_0);
            
            
        }
        else if (chann == 1){
            adc_raw = adc1_get_raw(ADC1_CHANNEL_1);
            
        }
    }

return(adc_raw);
}

void adc_task(){

    while(1){

        data.damper_fr = get_adc(0);
        data.damper_fh = get_adc(1);
        vTaskDelay(100 / portTICK_PERIOD_MS);


    }


}