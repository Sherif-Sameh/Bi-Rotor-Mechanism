/*
 * This is an auxillary file to the example of the esp_master_spi.h driver. This is the code to be uploaded to the arduino in order to test the
 * interaction between the Master device (ESP32) and the slave device (ATmega328P)
*/
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
  bool F = false;
  uint8_t t = 0x0F;
int main (void){
  DDRB = (1<<PB4);
  SPCR = 0b11000000;
  DDRD = 0xFF;
  PORTD = 0x00;
  DDRC = 0x00;
  PORTC = 0x01;
  //enabling interrupts
  sei();
  Serial.begin(9600);
  while(1){
    if(!(PINC&0x01)){
      F = true;
      PORTD = (1<<PD4);
    }
    }
}
ISR(SPI_STC_vect){
   PORTD = 0x00;
  if(F == true){
    SPDR = t;
    F = false;
  }
  else{
  uint8_t x = SPDR;
  Serial.println(x,HEX);
  Serial.flush();
  if (x>0x0F){
    PORTD = 0x04;
  }
  else{
    if(x<0x0F){
      PORTD = 0x08;
    }
  }
 }
}
