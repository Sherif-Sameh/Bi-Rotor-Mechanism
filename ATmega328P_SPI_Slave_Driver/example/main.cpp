#include <Arduino.h>
#include <util/delay.h>
/*
 * This is an auxillary file to the example of the esp_master_spi.h driver. This is the code to be uploaded to the arduino in order to test the
 * interaction between the Master device (ESP32) and the slave device (ATmega328P)
*/
#define F_CPU 16000000L
#include <SPI_Slave.h>
#include <avr/io.h>
int main (void){
  spiSetSlave(0);
  DDRD = 0xFF;
  PORTD = 0x00;
  Serial.begin(9600);
  DDRC = 0x00;
  PORTC = 0x01;
  uint8_t f = 0;
  while(1){
    if(f==0){
      if(!(PINC & 0x01)){
        PORTD = (1<<PD4);
        spiSendSlave(0x0F);
        f=1;
      }
      }
    if(f==1){
      uint8_t x = spiReceiveSlave();
      Serial.println(x,HEX);
      Serial.flush();
      if (x>0x0F){
        PORTD = 0x04;
        f=0;
      }
      else{
        if(x<0x0F){
          PORTD = 0x08;
          f=0;
        }
      }
    }
  }
}