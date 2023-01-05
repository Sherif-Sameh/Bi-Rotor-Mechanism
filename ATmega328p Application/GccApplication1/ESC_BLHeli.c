
#include "ESC_BLHeli.h"

/* Global Variables */
static uint8_t OCR0A_value;
static float maxOCRB_BLHeli;
static float minOCRB_BLHeli;

/* Function Definitions */
void initESC_BLHeli()
{
    initTimer0(ESC_BLHELI_PWM_FREQ_HZ, ESC_BLHELI_WORKING_MODE);
    setDutyCycleTimer0(0);
    OCR0A_value = getOCRATimer0();
    float periodMs = (1000 / ESC_BLHELI_PWM_FREQ_HZ);
    maxOCRB_BLHeli = (ESC_BLHELI_MAX_PULSE_WIDTH_MS / periodMs) * OCR0A_value;
    minOCRB_BLHeli = (ESC_BLHELI_MIN_PULSE_WIDTH_MS / periodMs) * OCR0A_value;
}

void setDutyCycleBLHeli(float voltage)
{
    float actualOCRB = ((voltage / ESC_BLHELI_MAX_VOLTAGE) * (maxOCRB_BLHeli - minOCRB_BLHeli)) + minOCRB_BLHeli;
    setDutyCycleTimer0((actualOCRB / OCR0A_value) * 100);
}