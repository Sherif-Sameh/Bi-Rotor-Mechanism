#ifndef LPC_SLAVE
#define LPC_SLAVE
#include "SPI_Slave.h"
#include "stdbool.h"

/**
 * @brief This is the types of messages that could be outputted
 * to the user.
 * 
 */
typedef enum{
        MSG_TYPE_PITCH,
        MSG_TYPE_ELEVATION,
        MSG_TYPE_REFERENCE,
        MSG_TYPE_CHANGE_MODE,
        MSG_TYPE_ACK,
        MSG_TYPE_NACK,
        MSG_FAILURE,
        MSG_SUCCESS
} msg_type_t;

/**
 * @brief A global union that is used to convert from byte to float and vice versa.
 * 
 */
typedef union{
        float float_num[2];
        uint8_t byte_num[8];
}f2b;
/**
 * 
 * @brief This is a method that will initialize the SPI of the LPC slave
 * 
 * @param SPI_MODE This is the SPI mode that we want. 0 --> mode 0,
 *  1 --> mode 1, 2 --> mode 2, 3 --> mode 3
 */
void initialize_LPC(uint8_t SPI_MODE);

/**
 * @brief Get the msg id of the transaction
 *       * MSG ID   |   MessageType     |  Length of message in Words (1 word = 4 bytes)
         * ------------------------------------------------------------------------------
         *    0     |   Change Mode     |                            1
         *    1     |   Pitch           |                            2
         *    2     |   Elevation       |                            2
         *    3     |   Reference point |                            2
         *    4     |   ACK             |                            0
         *    5     |   NACK            |                            0
         *    6     |   Reserved        |                            -
         *    7     |   Reserved        |                            -
 * 
 * @param start_byte This is the first byte received during an LPC interaction
 * @return uint8_t this is the returned ID
 */
uint8_t get_msg_id(uint8_t start_byte);

/**
 * @brief Get the msg length
 * 
 * @param msg_id The ID of the message as embedded in the start byte.
 * @return uint8_t which is the length of the message in bytes
 */
uint8_t get_msg_length(uint8_t msg_id);

/**
 * @brief Sends ACK to the master indicating successful data transaction
 * 
 */
void send_ACK();

/**
 * @brief Sends NACK to the master indicating a failed data transaction
 * 
 */
void send_NACK();

/**
 * @brief computes crc8 and compares it to that computed by the master to
 * signify whether it was a successful or a failed data transaction.
 * 
 * @param msg_received the full message as received from master
 * @param msg_length Length of the msg_received array
 * @return true 
 * @return false 
 */
bool compare_crc8(uint8_t* msg_received, uint8_t msg_length);

/**
 * @brief Decodes the LPC message
 * 
 * @param msg_received The entire received message, including the crc
 * and start bytes
 * @param msg_id The id of the received message.
 * @return msg_type_t which contains the type of message that was decoded.
 * The method also sets the global variables with the data decoded.
 */
msg_type_t LPC_decode(uint8_t* msg_received,uint8_t msg_id);

/**
 * @brief Get the current mode of operation
 * 
 * @return uint8_t
 */
uint8_t get_current_mode();

/**
 * @brief Get the decoded data that we received and copies it to an array
 * input by the user
 * 
 * @param output_data pointer to the array input from the user that we want
 * to copy teh decoded data into
 */
void get_decoded_data(float* output_data);

/**
 * @brief a method to receive data via LPC
 * 
 * @return msg_type_t 
 */
msg_type_t receive_data();
#endif