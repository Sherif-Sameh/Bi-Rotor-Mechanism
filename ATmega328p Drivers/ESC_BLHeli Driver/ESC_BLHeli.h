/**
 * @file ESC_BLHeli.h
 * @author Sherif Sameh
 * @brief A driver for controlling the BLHeli brushless motor ESC.
 * @attention In its current configuration, this driver is bound to Timer 0 of the ATmega
 * @version 1.0
 * @date 2023-01-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef ESC_BLHELI
#define ESC_BLHELI

#include "MCAL_TIMER0.h"

/* Symbolic Constants */

#define ESC_BLHELI_PWM_FREQ_HZ          250
#define ESC_BLHELI_MAX_PULSE_WIDTH_MS   ((float) 2)
#define ESC_BLHELI_MIN_PULSE_WIDTH_MS   ((float) 1)
#define ESC_BLHELI_MAX_VOLTAGE          ((float) 12)
#define ESC_BLHELI_WORKING_MODE         TIMER0_PWM_NON_INVERTED

/**
 * @def ESC_BLHELI_PWM_FREQ_HZ
 * This macro is used to set the frequency of the PWM signal in Hz. Note: This value
 * is fixed and cannot be changed during runtime because the ESCs have to be recalibrated
 * in order to work with a different frequency so it would not have made sense to make it a
 * variable.
 */

/**
 * @def ESC_BLHELI_MAX_PULSE_WIDTH_MS
 * This macros sets the maximum pulse width in ms that corresponds to 100% duty cycle as
 * specefied by the manufacturer of the ESC.
 */

/**
 * @def ESC_BLHELI_MIN_PULSE_WIDTH_MS
 * This macros sets the minimum pulse width in ms that corresponds to 0% duty cycle as
 * specefied by the manufacturer of the ESC.
 */

/**
 * @def ESC_BLHELI_MAX_VOLTAGE
 * This macro sets the maximum voltage that is available to the motors and is used in order 
 * to calculate the required pulse width depending on the desired voltage level.
 */

/**
 * @def ESC_BLHELI_WORKING_MODE
 * This macro is used to choose between inverted and non-inverted COMs for the underlying timer.
 * @see TIMER0_PWM_COM
 */


/* Function Declerations */

/**
 * @brief Initializes the underlying timer according to the desired settings as well as calculating
 * all the neccessary parameters for the ESCs normal operation.
 * @param
 */
void initESC_BLHeli();

/**
 * @brief Sets the Duty Cycle for the ESC by converting the voltage into a duty cycle then
 * communicating with the timer to set that duty cycle.
 * @param voltage The desired voltage to be applied to the motors. Ranges from 0 to the max defined voltage.
 */
void setDutyCycleBLHeli(float voltage);

#endif