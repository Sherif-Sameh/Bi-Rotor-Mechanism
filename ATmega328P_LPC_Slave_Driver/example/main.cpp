/**
 * @file main.cpp
 * @author Ziad Alwareh & Mina Wassfy
 * @brief This is an example implementation of the LPC slave code, where it receives data from the master and 
 * decodes them.
 * @version 0.1
 * @date 2023-01-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include <LPC_Slave.h>

msg_type_t msg;
float messages[2];

int main(void){
  Serial.begin(9600);
  float* p =messages;
  initialize_LPC(0);
  while(1){
    msg = receive_data();
    get_decoded_data(p);
    if(msg==MSG_TYPE_CHANGE_MODE){
     Serial.print("Mode is ");
     Serial.println(get_current_mode());
     Serial.flush();
    }
    if(msg==MSG_TYPE_PITCH){
     Serial.println("Pitch is ");
     Serial.flush();
      get_decoded_data(p);
      for(int i=0;i<2;i++){
        Serial.println(messages[i]);
        Serial.flush();
      }
    }
    if(msg==MSG_TYPE_ELEVATION){
      Serial.println("Elevation is ");
      Serial.flush();
      get_decoded_data(p);
      for(int i=0;i<2;i++){
        Serial.println(messages[i],HEX);
        Serial.flush();
      }
    }
    if(msg==MSG_TYPE_REFERENCE){
      Serial.println("Reference is ");
      Serial.flush();
      get_decoded_data(p);
      for(int i=0;i<2;i++){
        Serial.println(messages[i],HEX);
        Serial.flush();
      }
    }
  }
}