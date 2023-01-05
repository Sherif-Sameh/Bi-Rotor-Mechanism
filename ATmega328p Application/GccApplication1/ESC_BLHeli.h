
#ifndef ESC_BLHELI
#define ESC_BLHELI

#include "MCAL_TIMER0.h"

/* Symbolic Constants */
#define ESC_BLHELI_PWM_FREQ_HZ          480
#define ESC_BLHELI_MAX_PULSE_WIDTH_MS   ((float) 2)
#define ESC_BLHELI_MIN_PULSE_WIDTH_MS   ((float) 1)
#define ESC_BLHELI_MAX_VOLTAGE          ((float) 12)
#define ESC_BLHELI_WORKING_MODE         TIMER0_PWM_NON_INVERTED

/* Function Declerations */
void initESC_BLHeli();
void setDutyCycleBLHeli(float voltage);

#endif