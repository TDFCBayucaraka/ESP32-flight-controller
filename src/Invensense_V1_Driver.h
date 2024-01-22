#ifndef INVENSENSE_V1_DRIVER_H
#define INVENSENSE_V1_DRIVER_H
#include "driver/spi_master.h"

enum invensense_v1_register {
    SELF_TEST_X_GYRO   = 0,
    SELF_TEST_Y_GYRO   = 1,
    SELF_TEST_Z_GYRO   = 2,
    SELF_TEST_X_ACCEL  = 13,
    SELF_TEST_Y_ACCEL  = 14,
    SELF_TEST_Z_ACCEL  = 15,
    XG_OFFSET_H        = 19,
    XG_OFFSET_L        = 20,
    YG_OFFSET_H        = 21,
    YG_OFFSET_L        = 22,
    ZG_OFFSET_H        = 23,
    ZG_OFFSET_L        = 24,
    SMPLRT_DIV         = 25,
    CONFIG             = 26,
    GYRO_CONFIG        = 27,
    ACCEL_CONFIG       = 28,
    ACCEL_CONFIG_2     = 29,
    LP_ACCEL_ODR       = 30,
    WOM_THR            = 31,
    FIFO_EN            = 35,
    I2C_MST_CTRL       = 36,
    I2C_SLV0_ADDR      = 37,
    I2C_SLV0_REG       = 38,
    I2C_SLV0_CTRL      = 39,
    I2C_SLV1_ADDR      = 40,
    I2C_SLV1_REG       = 41,
    I2C_SLV1_CTRL      = 42,
    I2C_SLV2_ADDR      = 43,
    I2C_SLV2_REG       = 44,
    I2C_SLV2_CTRL      = 45,
    I2C_SLV3_ADDR      = 46,
    I2C_SLV3_REG       = 47,
    I2C_SLV3_CTRL      = 48,
    I2C_SLV4_ADDR      = 49,
    I2C_SLV4_REG       = 50,
    I2C_SLV4_DO        = 51,
    I2C_SLV4_CTRL      = 52,
    I2C_SLV4_DI        = 53,
    I2C_MST_STATUS     = 54,
    INT_PIN_CFG        = 55,
    INT_ENABLE         = 56,
    INT_STATUS         = 58,
    ACCEL_XOUT_H       = 59,
    ACCEL_XOUT_L       = 60,
    ACCEL_YOUT_H       = 61,
    ACCEL_YOUT_L       = 62,
    ACCEL_ZOUT_H       = 63,
    ACCEL_ZOUT_L       = 64,
    TEMP_OUT_H         = 65,
    TEMP_OUT_L         = 66,
    GYRO_XOUT_H        = 67,
    GYRO_XOUT_L        = 68,
    GYRO_YOUT_H        = 69,
    GYRO_YOUT_L        = 70,
    GYRO_ZOUT_H        = 71,
    GYRO_ZOUT_L        = 72,
    EXT_SENS_DATA_00   = 73,
    EXT_SENS_DATA_01   = 74,
    EXT_SENS_DATA_02   = 75,
    EXT_SENS_DATA_03   = 76,
    EXT_SENS_DATA_04   = 77,
    EXT_SENS_DATA_05   = 78,
    EXT_SENS_DATA_06   = 79,
    EXT_SENS_DATA_07   = 80,
    EXT_SENS_DATA_08   = 81,
    EXT_SENS_DATA_09   = 82,
    EXT_SENS_DATA_10   = 83,
    EXT_SENS_DATA_11   = 84,
    EXT_SENS_DATA_12   = 85,
    EXT_SENS_DATA_13   = 86,
    EXT_SENS_DATA_14   = 87,
    EXT_SENS_DATA_15   = 88,
    EXT_SENS_DATA_16   = 89,
    EXT_SENS_DATA_17   = 90,
    EXT_SENS_DATA_18   = 91,
    EXT_SENS_DATA_19   = 92,
    EXT_SENS_DATA_20   = 93,
    EXT_SENS_DATA_21   = 94,
    EXT_SENS_DATA_22   = 95,
    EXT_SENS_DATA_23   = 96,
    I2C_SLV0_DO        = 99,
    I2C_SLV1_DO        = 100,
    I2C_SLV2_DO        = 101,
    I2C_SLV3_DO        = 102,
    I2C_MST_DELAY_CTRL = 103,
    SIGNAL_PATH_RESET  = 104,
    ACCEL_INTEL_CTRL   = 105,
    USER_CTRL          = 106,
    PWR_MGMT_1         = 107,
    PWR_MGMT_2         = 108,
    FIFO_COUNT_H       = 114,
    FIFO_COUNT_L       = 115,
    FIFO_R_W           = 116,
    WHO_AM_I           = 117,
    XA_OFFSET_H        = 119,
    XA_OFFSET_L        = 120,
    YA_OFFSET_H        = 122,
    YA_OFFSET_L        = 123,
    ZA_OFFSET_H        = 125,
    ZA_OFFSET_L        = 126,
}; 

spi_bus_config_t bus_config = {
    .mosi_io_num = MPU6500_MOSI_PIN,
    .miso_io_num = MPU6500_MISO_PIN,
    .sclk_io_num = MPU6500_SCK_PIN,
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
    .spics_io_num = MPU6500_CS_PIN,
    .flags = SPI_DEVICE_HALFDUPLEX, 
    .pre_cb = NULL,
    .post_cb = NULL,
    .queue_size = 1,
};

//base function for exchanging data from/to sensor
//accepting data length in BYTES
void exchange_data(spi_device_handle_t handler, uint8_t tx_length, uint8_t rx_length, uint8_t *buffer_tx, uint8_t *buffer_rx) {
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

void read_register(spi_device_handle_t handler, uint8_t sensor_register, uint8_t rx_length, uint8_t *buffer_rx) {
    uint8_t transmit_buffer = 0b10000000 | sensor_register;
    exchange_data(handler, 1, rx_length, &transmit_buffer, buffer_rx);
}

void write_register(spi_device_handle_t handler, uint8_t sensor_register, uint8_t data_length, uint8_t *buffer_tx) {
    uint8_t transmit_buffer[data_length + 1];
    transmit_buffer[0] = 0b00000000 | sensor_register;
    for (int i = 0;i<data_length;i++) {
        transmit_buffer[i+1] = *(buffer_tx + i);
    }
    exchange_data(handler, data_length, 0, transmit_buffer, NULL);
}

void startup_reset(spi_device_handle_t handler) {
    uint8_t transmit_buffer = 0b10000000;
    write_register(handler, PWR_MGMT_1, 1, &transmit_buffer);
    vTaskDelay(100/portTICK_PERIOD_MS);
    transmit_buffer = 0b00000111;
    write_register(handler, SIGNAL_PATH_RESET, 1, &transmit_buffer);
    vTaskDelay(100/portTICK_PERIOD_MS);
} 
/*
void get_fifo_data(spi_device_handle_t handler, uint8_t *data_buffer) {
    for
    read_register(handler, FIFO_R_W, data_buffer)
}

void get_accelerometer_data() {

}
*/

#endif //INVENSENSE_V1_DRIVER