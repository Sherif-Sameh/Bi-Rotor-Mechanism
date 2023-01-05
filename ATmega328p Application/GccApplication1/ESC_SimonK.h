
#ifndef ESC_SIMONK
#define ESC_SIMONK

#include "MCAL_TIMER2.h"

/* Symbolic Constants */
#define ESC_SIMONK_PWM_FREQ_HZ          500
#define ESC_SIMONK_MAX_PULSE_WIDTH_MS   ((float) 2)
#define ESC_SIMONK_MIN_PULSE_WIDTH_MS   ((float) 1)
#define ESC_SIMONK_MAX_VOLTAGE          ((float) 12)
#define ESC_SIMONK_WORKING_MODE         TIMER2_PWM_NON_INVERTED

/* Function Declerations */
void initESC_SimonK();
void setDutyCycleSimonK(float voltage);

#endif