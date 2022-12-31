#ifndef SPI_Z
#define SPI_Z
#include <Arduino.h>
#include "SPI.h"

/*
This is a method that would initialize the spi itself. spi_num should be 2 for HSPI or 3 for VSPI.
Keep in Mind the correct pin connections for operating the SPI's.
The method creates and returns a pointer of type SPIClass that is later used to send data over SPI.
*/
/**
 * @brief This is a method that would initialize the spi itself. We could select the SPI to be VSPI or
 * HSPI. Make sure propper pins are used for each SPI perepheral used.
 * 
 * For HSPI
 * MOSI --> 13   MISO --> 12    SS --> 15   SCLK --> 14
 * 
 * For VSPI
 * MOSI --> 23   MISO --> 19    SS --> 5   SCLK --> 18
 * 
 * @param spi_num This should be either 2 or 3. 2 is for SPI2 (HSPI) and 3 is for SPI3 (VSPI)
 * @return SPIClass* 
 */
SPIClass *initialize_spi(uint8_t spi_num){
    SPIClass * spi = NULL;
if(spi_num==2){
    #define MOSI 13
    #define MISO 12
    #define SS 15
    #define SCK 14
    spi = new SPIClass(HSPI);
}
else{
    #define MOSI 23
    #define MISO 19
    #define SS 5
    #define SCK 18
    spi = new SPIClass(VSPI);
    spi ->begin();
    pinMode(spi->pinSS(),OUTPUT);
}
    spi ->begin();
    pinMode(spi->pinSS(),OUTPUT);
    return spi;
}

/**
 * @brief Set the spi transfer settings. This is used when we want to use the same SPI transfer settings
 * everytime we initiate an SPI transfer command. Instead of having to define the transfer settings 
 * everytime, we could define it once and pass this object as a parameter. 
 * 
 * @param clkf This is the SCLK frequency we want to set for the SPI
 * @param spi_mode This is the mode of transfer. Could be input as 0,1,2,3. Each mode refers to the clock
 * polarity as well as the phase. For mode details check ATmega328p doccumentation for SPI data modes.
 * @param first_bit This could be either 'M' indicating Most significant bit first to be transfered, or
 * 'L' for transfering the least significant bit first.
 * @return SPISettings object would contain the transfer settings we want to fix for our SPI transfer
 * operations
 */
SPISettings set_spi_settings(uint32_t clkf, uint8_t spi_mode, char first_bit){
    uint8_t bit;
  SPISettings mysettings = SPISettings();
  if (first_bit == 'M'){
    bit = MSBFIRST;
  }
    else
        bit = LSBFIRST;

  
  switch(spi_mode){
    case 1: mysettings = SPISettings(clkf,bit,SPI_MODE1);break;
    case 2: mysettings = SPISettings(clkf,bit,SPI_MODE2);break;
    case 3: mysettings = SPISettings(clkf,bit,SPI_MODE3);break;
    default: mysettings = SPISettings(clkf,bit,SPI_MODE0);break;
  }
  return mysettings;
}

/**
 * @brief This is a funciton that will allow us to Send data over SPI with transfer conditions defined 
 * only once for this specific trnasfer operation.
 * 
 * @param spi : This is the pointer to the SPI class that we have created earlier containing the data of
 * the SPI device we are utilizing.
 * @param data This is the byte of data we want to send.
 * @param clkf This is the SCLK frequency.
 * @param spi_mode This is the mode of transfer. Could be input as 0,1,2,3. Each mode refers to the clock
 * polarity as well as the phase. For mode details check ATmega328p doccumentation for SPI data modes.
 * @param first_bit This could be either 'M' indicating Most significant bit first to be transfered, or
 * 'L' for transfering the least significant bit first.
 */
void spi_send(SPIClass *spi,uint8_t data, uint32_t clkf, uint8_t spi_mode, char first_bit){
    //Creating a SPISettings object containing the SPI transfer settings to be used in the SPI transaction
  uint8_t bit;
  SPISettings mysettings = SPISettings();
  if (first_bit == 'M'){
    bit = MSBFIRST;
  }
    else
        bit = LSBFIRST;

  
  switch(spi_mode){
    case 1: mysettings = SPISettings(clkf,bit,SPI_MODE1);break;
    case 2: mysettings = SPISettings(clkf,bit,SPI_MODE2);break;
    case 3: mysettings = SPISettings(clkf,bit,SPI_MODE3);break;
    default: mysettings = SPISettings(clkf,bit,SPI_MODE0);break;
  }
  //Initializing an SPI transaction using the defined SPI settings
  spi->beginTransaction(mysettings);
  //Driving the SS pin low
  digitalWrite(spi->pinSS(),LOW);
  //transfering the data
  spi->transfer(data);
  //Driving the SS pin high to end the transaction
  digitalWrite(spi->pinSS(),HIGH);
  //ending the SPI transaction
  spi->endTransaction();
}

