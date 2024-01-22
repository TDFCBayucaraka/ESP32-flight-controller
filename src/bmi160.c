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
#define BMI160_MOSI_PIN   5 //GPIO_NUM_5
#define BMI160_MISO_PIN   18 //GPIO_NUM_18
#define BMI160_SCK_PIN    19 //GPIO_NUM_19

#include "BMI160_Driver.h"

spi_device_handle_t bmi_160;
uint8_t transmit_buffer;
uint8_t receive_buffer;

void app_main(void) {
    gpio_set_level(GPIO_NUM_16, 0);
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_config, SPI_DMA_DISABLED));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &device_config, &bmi_160));
    ESP_ERROR_CHECK(spi_device_acquire_bus(bmi_160, portMAX_DELAY));
    while (1) {
        bmi160_read_register(bmi_160, CHIP_ID, 1, &receive_buffer);
        printf("%d \n", receive_buffer); 
        vTaskDelay(1000/portTICK_PERIOD_MS); 
    }
}