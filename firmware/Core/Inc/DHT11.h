/*
 * DHT11.h
 *
 *  Created on: May 9, 2024
 *      Author: Jules
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "main.h"

typedef struct
{
	GPIO_TypeDef *GPIO_Port;
	uint16_t GPIO_Pin;

	uint8_t int_humidity;
	uint8_t dec_humidity;
	uint8_t int_temperature;
	uint8_t dec_temperature;

	float humidity;
	float temperature;

} DHT11_data;

void DHT_GetData(DHT11_data *data);
void DHT11_Start(DHT11_data *data);
uint8_t DHT11_Read(DHT11_data *data);
uint8_t DHT11_Check_Response(DHT11_data *data);
void Set_Pin_Output(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin);
void Set_Pin_Input(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin);

#endif /* INC_DHT11_H_ */
