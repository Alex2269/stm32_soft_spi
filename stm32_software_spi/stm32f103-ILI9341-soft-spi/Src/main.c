/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "pins_ext.h"
#include "soft_spi.h"
#include <math.h>
//#include <stdlib.h>
#include "utoa.h"
#include "ILI9341.h"

#define M_PI 3.14159

uint32_t adcResult = 0;
uint32_t max_result = 0;
float real_result[320] = {0};

uint16_t adc_counter = 0;
uint16_t i = 0;

uint16_t coord_x[320] = {0};
uint16_t coord_y[320] = {0};

uint32_t tim_counter = 0;
void fill_text(uint16_t i);

void fill_buff(uint16_t i)
{
  if(i>318)
  {
    max_result = 0;
    for(uint16_t i = 0; i < 320; i++)
    {
    //ILI9341_fillCircle(coord_x[i], coord_y[i], 2, ILI9341_BLACK);
    //ILI9341_drawLine(coord_x[i], coord_y[i], coord_x[i-1], coord_y[i-1], ILI9341_BLACK); //(  x0,y0,x1,y1,color);
    //ILI9341_drawLine(coord_x[i], coord_y[i]+1, coord_x[i-1], coord_y[i-1]+1, ILI9341_BLACK); //(  x0,y0,x1,y1,color);
    ILI9341_drawLine(coord_x[i], coord_y[i]-1, coord_x[i-1], coord_y[i-1]-1, ILI9341_BLACK); //(  x0,y0,x1,y1,color);
    //ILI9341_drawLine(coord_x[i]+1, coord_y[i], coord_x[i-1]+1, coord_y[i-1], ILI9341_BLACK); //(  x0,y0,x1,y1,color);
    ILI9341_drawLine(coord_x[i]-1, coord_y[i], coord_x[i-1]-1, coord_y[i-1], ILI9341_BLACK); //(  x0,y0,x1,y1,color);
    }

    //ILI9341_fillScreen(ILI9341_BLACK);
    for(uint16_t i = 5; i<320; i+=23) ILI9341_drawLine( i, 10, i, 230, ILI9341_WHITE); //draw vertical lines
    for(uint16_t i = 10; i<240; i+=22) ILI9341_drawLine( 10, i, 300, i, ILI9341_WHITE); //draw horizontal lines
    ILI9341_drawLine( 10, 119, 300, 119, ILI9341_WHITE); //draw horizontal lines    
    //ILI9341_drawLine( 10, 121, 300, 121, ILI9341_WHITE); //draw horizontal lines
    fill_text(i);
  }

  while(adc_counter < 128) // Пока количество измерений меньше N
  {
    adcResult += HAL_ADC_GetValue(&hadc1); // Сумируем полученные значения ацп
    adc_counter++; // Увеличиваем счетчик измерений
  }

  coord_x[i] = i; // сохраняем координаты <cord_x/y> в буфер для последующей очистки дисплея
  coord_y[i] = 230-adcResult/adc_counter/18; // Вычисляем среднее значение
  real_result[i] = adcResult/adc_counter*0.82; // 3300/4096=0.80566 Вычисляем среднее значение
  
  if(real_result[i]<500) real_result[i] = 0;
  if(real_result[i]>500) real_result[i] -= 500;
  real_result[i] *= .4; // значение в барах

  
  adcResult = 0; // Обнуляем значение
  adc_counter = 0; // Обнуляем счетчик измерений
  if(real_result[i]>max_result) max_result = real_result[i]; // сохранение максимального числа
}

