/*
 Created By: Sherif Sameh

 Brief Description: This sketch, Thrust_Test, was written to test the magnitude of the thrust force
 generated when combining the EMAX A2212 1400Kv Brushless DC Motor with a 1045 propeller while monitoring
 the current that is being pulled by the motor from the power supply to find its safe operating point and
 to calculate a thrust force constant for the motor+propeller combo for any applied voltage.
 
 Code Description: The Arduino Servo library is used to send the control signals needed to the ESC that
 drives the motor as the ESC accepts the exact same signal for its control as that used by servo motors,
 where a 1ms Pulse equates to 0 deg and 2ms to 180 deg. To change the duty cycle to a known set value, a
 push button is used to increment the duty cycle while another push button is used to stop the motor instantly.

*/
#define PB1 PINC0
#define PB2 PINC1

#define ESC_DUTY_CYCLE_0 0
#define ESC_DUTY_CYCLE_20 36
#define ESC_DUTY_CYCLE_40 72
#define ESC_DUTY_CYCLE_60 108
#define ESC_DUTY_CYCLE_70 126
#define ESC_DUTY_CYCLE_80 144
#define ESC_DUTY_CYCLE_90 162
#define ESC_DUTY_CYCLE_100 180

#include <Servo.h>

Servo BLDC_ESC; // Create servo object to be controlled by the library
int dutyCycles[] = {ESC_DUTY_CYCLE_0, ESC_DUTY_CYCLE_20, ESC_DUTY_CYCLE_40, ESC_DUTY_CYCLE_60,
ESC_DUTY_CYCLE_70, ESC_DUTY_CYCLE_80, ESC_DUTY_CYCLE_90, ESC_DUTY_CYCLE_100}; // Given to servo library
int dutyCyclesNumbers[] = {0, 20, 40, 60, 70, 80, 90, 100}; // Used soley for printing to serial monitor
int currDutyCycle = ESC_DUTY_CYCLE_0;
unsigned char PB1Old, PB2Old, i;

void setup()
{
  DDRC = 0; // All inputs
  PB1Old = (PINC >> 0) & (1 << 0);
  PB2Old = (PINC >> 1) & (1 << 0);
  
  BLDC_ESC.attach(9,1000,2000); // PWM signal output onto pin 9

  Serial.begin(115200);
}

void loop()
{
  // Check on status of both PushButtons
  unsigned char PB1New = (PINC >> 0) & (1 << 0);
  if( PB1New && (PB1New != PB1Old) )
  {
    delay(40);
    if( PB1New && (PB1New != PB1Old) )
    {
      i++;
    }
  }
  unsigned char PB2New = (PINC >> 1) & (1 << 0);
  if( PB2New && (PB2New != PB2Old) )
  {
    delay(40);
    if( PB2New && (PB2New != PB2Old) )
    {
      i = 0;
    }
  }
  
  // Update the current duty cycle and send PWM signal to ESC
  if( i > 7)
  {
    i = 0;
  }
  currDutyCycle = dutyCycles[i];
  BLDC_ESC.write(currDutyCycle);
  Serial.print("Current Duty Cycle is ");
  Serial.println(dutyCyclesNumbers[i]);

  // Update old status of both PushButtons
  PB1Old = PB1New;
  PB2Old = PB2New;
}
