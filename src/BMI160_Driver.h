#ifndef BMI160_DRIVER_H
#define BMI160_DRIVER_H
#include "driver/spi_master.h"

/*
BMI160 DRIVER
Raditya Eka Putra (2024)
All Rights Reserved. 

This driver is made specifically for BMI160, created for use in drone flight controller program. 
This driver is made to be used with ESP-IDF and ESP32 series microcontroller. 
The driver utilizes SPI Master driver provided by ESP-IDF, as SPI is superior in terms of speed compared to
I2C. It also does not use Slave Addressing concept which means lower latency as it doesn't need to provide 
slave address

below is all register available in BMI160 IMU, as well as some method to access its register to ease 
development. Usage of methods and functions as well as certain important concept will be explained in the comments
alongside the code, scattered in this header file. 

Currently this driver may only be used by the author and Bayucaraka ITS Team for internal development
*/

typedef enum {
    CHIP_ID       = 0x00, 
    ERR_REG       = 0x02, 
    PMU_STATUS    = 0x03, 
    DATA_0        = 0x04, 
    DATA_1        = 0x05, 
    DATA_2        = 0x06, 
    DATA_3        = 0x07, 
    DATA_4        = 0x08, 
    DATA_5        = 0x09, 
    DATA_6        = 0x0A, 
    DATA_7        = 0x0B, 
    DATA_8        = 0x0C, 
    DATA_9        = 0x0D, 
    DATA_10       = 0x0E, 
    DATA_11       = 0x0F, 
    DATA_12       = 0x10, 
    DATA_13       = 0x11, 
    DATA_14       = 0x12, 
    DATA_15       = 0x13, 
    DATA_16       = 0x14, 
    DATA_17       = 0x15, 
    DATA_18       = 0x16, 
    DATA_19       = 0x17, 
    SENSORTIME_0  = 0x18, 
    SENSORTIME_1  = 0x19, 
    SENSORTIME_2  = 0x1A, 
    STATUS        = 0x1B, 
    INT_STATUS_0  = 0x1C, 
    INT_STATUS_1  = 0x1D, 
    INT_STATUS_2  = 0x1E, 
    INT_STATUS_3  = 0x1F, 
    TEMPERATURE_0 = 0x20, 
    TEMPERATURE_1 = 0x21, 
    FIFO_LENGTH_0 = 0x22, 
    FIFO_LENGTH_1 = 0x23, 
    FIFO_DATA     = 0x24, 
    ACC_CONF      = 0x40, 
    ACC_RANGE     = 0x41, 
    GYRO_CONF     = 0x42, 
    GYRO_RANGE    = 0x43, 
    MAG_CONF      = 0x44, 
    FIFO_DOWNS    = 0x45, 
    FIFO_CONFIG_0 = 0x46, 
    FIFO_CONFIG_1 = 0x47, 
    MAG_IF_0      = 0x4B, 
    MAG_IF_1      = 0x4C, 
    MAG_IF_2      = 0x4D, 
    MAG_IF_3      = 0x4E, 
    MAG_IF_4      = 0x4F, 
    INT_EN_0      = 0x50, 
    INT_EN_1      = 0x51, 
    INT_EN_2      = 0x52, 
    INT_OUT_CTRL  = 0x53, 
    INT_LATCH     = 0x54, 
    INT_MAP_0     = 0x55, 
    INT_MAP_1     = 0x56, 
    INT_MAP_2     = 0x57,   
    INT_DATA_0    = 0x58,  
    INT_DATA_1    = 0x59,  
    INT_LOWHIGH_0 = 0x5A,  
    INT_LOWHIGH_1 = 0x5B,  
    INT_LOWHIGH_2 = 0x5C,  
    INT_LOWHIGH_3 = 0x5D,  
    INT_LOWHIGH_4 = 0x5E,  
    INT_MOTION_0  = 0x5F,  
    INT_MOTION_1  = 0x60,  
    INT_MOTION_2  = 0x61,  
    INT_MOTION_3  = 0x62,  
    INT_TAP_0     = 0x63,  
    INT_TAP_1     = 0x64,  
    INT_ORIENT_0  = 0x65,  
    INT_ORIENT_1  = 0x66,  
    INT_FLAT_0    = 0x67,  
    INT_FLAT_1    = 0x68,  
    FOC_CONF      = 0x69,  
    CONF          = 0x6A,  
    IF_CONF       = 0x6B,  
    PMU_TRIGGER   = 0x6C, 
    SELF_TEST     = 0x6D, 
    NV_CONF       = 0x70, 
    OFFSET_0      = 0x71, 
    OFFSET_1      = 0x72, 
    OFFSET_2      = 0x73, 
    OFFSET_3      = 0x74, 
    OFFSET_4      = 0x75, 
    OFFSET_5      = 0x76, 
    OFFSET_6      = 0x77, 
    STEP_CNT_0    = 0x78, 
    STEP_CNT_1    = 0x79, 
    STEP_CONF_0   = 0x7A, 
    STEP_CONF_1   = 0x7B, 
    CMD           = 0x7E,  
} bmi160_register; 

