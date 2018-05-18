/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#define NGV_SYS_VERSION "180119"

#include "usbd_core.h"
#include <setjmp.h>

#include "lcd.h"
#include "logo.h"
#include "nshel.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SD_HandleTypeDef hsd;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim12;

UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
LCD* lcd;
pIO rd = { LCD_RD_GPIO_Port, LCD_RD_Pin },
	wr = { LCD_WR_GPIO_Port, LCD_WR_Pin },
	rs = { LCD_RS_GPIO_Port, LCD_RS_Pin },
	cs = { LCD_CS_GPIO_Port, LCD_CS_Pin },
	rst = { LCD_RST_GPIO_Port, LCD_RST_Pin };
pIO data[24] = {
	{ LCD_D0_GPIO_Port, LCD_D0_Pin }, { LCD_D1_GPIO_Port, LCD_D1_Pin },
	{ LCD_D2_GPIO_Port, LCD_D2_Pin }, { LCD_D3_GPIO_Port, LCD_D3_Pin },
	{ LCD_D4_GPIO_Port, LCD_D4_Pin }, { LCD_D5_GPIO_Port, LCD_D5_Pin },
	{ LCD_D6_GPIO_Port, LCD_D6_Pin }, { LCD_D7_GPIO_Port, LCD_D7_Pin },
	{ LCD_D8_GPIO_Port, LCD_D8_Pin }, { LCD_D9_GPIO_Port, LCD_D9_Pin },
	{ LCD_D10_GPIO_Port, LCD_D10_Pin }, { LCD_D11_GPIO_Port, LCD_D11_Pin },
	{ LCD_D12_GPIO_Port, LCD_D12_Pin }, { LCD_D13_GPIO_Port, LCD_D13_Pin },
	{ LCD_D14_GPIO_Port, LCD_D14_Pin }, { LCD_D15_GPIO_Port, LCD_D15_Pin },
	{ LCD_D16_GPIO_Port, LCD_D16_Pin }, { LCD_D17_GPIO_Port, LCD_D17_Pin },
	{ LCD_D18_GPIO_Port, LCD_D18_Pin }, { LCD_D19_GPIO_Port, LCD_D19_Pin },
	{ LCD_D20_GPIO_Port, LCD_D20_Pin }, { LCD_D21_GPIO_Port, LCD_D21_Pin },
	{ LCD_D22_GPIO_Port, LCD_D22_Pin }, { LCD_D23_GPIO_Port, LCD_D23_Pin }
};
jmp_buf rstPos;
uint8_t FS_OK = 0;
HAL_SD_CardInfoTypeDef cardInfo;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM12_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void greenScreen(char* head) {
	lcd->colorb(lcd->p, 0x007F00);
	lcd->colorf(lcd->p, 0xFFFFFF);
	lcd->font(lcd->p, Small);
	lcd->clear(lcd->p);
	lcd->printfc(lcd->p, 64, "%s", head);
	lcd->printfc(lcd->p, 100, "WE DO NOT KNOW");
	lcd->printfc(lcd->p, 110, "WHAT HAD HAPPENED");
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 250; j++)
			for (int k = 0; k < 168000; k++)
				asm("nop");
	NVIC_SystemReset();
}
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	lcd = LCDInit(&rd, &wr, &rs, &cs, &rst, data);
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
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  MX_USART6_UART_Init();
  MX_TIM12_Init();

  /* USER CODE BEGIN 2 */
	setjmp(rstPos);
	USBD_Stop(&hUsbDeviceHS);

	lcd->init(lcd->p);
	lcd->rotate(lcd->p, LCD_LANDSCAPE);
	lcd->font(lcd->p, Small);
	lcd->colorb(lcd->p, 0xFFFFFF);
	lcd->colorf(lcd->p, 0x000000);
	lcd->clear(lcd->p);

	lcd->bitmapsc(lcd->p, lcd->p->width / 2, 140, 64, 64, getLogo());
	lcd->printfc(lcd->p, 180, "nyagame vita");
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
	HAL_Delay(1000);
	lcd->clear(lcd->p);

	lcd->printfa(lcd->p, "NyaGame Vita Factory System\n");
	lcd->printfa(lcd->p, "Version: %s\n\n", NGV_SYS_VERSION);

	/* Initialize device */
	uint8_t result = 0;
	if (HAL_SD_Init(&hsd) == HAL_OK) {
		lcd->printfa(lcd->p, "Init SD card... OK\n");
	} else {
		lcd->printfa(lcd->p, "Init SD card... ERR\n");
	}

	lcd->printfa(lcd->p, "Mount SD card...\n");
	f_mount(&SDFatFS, SDPath, 1);
	result = f_mount(&SDFatFS, SDPath, 1);
	if(result == FR_OK) {
		char path[] = "NGV_INFO.TXT";
		FIL boardInfo;
		f_open(&boardInfo, path, FA_WRITE | FA_CREATE_ALWAYS);
		f_printf(&boardInfo, "NyaGame Vita Factory Edition with STM32F407\n");
		f_printf(&boardInfo, "by NyaSama Developer Network\n");
		f_printf(&boardInfo, "Firmware Version: %s\n", NGV_SYS_VERSION);
		f_close(&boardInfo);
		lcd->printfa(lcd->p, "Test SD card... OK\n");
		FS_OK = 1;
	} else {
		lcd->printfa(lcd->p, "Test SD card... ERR: %02X\n", result);
		FS_OK = 0;
	}

	if (FS_OK) {
		HAL_SD_GetCardInfo(&hsd, &cardInfo);
		lcd->printfa(lcd->p, "Init USB Mass Storage...\n");
		USBD_Start(&hUsbDeviceHS);
	}

	/* Initialize end */

	HAL_Delay(500);

	lcd->printfa(lcd->p, "\n");
	char* args[] = { "nshel", "init.d" };
	nshel(2, args);
	lcd->printfa(lcd->p, "\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint8_t cnt = 0;
  while (1)
  {
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	lcd->colorb(lcd->p, 0xFF0000);
	lcd->clear(lcd->p);
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	lcd->colorb(lcd->p, 0x00FF00);
	lcd->clear(lcd->p);
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	lcd->colorb(lcd->p, 0x0000FF);
	lcd->clear(lcd->p);

	if (cnt < 16) cnt += 1;
	else greenScreen("END OF SYSTEM");

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Enables the Clock Security System 
    */
  HAL_RCC_EnableCSS();

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SDIO init function */
static void MX_SDIO_SD_Init(void)
{

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM12 init function */
static void MX_TIM12_Init(void)
{

  TIM_OC_InitTypeDef sConfigOC;

  htim12.Instance = TIM12;
  htim12.Init.Prescaler = 335;
  htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim12.Init.Period = 100;
  htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim12) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 90;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim12);

}

/* USART6 init function */
static void MX_USART6_UART_Init(void)
{

  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LCD_RD_Pin|LCD_RST_Pin|LCD_D0_Pin|LCD_D1_Pin 
                          |LCD_D2_Pin|LCD_D3_Pin|LCD_D4_Pin|LCD_D5_Pin 
                          |LCD_D6_Pin|LCD_D7_Pin|LCD_D8_Pin|LCD_D9_Pin 
                          |LCD_D10_Pin|LCD_D11_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LCD_D16_Pin|LCD_D17_Pin|LCD_D18_Pin|LCD_D19_Pin 
                          |LCD_D20_Pin|LCD_D21_Pin|LCD_D22_Pin|LCD_D23_Pin 
                          |LCD_CS_Pin|LCD_RS_Pin|LCD_WR_Pin|LCD_D12_Pin 
                          |LCD_D13_Pin|LCD_D14_Pin|LCD_D15_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LAN_CS_GPIO_Port, LAN_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, LCD_NULL1_Pin|LCD_NULL2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BL_NULL_GPIO_Port, LCD_BL_NULL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, LD1_Pin|LD2_Pin|LD3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LCD_RD_Pin LCD_RST_Pin LCD_D0_Pin LCD_D1_Pin 
                           LCD_D2_Pin LCD_D3_Pin LCD_D4_Pin LCD_D5_Pin 
                           LCD_D6_Pin LCD_D7_Pin LCD_D8_Pin LCD_D9_Pin 
                           LCD_D10_Pin LCD_D11_Pin */
  GPIO_InitStruct.Pin = LCD_RD_Pin|LCD_RST_Pin|LCD_D0_Pin|LCD_D1_Pin 
                          |LCD_D2_Pin|LCD_D3_Pin|LCD_D4_Pin|LCD_D5_Pin 
                          |LCD_D6_Pin|LCD_D7_Pin|LCD_D8_Pin|LCD_D9_Pin 
                          |LCD_D10_Pin|LCD_D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D16_Pin LCD_D17_Pin LCD_D18_Pin LCD_D19_Pin 
                           LCD_D20_Pin LCD_D21_Pin LCD_D22_Pin LCD_D23_Pin 
                           LCD_CS_Pin LCD_RS_Pin LCD_WR_Pin LCD_D12_Pin 
                           LCD_D13_Pin LCD_D14_Pin LCD_D15_Pin */
  GPIO_InitStruct.Pin = LCD_D16_Pin|LCD_D17_Pin|LCD_D18_Pin|LCD_D19_Pin 
                          |LCD_D20_Pin|LCD_D21_Pin|LCD_D22_Pin|LCD_D23_Pin 
                          |LCD_CS_Pin|LCD_RS_Pin|LCD_WR_Pin|LCD_D12_Pin 
                          |LCD_D13_Pin|LCD_D14_Pin|LCD_D15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : LAN_INT_Pin */
  GPIO_InitStruct.Pin = LAN_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LAN_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LAN_CS_Pin */
  GPIO_InitStruct.Pin = LAN_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LAN_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_NULL1_Pin LCD_NULL2_Pin */
  GPIO_InitStruct.Pin = LCD_NULL1_Pin|LCD_NULL2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_BL_NULL_Pin */
  GPIO_InitStruct.Pin = LCD_BL_NULL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BL_NULL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD2_Pin LD3_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD2_Pin|LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
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
