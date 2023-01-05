/**
 * @file esp_master_spi.h
 * @author Ziad Alwareh
 * @brief 
 * @version 1.0
 * @date 2022-01-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SPI_Z
#define SPI_Z
#include <Arduino.h>
#include "SPI.h"

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
SPIClass *initialize_spi(uint8_t spi_num);
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
SPISettings set_spi_settings(uint32_t clkf, uint8_t spi_mode, char first_bit);
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
void spi_send(SPIClass *spi,uint8_t data, uint32_t clkf, uint8_t spi_mode, char first_bit);
/**
 * @brief Allows us to transfer data through SPI without having to define the transfer settings everytime,
 * we can simply just pass an SPISettings object containing all the settings we want for the transferal.
 * 
 * @param spi This is the pointer to the SPI class that we have created earlier containing the data of
 * the SPI device we are utilizing.
 * @param data This is the byte of data we want to transfer over SPI.
 * @param mysettings This is the SPISettings object containing the SPI transfer settings.
 */
void spi_send(SPIClass *spi,uint8_t data, SPISettings mysettings);
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
uint8_t spi_receive(SPIClass *spi, uint32_t clkf, uint8_t spi_mode, char first_bit);
/**
 * @brief This is a function that would allow us to receive a byte from the slave using a preset SPI
 * setting object defined in a SPISettings object
 * 
 * @param spi This is the pointer to the SPI class that we have created earlier containing the data of
 * the SPI device we are utilizing. 
 * @param mysettings This is the SPISettings object containing the SPI transfer settings.
 * @return uint8_t, which is the byte received from the slave device.
 */
uint8_t spi_receive(SPIClass *spi, SPISettings mysettings);
#endif

