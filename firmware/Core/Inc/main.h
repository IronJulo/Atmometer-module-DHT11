#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32c0xx_hal.h"

void Error_Handler(void);

#define SOCKET_ADDR_Pin GPIO_PIN_0
#define SOCKET_ADDR_GPIO_Port GPIOA
#define LED_STATUS_Pin GPIO_PIN_5
#define LED_STATUS_GPIO_Port GPIOA
#define LED_FAULT_Pin GPIO_PIN_6
#define LED_FAULT_GPIO_Port GPIOA
#define LED_OK_Pin GPIO_PIN_7
#define LED_OK_GPIO_Port GPIOA

#define DHT11_Data_Pin GPIO_PIN_1
#define DHT11_Data_GPIO_Port GPIOA

void microsecond_delay(uint16_t duration);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
