#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define BMI160_CS_PIN     16 //GPIO_NUM_16  
#define BMI160_MOSI_PIN   18 //GPIO_NUM_5
#define BMI160_MISO_PIN   5 //GPIO_NUM_18
#define BMI160_SCK_PIN    19 //GPIO_NUM_19

#include "BMI160_Driver.h"

spi_device_handle_t my_handler;
uint8_t transmit_buffer;
uint8_t receive_buffer;

void app_main(void) {
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_config, SPI_DMA_DISABLED));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &device_config, &my_handler));
    ESP_ERROR_CHECK(spi_device_acquire_bus(my_handler, portMAX_DELAY));
    startup_reset(my_handler);
    transmit_buffer = 0b00000000;
    write_register(my_handler, PWR_MGMT_2, 1, &transmit_buffer);
    transmit_buffer = 0b00011000;
    write_register(my_handler, GYRO_CONFIG, 1, &transmit_buffer);
    transmit_buffer = 0b00001000;
    write_register(my_handler, ACCEL_CONFIG, 1, &transmit_buffer);
    transmit_buffer = 0b01110100;
    write_register(my_handler, USER_CTRL, 1, &transmit_buffer);
    transmit_buffer = 0b01110000;
    write_register(my_handler, FIFO_EN, 1, &transmit_buffer);
    transmit_buffer = 0b00000000;
    write_register(my_handler, CONFIG, 1, &transmit_buffer);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    uint8_t registers_to_be_accessed[6] = {GYRO_XOUT_H, GYRO_XOUT_L, GYRO_YOUT_H, GYRO_YOUT_L, GYRO_ZOUT_H, GYRO_ZOUT_L};
    uint8_t results[6];
    for (int i = 0;i < 6;i++) {
        read_register(my_handler, registers_to_be_accessed[i], 1, &receive_buffer);
        //read_register(my_handler, FIFO_R_W, 1, &receive_buffer);
        results[i] = receive_buffer;
    }

    registers_to_be_accessed[0] = XG_OFFSET_H;
    registers_to_be_accessed[1] = XG_OFFSET_L;
    registers_to_be_accessed[2] = YG_OFFSET_H;
    registers_to_be_accessed[3] = YG_OFFSET_L;
    registers_to_be_accessed[4] = ZG_OFFSET_H;
    registers_to_be_accessed[5] = ZG_OFFSET_L;

    for (int i = 0;i < 6;i++) {
        transmit_buffer = results[i];
        write_register(my_handler, registers_to_be_accessed[i], 1, &transmit_buffer);
        //read_register(my_handler, FIFO_R_W, 1, &receive_buffer);
    }

    registers_to_be_accessed[0] = GYRO_XOUT_H;
    registers_to_be_accessed[1] = GYRO_XOUT_L;
    registers_to_be_accessed[2] = GYRO_YOUT_H;
    registers_to_be_accessed[3] = GYRO_YOUT_L;
    registers_to_be_accessed[4] = GYRO_ZOUT_H;
    registers_to_be_accessed[5] = GYRO_ZOUT_L;

    while (1) {
        for (int i = 0;i < 6;i++) {
            read_register(my_handler, registers_to_be_accessed[i], 1, &receive_buffer);
            //read_register(my_handler, FIFO_R_W, 1, &receive_buffer);
            results[i] = receive_buffer;
        }
        int16_t gyro_x = (results[0] << 8) | results[1];
        int16_t gyro_y = (results[2] << 8) | results[3];
        int16_t gyro_z = (results[4] << 8) | results[5];
        printf("%d, %d, %d \n", gyro_x, gyro_y, gyro_z);         
        vTaskDelay(1);
    }
}