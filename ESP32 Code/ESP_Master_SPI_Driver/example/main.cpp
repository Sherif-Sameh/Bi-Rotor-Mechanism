/**
 * @file main.cpp
 * @author Ziad Alwareh
 * @brief This is an example cpp code on how to send/receive data over SPI using an ESP configured as master device.
 * The ESP is connected to 2 push buttons connected to GPIO 21 and 17. Depending on which button is pressed
 * Either 0x00 or 0xFF is sent over SPI to the slave device. If the ESP received a logic High on GPIO22,
 * it will trigger a receive data from slave operation and turns on an LED connected to GPIO16.
 * @version 0.1
 * @date 2022-12-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
/**
 * The Arduino.h file is needed since PlatformIO was used on VSCode and it requires this headerfile for
 * it to be able to utilize Arduino cpp
*/
#include <Arduino.h>
#include <esp_master_spi.h>
//creating the SPISettings object for it to be globally defined for later usage.
SPISettings mysettings = set_spi_settings(4000000,0,'M');
//We need to initialize the SPIClass* object globally to avoid errors.
SPIClass* spi = NULL;

//variable to store received byte from slave
uint8_t x;
void setup(){
  Serial.begin(115200);
  pinMode(21,INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(22,INPUT);
  pinMode(16, OUTPUT);
  digitalWrite(16,LOW);
  //initializing spi utilizing VSPI.
  spi = initialize_spi(3);
  //defining the SPI clocking frequency to be 4MHz, Mode 0, and Most significant bit first to be transfered.
  mysettings = set_spi_settings(4000000,0,'M');
}
void loop(){
    if(digitalRead(21)==LOW){
      digitalWrite(16,LOW);
      spi_send(spi,0xFF,4000000,0,'M');
    }
    if(digitalRead(17)==LOW){
      digitalWrite(16,LOW);
      spi_send(spi,0x00,mysettings);
    }
    if(digitalRead(22)==HIGH){
      x = spi_receive(spi,mysettings);
      if (x == 0x0F)
      {
        digitalWrite(16,HIGH);
      }
    }
    delay(250);
}