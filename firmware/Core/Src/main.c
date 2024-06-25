#include "main.h"

#include "module_register.h"
#include "module_address.h"
#include "i2c_slave.h"
#include "module_available.h"

#include "DHT11.h"

#define MODULE_TYPE MODULE_TYPE_DHT11_H
#define MODULE_ID MODULE_ADDRESS_NOT_CONFIGURED

ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;

uint8_t  module_slave_address = MODULE_ADDRESS_NOT_CONFIGURED;
uint32_t global_adc_readout = 0;
uint64_t global_counter = 0;

DHT11_data dht11_data;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);

void flash_slave_address(uint8_t address);

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_TIM1_Init();

	HAL_Delay(1000);

	module_slave_address = get_device_address_stable(&hadc1);

	flash_slave_address(module_slave_address);

	module_slave_address <<= 1; // Allow space for read/write bit

	HAL_Delay(1000);

	MX_I2C1_Init();

	HAL_Delay(1000);

	if (HAL_TIM_Base_Start(&htim1) != HAL_OK)      Error_Handler();
	if (HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK) Error_Handler();

	set_sensor_config_sequentialRead(true);
	set_sensor_type(MODULE_TYPE);
	set_sensor_id(MODULE_ID);

	dht11_data.GPIO_Pin = DHT11_Data_Pin;
	dht11_data.GPIO_Port = DHT11_Data_GPIO_Port;

	while (! false)
	{
		global_counter++;
		set_sensor_value_1(global_counter);
		DHT_GetData(&dht11_data);

		HAL_Delay(1000);
	}
}



void flash_slave_address(uint8_t address)
{

	HAL_GPIO_WritePin(LED_OK_GPIO_Port, LED_OK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_FAULT_GPIO_Port, LED_FAULT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin, GPIO_PIN_RESET);

	for (int var = 0; var < address; ++var) {
		HAL_GPIO_WritePin(LED_OK_GPIO_Port, LED_OK_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(LED_OK_GPIO_Port, LED_OK_Pin, GPIO_PIN_RESET);
		HAL_Delay(500);
	}

	HAL_GPIO_WritePin(LED_OK_GPIO_Port, LED_OK_Pin, GPIO_PIN_RESET);
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV4;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) Error_Handler();
}

static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;

  if (HAL_ADC_Init(&hadc1) != HAL_OK) Error_Handler();

  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;

  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) Error_Handler();
}

static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x40000A0B;
  hi2c1.Init.OwnAddress1 = module_slave_address;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(&hi2c1) != HAL_OK) Error_Handler();
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) Error_Handler();
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) Error_Handler();
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, LED_STATUS_Pin|LED_FAULT_Pin|LED_OK_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DHT11_Data_GPIO_Port, DHT11_Data_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = LED_STATUS_Pin|LED_FAULT_Pin|LED_OK_Pin|DHT11_Data_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = TIM1_PRESCALER;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = TIM1_ARR;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_Base_Init(&htim1) != HAL_OK) Error_Handler();

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) Error_Handler();

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) Error_Handler();

}

void microsecond_delay(uint16_t duration)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);                 // set the counter value to 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < duration); // wait for the counter to reach the us input in the parameter
}


void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
