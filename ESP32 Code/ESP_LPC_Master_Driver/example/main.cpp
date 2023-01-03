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
#include <LPC.h>
//#define CONFIG_ARDUINO_LOOP_STACK_SIZE 24576
LPC lpc(3,1000000,0,'M');

bool flag = false;
void setup(){
  Serial.begin(115200);
  pinMode(21,INPUT_PULLUP);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  pinMode(17, INPUT_PULLUP);
  pinMode(15,OUTPUT);
  digitalWrite(15,LOW);
  pinMode(22,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  pinMode(16,INPUT_PULLUP);
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  pinMode(4,INPUT_PULLUP);
  pinMode(14,OUTPUT);
  digitalWrite(14,LOW);
  pinMode(0,INPUT_PULLUP);
  pinMode(27,OUTPUT);
  digitalWrite(27,LOW);
}
void loop(){
  if(digitalRead(21)==LOW){
      Serial.println("ACK");
      Serial.flush();
      digitalWrite(2,HIGH);
      lpc.send_ACK();
      delay(200);
      digitalWrite(2,LOW);
    }
  if(digitalRead(17)==LOW){
      Serial.println("NACK");
      Serial.flush();
      digitalWrite(15,HIGH);
      lpc.send_NACK();
      delay(200);
      digitalWrite(15,LOW);
  }
  if(digitalRead(22)==LOW){
      Serial.println("Mode");
      Serial.flush();
      digitalWrite(13,HIGH);
      if(flag){
        Serial.println(lpc.set_mode(0));
        flag=false;
        delay(100);
        Serial.println("true");
        Serial.flush();
      }
      else{
      if(!flag){
        Serial.println(lpc.set_mode(1));
        flag=true;
        delay(100);
        Serial.println("false");
        Serial.flush();
      }
      }
      delay(200);
      digitalWrite(13,LOW);
  }
    if(digitalRead(16)==LOW){
      Serial.println("REF");
      Serial.flush();
      digitalWrite(12,HIGH);
      float f[2]={3.3,4.4};
      Serial.println(lpc.send_reference(f));
      delay(200);
      digitalWrite(12,LOW);
      Serial.flush();
    }
    if(digitalRead(4)==LOW){
      Serial.println("pitch");
      Serial.flush();
      digitalWrite(14,HIGH);
      float f[2]={5.5,6.6};
      Serial.println(lpc.send_pitch(f));
      Serial.flush();
      delay(200);
      digitalWrite(14,LOW);
    } 
    if(digitalRead(0)==LOW){
      Serial.println("ELV");
      Serial.flush();
      digitalWrite(27,HIGH);
      float f[2]={7.7,8.8};
      Serial.println(lpc.send_elevation(f));
      delay(200);
      digitalWrite(27,LOW);
      Serial.flush();
    }
}