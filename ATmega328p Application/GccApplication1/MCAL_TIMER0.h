/* This driver is responsible for configuring Timer 0 of the ATMega32 to work in phase correct 
    PWM mode with the specified frequecy as well as controlling the duty cycle of the PWM signal. */

#ifndef MCAL_TIMER0
#define MCAL_TIMER0

#include <avr/io.h>

/* Preprocessor Macros */
#ifndef CPU_CLK_FREQ_HZ
#define CPU_CLK_FREQ_HZ     ((uint32_t) 16000000)
#endif
#define OCR0B_PIN           PD5

/* Enum Definitons */
typedef enum {
    TIMER0_N_1 = 1,
    TIMER0_N_8 = 2,
    TIMER0_N_64 = 3,
    TIMER0_N_256 = 4,
    TIMER0_N_1024 = 5
} TIMER0_PRESCALAR;

typedef enum {
    TIMER0_PWM_NON_INVERTED,
    TIMER0_PWM_INVERTED
} TIMER0_PWM_COM;

/* Function Declerations */
uint8_t getOCRATimer0();
void initTimer0(uint32_t pwmFrequencyHz, TIMER0_PWM_COM workingMode);
void disableTimer0();
void setDutyCycleTimer0(float dutyCycle);

#endif
