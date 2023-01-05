/**
 * @file SPI_Slave.h
 * @author Ziad Alwareh
 * @brief 
 * @version 1.0
 * @date 2023-01-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SPI_M
#define SPI_M
#include <avr/io.h>
#include <Arduino.h>
#include <stdint.h>
#define miso 12
#define mosi 11
#define ss 10
#define sck 13

/**
 * @brief Initializes the SPI of the slave.
 * 
 * @param mode the mode of the SPI transaction. 
 * 0-->mode0  1-->mode1  2-->mode2 3-->mode3
 */
void spiSetSlave(uint8_t mode);

/**
 * @brief Sends a byte to the Master through SPI.
 * 
 * @param sdata The data we want to send to the master over SPI
 */
void spiSendSlave(uint8_t sdata);

/**
 * @brief receives a byte from the Master thorugh SPI
 * 
 * @return uint8_t the byte of data we received.
 */
uint8_t spiReceiveSlave();
#endif
