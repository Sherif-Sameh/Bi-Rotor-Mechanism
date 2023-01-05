/**
 * @file MPU_Basic.h
 * @author Sherif Sameh
 * @brief For basic operation of the MPU6050.
 * This file includes all the prototypes for the basic functions of the MPU6050.
 * @version 1.2
 * @date 2022-12-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#ifndef BASIC_H
#define BASIC_H

#include "MPU_Shared.h"

/* Sumbolic Constants */
#define ACCELEROMETER_X_BIAS_FACTOR                 ((float) 259.5)
#define ACCELEROMETER_Y_BIAS_FACTOR                 ((float) 86.2)
#define ACCELEROMETER_Z_BIAS_FACTOR                 ((float) -1825.2)
#define ACCELEROMETER_X_SF_CORRECTION_FACTOR        ((float) 0.9493)
#define ACCELEROMETER_Y_SF_CORRECTION_FACTOR        ((float) 1.0307)
#define ACCELEROMETER_Z_SF_CORRECTION_FACTOR        ((float) 0.9842)
#define GYRO_X_BIAS_FACTOR                          ((float) -306.7)
#define GYRO_Y_BIAS_FACTOR                          ((float) 65.43)
#define GYRO_Z_BIAS_FACTOR                          ((float) -15.2)

/**
 * @def ACCELEROMETER_X_BIAS_FACTOR
 * The bias factor used in correcting the X-axis accelerometer's reading against constant bias errors.
 */

/**
 * @def ACCELEROMETER_Y_BIAS_FACTOR
 * The bias factor used in correcting the Y-axis accelerometer's reading against constant bias errors.
 */

/**
 * @def ACCELEROMETER_Z_BIAS_FACTOR
 * The bias factor used in correcting the Z-axis accelerometer's reading against constant bias errors.
 */

/**
 * @def ACCELEROMETER_X_SF_CORRECTION_FACTOR
 * The factor used in correcting the X-axis accelerometer's readings against scale factor errors.
 */

/**
 * @def ACCELEROMETER_Y_SF_CORRECTION_FACTOR
 * The factor used in correcting the Y-axis accelerometer's readings against scale factor errors.
 */

/**
 * @def ACCELEROMETER_Z_SF_CORRECTION_FACTOR
 * The factor used in correcting the Z-axis accelerometer's readings against scale factor errors.
 */

/**
 * @def GYRO_X_BIAS_FACTOR 
 * The bias factor used in correcting the X-axis gyroscope's readings against constant bias errors.
 */

/**
 * @def GYRO_Y_BIAS_FACTOR 
 * The bias factor used in correcting the Y-axis gyroscope's readings against constant bias errors.
 */

/**
 * @def GYRO_Z_BIAS_FACTOR 
 * The bias factor used in correcting the Z-axis gyroscope's readings against constant bias errors.
 */


/* Function-like Macros */
#define ACCELEROMETER_APPLY_CALIBRATION(RAW,BIAS_FACTOR,CORRECTION_FACTOR) \
                        (CORRECTION_FACTOR * ((float) RAW - BIAS_FACTOR))
#define GYRO_APPLY_CALIBRATION(RAW,BIAS_FACTOR) \
                        ((float) RAW - BIAS_FACTOR)
#define ACCELEROMETER_CONVERT_SF_SI(SF)             (SF / 9.81)
#define GYRO_CONVERT_SF_SI(SF)                      ((SF * 180) / 3.1416)

/**
 * @def ACCELEROMETER_APPLY_CALIBRATION
 * This macro is used to apply the neccesary calibration to the accelerometer's raw readings.
 * @param RAW The sensor's raw readings.
 * @param BIAS_FACTOR The sensor's constant bias correction factor.
 * @param CORRECTION_FACTOR The sensor's sensitivity scale factor correction factor.
 */

/**
 * @def GYRO_APPLY_CALIBRATION
 * This macro is used to apply the neccesary calibration to the gryoscope's raw readings.
 * @param RAW The sensor's raw readings.
 * @param BIAS_FACTOR The sensor's constant bias correction factor.
 */

/**
 * @def ACCELEROMETER_CONVERT_SF_SI
 * This macro is used to convert the sensor's scale factor from LSB/g to LSB/(m/s^2).
 * @param SF The sensor's scale factor in LSB/g.
 */

/**
 * @def GYRO_CONVERT_SF_SI
 * This macro is used to convert the sensor's scale factor from LSB/dps to LSB/(rad/s).
 * @param SF The sensor's scale factor in LSB/dps.
 */


/* Enum Definitions */

/**
 * @enum acc_FS_range
 * This enum is used to select the full scale range of the accelerometers.
 */
typedef enum {
    ACCELEROMETER_FS_RANGE_2G,      /**< FS Range = +-2g */
    ACCELEROMETER_FS_RANGE_4G,      /**< FS Range = +-4g */
    ACCELEROMETER_FS_RANGE_8G,      /**< FS Range = +-8g */
    ACCELEROMETER_FS_RANGE_16G      /**< FS Range = +-16g */
} acc_FS_range;

