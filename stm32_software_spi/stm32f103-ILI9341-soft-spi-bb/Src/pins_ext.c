#include "pins_ext.h"

void tic_delay(uint32_t cycle_count)
{
  uint32_t count=0;
  for(count=0;count<cycle_count;count++)
  {
    __asm volatile ("nop");
  }
}

uint32_t pin_read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  pin_mode(GPIOx, GPIO_Pin, GPIO_MODE_INPUT);

  if(GPIOx->IDR & GPIO_Pin)
  {
    return 1UL;
  }
  else
  {
    return 0UL;
  }
  HAL_GPIO_DeInit(GPIOx, GPIO_Pin);
}

void pin_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int state)
{
  pin_mode(GPIOx, GPIO_Pin, GPIO_MODE_OUTPUT_PP);

  if(state)
  {
    GPIOx->ODR |= GPIO_Pin;
  }
  else
  {
    GPIOx->ODR &= ~GPIO_Pin;
  }
}

void pin_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  pin_mode(GPIOx, GPIO_Pin, GPIO_MODE_OUTPUT_PP);

  GPIOx->ODR ^= GPIO_Pin;
}

void pin_pulse_hi_low(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t delay)
{
  pin_mode(GPIOx, GPIO_Pin, GPIO_MODE_OUTPUT_PP);

  GPIOx->ODR |= GPIO_Pin;
  tic_delay(delay);
  GPIOx->ODR &= ~GPIO_Pin;
  tic_delay(delay);
}

void pin_pulse_low_hi(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t delay)
{
  pin_mode(GPIOx, GPIO_Pin, GPIO_MODE_OUTPUT_PP);

  GPIOx->ODR &= ~GPIO_Pin;
  tic_delay(delay);
  GPIOx->ODR |= GPIO_Pin;
  tic_delay(delay);
}

void pin_mode(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t GPIO_Mode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  // GPIOx Periph clock enable
  if(GPIOx==GPIOA)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
  }
  else if(GPIOx==GPIOB)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
  }
  else if(GPIOx==GPIOC)
  {
    __HAL_RCC_GPIOC_CLK_ENABLE();
  }
  else if(GPIOx==GPIOD)
  {
    __HAL_RCC_GPIOD_CLK_ENABLE();
  }
  else if(GPIOx==GPIOE)
  {
    __HAL_RCC_GPIOE_CLK_ENABLE();
  }
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_Mode;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
