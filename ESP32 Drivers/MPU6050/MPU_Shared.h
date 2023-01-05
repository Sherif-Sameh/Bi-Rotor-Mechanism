/**
 * @file MPU_Shared.h
 * @author Sherif Sameh
 * @brief Shared functions and macros of the MPU6050.
 * This file includes all the function prototypes, macros, and needed header files for the MPU_Shared.c
 * which has the definitions of all the functions that are needed by all the other source files of the 
 * MPU6050 library. It does not need to be included as all other files include it.
 * @version 1.1
 * @date 2022-12-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SHARED_H
#define SHARED_H

#include "driver/i2c.h"                                             /* Driver for the ESP32 I2C peripheral. Part of the ESP32 IDF.*/

/* Define all symbolic constants for the I2C configuaration */
#define I2C_MASTER_SCL_IO               CONFIG_I2C_MASTER_SCL       
#define I2C_MASTER_SDA_IO               CONFIG_I2C_MASTER_SDA      
#define I2C_PORT_NUMBER                 I2C_NUM_0                   
#define I2C_MASTER_FREQ_HZ              CONFIG_I2C_MASTER_FREQ_HZ   
#define I2C_MASTER_TX_BUF_DISABLE       0                           
#define I2C_MASTER_RX_BUF_DISABLE       0                           
#define I2C_MASTER_TIMEOUT_MS           20                          
#define I2C_MASTER_ACK_EN               true                        

/**
 * @def I2C_MASTER_SCL_IO
 * The GPIO pin assigned on the ESP32 to SCL (I2C master clock line). Set from inside the menuconfig.
 */

/**
 * @def I2C_MASTER_SDA_IO
 * The GPIO pin assigned on the ESP32 to SDA (I2C master data line). Set from inside the menuconfig.
 */

/**
 * @def I2C_PORT_NUMBER
 * Defines which I2C port is used on the ESP32 of the available two ports number 0 and 1.
 */

/**
 * @def I2C_MASTER_FREQ_HZ
 * The clock frequency used for SCL in Hz. Set from inside the menuconfig.
 */

/**
 * @def I2C_MASTER_TX_BUF_DISABLE
 * A macro used to define the software buffer allocated for transmitting data in I2C slave mode, hence equals 0.
 */

/**
 * @def I2C_MASTER_RX_BUF_DISABLE
 * A macro used to define the software buffer allocated for receiving data in I2C slave mode, hence equals 0.
 */

/**
 * @def I2C_MASTER_TIMEOUT_MS
 * The max amount of time, in ms, that the master will wait for a response from the slave before declaring a timeout.
 */

/**
 * @def I2C_MASTER_ACK_EN
 * A macro which defines whether the I2C master should expect an acknowledge from the slave or not.
 */

/* Symbolic Constants for the MPU6050 */
#define MPU6050_ADDRESS 0x34
/**
 * @def MPU6050_ADDRESS
 * The 6 bit I2C address of the MPU6050 which excludes AD0.
 */


/* Function Declerations */

/**
 * This function sets the value used for the AD0 bit in the MPU6050's I2C 7 bit address and
 * its value should match that of the pin i.e. 1 (High) or  0 (Low).
 * @param pinValue The value of the AD0 pin on the MPU6050 IC. Note: all values other than 0 will be taken as 1.
 */
void setAD0(uint8_t pinValue);

/**
 * @brief This function writes a byte into a register on the MPU6050.
 * @param regAddr The address of the register in the MPU6050's register map.
 * @param data The value that will be written to that register.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 * @attention This function is used internally by the driver and not for applications to use.
 */
esp_err_t writeToReg(uint8_t regAddr, uint8_t data);

/**
 * @brief This function will read from a register/s on the MPU6050.
 * @param regAddr The address of the first register to read from in the MPU6050's register map.
 * @param data A pointer to the start of an array of bytes that the read data will be written to.
 * @param n The number of registers to read from.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 * @attention This function is used internally by the driver and not for applications to use.
 */
esp_err_t readFromReg(uint8_t regAddr, uint8_t *data, uint8_t n);

/** 
 * @brief Reads the value of the interrupt status register.
 * This function reads the status of all 3 possible interrupt sources (FIFO OVF, I2C Master INT, Data RDY INT)
 * on the MPU6050 to determine the source which triggered the interrupt.
 * @param value pointer to char that will hold the read value.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t readInterruptStatus(uint8_t *value);

/** 
 * @brief function disables the all interrupts on the MPU6050.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t disableInterrupts();

#endif