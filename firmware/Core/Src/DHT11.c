/*
 * DHT11.c
 *
 *  Created on: May 9, 2024
 *      Author: Jules
 */

#include "DHT11.h"

#define PIN_WAIT_TIMEOUT 100

void Set_Pin_Output(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIO_Port, &GPIO_InitStruct);
}

void Set_Pin_Input(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIO_Port, &GPIO_InitStruct);
}

void DHT11_Start(DHT11_data *data)
{
	Set_Pin_Output(data->GPIO_Port, data->GPIO_Pin);        // set the pin as output
	HAL_GPIO_WritePin(data->GPIO_Port, data->GPIO_Pin, 0);  // pull the pin low
	microsecond_delay(18000);                               // wait for 18ms
	HAL_GPIO_WritePin (data->GPIO_Port, data->GPIO_Pin, 1); // pull the pin high
	microsecond_delay(20);
	Set_Pin_Input(data->GPIO_Port, data->GPIO_Pin);         // set as input
}

uint8_t DHT11_Check_Response(DHT11_data *data)
{
	uint8_t Response = 0;
	microsecond_delay(40);
	if (!(HAL_GPIO_ReadPin (data->GPIO_Port, data->GPIO_Pin)))
	{
		microsecond_delay(80);
		if ((HAL_GPIO_ReadPin (data->GPIO_Port, data->GPIO_Pin))) Response = 1;
		else Response = -1;
	}
	uint8_t counter = 0;
	while ((HAL_GPIO_ReadPin (data->GPIO_Port, data->GPIO_Pin)) && counter < PIN_WAIT_TIMEOUT);   // wait for the pin to go low

	return Response;
}

uint8_t DHT11_Read(DHT11_data *data)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		uint8_t counter = 0;
		while (!(HAL_GPIO_ReadPin (data->GPIO_Port, data->GPIO_Pin)) && counter < PIN_WAIT_TIMEOUT) // wait for the pin to go high
			counter++;
		microsecond_delay(40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (data->GPIO_Port, data->GPIO_Pin)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else // if the pin is high
		{
			i|= (1<<(7-j));  // write 1
		}
		counter = 0;
		while ((HAL_GPIO_ReadPin (data->GPIO_Port, data->GPIO_Pin)) && counter < PIN_WAIT_TIMEOUT) // wait for the pin to go low
			counter++;
	}
	return i;
}

void DHT_GetData(DHT11_data *data)
{
    DHT11_Start (data);
    uint8_t presence = DHT11_Check_Response (data);
    (void) presence;

	data->int_humidity = DHT11_Read (data);
	data->dec_humidity = DHT11_Read (data);
	data->int_temperature = DHT11_Read (data);
	data->dec_temperature = DHT11_Read (data);

	uint8_t checksum = DHT11_Read(data);

	if (checksum == (data->int_humidity + data->dec_humidity + data->int_temperature + data->dec_temperature))
	{
		data->temperature = data->int_temperature;
		data->temperature += (float)data->dec_temperature/10;
		data->humidity = data->int_humidity;
		data->humidity += (float)data->dec_humidity/10;
	}
}

