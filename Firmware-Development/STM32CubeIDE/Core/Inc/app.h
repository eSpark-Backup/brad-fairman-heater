/*
 * app.h
 *
 *  Created on: Dec 6, 2024
 *      Author: Ash
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include <stdbool.h>

#define ADC_12BIT_MAX 4095.0f
#define ADC_REF_VOLTAGE 3.3f
#define ADC_DIVIDER_RATIO 2.0f
#define HEATER_PROCESS_DURATION (7 * (1000 * 60) )

typedef enum
{
	HEATER_MODE,
	ROTARY_CLEANER_MODE
} HeaterBoardModes;

typedef struct
{
	bool button;
	float temperature;
	float battery;
} HeaterBoardSensors;

void appSetup(void);
void appLoop(void);

#endif /* INC_APP_H_ */
