// This file inlcudes all the needed function definitions for the MPU_Basic.h header file

#include "MPU_Basic.h"

float const accScaleFactors[4]  = {16384, 8192, 4096, 2048};     /*!< Scale Factors in LSB/g */
float const gyroScaleFactors[4]    = {131, 65.5, 32.8, 16.4};       /*!< Scale Factors in LSB/(dps) */

acc_FS_range accCurrentRange      = ACCELEROMETER_FS_RANGE_2G;
float accCurrentScaleFactor       = accScaleFactors[0];
gyro_FS_range gyroCurrentRange    = GYRO_FS_RANGE_250DPS;
float gyroCurrentScaleFactor      = gyroScaleFactors[0];

#ifdef CONFIG_I2C_MASTER_PULLUP_EN

esp_err_t mpuInit(uint8_t AD0)
{
  /* I2C Configuration */
  i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(I2C_PORT_NUMBER, &conf);
  esp_err_t status = i2c_driver_install(I2C_PORT_NUMBER, conf.mode,
                    I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
  
  /* Setting the MPU6050's address and waking it up from sleep mode */
  setAD0(AD0);
  return (status == ESP_OK) ? wakeUp() : status;
}

#else

esp_err_t mpuInit(uint8_t AD0)
{
  /* I2C Configuration */
  i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(I2C_PORT_NUMBER, &conf);
  esp_err_t status = i2c_driver_install(I2C_PORT_NUMBER, conf.mode,
                    I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
  
  /* Setting the MPU6050's address and waking it up from sleep mode */
  setAD0(AD0);
  return (status == ESP_OK) ? wakeUp() : status;
}

#endif

esp_err_t wakeUp()
{
  return writeToReg(0x6B, 0x00);
}

esp_err_t deviceReset()
{ 
  return writeToReg(0x6B, 0x80);
}

esp_err_t setAccFS_Range(acc_FS_range FS_SELECT)
{
  accCurrentRange = FS_SELECT;
  accCurrentScaleFactor = accScaleFactors[FS_SELECT];
  uint8_t current;
  esp_err_t status = readFromReg(0x1C, &current, 1);
  current &= ~((1<<3) | (1<<4));
  switch(FS_SELECT)
  {
    case ACCELEROMETER_FS_RANGE_2G: current = current; break;
    case ACCELEROMETER_FS_RANGE_4G: current |= (1<<3); break;
    case ACCELEROMETER_FS_RANGE_8G: current |= (1<<4); break;
    case ACCELEROMETER_FS_RANGE_16G: current |= ((1<<3) | (1<<4)); break;
  }
  return (status == ESP_OK) ? writeToReg(0x1C, current) : status;
}

esp_err_t setGyroFS_Range(gyro_FS_range FS_SELECT)
{
  gyroCurrentRange = FS_SELECT;
  gyroCurrentScaleFactor = gyroScaleFactors[FS_SELECT];
  uint8_t current;
  esp_err_t status = readFromReg(0x1B, &current, 1);
  current &= ~((1<<3) | (1<<4));
  switch(FS_SELECT)
  {
    case GYRO_FS_RANGE_250DPS: current = current; break;
    case GYRO_FS_RANGE_500DPS: current |= (1<<3); break;
    case GYRO_FS_RANGE_1000DPS: current |= (1<<4); break;
    case GYRO_FS_RANGE_2000DPS: current |= ((1<<3) | (1<<4)); break;
  }
  return (status == ESP_OK) ? writeToReg(0x1B, current) : status;
}

esp_err_t getAccReadings(int16_t *data, uint8_t skipFromBeginning, uint8_t skipFromEnd)
{
  int8_t c[6-2*(skipFromBeginning+skipFromEnd)];
  esp_err_t status = readFromReg((0x3B + 2*skipFromBeginning), (uint8_t *)c, 6-2*(skipFromBeginning+skipFromEnd));
  for( uint8_t i = 0; i < (6-2*(skipFromBeginning + skipFromEnd)); i+=2)
  {
    data[i/2] = ((c[i]<<8) | ((uint8_t) c[i+1]));
  }
  return status;
}

esp_err_t getAccReadingsCalibrated(float *data)
{
  int16_t rawData[3];
  esp_err_t status = getAccReadings(rawData, 0, 0);
  if( status == ESP_OK)
  {
    data[0] = ACCELEROMETER_APPLY_CALIBRATION(rawData[0], ACCELEROMETER_X_BIAS_FACTOR, ACCELEROMETER_X_SF_CORRECTION_FACTOR);
    data[1] = ACCELEROMETER_APPLY_CALIBRATION(rawData[1], ACCELEROMETER_Y_BIAS_FACTOR, ACCELEROMETER_Y_SF_CORRECTION_FACTOR);
    data[2] = ACCELEROMETER_APPLY_CALIBRATION(rawData[2], ACCELEROMETER_Z_BIAS_FACTOR, ACCELEROMETER_Z_SF_CORRECTION_FACTOR);
  }
  return status;
}

/*!< This function assumes the scaled data should be calibrated first */
esp_err_t getAccReadingsScaled(float *data)
{
  esp_err_t status = getAccReadingsCalibrated(data);
  if(status == ESP_OK)
  {
    for( uint8_t i = 0; i < 3; i++)
    {
      data[i] = data[i] / accCurrentScaleFactor;
    }
  }
  return status;
}

esp_err_t getAccReadingsScaledSI(float *data)
{
  esp_err_t status = getAccReadingsCalibrated(data);
  if(status == ESP_OK)
  {
    for( uint8_t i = 0; i < 3; i++)
    {
      data[i] = data[i] / ACCELEROMETER_CONVERT_SF_SI(accCurrentScaleFactor);
    }
  }
  return status;
}

esp_err_t getGyroReadings(int16_t *data, uint8_t skipFromBeginning, uint8_t skipFromEnd)
{
  int8_t c[6-2*(skipFromBeginning+skipFromEnd)];
  esp_err_t status = readFromReg((0x43 + 2*skipFromBeginning), (uint8_t *)c, 6-2*(skipFromBeginning+skipFromEnd));
  for( uint8_t i = 0; i < (6-2*(skipFromBeginning + skipFromEnd)); i+=2)
  {
    data[i/2] = ((c[i]<<8) | ((uint8_t) c[i+1]));
  }
  return status;
}

esp_err_t getGyroReadingsCalibrated(float *data)
{
  int16_t rawData[3];
  esp_err_t status = getGyroReadings(rawData, 0, 0);
  if( status == ESP_OK)
  {
    data[0] = GYRO_APPLY_CALIBRATION(rawData[0], GYRO_X_BIAS_FACTOR);
    data[1] = GYRO_APPLY_CALIBRATION(rawData[1], GYRO_Y_BIAS_FACTOR);
    data[2] = GYRO_APPLY_CALIBRATION(rawData[2], GYRO_Z_BIAS_FACTOR);
  }
  return status;
}

esp_err_t getGyroReadingsScaled(float *data)
{
  esp_err_t status = getGyroReadingsCalibrated(data);
  if(status == ESP_OK)
  {
    for( uint8_t i = 0; i < 3; i++)
    {
      data[i] = data[i] / gyroCurrentScaleFactor;
    }
  }
  return status;
}

esp_err_t getGyroReadingsScaledSI(float *data)
{
  esp_err_t status = getGyroReadingsCalibrated(data);
  if(status == ESP_OK)
  {
    for( uint8_t i = 0; i < 3; i++)
    {
      data[i] = data[i] / GYRO_CONVERT_SF_SI(gyroCurrentScaleFactor);
    }
  }
  return status;
}

esp_err_t getTempReadings(int16_t *data)
{
	int8_t c[2];
	esp_err_t status = readFromReg(0x41, (uint8_t *)c, 2);
	*data = ((c[0]<<8) | ((uint8_t) c[1]));
	return status;
}