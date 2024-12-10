/*
 * app.c
 *
 *  Created on: Dec 6, 2024
 *      Author: Ash
 */

#include <math.h>
#include "app.h"
#include "main.h"
#include "adc.h"

HeaterBoardSensors sensors;

bool App_longPressDetect(void)
{
    static uint32_t buttonPressStartTime = 0;
	static bool isButtonPressed = false;

	uint8_t status = (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_RESET);
	if(status)
	{
		if (!isButtonPressed)
		{
			isButtonPressed = true;
			buttonPressStartTime = HAL_GetTick();
		}
		else if (HAL_GetTick() - buttonPressStartTime >= LONG_PRESS_DURATION)
		{
			isButtonPressed = false;
			return true;
		}
	}
	else
	{
	   isButtonPressed = false;
	}

	return false;
}

void App_resetBoard(void)
{
	HAL_GPIO_WritePin(GPIOB, GEAR_MOTOR_CTRL_Pin|HEATER_CTRL_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, BOOST_Pin|LED_G_Pin|LED_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(VIBE_MOTOR_CTRL_GPIO_Port, VIBE_MOTOR_CTRL_Pin, GPIO_PIN_SET);
}

void App_shutoffSystem(void)
{
	HAL_SuspendTick();
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	HAL_ResumeTick();
}

void App_activateVibrationMotor()
{
	HAL_GPIO_WritePin(GEAR_MOTOR_CTRL_GPIO_Port, GEAR_MOTOR_CTRL_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(VIBE_MOTOR_CTRL_GPIO_Port, VIBE_MOTOR_CTRL_Pin, GPIO_PIN_RESET);
}

void App_activateGearMotor()
{
	HAL_GPIO_WritePin(VIBE_MOTOR_CTRL_GPIO_Port, VIBE_MOTOR_CTRL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GEAR_MOTOR_CTRL_GPIO_Port, GEAR_MOTOR_CTRL_Pin, GPIO_PIN_SET);
}

float App_getTemperatureC(uint16_t raw)
{
	float voltage = (float)raw * (ADC_REF_VOLTAGE / ADC_12BIT_MAX); //i.e. 4096 for 12Bit ADC

	// Convert voltage to NTC current Resistance using voltage divider formula
	float ntcResistance = RDIV * ((ADC_REF_VOLTAGE/voltage) - 1);

	// Convert NTC resistance to temperature in *C using formula
	float temperatureK = NTC_BETA / (log(ntcResistance / NTC_R25) + (NTC_BETA / NTC_T25));

	float temperatureC =  temperatureK - 273.15;

  return temperatureC;
}

HeaterBoardSensors App_readADC(void)
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
			sensors.temperature.raw = result;
			sensors.temperature.val = App_getTemperatureC(result);
			break;
		case 1:
			sensors.battery.raw = result;
			sensors.battery.val = (result/ADC_12BIT_MAX) * ADC_REF_VOLTAGE * ADC_DIVIDER_RATIO;
			break;
		default:
			break;
		}
	}

	return sensors;
}

HeaterBoardModes App_getMode(void)
{
    HeaterBoardSensors sensors = App_readADC();
    static int countRotCleaner = 0;
    static int countHeater = 0;
    static HeaterBoardModes currentMode = UNKNOWN_MODE;
    const int threshold = 5;

    if (sensors.temperature.raw < 100)
    {
        countRotCleaner++;
        countHeater = 0;

        if (countRotCleaner >= threshold && currentMode != ROTARY_CLEANER_MODE)
        {
            currentMode = ROTARY_CLEANER_MODE;
        }
    }
    else
    {
        countHeater++;
        countRotCleaner = 0;

        if (countHeater >= threshold && currentMode != HEATER_MODE)
        {
            currentMode = HEATER_MODE;
        }
    }

    return currentMode;
}

void App_runRotaryCleaner(void)
{
	static bool rotaryCleanerRunning;
	static uint32_t startTime;

	if(sensors.button && !rotaryCleanerRunning)
	{
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(BOOST_GPIO_Port, BOOST_Pin, GPIO_PIN_SET);
		App_activateGearMotor();
		rotaryCleanerRunning = true;
		startTime = HAL_GetTick();
		sensors.button = false;
	}
	else if(sensors.button)
	{
		sensors.button = false;
	}

	if(rotaryCleanerRunning)
	{
		uint32_t currentTime = HAL_GetTick();
		if(currentTime - startTime > ROTARY_CLEANER_PROCESS_DURATION)
		{
			rotaryCleanerRunning = false;
			App_resetBoard();
			App_shutoffSystem();
		}
	}

	if(App_longPressDetect())
	{
		rotaryCleanerRunning = false;
		App_resetBoard();
		App_shutoffSystem();
	}
}

void App_runHeater(void)
{
	static bool heaterRunning;
	static uint32_t startTime;

	if(sensors.button && !heaterRunning)
	{
		HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(BOOST_GPIO_Port, BOOST_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(HEATER_CTRL_GPIO_Port, HEATER_CTRL_Pin, GPIO_PIN_SET);
		heaterRunning = true;
		startTime = HAL_GetTick();
		sensors.button = false;
	}
	else if(sensors.button)
	{
		sensors.button = false;
	}

	if(heaterRunning)
	{
		if(sensors.temperature.val > TEMPERATURE_THRESHOLD_VIB_MOTOR)
		{
			App_activateVibrationMotor();
		}

		if(sensors.temperature.val < TEMPERATURE_HEATER_LOW_THRESHOLD)
		{
			HAL_GPIO_WritePin(HEATER_CTRL_GPIO_Port, HEATER_CTRL_Pin, GPIO_PIN_SET);
		}
		else if(sensors.temperature.val > TEMPERATURE_HEATER_HIGH_THRESHOLD)
		{
			HAL_GPIO_WritePin(HEATER_CTRL_GPIO_Port, HEATER_CTRL_Pin, GPIO_PIN_RESET);
		}

		uint32_t currentTime = HAL_GetTick();
		if(currentTime - startTime > HEATER_PROCESS_DURATION)
		{
			heaterRunning = false;
			App_resetBoard();
			App_shutoffSystem();
		}
	}
}

void App_setup(void)
{
	App_resetBoard();
	App_shutoffSystem();
}

void App_loop(void)
{
	switch(App_getMode())
	{
		case HEATER_MODE:
			App_runHeater();
			break;

		case ROTARY_CLEANER_MODE:
			App_runRotaryCleaner();
			break;

		default:
			break;
	}

}
