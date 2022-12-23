/* 
 Author's Name: Eng. Sherif Sameh
 
 This sketch, MPU6050_Validation, was created to conduct the needed tests on the MPU6050
 IMU using its embedded self test features to ensure that the device is still capable of
 delivering accurate results. It relies on a dedicated library, which I created, that acts
 as the driver for all communications between the MCU and the IMU.
    
*/

#include "MPU_Basic.h"
#include "MPU_SelfTest.h"

int16_t gyroReadings[3] = {0,0,0}; 
int16_t accReadings[3] = {0,0,0}; 
int16_t gyroFactoryTrim[3] = {0,0,0};
int16_t accFactoryTrim[3] = {0,0,0}; // X then Y then Z
const uint16_t noOfSamples = 1000; // Total number of samples to be taken for each state
uint16_t samplesTaken = 0; // Current number of samples taken
unsigned long t0 = 0; // Variable for monitoring the sampling rate using micros()
uint8_t state = 0; // 0 = wait; 1 = Take measurements with Self-Test disabled; 2 = Take measurements with Self-Test enabled; 3 = Send Factory Trim Data

void setup()
{
  Serial.begin(115200);
  
  // TWI setup: SCL Freq, Device Address, and General call recognition.
  enableTWI();
  setDeviceAddress(50,0); // Address value insignificant as it's only a 2 device comm.
  setSCLFrequency(18); // SCL Freq = 100Khz
  DDRC &= ~((1<<4) | (1<<5));
  PORTC |= ((1<<4) | (1<<5)); // Enable Internal Pull-up resistors for SDA and SCL 

  // MPU6050 setup:
  setAD0(0);
  deviceReset(); // To ensure that any old configurations are erased
  delay(50);
  wakeUp(); // Wake up device from sleep mode and wait for some time
  delay(100);
  setGyroFS_Range(0); // +-250dps
  setAccFS_Range(2); //  +-8g
  
  // Retrieve Manufacturer's Factory Trim data from device's internal registers
  // Note: This is a constant value for each included gyroscope and accelerometer that does not need multiple measurements
  getFactoryTrimGyro(gyroFactoryTrim);
  getFactoryTrimAcc(accFactoryTrim);
}

void loop()
{
  if(Serial.available() > 0)
  {
    state = updateState((uint8_t)Serial.read());
  }
  
  if((state == 1) || (state == 2))
  {
    while(samplesTaken < noOfSamples)
    {
      t0 = micros();
      getGyroReadings(gyroReadings,0,0);
      getAccReadings(accReadings,0,0);
      for(uint8_t i = 0; i < 3; i++)
      {
        writeInt(gyroReadings[i]);
        writeInt(accReadings[i]); 
      }
      samplesTaken++;
      while((micros() - t0) < 10000); // Sampling Rate = 100Hz
    }
    state = 0;
  }
  
  if(state == 3)
  {
     for(uint8_t i = 0; i < 3; i++)
    {
      writeInt(gyroFactoryTrim[i]);
      writeInt(accFactoryTrim[i]);
    }
    state = 0;
  }
}

uint8_t updateState (uint8_t s)
{
  samplesTaken = 0;
  if(s == 1)
    {
      return 1;
    }
  else if (s == 2)
    {
      gyroSelfTestEn();
      accSelfTestEn();
      return 2;
    }
  else if (s == 3)
  {
    return 3;  
  }
  return 0;
}

void writeInt (int16_t data)
{
  int8_t high = (int8_t) (data>>8);
  uint8_t low = (uint8_t)data;
  Serial.write(high);
  Serial.write(low); 
}
