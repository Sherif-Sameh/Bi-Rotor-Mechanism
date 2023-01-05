/* This driver is responsible for configuring Timer 2 of the ATMega32 to work in phase correct 
    PWM mode with the specified frequecy as well as controlling the duty cycle of the PWM signal. */

#ifndef MCAL_TIMER2
#define MCAL_TIMER2

#include <avr/io.h>

/* Preprocessor Macros */
#define CPU_CLK_FREQ_HZ     ((uint32_t) 16000000)
#define OCR2B_PIN           PD3

/* Enum Definitons */
typedef enum {
    TIMER2_N_1 = 1,
    TIMER2_N_8 = 2,
    TIMER2_N_32 = 3,
    TIMER2_N_64 = 4,
    TIMER2_N_128 = 5,
    TIMER2_N_256 = 6,
    TIMER2_N_1024 = 7
} TIMER2_PRESCALAR;

typedef enum {
    TIMER2_PWM_NON_INVERTED,
    TIMER2_PWM_INVERTED
} TIMER2_PWM_COM;

/* Function Declerations */
uint8_t getOCRATimer2();
void initTimer2(uint32_t pwmFrequencyHz, TIMER2_PWM_COM workingMode);
void disableTimer2();
void setDutyCycleTimer2(float dutyCycle);

#endif