/**
 * @brief Allows us to transfer data through SPI without having to define the transfer settings everytime,
 * we can simply just pass an SPISettings object containing all the settings we want for the transferal.
 * 
 * @param spi This is the pointer to the SPI class that we have created earlier containing the data of
 * the SPI device we are utilizing.
 * @param data This is the byte of data we want to transfer over SPI.
 * @param mysettings This is the SPISettings object containing the SPI transfer settings.
 */
void spi_send(SPIClass *spi,uint8_t data, SPISettings mysettings){
 //Initializing an SPI transaction using the defined SPI settings
  spi->beginTransaction(mysettings);
  //Driving the SS pin low
  digitalWrite(spi->pinSS(),LOW);
  //transfering the data
  spi->transfer(data);
  //Driving the SS pin high to end the transaction
  digitalWrite(spi->pinSS(),HIGH);
  //ending the SPI transaction
  spi->endTransaction();
}

/**
 * @brief This is a function that would allow us to receive a byte from the slave while allowing us to set
 * the SPI settings for that particular transaction.
 * 
 * @param spi : This is the pointer to the SPI class that we have created earlier containing the data of
 * the SPI device we are utilizing.
 * @param clkf This is the SCLK frequency.
 * @param spi_mode This is the mode of transfer. Could be input as 0,1,2,3. Each mode refers to the clock
 * polarity as well as the phase. For mode details check ATmega328p doccumentation for SPI data modes.
 * @param first_bit This could be either 'M' indicating Most significant bit first to be transfered, or
 * 'L' for transfering the least significant bit first.
 * @return uint8_t, which is the byte received from the slave device.
 */
uint8_t spi_receive(SPIClass *spi, uint32_t clkf, uint8_t spi_mode, char first_bit){
  //Creating a SPISettings object containing the SPI transfer settings to be used in the SPI transaction
  uint8_t bit;
  SPISettings mysettings = SPISettings();
  if (first_bit == 'M'){
    bit = MSBFIRST;
  }
    else
        bit = LSBFIRST;

  
  switch(spi_mode){
    case 1: mysettings = SPISettings(clkf,bit,SPI_MODE1);break;
    case 2: mysettings = SPISettings(clkf,bit,SPI_MODE2);break;
    case 3: mysettings = SPISettings(clkf,bit,SPI_MODE3);break;
    default: mysettings = SPISettings(clkf,bit,SPI_MODE0);break;
  }
  //Initializing an SPI transaction using the defined SPI settings
  spi->beginTransaction(mysettings);
  digitalWrite(spi->pinSS(),LOW);
  //sending a dummy byte. Usually, after the transaction. The content of the slave register and the
  // master register are switched. Therefore, we can store the returned content from the transfer.
  uint8_t result = spi->transfer(0x0F);
  digitalWrite(spi->pinSS(),HIGH);
  spi->endTransaction();
  return result;
}

/**
 * @brief This is a function that would allow us to receive a byte from the slave using a preset SPI
 * setting object defined in a SPISettings object
 * 
 * @param spi This is the pointer to the SPI class that we have created earlier containing the data of
 * the SPI device we are utilizing. 
 * @param mysettings This is the SPISettings object containing the SPI transfer settings.
 * @return uint8_t, which is the byte received from the slave device.
 */
uint8_t spi_receive(SPIClass *spi, SPISettings mysettings){
  //initialize the transaction with the defined SPI settings
  spi->beginTransaction(mysettings);
  digitalWrite(spi->pinSS(),LOW);
  //sending a dummy byte. Usually, after the transaction. The content of the slave register and the
  // master register are switched. Therefore, we can store the returned content from the transfer.
  uint8_t result = spi->transfer(0x0F);
  digitalWrite(spi->pinSS(),HIGH);
  spi->endTransaction();
  return result;
}

#endif