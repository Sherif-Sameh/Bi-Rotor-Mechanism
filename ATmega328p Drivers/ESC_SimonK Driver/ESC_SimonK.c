
#include "ESC_SimonK.h"

/* Global Variables */
static uint8_t OCR2A_value;
static float maxOCRB_SimonK;
static float minOCRB_SimonK;

/* Function Definitions */
void initESC_SimonK()
{
    initTimer2(ESC_SIMONK_PWM_FREQ_HZ, ESC_SIMONK_WORKING_MODE);
    setDutyCycleTimer2(0);
    OCR2A_value = getOCRATimer2();
    float periodMs = (1000 / ESC_SIMONK_PWM_FREQ_HZ);
    maxOCRB_SimonK = (ESC_SIMONK_MAX_PULSE_WIDTH_MS / periodMs) * OCR2A_value;
    minOCRB_SimonK = (ESC_SIMONK_MIN_PULSE_WIDTH_MS / periodMs) * OCR2A_value;
}

void setDutyCycleSimonK(float voltage)
{
    float actualOCRB = ((voltage / ESC_SIMONK_MAX_VOLTAGE) * (maxOCRB_SimonK - minOCRB_SimonK)) + minOCRB_SimonK;
    setDutyCycleTimer2((actualOCRB / OCR2A_value) * 100);
}