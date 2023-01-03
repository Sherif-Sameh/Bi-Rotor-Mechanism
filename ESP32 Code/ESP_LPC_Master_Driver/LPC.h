#ifndef LPC_ESP
#define LPC_ESP
#include <Arduino.h>
#include "esp_master_spi.h"

typedef union {
    float float_num[2];
    uint8_t byte_eqv[8];
} F2B;
class LPC
{
    private:
        SPISettings Mysettings;
        SPIClass * SPI;
        F2B MSG_data;
    public:
    /**
     * @brief Construct a new LPC object
     * 
     * @param spi_port_num This defines the SPI module we want to us
     * HSPI, or VSPI. For HSPI, define this as 2, and for VSPI, define
     * it as 3.
     * @param clkf This is the SPI master's CLK frequency
     * @param spi_mode This is the mode of SPI transaction
     * mode0 -->0   mode1 -->1   mode2 -->2  mode3 -->3
     * @param first_bit_transfered This is the order of bit transferal.
     * 'L' for LSB first and 'M' for MSB first.
     */
        LPC(uint8_t spi_port_num, uint32_t clkf, uint8_t spi_mode, char first_bit_transfered);

        /**
         * @brief This is a funciton to send an ACK
         * 
         */
        void send_ACK();

        /**
         * @brief This is a function to send a NACK
         * 
         */
        void send_NACK();

        /**
         * @brief Set the mode of operation of the slave device
         * 
         * @param mode_num This is the mode number. We have 2 modes
         * so this could be     0--> mode 0     1--> mode 1 
         * @return true incase of successful transaction.
         * @return false incase of failed transaction.
         */
        bool set_mode(uint8_t mode_num);

        /**
         * @brief A method that would send the pitch and elevation reference.
         * 
         * @param data This is a pointer to an array that contians the data of
         * the reference. The first dataset for the elevation, while the other
         * is for the pitch. The data should be of type float.
         * @return true incase of successful transaction.
         * @return false incase of failed transaction.
         */
        bool send_reference(float * data);

        /**
         * @brief waits the ACK from the slave and replies with a NACK
         * signaling the successful end of a communication.
         * 
         * @return bool signifying the message was sent received or not.
         */
        bool end_transaction();
        
        /**
         * @brief This is the function that will send the message over the LPC.
         * 
         * @param data This is the data that we want to send. Data should be of 
         * Type float.
         * @param id This is the id of the message we want to send over LPC
         * @return true incase of successful transaction.
         * @return false incase of failed transaction.
         */
        bool Send_message(float * data, uint8_t id);
        /**
         * @brief A method that would send the pitch data to the slave.
         * 
         * @param data This is a pointer to an array that contians the data of
         * the pitch. The data should be of type float.
         * @return true incase of successful transaction.
         * @return false incase of failed transaction.
         */
        bool send_pitch(float * data);

        /**
         * @brief A method that would send the elevation data to the slave.
         * 
         * @param data This is a pointer to an array that contians the data of
         * the elevation. The data should be of type float.
         * @return true incase of successful transaction.
         * @return false incase of failed transaction.
         */
        bool send_elevation(float * data);

        /**
         * @brief A method to convert the input float data into an array of bytes inside
         * the LPC object.
         * 
         * @param data The pointer to the array of floats that we are enterring.
         */
        void set_MSG_data(float* data);
};

        /**
         * @brief A method to decode the message we want to send over LPC
         * and puts it in LPC format.
         * 
         * @param data This is a pointer to the array of data we want to send.
         * @param data_array_length This is the length of the array we passed a
         * pointer referring to.
         * @param message_id This is the id of the message we want to send according
         * to the LPC protocol.
         * MSG ID   |   MessageType     |  Length of message in Words (1 word = 4 bytes)
         * ------------------------------------------------------------------------------
         *    0     |   Change Mode     |                            1
         *    1     |   Pitch           |                            2
         *    2     |   Elevation       |                            2
         *    3     |   Reference point |                            2
         *    4     |   ACK             |                            0
         *    5     |   NACK            |                            0
         *    6     |   Reserved        |                            -
         *    7     |   Reserved        |                            -
         */
        void encode_message(uint8_t * data, uint8_t data_array_length, uint8_t message_id);

        /**
         * @brief This is a function that generates the very first byte of data that will be 
         * sent using the LPC. This is the byte that contains the characteristics of the message
         * 
         * @param message_id This is the id of the message we want to send according
         * to the LPC protocol.
         * MSG ID   |   MessageType     |  Length of message in Words (1 word = 4 bytes)
         * ------------------------------------------------------------------------------
         *    0     |   Change Mode     |                            1
         *    1     |   Pitch           |                            2
         *    2     |   Elevation       |                            2
         *    3     |   Reference point |                            2
         *    4     |   ACK             |                            0
         *    5     |   NACK            |                            0
         *    6     |   Reserved        |                            -
         *    7     |   Reserved        |                            -
         * @return uint8_t this is the created byte
         */
        uint8_t gen_first_byte(uint8_t message_id);

        /**
         * @brief Get the message length of a message
         * 
         * @param mesg_id The id of the message that will be sent
         * @return uint8_t which is the length of the message
         */
        uint8_t get_message_length(uint8_t mesg_id);

        /**
         * @brief gets the id of the message in order to know the type of data received
         * 
         * @param first_msg_byte This is the first received byte from the transaction
         * @return uint8_t containing the id of the message
         */
        uint8_t decode_id(uint8_t first_msg_byte);
#endif