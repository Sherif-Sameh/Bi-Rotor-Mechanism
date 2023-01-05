/* Include Hierarchy */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

#include "StateSpaceController_SL.h"
#include "ESC_BLHeli.h"
#include "ESC_SimonK.h"
#include "LPC_Slave.h"

/* Symbolic Constants */
#define tskStateFeedbackController_PRIORITY     (tskIDLE_PRIORITY + 2)
#define tskLPCReceiveMessage_PRIORITY           (tskIDLE_PRIORITY + 1)
#define LPC_SPI_COMMUNICATION_MODE              0

/* Global Variables */
float referenceAngle[2];
float stateVector[4];
float ESCValues[2];
TaskHandle_t controllerTaskHandle;

void stateFeedbackControllerTask(void *pvParameters)
{
	vTaskDelay(pdMS_TO_TICKS(8000));
	while(1)
	{
		xTaskNotifyWait(0, 0xffffffff, NULL, portMAX_DELAY);
		xTaskNotifyWait(0, 0xffffffff, NULL, portMAX_DELAY);
		updateController(stateVector, ESCValues);
		setDutyCycleSimonK(ESCValues[1]);
		setDutyCycleBLHeli(ESCValues[0]);
	}
}

void LPCReceiveMessagesTask(void *pvParameters)
{
	msg_type_t msgType;
	float outputData[2];
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const uint16_t periodMs = 10;
	while(1)
	{
		msgType = recieve_data();
		get_decoded_data(outputData);
		switch(msgType)
		{
			case MSG_TYPE_PITCH:
			stateVector[0] = outputData[0];
			stateVector[1] = outputData[1];
			xTaskNotify(controllerTaskHandle, ((1 << 0) | (1 << 1)), eSetBits);
			break;
			case MSG_TYPE_ELEVATION:
			stateVector[2] = outputData[0];
			stateVector[3] = outputData[1];
			xTaskNotify(controllerTaskHandle, ((1 << 2) | (1 << 3)), eSetBits);
			break;

			case MSG_TYPE_REFERENCE:
			setReferenceAngles(outputData);
			break;
			default: break;
		}
		if(msgType != MSG_FAILURE)
		{
			xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(periodMs));
		}
	}
}

int main() {
	
	/* Initialize Both ESCs */
	initESC_BLHeli();
	initESC_SimonK();

	/* Initialize the LPC messaging protocol and the SPI driver */
	initialize_LPC(LPC_SPI_COMMUNICATION_MODE);
	
	xTaskCreate(stateFeedbackControllerTask, "SF Control", 256, NULL, tskStateFeedbackController_PRIORITY, &controllerTaskHandle);
	xTaskCreate(LPCReceiveMessagesTask, "ReceivingData", 256, NULL, tskLPCReceiveMessage_PRIORITY, NULL);
	sei();
	vTaskStartScheduler();
	while(1);
}
