/*
 * app.h
 *
 *  Created on: Dec 6, 2024
 *      Author: Ash
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include <stdbool.h>
#include <stdint.h>

#define ADC_12BIT_MAX 4096.0f
#define ADC_REF_VOLTAGE 3.3f
#define ADC_DIVIDER_RATIO 2.0f
#define NTC_BETA 3892.0f		//Beta value constant
#define RDIV 50000.0f	//Resistance (ohms) of Divider
#define NTC_R25 50000.0f		//Resistance (ohms) at 25*C
#define NTC_T25 298.15f

#define HEATER_PROCESS_DURATION (7 * (1000 * 60) ) // 7 minutes
#define ROTARY_CLEANER_PROCESS_DURATION (7 * (1000 * 60) ) // 7 minutes
#define LONG_PRESS_DURATION (5 * (1000 * 60) ) // 5 minutes

#define TEMPERATURE_THRESHOLD_VIB_MOTOR 30.0f // in Celcius
#define TEMPERATURE_HEATER_LOW_THRESHOLD 40.0f // in Celcius
#define TEMPERATURE_HEATER_HIGH_THRESHOLD 70.0f // in Celcius

typedef enum
{
	HEATER_MODE,
	ROTARY_CLEANER_MODE,
	UNKNOWN_MODE
} HeaterBoardModes;

typedef struct
{
	uint16_t raw;
	float val;
} SensorsADC;

typedef struct
{
	SensorsADC temperature;
	SensorsADC battery;
	bool button;
} HeaterBoardSensors;

void App_setup(void);
void App_loop(void);

#endif /* INC_APP_H_ */
