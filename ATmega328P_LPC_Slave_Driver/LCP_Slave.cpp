#include "LPC_Slave.h"
#include "crc8.h"

static uint8_t mode;
static f2b convert_to_float;
void initialize_LPC(uint8_t SPI_MODE){
    spiSetSlave(SPI_MODE);
}

uint8_t get_msg_id(uint8_t start_byte){
    return ((start_byte>>2)&0x07);
}

uint8_t get_msg_length(uint8_t msg_id){
    uint8_t length;
    switch(msg_id){
        case 0: length = 4; break;
        case 4:
        case 5: length = 0; break;
        default: length = 8; break;
    }
    return length;
}

void send_ACK(){
    spiSendSlave(0b00010000);
    spiReceiveSlave();
}

void send_NACK(){
    spiSendSlave(0b00010100);
}


bool compare_crc8(uint8_t* msg_received,uint8_t msg_length){
    uint8_t crc = CRC8Calculate((msg_received+1),msg_length);
    if (crc == *(msg_received+msg_length+1))
        return true;
    return false;
}

msg_type_t LPC_decode(uint8_t* msg_received, uint8_t msg_id){
    switch(msg_id){
        case 0: 
            if(*(msg_received+4)==0x3F)
                mode = 1;
            else
                mode = 0;
            delete msg_received;
            return MSG_TYPE_CHANGE_MODE;
        case 1:
            for(uint8_t i=0;i<8;i++)
                convert_to_float.byte_num[i] = *(msg_received +1+i);
            delete msg_received;
            return MSG_TYPE_PITCH;
        case 2:
            for(uint8_t i=0;i<8;i++)
                convert_to_float.byte_num[i] = *(msg_received +1+i);
            delete msg_received;
            return MSG_TYPE_ELEVATION;
        case 3:
            for(uint8_t i=0;i<8;i++)
                convert_to_float.byte_num[i] = *(msg_received +1+i);
            delete msg_received;
            return MSG_TYPE_REFERENCE;
        case 4: delete msg_received; return MSG_TYPE_ACK;
        case 5: delete msg_received; return MSG_TYPE_NACK;
        default: delete msg_received; return MSG_FAILURE;
    }
}

uint8_t get_current_mode(){
    return mode;
}

void get_decoded_data(float* output_data){
    memcpy(output_data,convert_to_float.float_num,8);
}

msg_type_t receive_data(){
    uint8_t first_byte = spiReceiveSlave();
    uint8_t msg_id = get_msg_id(first_byte);
    uint8_t msg_length = get_msg_length(msg_id);
    if(msg_id ==5 )
        return MSG_SUCCESS;
    else{
        uint8_t* p = new uint8_t[sizeof(uint8_t*)*(msg_length+2)];
        *p = (first_byte);
        for(uint8_t i=0;i<msg_length+1;i++)
            *(p+1+i)=spiReceiveSlave();
        if(!(compare_crc8(p,msg_length))){
            send_NACK();
            return MSG_FAILURE;
        }
        send_ACK();
        return LPC_decode(p,msg_id);
    }
}