/**
 * @enum gyro_FS_range
 * This enum is used to select the full scale range of the gyroscopes.
 */
typedef enum {
    GYRO_FS_RANGE_250DPS,           /**< FS Range = +-250dps */
    GYRO_FS_RANGE_500DPS,           /**< FS Range = +-500dps */
    GYRO_FS_RANGE_1000DPS,          /**< FS Range = +-1000dps */
    GYRO_FS_RANGE_2000DPS           /**< FS Range = +-2000dps */
} gyro_FS_range;

/**
 * @brief This function intializes the IMU by setting the value of the AD0 bit in its address,
 * Initializing the I2C port chosen, and then waking up the IMU from sleep mode. 
 * 
 * @param AD0 This variable reflects the state of the AD0 pin on the MPU6050 where 1 means its
 * connected to VCC or 0 when its connected to GND.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t mpuInit(uint8_t AD0);

/**
 * @brief Wake up the IMU from sleep mode.
 * This function writes to MPU6050's power management register in order to get the device out of
 * its default state at startup which is sleep mode.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t wakeUp();

/**
 * This function will reset all the MPU6050's registers to their default values.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t deviceReset();

/**
 * This function sets full scale range for the 3 accelerometers. Obviously as the range goes up, the sensor's resolution
 * becomes worse.
 * @param FS_SELECT This parameter chooses the FS range for the accelerometers.
 * @see acc_FS_range
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t setAccFS_Range(acc_FS_range FS_SELECT);

/**
 * This function sets full scale range for the 3 gyroscopes. Obviously as the range goes up, the sensor's resolution
 * becomes worse.
 * @param FS_SELECT This parameter chooses the FS range for the gyroscopes.
 * @see gyro_FS_range
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t setGyroFS_Range(gyro_FS_range FS_SELECT);

/**
 * @brief This function retrieves the raw accelerometer readings from the MPU6050.
 * @param data A pointer to the start of a signed int16_t array that will hold the values read from the IMU.
 * @param skipFromBeginning This parameter can be used to retrieve only specific accelerometer readings and not all 3.
 * @param skipFromEnd This parameter can be used to retrieve only specific accelerometer readings and not all 3.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t getAccReadings(int16_t *data, uint8_t skipFromBeginning, uint8_t skipFromEnd);

/**
 * @brief This function will retrieve the raw accelerometer readings and appy the neccessary
 * calibration model to them to correct them.
 * @param data A pointer to the start of an array of floats that will store all 3 readings of the 3
 * accelerometers.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t getAccReadingsCalibrated(float *data);

/**
 * @brief This function will retrieve the raw readings, apply the calibration model to them and
 * finally scale them into the default units of the MPU6050 which are g's.
 * @param data A pointer to the start of an array of floats that will store all 3 readings of the 3
 * accelerometers.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t getAccReadingsScaled(float *data);

/**
 * @brief This function will retrieve the raw readings, apply calibration modelto them and
 * finally scale them into SI units of acceleration which are m/s^2.
 * @param data A pointer to the start of an array of floats that will store all 3 readings of the 3
 * accelerometers.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t getAccReadingsScaledSI(float *data);
 
/**
 * @brief This function retrieves the raw gyroscope readings from the MPU6050.
 * @param data A pointer to the start of a signed int16_t array that will hold the values read from the IMU.
 * @param skipFromBeginning This parameter can be used to retrieve only specific gyroscope readings and not all 3.
 * @param skipFromEnd This parameter can be used to retrieve only specific gyroscope readings and not all 3.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t getGyroReadings(int16_t *data, uint8_t skipFromBeginning, uint8_t skipFromEnd);

/**
 * @brief This function will retrieve the raw gyroscope readings then apply the gyro's calibration
 * model to them in order to correct them.
 * @param data A pointer to the start of an array of floats that will store all 3 readings of the 3
 * gyroscopes.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t getGyroReadingsCalibrated(float *data);

/**
 * @brief This function will retrieve the raw readings, apply the calibration model to them and
 * finally scale them into the default units of the MPU6050 which are degrees per second (DPS).
 * @param data A pointer to the start of an array of floats that will store all 3 readings of the 3
 * gyroscopes.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t getGyroReadingsScaled(float *data);

/**
 * @brief This function will retrieve the raw readings, apply the calibration model to them and
 * finally scale them into the SI units of angular rate/velocity which are radians per second (rad/s).
 * @param data A pointer to the start of an array of floats that will store all 3 readings of the 3
 * gyroscopes.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t getGyroReadingsScaledSI(float *data);

/**
 * This function retrieves the raw temperature sensor reading from the MPU6050.
 * @param data A pointer to a signed int16_t that will hold the value read from the IMU.
 * @return esp_err_t Will return ESP_OK upon successful completion or an error otherwise.
 */
esp_err_t getTempReadings(int16_t *data);
 
#endif