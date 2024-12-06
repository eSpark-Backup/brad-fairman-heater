/*
 * app.c
 *
 *  Created on: Dec 6, 2024
 *      Author: Ash
 */

#include "app.h"
#include "main.h"
#include "adc.h"

HeaterBoardSensors sensors;

float Read_NTC10k(uint16_t tempRaw)
{
//	float voltage = (float)tempRaw * (VREF / ADC_12BIT_MAX); //i.e. 4096 for 12Bit ADC
//
//	// Convert voltage to NTC current Resistance using voltage divider formula
//	float NTC_res = RDIV * ((VREF/voltage) - 1);
//
//	// Convert NTC resistance to temperature in *C using formula
//	float temperature_k = BETA / (log(NTC_res / R25) + (BETA / 298.15));
//
//	float temperature =  temperature_k - 273.15;


  return tempRaw;
}

HeaterBoardSensors readADC(void)
{

	uint16_t result = 0;

	for(int i=0; i<2; i++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 10);
		result = HAL_ADC_GetValue(&hadc1);
		switch(i)
		{
		case 0:
			sensors.temperature = Read_NTC10k(result);
			break;
		case 1:
			sensors.battery = (result/ADC_12BIT_MAX) * ADC_REF_VOLTAGE * ADC_DIVIDER_RATIO;
			break;
		default:
			break;
		}
	}

	return sensors;
}

HeaterBoardModes getMode(void)
{
	HeaterBoardSensors sensors = readADC();
	if(sensors.temperature < 100)
	{
		return ROTARY_CLEANER_MODE;
	}

	return HEATER_MODE;
}


void appSetup(void)
{
}

void runRotaryCleaner(void)
{
	static bool rotaryCleanerRunning;
	static uint32_t startTime;

	if(sensors.button)
	{
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(HEATER_CTRL_GPIO_Port, HEATER_CTRL_Pin, GPIO_PIN_SET);
		rotaryCleanerRunning = 1;
		startTime = HAL_GetTick();
		sensors.button = 0;
	}

	if(rotaryCleanerRunning)
	{
		uint32_t currentTime = HAL_GetTick();
		if(currentTime - startTime > HEATER_PROCESS_DURATION)
		{
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GEAR_MOTOR_CTRL_GPIO_Port, GEAR_MOTOR_CTRL_Pin, GPIO_PIN_RESET);
			rotaryCleanerRunning = 0;
		}
	}
}

void runHeater(void)
{
	static bool heaterRunning;
	static uint32_t startTime;

	if(sensors.button)
	{
		HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(HEATER_CTRL_GPIO_Port, HEATER_CTRL_Pin, GPIO_PIN_SET);
		heaterRunning = 1;
		startTime = HAL_GetTick();
		sensors.button = 0;
	}

	if(heaterRunning)
	{
		uint32_t currentTime = HAL_GetTick();
		if(currentTime - startTime > HEATER_PROCESS_DURATION)
		{
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(HEATER_CTRL_GPIO_Port, HEATER_CTRL_Pin, GPIO_PIN_RESET);
			heaterRunning = 0;
		}
	}
}


void appLoop(void)
{
	switch(getMode())
	{
		case HEATER_MODE:
			runHeater();
			break;

		case ROTARY_CLEANER_MODE:
			runRotaryCleaner();
			break;

		default:
			break;
	}

}
