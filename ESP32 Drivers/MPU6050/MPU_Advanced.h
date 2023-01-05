/**
 * @file MPU_Advanced.h
 * @author Sherif Sameh
 * @brief For changing the device's configuration.
 * This file includes all the function prototypes and macros for changing the configuration
 * of the MPU6050 such as clock source, sample rate, enable data ready interrupt, etc....
 * @version 1.1
 * @date 2022-12-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ADVANCED_H
#define ADVANCED_H

#include "MPU_Shared.h"

/** An enum for choosing the clock source.
 *  Chooses between internal oscillator or one of the gyros.
 */
enum clockSource 
{
	INTOSC, 	/**< Internal oscillator */
	GYRO_X,	 	/**< X-axis gyroscope */
	GYRO_Y, 	/**< Y-axis gyroscope */
	GYRO_Z 		/**< Z-axis gyroscope */
};

/**
 * @brief This function is used to set a clock source, by default the internal oscillator is used.
 * @param CS The enum of type clockSource used to set the clock source of the MPU6050.
 * @attention It is recommended by the manufacturer to use one of the gyros for improved clock stability.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t setClockSource(enum clockSource CS);

/**
 * @brief This function will revert the ADC and DLPF settings for the accelerometers to their default values.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t resetAccSignalPath();

/**
 * @brief This function will revert the ADC and DLPF settings for the gyroscopes to their default values.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t resetGyroSignalPath();

/**
 * @brief function will revert the ADC and DLPF settings for the temperature sensor to their default values.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t resetTempSignalPath();

/**
 * @brief Enable data_ready interrupt.
 * This function will enable the data ready interrupt which is set everytime new readings are written to the
 * sensor registers after passing by the ADC and the DLPF.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t dataRDY_IE();

/**
 * This function is used to configure the digital low-pass filter (DLPF) for both the gyroscope and
 * accelerometer readings.
 * @param DLPF_CFG This is the value written to the register and ranges from 0 to 6, the exact bandwidth
 * and delay associated with each value can found in page 13 of the MPU6050's register map document.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t setDLPF(uint8_t DLPF_CFG);

/**
 * This function sets the sample rate divider of the MPU6050 according to the desired sample rate
 * and the current DLPF settings.
 * @param sampleRate The desired sampling rate.
 * @attention It's advised to recall this function everytime setDLPF(char DLPF_CFG) is called
 * because that could indirectly change the sampling rate.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t setSampleRate(uint16_t sampleRate);

#endif