spi_bus_config_t bus_config = {
    .mosi_io_num = BMI160_MOSI_PIN,
    .miso_io_num = BMI160_MISO_PIN,
    .sclk_io_num = BMI160_SCK_PIN,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 32,
    .flags = SPICOMMON_BUSFLAG_MASTER,
};

spi_device_interface_config_t device_config = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = 0,
    .duty_cycle_pos = 128,
    .cs_ena_pretrans = 0,
    .cs_ena_posttrans = 0,
    .clock_speed_hz = 1000000,
    //.input_delay_ns = 0,
    .spics_io_num = BMI160_CS_PIN,
    .flags = SPI_DEVICE_HALFDUPLEX, 
    .pre_cb = NULL,
    .post_cb = NULL,
    .queue_size = 1,
};

//base function for exchanging data from/to sensor
//accepting data length in BYTES
void bmi160_exchange_data(spi_device_handle_t handler, uint8_t tx_length, uint8_t rx_length, uint8_t *buffer_tx, uint8_t *buffer_rx) {
    spi_transaction_t transaction_config = {
        //.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_RXDATA,
        //.cmd = (0b10000000 | WHO_AM_I),
        //.addr = WHO_AM_I,
        .length = tx_length*8,
        .rxlength = rx_length*8,
        .rx_buffer = buffer_rx,
        .tx_buffer = buffer_tx,
    };
    spi_device_polling_transmit(handler, &transaction_config);
}

void bmi160_read_register(spi_device_handle_t handler, bmi160_register sensor_register, uint8_t rx_length, uint8_t *buffer_rx) {
    uint8_t transmit_buffer = 0b10000000 | sensor_register;
    bmi160_exchange_data(handler, 1, rx_length, &transmit_buffer, buffer_rx);
}

void bmi160_write_register(spi_device_handle_t handler, bmi160_register sensor_register, uint8_t data_length, uint8_t *buffer_tx) {
    uint8_t transmit_buffer[data_length + 1];
    transmit_buffer[0] = 0b00000000 | sensor_register;
    for (int i = 0;i<data_length;i++) {
        transmit_buffer[i+1] = *(buffer_tx + i);
    }
    bmi160_exchange_data(handler, data_length, 0, transmit_buffer, NULL);
}

/*
void bmi160_startup_reset(spi_device_handle_t handler) {
    uint8_t transmit_buffer = 0b10000000;
    bmi160_write_register(handler, PWR_MGMT_1, 1, &transmit_buffer);
    vTaskDelay(100/portTICK_PERIOD_MS);
    transmit_buffer = 0b00000111;
    write_register(handler, SIGNAL_PATH_RESET, 1, &transmit_buffer);
    bmi160_vTaskDelay(100/portTICK_PERIOD_MS);
}

void get_fifo_data(spi_device_handle_t handler, uint8_t *data_buffer) {
    for
    read_register(handler, FIFO_R_W, data_buffer)
}

void get_accelerometer_data() {

}
*/

#endif //BMI160_DRIVER_H