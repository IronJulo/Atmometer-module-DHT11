/*
 * module_register.h
 *
 *  Created on: Jun 22, 2024
 *      Author: jules
 */

#ifndef INC_MODULE_REGISTER_H_
#define INC_MODULE_REGISTER_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*
Read only registers 0x00 <-> 0x80

0x00:0x01 [sensorTypeMSB][sensorTypeLSB]										// Sensor type on 16 bits
0x02:0x05 [sensorIDMSB1][sensorIDMSB2][sensorIDLSB1][sensorIDLSB1]				// Sensor ID on 32 bits
0x06:0x09 [sensorValueMSB1][sensorValueMSB2][sensorValueLSB1][sensorValueLSB1]	// Sensor value on 32 bit
0x0A:0x0B [sensorErrorMSB][sensorErrorLSB]										// Bit field that represent which error occured
																				//      0:   no error
																				//      1:
																				//      2:
																				//      4:
																				//      8:
																				//      16:
																				//      32:
																				//      64:
																				//      128:

Read Write registers 0x81 <-> 0xFF

0x81:0x81 [sensorConf] -> [readInProgress][idle][autoIncrement]...				// Sit field used to manage sensor
																				// readInProgress:
																				// set this value before reading data from sensor to prevent byte mangling
																				//      0: sensor can update value
																				//      1: sensor can't update value
																				//
																				// idle:
																				// set the device in power saving mode
																				//      0: sensor operate normally
																				//      1: sensor only listen on I2c line to consume less power
0x82:0x82 [sensorValueUpdatePeriod]												// Time between each value update of the sensor
0x83:0x83 [sensorReadAddress]
*/

typedef enum
{
	// Read Only
	SENSOR_TYPE_MSB,
	SENSOR_TYPE_LSB,

	SENSOR_ID_MSB1,
	SENSOR_ID_MSB2,
	SENSOR_ID_LSB1,
	SENSOR_ID_LSB2,

	SENSOR_VALUE_1_MSB1,
	SENSOR_VALUE_1_MSB2,
	SENSOR_VALUE_1_LSB1,
	SENSOR_VALUE_1_LSB2,

	SENSOR_ERROR_MSB,
	SENSOR_ERROR_LSB,

	SENSOR_VALUE_2_MSB1,
	SENSOR_VALUE_2_MSB2,
	SENSOR_VALUE_2_LSB1,
	SENSOR_VALUE_2_LSB2,

    // Read/Write
	SENSOR_CONFIG = 0X81,
	SENSOR_UPDATE_PERIOD,
	SENSOR_READ_ADDRESS,

} Register;

#define REGISTER_READ_ONLY_BEGIN	SENSOR_TYPE_MSB
#define REGISTER_READ_ONLY_END		SENSOR_ERROR_LSB

#define REGISTER_READ_WRITE_BEGIN	SENSOR_CONFIG
#define REGISTER_READ_WRITE_END		SENSOR_READ_ADDRESS

#define REGISTER_MAX_VALUE			SENSOR_READ_ADDRESS

typedef enum
{
	CBF_READ_IN_PROGRESS,
	CBF_IDLE,
	CBF_SEQUENTIAL_READ,
} RegisterSensorConfigBitField;

#define REGISTER_ERROR_OK			(0 << 0)
#define REGISTER_ERROR_OUT_OF_BOUND	(1 << 1)
#define REGISTER_ERROR_READ_ONLY	(1 << 2)

uint8_t write_register(uint8_t address, uint8_t value);
uint8_t read_register(uint8_t address, uint8_t *value);
uint8_t read_selected_register(uint8_t *value);

void set_sensor_type(uint16_t value);
void set_sensor_id(uint32_t value);
void set_sensor_config_readInProgress(bool value);
void set_sensor_config_idle(bool value);
void set_sensor_config_sequentialRead(bool value);
void set_sensor_updatePeriod(uint8_t value);
void set_sensor_error(uint16_t value);
void set_sensor_value_1(uint32_t value);
void set_sensor_value_2(uint32_t value);
void set_sensor_read_address(uint8_t value);

uint16_t get_sensor_type();
uint32_t get_sensor_id();
bool get_sensor_config_ReadInProgress();
bool get_sensor_config_Idle();
bool get_sensor_config_SequentialRead();
uint8_t get_sensor_updatePeriod();
uint16_t get_sensor_error();
uint32_t get_sensor_value_1();
uint8_t get_sensor_read_address();


#endif /* INC_MODULE_REGISTER_H_ */
