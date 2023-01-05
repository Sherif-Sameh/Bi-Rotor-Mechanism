/**
 * @file MCAL_TIMER2.h
 * @author Sherif Sameh
 * @brief For Confiuring Timer 2 in Phase Correct PWM mode.
 * This driver is responsible for configuring Timer 2 of the ATMega32 to work in phase correct 
 * PWM mode with the specified frequecy as well as controlling the duty cycle of the PWM signal.
 * @version 1.0
 * @date 2023-01-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MCAL_TIMER2
#define MCAL_TIMER2

#include <avr/io.h>

/* Preprocessor Macros */

#ifndef CPU_CLK_FREQ_HZ
#define CPU_CLK_FREQ_HZ     ((uint32_t) 16000000)
#endif
#define OCR2B_PIN           PD3

/**
 * @def CPU_CLK_FREQ_HZ 
 * This macro is used to define the CPU's operating clock frequeny in Hz as to have the ability
 * to automatically adjust the values needed in the Output compare registers in order to generate
 * the PWM signal at the desired frequency.
 */


/**
 * @def OCR2B_PIN
 * This macro defines the pin that the PWM signal is output to in order to be able to configure
 * it as an output during the initialization process.
 */

/* Enum Definitons */

/**
 * @enum TIMER2_PRESCALAR
 * @brief Enum used to abstract the register values needed for each prescalar value. 
 */
typedef enum {
    TIMER2_N_1 = 1,         /**< Prescalar = 1 */
    TIMER2_N_8 = 2,         /**< Prescalar = 8 */
    TIMER2_N_32 = 3,        /**< Prescalar = 32 */
    TIMER2_N_64 = 4,        /**< Prescalar = 64 */
    TIMER2_N_128 = 5,       /**< Prescalar = 128 */
    TIMER2_N_256 = 6,       /**< Prescalar = 256 */
    TIMER2_N_1024 = 7       /**< Prescalar = 1024 */
} TIMER2_PRESCALAR;

/**
 * @enum TIMER2_PWM_COM
 * @brief Enum used for choosing the PWM's compare output mode (COM).
 */
typedef enum {
    TIMER2_PWM_NON_INVERTED,        /**< COM = Non Inverted */
    TIMER2_PWM_INVERTED             /**< COM = Inverted */
} TIMER2_PWM_COM;

/* Function Declerations */

/**
 * @brief A function to return the value of OCRA
 * @return uint8_t Return the values stored in 
 * register OCR2A (sets the frequency of the PWM signal).
 */
uint8_t getOCRATimer2();

/**
 * @brief Initialize Timer 2 in Phase Correct PWM Mode.
 * This function will configures all the relevant uC registers in order to initialize the
 * timer in PC PWM mode with the desired frequency and working mode. 
 * @param pwmFrequencyHz The desired PWM frequency in Hz.
 * @param workingMode The working mode.
 * @see TIMER2_PWM_COM
 */
void initTimer2(uint32_t pwmFrequencyHz, TIMER2_PWM_COM workingMode);

/**
 * @brief Disables Timer 2.
 * This function will reset all relevant uC registers in order to totally disable
 * the timer's operation.
 * @param
 */
void disableTimer2();

/**
 * @brief Set the Duty Cycle of Timer 2
 * This function will calculate the needed values for the OC register in order to get
 * the desired duty cycle at the current frequency and COM.
 * @param dutyCycle The desired duty cycle. Acceptable values range from 0.0 to 100.0
 */
void setDutyCycleTimer2(float dutyCycle);

#endif