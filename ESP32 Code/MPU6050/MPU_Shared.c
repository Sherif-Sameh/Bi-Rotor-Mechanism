// This file include all the definitions of functions that are shared by all the other MPU6060 specific files

#include "MPU_Shared.h"

uint8_t AD0 = 0;

void setAD0(uint8_t pinValue)
{
  if(pinValue == 0)
  {
    AD0 = 0;
  }
  else
  {
    AD0 = 1;
  }
}

esp_err_t writeToReg(uint8_t regAddr, uint8_t data)
{
  const uint8_t writeBuf[2] = {regAddr, data};
  esp_err_t status = i2c_master_write_to_device(I2C_PORT_NUMBER, ((MPU6050_ADDRESS << 1) | AD0),
                    writeBuf, 2, I2C_MASTER_TIMEOUT_MS);
  return status;
}

esp_err_t readFromReg(uint8_t regAddr, uint8_t *data, uint8_t n)
{
  esp_err_t status = i2c_master_write_read_device(I2C_PORT_NUMBER,((MPU6050_ADDRESS << 1) | AD0),
                    (const uint8_t *) &regAddr, 1, data, n, I2C_MASTER_TIMEOUT_MS);
  return status;
}

esp_err_t readInterruptStatus(uint8_t *value)
{
  return readFromReg(0x3A, value, 1);
}

esp_err_t disableInterrupts()
{
  return writeToReg(0x38,0x00);
}
