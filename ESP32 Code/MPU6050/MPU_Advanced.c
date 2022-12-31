// This file includes all the function definitions of the functions that would be needed by a more advanced user for the MPU6050

#include "MPU_Advanced.h"

esp_err_t setClockSource(enum clockSource CS)
{
  uint8_t data;
  switch(CS)
  {
    case 1: data = 0x01; break;
    case 2: data = 0x02; break;
    case 3: data = 0x03; break;
    default: data = 0x00;
  }
  return writeToReg(0x6B, data);
}

esp_err_t resetAccSignalPath()
{
  return writeToReg(0x68, 0x02);
}

esp_err_t resetGyroSignalPath()
{
  return writeToReg(0x68, 0x04);
}

esp_err_t resetTempSignalPath()
{
	return writeToReg(0x68, 0x01);
}

esp_err_t dataRDY_IE()
{
  uint8_t current;
  esp_err_t status = readFromReg(0x38, &current,1);
  current |= (1<<0);
  return (status == ESP_OK) ? writeToReg(0x38, current) : status;
}

esp_err_t setDLPF(uint8_t DLPF_CFG)
{
  if(DLPF_CFG>6)
  {
    DLPF_CFG = 0;
  }
  return writeToReg(0x1A, DLPF_CFG);
}

esp_err_t setSampleRate(uint16_t sampleRate)
{
  uint8_t DLPF;
  int8_t sampleRateDivider;
  esp_err_t status = readFromReg(0x1A, &DLPF, 1);
  if(DLPF == 0)
  {
    sampleRateDivider = (8000/sampleRate) - 1;
  }
  else
  {
    sampleRateDivider = (1000/sampleRate) - 1;
  }
  if(sampleRateDivider<0)
  {
    sampleRateDivider = 0;
  }
  return (status == ESP_OK) ? writeToReg(0x19, (uint8_t)sampleRateDivider) : status;
}