void write_graph(uint16_t i)
{
  if(i) // draw values
  {
    fill_text(i);
    //ILI9341_fillCircle(coord_x[i], coord_y[i], 2, ILI9341_YELLOW);
    //ILI9341_drawLine(coord_x[i], coord_y[i], coord_x[i-1], coord_y[i-1], ILI9341_CYAN); //(  x0,y0,x1,y1,color);
    //ILI9341_drawLine(coord_x[i], coord_y[i]+1, coord_x[i-1], coord_y[i-1]+1, ILI9341_GREEN); //(  x0,y0,x1,y1,color);
    ILI9341_drawLine(coord_x[i], coord_y[i]-1, coord_x[i-1], coord_y[i-1]-1, ILI9341_CYAN); //(  x0,y0,x1,y1,color);
    //ILI9341_drawLine(coord_x[i]+1, coord_y[i], coord_x[i-1]+1, coord_y[i-1], ILI9341_GREEN); //(  x0,y0,x1,y1,color);
    ILI9341_drawLine(coord_x[i]-1, coord_y[i], coord_x[i-1]-1, coord_y[i-1], ILI9341_CYAN); //(  x0,y0,x1,y1,color);
  }
}

void fill_text(uint16_t i)
{
  ILI9341_setTextBgColor(ILI9341_BLACK);

  ILI9341_setCursor(10, 15);
  ILI9341_setTextSize(2);
  ILI9341_setTextColor(ILI9341_CYAN);
  char lb_real[16] = " bar ";
  ILI9341_writeString(lb_real);

  ILI9341_setTextSize(3);
  ILI9341_setTextColor(ILI9341_YELLOW);

  char real_mv[16];
  utoa((int)real_result[i], real_mv, 10);
  if(real_result[i]<1000)
      ILI9341_write('0');
  if(real_result[i]<100)
      ILI9341_write('0');
  ILI9341_writeString(real_mv);

  ILI9341_setCursor(160, 15);
  ILI9341_setTextSize(2);
  ILI9341_setTextColor(ILI9341_CYAN);
  char lb_max[16] = " Max ";
  ILI9341_writeString(lb_max);

  ILI9341_setTextSize(3);
  ILI9341_setTextColor(ILI9341_YELLOW);

  char max_mv[16];
  utoa((int)max_result, max_mv, 10);
  if(max_result<1000)
      ILI9341_write('0');
  if(max_result<100)
      ILI9341_write('0');
  ILI9341_writeString(max_mv);
}

void stled_on(void)
{
  HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);
}

void stled_off(void)
{
  HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
}

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();

  /* USER CODE BEGIN 2 */
  lcd_reset();
  HAL_GPIO_WritePin(GPIOA, LCD_LED_Pin, GPIO_PIN_SET);
  soft_spi_set_bit_order(SPI_MSB_FIRST);
  soft_spi_set_data_mode(SPI_MODE0);
  soft_spi_set_clock_divider(SPI_CLOCK_DIV2);
  ILI9341_begin(/*&hspi1*/);
  
  ILI9341_setRotation(0); // clear display & draw grid
  ILI9341_fillScreen(ILI9341_BLACK);
  ILI9341_setRotation(3);
  ILI9341_fillScreen(ILI9341_BLACK);

  for(uint16_t i = 5; i<320; i+=23) ILI9341_drawLine( i, 10, i, 230, ILI9341_WHITE); //draw vertical lines
  for(uint16_t i = 10; i<240; i+=22) ILI9341_drawLine( 10, i, 300, i, ILI9341_WHITE); //draw horizontal lines
  ILI9341_drawLine( 10, 119, 300, 119, ILI9341_WHITE); //draw horizontal lines    
  //ILI9341_drawLine( 10, 121, 300, 121, ILI9341_WHITE); //draw horizontal lines

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  HAL_TIM_Base_Init(&htim3);
  HAL_TIM_PeriodElapsedCallback(&htim3);
  HAL_TIM_Base_Start_IT(&htim3); //запустим таймер
  
//   HAL_TIM_Base_Init(&htim4);
//   HAL_TIM_Base_Start_IT(&htim4); //запустим таймер
  
  HAL_ADC_Start(&hadc1);//запустим аналогово-цифровое преобразование
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    for(uint16_t i = 0; i < 320; i++) // measurement and display redraw
    {
      //HAL_Delay(2);
      fill_buff(i);
      write_graph(i);
    }
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3)
{
  HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
   *    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
