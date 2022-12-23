/*
 Created By: Sherif Sameh

 This sketch, MPU6050_Calibration, was created to capture data from the accelerometers
 and gyroscopes seperately and to send that data to MATLAB to be analysed and to find
 the bias factors and the senstivity scale factors corrections from the manufacturer's reported 
 senstivity for each axis to be able to correct the raw data to give more accurate readings.

*/


#include "MPU_Basic.h"
#include "MPU_Advanced.h"

int16_t accReadings[3] = {0,0,0};
int16_t gyroReadings[3] = {0,0,0};
const uint16_t noOfSamples = 501;
uint16_t samplesTaken = 0;
uint32_t t0 = 0;
int8_t state = 0; // 0: means wait and 1: indicates the start signal

void setup() 
{
  Serial.begin(115200);

  // TWI setup: SC Freq, Device Address, and General call recognition.
  enableTWI();
  setDeviceAddress(50,0); // Address value insignificant as it's only a 2 device comm.
  setSCLFrequency(3); // SCL Freq = 400Khz
  DDRC &= ~((1<<4) | (1<<5));
  PORTC |= ((1<<4) | (1<<5)); // Enable Internal Pull-up resistors for SDA and SCL  

  // MPU6050 setup:
  setAD0(0);
  deviceReset(); // To ensure that any old configurations are erased
  delay(50);
  wakeUp(); // Wake up device from sleep mode and wait for some time
  delay(100);
  // Set the Clock source to be one of the gyros for improved clock stability as recommeded by the manufacturer
  enum clockSource cs = gyroZ;
  setClockSource(cs);
  setGyroFS_Range(0); // +-250dps
  setAccFS_Range(0); //  +-2g
  setDLPF(3);
}

void loop() 
{
  if(Serial.available() > 0)
  {
    samplesTaken = 0;
    state = (int8_t)Serial.read();
  }

  if(state)
  {
    while(samplesTaken < noOfSamples)
    {
      t0 = micros();
      // getAccReadings(accReadings,0,0);
      getGyroReadings(gyroReadings,0,0);
      for(uint8_t i = 0; i < 3; i++)
      {
        /* For Accelerometer Calibration */
//        writeInt(0); // In place of gyro readings
//        writeInt(accReadings[i]); 

        /* For Gyro Calibration */
        writeInt(gyroReadings[i]);
        writeInt(0); // In place of accelerometer readings
      }
      samplesTaken++;
      while((micros() - t0) < 10000); // Sampling Rate = 100Hz
    }
    state = 0;
  }
}

void writeInt (int16_t data)
{
  int8_t high = (int8_t) (data>>8);
  uint8_t low = (uint8_t)data;
  Serial.write(high);
  Serial.write(low); 
}
