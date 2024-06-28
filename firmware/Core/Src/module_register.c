/*
 * module_register.c
 *
 *  Created on: Jun 22, 2024
 *      Author: jules
 */
#include "module_register.h"


volatile uint8_t module_registers[REGISTER_MAX_VALUE] = {0};

uint8_t write_register(uint8_t address, uint8_t value)
{
	if (address < REGISTER_READ_WRITE_BEGIN)
		return REGISTER_ERROR_READ_ONLY;
	if (address > REGISTER_READ_WRITE_END)
		return REGISTER_ERROR_OUT_OF_BOUND;

	module_registers[address] = value;
	return REGISTER_ERROR_OK;
}

uint8_t read_register(uint8_t address, uint8_t *value)
{
	if (address >= REGISTER_MAX_VALUE)
		return REGISTER_ERROR_OUT_OF_BOUND;

	*value = module_registers[address];
	return REGISTER_ERROR_OK;
}

uint8_t read_selected_register(uint8_t *value)
{
	uint8_t address = module_registers[SENSOR_READ_ADDRESS];
	uint8_t error = read_register(address, value);

	if(error != REGISTER_ERROR_OK)
		return error;
	if (get_sensor_config_SequentialRead())
		module_registers[SENSOR_READ_ADDRESS]++;
	return REGISTER_ERROR_OK;
}

void set_sensor_type(uint16_t value)
{
	module_registers[SENSOR_TYPE_MSB] = ((value & 0xff00) >> 8);
	module_registers[SENSOR_TYPE_LSB] = ((value & 0x00FF) >> 0);
}
void set_sensor_id(uint32_t value)
{
	module_registers[SENSOR_ID_MSB1] = ((value & 0xff000000) >> 24);
	module_registers[SENSOR_ID_MSB2] = ((value & 0x00ff0000) >> 16);
	module_registers[SENSOR_ID_LSB1] = ((value & 0x0000ff00) >>  8);
	module_registers[SENSOR_ID_LSB2] = ((value & 0x000000ff) >>  0);
}
void set_sensor_config_readInProgress(bool value)
{
	uint8_t reg_value = module_registers[SENSOR_CONFIG];

	if (value)
		reg_value |= (1 << CBF_READ_IN_PROGRESS);
	else
		reg_value &= ~(1 << CBF_READ_IN_PROGRESS);
	module_registers[SENSOR_CONFIG] = reg_value;
}

void set_sensor_config_idle(bool value)
{
	uint8_t reg_value = module_registers[SENSOR_CONFIG];

	if (value)
		reg_value |= (1 << CBF_IDLE);
	else
		reg_value &= ~(1 << CBF_IDLE);

	module_registers[SENSOR_CONFIG] = reg_value;
}

void set_sensor_config_sequentialRead(bool value)
{
	uint8_t reg_value = module_registers[SENSOR_CONFIG];

	if (value)
		reg_value |= (1 << CBF_SEQUENTIAL_READ);
	else
		reg_value &= ~(1 << CBF_SEQUENTIAL_READ);

	module_registers[SENSOR_CONFIG] = reg_value;
}


void set_sensor_updatePeriod(uint8_t value)
{
	module_registers[SENSOR_UPDATE_PERIOD] = value;
}
void set_sensor_error(uint16_t value)
{
	module_registers[SENSOR_ERROR_MSB] = ((value & 0xff00) >> 8);
	module_registers[SENSOR_ERROR_LSB] = ((value & 0x00FF) >> 0);
}

void set_sensor_value_1(uint32_t value)
{
	module_registers[SENSOR_VALUE_1_MSB1] = ((value & 0xff000000) >> 24);
	module_registers[SENSOR_VALUE_1_MSB2] = ((value & 0x00ff0000) >> 16);
	module_registers[SENSOR_VALUE_1_LSB1] = ((value & 0x0000ff00) >>  8);
	module_registers[SENSOR_VALUE_1_LSB2] = ((value & 0x000000ff) >>  0);
}

void set_sensor_value_2(uint32_t value)
{
	module_registers[SENSOR_VALUE_2_MSB1] = ((value & 0xff000000) >> 24);
	module_registers[SENSOR_VALUE_2_MSB2] = ((value & 0x00ff0000) >> 16);
	module_registers[SENSOR_VALUE_2_LSB1] = ((value & 0x0000ff00) >>  8);
	module_registers[SENSOR_VALUE_2_LSB2] = ((value & 0x000000ff) >>  0);
}

void set_sensor_read_address(uint8_t value)
{
	module_registers[SENSOR_READ_ADDRESS] = value;
}

uint16_t get_sensor_type()
{
	uint16_t result = 0;
	result = module_registers[SENSOR_ERROR_MSB];
	result <<= 8;
	result |= module_registers[SENSOR_ERROR_LSB];
	return result;
}

uint32_t get_sensor_id()
{
	uint32_t result = 0;
	result = module_registers[SENSOR_ID_MSB1];
	result <<= 8;
	result |= module_registers[SENSOR_ID_MSB2];
	result <<= 8;
	result |= module_registers[SENSOR_ID_LSB1];
	result <<= 8;
	result |= module_registers[SENSOR_ID_LSB2];
	return result;
}
bool get_sensor_config_ReadInProgress()
{
	return((module_registers[SENSOR_CONFIG] >> CBF_READ_IN_PROGRESS) & 0x01);
}
bool get_sensor_config_Idle()
{
	return((module_registers[SENSOR_CONFIG] >> CBF_IDLE) & 0x01);
}
bool get_sensor_config_SequentialRead()
{
	return((module_registers[SENSOR_CONFIG] >> CBF_SEQUENTIAL_READ) & 0x01);
}
uint8_t get_sensor_updatePeriod()
{
	return module_registers[SENSOR_UPDATE_PERIOD];
}
uint16_t get_sensor_error()
{
	uint16_t result = 0;
	result = module_registers[SENSOR_ERROR_MSB];
	result <<= 8;
	result |= module_registers[SENSOR_ERROR_LSB];
	return result;
}

uint32_t get_sensor_value_1()
{
	uint32_t result = 0;

	result = module_registers[SENSOR_VALUE_1_MSB1];
	result <<= 8;
	result |= module_registers[SENSOR_VALUE_1_MSB2];
	result <<= 8;
	result |= module_registers[SENSOR_VALUE_1_LSB1];
	result <<= 8;
	result |= module_registers[SENSOR_VALUE_1_LSB2];

	return result;
}

uint8_t get_sensor_read_address()
{
	return module_registers[SENSOR_READ_ADDRESS];
}
