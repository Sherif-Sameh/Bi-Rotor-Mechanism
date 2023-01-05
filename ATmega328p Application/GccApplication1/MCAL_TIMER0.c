
#include "MCAL_TIMER0.h"

/* Global Variables */
static TIMER0_PWM_COM currentWorkingMode = TIMER0_PWM_NON_INVERTED;

/* Static Function Prototypes */
static uint8_t calcPrescalarTimer0(uint32_t pwmFrequencyHz);

/* Function Definitions */

uint8_t getOCRATimer0()
{
    return OCR0A;
}

static uint8_t calcPrescalarTimer0(uint32_t pwmFrequencyHz)
{
    uint32_t OCRA_Value = 0;

    if((CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 1)) < 256)
    {
        TCCR0B = TIMER0_N_1;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 1);
    }
    else if((CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 8)) < 256)
    {
        TCCR0B = TIMER0_N_8;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 8);
    }
    else if((CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 64)) < 256)
    {
        TCCR0B = TIMER0_N_64;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 64);
    }
    else if((CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 256)) < 256)
    {
        TCCR0B = TIMER0_N_256;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 256);
    }
    else
    {
        TCCR0B = TIMER0_N_1024;
        OCRA_Value = CPU_CLK_FREQ_HZ / (pwmFrequencyHz * 2 * 1024);
    }
    return ((uint8_t) OCRA_Value); 
}

void initTimer0(uint32_t pwmFrequencyHz, TIMER0_PWM_COM workingMode)
{
    if(pwmFrequencyHz < (CPU_CLK_FREQ_HZ / (((uint32_t) 1024) * 510)))
    {
        return;             /*!< Frequencies less than this are impossible to achieve*/
    }

    DDRD |= (1 << OCR0B_PIN); 
    OCR0A = calcPrescalarTimer0(pwmFrequencyHz);
    TCCR0B |= (1 << WGM02); 
    currentWorkingMode = workingMode; 
    if(workingMode == TIMER0_PWM_NON_INVERTED)
    {
        TCCR0A = (1 << WGM00) | (1 << COM0B1);
        OCR0B = 0;
    }
    else
    {
        TCCR0A = (1 << WGM00) | (1 << COM0B0) | (1 << COM0B1);
        OCR0B = OCR0A;
    }
}

void disableTimer0()
{
    DDRD &= ~(1 << OCR0B_PIN);
    OCR0A = 0;
    OCR0B = 0;
    TCCR0A = 0;
    TCCR0B = 0;
}

void setDutyCycleTimer0(float dutyCycle)
{
    uint8_t OCRB_value;
    float actualOCRB = ((dutyCycle / 100) * OCR0A);
    if(actualOCRB > OCR0A)
    {
        OCRB_value = OCR0A - 2;
    }
    else
    {
        OCRB_value = ((uint8_t) actualOCRB);
    }

    if(currentWorkingMode == TIMER0_PWM_NON_INVERTED)
    {
        OCR0B = OCRB_value;
    }
    else
    {
        OCR0B = (OCR0A - OCRB_value);
    }
}
