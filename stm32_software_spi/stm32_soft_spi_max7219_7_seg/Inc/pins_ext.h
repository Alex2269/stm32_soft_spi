/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __pins_ext_H
#define __pins_ext_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

void tic_delay(uint32_t cycle_count);
uint32_t pin_read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void pin_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int state);
void pin_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void pin_pulse_hi_low(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t delay);
void pin_pulse_low_hi(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t delay);
void pin_mode(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t GPIO_Mode);

// void HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin);
// mode: GPIO_MODE_INPUT;GPIO_MODE_OUTPUT_PP;GPIO_MODE_OUTPUT_OD; "pull push"/"open drain"

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */
