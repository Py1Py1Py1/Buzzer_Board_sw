/**
 ***************************************************************************************************
 * This file is part of WE Sensors SDK:
 * https://www.we-online.com/sensors
 *
 * THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE THAT WÜRTH ELEKTRONIK
 * EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY KIND RELATED TO, BUT NOT LIMITED
 * TO THE NON-INFRINGEMENT OF THIRD PARTIES’ INTELLECTUAL PROPERTY RIGHTS OR THE
 * MERCHANTABILITY OR FITNESS FOR YOUR INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT
 * WARRANT OR REPRESENT THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY PATENT
 * RIGHT, COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY RIGHT RELATING TO ANY
 * COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT IS USED. INFORMATION PUBLISHED BY
 * WÜRTH ELEKTRONIK EISOS REGARDING THIRD-PARTY PRODUCTS OR SERVICES DOES NOT CONSTITUTE A LICENSE
 * FROM WÜRTH ELEKTRONIK EISOS TO USE SUCH PRODUCTS OR SERVICES OR A WARRANTY OR ENDORSEMENT
 * THEREOF
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS DRIVER PACKAGE.
 *
 * COPYRIGHT (c) 2023 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/


#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ESP32C3_Platform.h"
#include "WSEN_HIDS.h"
#include "WSEN_ISDS.h"
#include "main.h"


static uint8_t deviceAddress = 0;

static gpio_num_t i2c_gpio_sda = 4;
static gpio_num_t i2c_gpio_scl = 5;

static uint32_t i2c_frequency = 50000;
static i2c_port_t i2c_port = I2C_NUM_0;

#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1 



int I2CInit(int address)
{

    deviceAddress = address;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = i2c_gpio_sda,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_io_num = i2c_gpio_scl,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = i2c_frequency,
        
    };
    CHECK(i2c_param_config(i2c_port, &conf));
    CHECK_M(i2c_driver_install(0,conf.mode,0,0,0),"2I2c driver error");

    return WE_SUCCESS;
}



void I2CSetAddress(int address) 
{ 
	deviceAddress = address;

}



int ReadReg(uint8_t RegAdr, int NumByteToRead, uint8_t *Data)
{


    esp_err_t tmp_err = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    CHECK(i2c_master_start(cmd));
    CHECK(i2c_master_write_byte(cmd, deviceAddress << 1 | I2C_MASTER_WRITE, 1));
    CHECK(i2c_master_write_byte(cmd, RegAdr, I2C_MASTER_ACK));
    CHECK(i2c_master_stop(cmd));
    CHECK(i2c_master_cmd_begin(0,cmd, 1000 / portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
    
    if (tmp_err != ESP_OK){
        return WE_FAIL;
    }
    
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, deviceAddress << 1 | I2C_MASTER_READ, 1);
    if(NumByteToRead > 1 ){
        CHECK(i2c_master_read(cmd, Data,NumByteToRead -1, I2C_MASTER_ACK));
    }
    CHECK(i2c_master_read_byte(cmd, Data+NumByteToRead-1, I2C_MASTER_NACK));
    i2c_master_stop(cmd);

    tmp_err = i2c_master_cmd_begin(0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    

    if (tmp_err != ESP_OK) return WE_FAIL;
    return WE_SUCCESS;
}



int WriteReg(uint8_t RegAdr, int NumByteToWrite, uint8_t *Data)
{   
    esp_err_t tmp_err = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    CHECK(i2c_master_start(cmd));
    CHECK(i2c_master_write_byte(cmd, deviceAddress << 1 | I2C_MASTER_WRITE, I2C_MASTER_ACK));
    CHECK(i2c_master_write_byte(cmd, RegAdr, I2C_MASTER_ACK));
    if(NumByteToWrite > 0 ){
    CHECK(i2c_master_write(cmd, Data,NumByteToWrite, I2C_MASTER_ACK));
    }
    CHECK(i2c_master_stop(cmd));
    tmp_err = i2c_master_cmd_begin(0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    

    if (tmp_err != ESP_OK) return WE_FAIL;
    return WE_SUCCESS;
}

 

void WSEN_task(){

    while(1){

    // set HIDS Adress and read
    I2CSetAddress(0x5f);

    CHECK(HIDS_getTemperature(&data.temp_degre));
    CHECK(HIDS_getHumidity(&data.humm_per))



    // set ISDS Adress an read
    I2CSetAddress(0x6B);

    CHECK(ISDS_getAccelerations_int(&data.acc_X, &data.acc_Y, &data.acc_Z));
    CHECK(ISDS_getAngularRates_int(&data.gyro_X, &data.gyro_Y, &data.gyro_Z));
    // CHECK(ISDS_getTemperature_int(&data.temp_degre))
    


    vTaskDelay(100 / portTICK_PERIOD_MS);
    }




}