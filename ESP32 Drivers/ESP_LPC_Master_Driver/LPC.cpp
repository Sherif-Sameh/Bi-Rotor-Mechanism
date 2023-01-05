#include "LPC.h"
#include "crc8.h"

static uint8_t final_encoded_message[10];
static uint8_t* p = final_encoded_message;
uint8_t gen_first_byte(uint8_t message_id){
    uint8_t first_byte;
    switch(message_id){
        case 0: first_byte = 0b00000001;break; //change mode
        case 1: first_byte = 0b00000110;break; //Pitch
        case 2: first_byte = 0b00001010;break; //Elevation
        case 3: first_byte = 0b00001110;break; //Reference
        case 4: first_byte = 0b00010000;break; //AKN
        default: first_byte = 0b00010100;break; //NAKN
    }
    return first_byte;
}

void encode_message(uint8_t * data, uint8_t data_array_length, uint8_t message_id){
    uint8_t first_byte = gen_first_byte(message_id);
    uint8_t message_length;
    if(message_id == 4 || message_id == 5){
        message_length = 2;
    }
    else
        message_length = 2+(4*(first_byte&0b00000011));
    *p = message_length;
    *(p+1) = first_byte;
    for(int i = 0;i<data_array_length;i++)
            *(p+i+2) = *(data+i);
    if(!(message_id == 4) && !(message_id == 5)){
        uint8_t crc8_byte = CRC8Calculate((p + 2),sizeof(uint8_t) * (message_length-2));
        *(p+data_array_length+2) = crc8_byte;
    }
}

uint8_t decode_id(uint8_t first_msg_byte){
    return ((first_msg_byte & 0b00011100)>>2);
}

bool LPC::end_transaction(){
    uint8_t received = spi_receive(this->SPI,this->Mysettings); 
    uint8_t id = decode_id(received);
    if(id==4){
        this->send_NACK();
        return true;
    }
    else
        return false;

}

uint8_t get_message_length(uint8_t mesg_id){
    uint8_t length;
    switch(mesg_id){
        case 0: length=4;break;
        case 1: length=8;break;
        case 2: length=8;break;
        case 3: length=8;break;
        default: length=0;break;
    }
    return length;
}

bool LPC::Send_message(float * data, uint8_t id){
   uint8_t data_msg_length = get_message_length(id);
    this->set_MSG_data(data);
    encode_message(this->MSG_data.byte_eqv,data_msg_length,id);
    //since the first entry of the returned array contains the length of the message
    uint8_t N= *p;
    for(int i=0;i<(N);i++)
            spi_send(this->SPI,*(p+1+i),this->Mysettings);
    return  this->end_transaction();
}

LPC::LPC(uint8_t spi_port_num, uint32_t clkf, uint8_t spi_mode, char first_bit_transfered){
    this->Mysettings = set_spi_settings(clkf,spi_mode,first_bit_transfered);
    this->SPI = initialize_spi(spi_port_num);
}

void LPC::set_MSG_data(float* data){
    memcpy(this->MSG_data.float_num,data,8);
}

void LPC::send_ACK(){
    encode_message(NULL,0,4);
    spi_send(this->SPI,*(p+1),this->Mysettings);
}

void LPC::send_NACK(){
    encode_message(NULL,0,5);
    spi_send(this->SPI,*(p+1),this->Mysettings);
}

bool LPC::set_mode(uint8_t mode_num){
    float  data[2] = {0,0};
    float * p = data;
    if(mode_num==0){
        data[0]=0.0;
        data[1]=0.0;
    }
    else{
        data[0]=1.0;
        data[1]=1.0;
    }
    return this->Send_message(p,0);
    
}

bool LPC::send_reference(float * data){
    return this->Send_message(data,3);
}

bool LPC::send_pitch(float * data){
    return this->Send_message(data,1);
}

bool LPC::send_elevation(float * data){
    return this->Send_message(data,2);
}