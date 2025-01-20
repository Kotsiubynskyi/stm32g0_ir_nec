#include "stm32g0xx_hal.h"

TIM_HandleTypeDef htim3 = {0};
TIM_HandleTypeDef htim14 = {0};

int main()
{
  HAL_Init();

  example1();
  HAL_Delay(1000);

  while (1)
  {
    HAL_Delay(100);
  }
  return 0;
}

void GPIO_PA4_Init()
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void InitTimerIcMode()
{
  __HAL_RCC_TIM1_CLK_ENABLE();

  TIM_HandleTypeDef hTim = {0};
  hTim.Instance = TIM1;
  hTim.Channel = HAL_TIM_ACTIVE_CHANNEL_4;
  hTim.Init.Prescaler = 16 - 1;
  hTim.Init.Period = 10000;

  TIM_IC_InitTypeDef hTimIcConfig = {0};
  hTimIcConfig.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;
  HAL_TIM_IC_ConfigChannel(&hTim, HAL_TIM_ACTIVE_CHANNEL_4);

  HAL_TIM_IC_Start(&hTim, HAL_TIM_ACTIVE_CHANNEL_4);
}
