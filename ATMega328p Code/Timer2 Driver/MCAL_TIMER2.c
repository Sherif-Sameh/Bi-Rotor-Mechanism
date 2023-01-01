
#include "MCAL_TIMER2.h"

/* Global Variables */
static TIMER2_PWM_COM currentWorkingMode = TIMER2_PWM_NON_INVERTED;

/* Static Function Prototypes */
static uint8_t calcPrescalarTimer2(uint32_t pwmFrequencyHz);

/* Function Definitions */
static uint8_t calcPrescalarTimer2(uint32_t pwmFrequencyHz)
{
    uint32_t OCRA_Value = 0;

    if((CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 1)) < 256)
    {
        TCCR2B = TIMER2_N_1;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 1);
    }
    else if((CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 8)) < 256)
    {
        TCCR2B = TIMER2_N_8;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 8);
    }
    else if((CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 32)) < 256)
    {
        TCCR2B = TIMER2_N_32;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 32);
    }
    else if((CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 64)) < 256)
    {
        TCCR2B = TIMER2_N_64;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 64);
    }
    else if((CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 128)) < 256)
    {
        TCCR2B = TIMER2_N_128;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 128);
    }
    else if((CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 256)) < 256)
    {
        TCCR2B = TIMER2_N_256;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 256);
    }
    else
    {
        TCCR2B = TIMER2_N_1024;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 1024);
    }
    return ((uint8_t) OCRA_Value); 
}

void initTimer2(uint32_t pwmFrequencyHz, TIMER2_PWM_COM workingMode)
{
    if(pwmFrequencyHz < (CPU_CLK_FREQ_HZ / (((uint32_t) 1024) * 510)))
    {
        return;             /*!< Frequencies less than this are impossible to achieve*/
    }

    DDRD |= (1 << OCR2B_PIN);
    OCR2A = calcPrescalarTimer2(pwmFrequencyHz);
    TCCR2B |= (1 << WGM22);
    currentWorkingMode = workingMode; 
    if(workingMode == TIMER2_PWM_NON_INVERTED)
    {
        TCCR2A = (1 << WGM20) | (1 << COM2B1);
        OCR2B = 0;
    }
    else
    {
        TCCR2A = (1 << WGM20) | (1 << COM2B0) | (1 << COM2B1);
        OCR2B = OCR2A;
    }
}

void disableTimer2()
{
    DDRD &= ~(1 << OCR2B_PIN);
    OCR2A = 0;
    OCR2B = 0;
    TCCR2A = 0;
    TCCR2B = 0;
}

void setDutyCycleTimer2(float dutyCycle)
{
    uint8_t OCRB_value = (uint8_t)((dutyCycle / 100) * OCR2A);  
    if(currentWorkingMode == TIMER2_PWM_NON_INVERTED)
    {
        OCR2B = OCRB_value;
    }
    else
    {
        OCR2B = (OCR2A - OCRB_value);
    }
}