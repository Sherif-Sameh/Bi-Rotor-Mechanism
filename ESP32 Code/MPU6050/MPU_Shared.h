/** @file MPU_Shared.h
 * @brief Shared functions and macros. 
 * This file includes all the function prototypes, macros, and needed header files for the MPU_Shared.c
 * which has the definitions of functions used by all the other source files of the MPU6050 library.
 * It does not need to be included as all other files include it.
 */
 

#ifndef SHARED_H
#define SHARED_H

/* Define all symbolic constants for the I2C configuaration */
#define I2C_MASTER_SCL_IO               CONFIG_I2C_MASTER_SCL       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO               CONFIG_I2C_MASTER_SDA       /*!< GPIO number used for I2C master data  */
#define I2C_PORT_NUMBER                 I2C_NUM_0                   /*!< I2C Port 0 is always used by this driver */
#define I2C_MASTER_FREQ_HZ              CONFIG_I2C_MASTER_FREQ_HZ   /*!< I2C Clock frequency in Hz for master mode */
#define I2C_MASTER_TX_BUF_DISABLE       0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE       0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS           20                        /*!< Timeout for waiting for slave to reply */
#define I2C_MASTER_ACK_EN               true                        /*!< Macro for Master to expect ACK from slave */

#include "driver/i2c.h" /* Driver for the ESP32 I2C peripheral. */
#define MPU6050_ADDRESS 0x34 /**< The 6 bit I2C address of the MPU6050 which excludes AD0. */

/**
 * This function sets the value used for the AD0 bit in the MPU6050's I2C 7 bit address and
 * its value should match that of the pin i.e. 1 (High) or  0 (Low).
 * @param pinValue The value of the AD0 pin on the MPU6050 IC. Note: all values other than 0 will be taken as 1.
 */
void setAD0(uint8_t pinValue);

/**
 * This function writes a byte into a register on the MPU6050.
 * @param regAddr The address of the register in the MPU6050's register map.
 * @param data The value that will be written to that register.
 * @return Will return 1 upon successful completion or 0 otherwise.
 */
esp_err_t writeToReg(uint8_t regAddr, uint8_t data);

/**
 * This function will read from a register/s on the MPU6050.
 * @param regAddr The address of the first register to read from in the MPU6050's register map.
 * @param data A pointer to the start of an array of bytes that the read data will be written to.
 * @param n The number of registers to read from.
 * @return Will return 1 upon successful completion or 0 otherwise.
 */
esp_err_t readFromReg(uint8_t regAddr, uint8_t *data, uint8_t n);

/** 
 * This function reads the status of all 3 possible interrupt sources (FIFO OVF, I2C Master INT, Data RDY INT)
 * on the MPU6050 to determine the source which triggered the interrupt.
 * @param value pointer to char that will hold the read value.
 * @return Will return 1 upon successful completion or 0 otherwise.
 */
esp_err_t readInterruptStatus(uint8_t *value);

/** 
 * This function disables the all interrupts on the MPU6050.
 * @return Will return 1 upon successful completion or 0 otherwise.
 */
esp_err_t disableInterrupts();

#endif