#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_tim.h"

TIM_HandleTypeDef hTim1 = {0};
void InitTimerIcMode();
void GPIO_PA11_Init();

int main()
{
  HAL_Init();

  GPIO_PA11_Init();
  InitTimerIcMode();

  while (1)
  {
    HAL_Delay(1000);
  }
  return 0;
}

void GPIO_PA11_Init()
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void InitTimerIcMode()
{
  __HAL_RCC_TIM1_CLK_ENABLE();

  hTim1.Instance = TIM1;
  hTim1.Init.Prescaler = 16 - 1;
  hTim1.Init.Period = 0xFFFF - 1;

  HAL_TIM_IC_Init(&hTim1);
  TIM_IC_InitTypeDef hTimIcConfig = {0};
  hTimIcConfig.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  hTimIcConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  hTimIcConfig.ICFilter = 0;
  HAL_TIM_IC_ConfigChannel(&hTim1, &hTimIcConfig, TIM_CHANNEL_4);

  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
  HAL_TIM_IC_Start_IT(&hTim1, TIM_CHANNEL_4);
}

uint32_t prevValue = 0;
uint32_t irMsg = 0;
uint8_t bitIndex = 0;
uint8_t command = 0;
uint8_t address = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
    processSignal(htim);
  }
}

void processSignal(TIM_HandleTypeDef *htim)
{
  uint32_t curValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);

  uint16_t pulseWidth = (curValue > prevValue) ? curValue - prevValue : (0xFFFF - prevValue) + curValue;
  prevValue = curValue;

  if (pulseWidth > 11475 && pulseWidth < 15525) // (9ms + 4.5ms) ± 15% -- start receiving
  {
    bitIndex = 0;
    irMsg = 0;
  }
  else if (pulseWidth > 956 && pulseWidth < 1293) //  (562.5μs + 562.5 μs) ± 15% -- received '0'
  {
    bitIndex++;
  }
  else if (pulseWidth > 1910 && pulseWidth < 2585) // (562.5μs + 3x562.5 μs) ± 15% -- received '1'
  {
    irMsg |= 1 << bitIndex;
    bitIndex++;
  }

  if (bitIndex >= 32)
  {
    command = irMsg & 0xFF;
    address = (irMsg >> 16) & 0xFF;

    bitIndex = 0;
    irMsg = 0;
  }
}