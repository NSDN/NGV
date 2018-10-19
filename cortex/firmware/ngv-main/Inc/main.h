/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define LED_R_Pin GPIO_PIN_13
#define LED_R_GPIO_Port GPIOC
#define LED_G_Pin GPIO_PIN_14
#define LED_G_GPIO_Port GPIOC
#define LED_B_Pin GPIO_PIN_15
#define LED_B_GPIO_Port GPIOC
#define SPI5_CS_Pin GPIO_PIN_6
#define SPI5_CS_GPIO_Port GPIOF
#define KEYBRD_F1_Pin GPIO_PIN_10
#define KEYBRD_F1_GPIO_Port GPIOF
#define SYS_LED_Pin GPIO_PIN_1
#define SYS_LED_GPIO_Port GPIOC
#define KEYBRD_F2_Pin GPIO_PIN_0
#define KEYBRD_F2_GPIO_Port GPIOA
#define KEYBRD_F3_Pin GPIO_PIN_1
#define KEYBRD_F3_GPIO_Port GPIOA
#define LCD_BLK_Pin GPIO_PIN_4
#define LCD_BLK_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_5
#define LCD_CS_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_6
#define LCD_RS_GPIO_Port GPIOA
#define LCD_RST_Pin GPIO_PIN_7
#define LCD_RST_GPIO_Port GPIOA
#define KEYBRD_F4_Pin GPIO_PIN_4
#define KEYBRD_F4_GPIO_Port GPIOC
#define KEYBRD_F5_Pin GPIO_PIN_5
#define KEYBRD_F5_GPIO_Port GPIOC
#define KEYBRD_F6_Pin GPIO_PIN_0
#define KEYBRD_F6_GPIO_Port GPIOB
#define KEYBRD_F7_Pin GPIO_PIN_1
#define KEYBRD_F7_GPIO_Port GPIOB
#define KEYBRD_F8_Pin GPIO_PIN_2
#define KEYBRD_F8_GPIO_Port GPIOB
#define SPI2_CS_Pin GPIO_PIN_12
#define SPI2_CS_GPIO_Port GPIOB
#define FPGA_CONF_Pin GPIO_PIN_6
#define FPGA_CONF_GPIO_Port GPIOG
#define FPGA_RST_Pin GPIO_PIN_7
#define FPGA_RST_GPIO_Port GPIOG
#define SDIO_INS_Pin GPIO_PIN_15
#define SDIO_INS_GPIO_Port GPIOA
#define FPGA_INT_Pin GPIO_PIN_3
#define FPGA_INT_GPIO_Port GPIOD
#define FPGA_INT_EXTI_IRQn EXTI3_IRQn
#define FPGA_CTL_Pin GPIO_PIN_11
#define FPGA_CTL_GPIO_Port GPIOG
#define KEYPAD_OUTA_Pin GPIO_PIN_13
#define KEYPAD_OUTA_GPIO_Port GPIOG
#define KEYPAD_OUTB_Pin GPIO_PIN_14
#define KEYPAD_OUTB_GPIO_Port GPIOG
#define KEYPAD_INA_Pin GPIO_PIN_3
#define KEYPAD_INA_GPIO_Port GPIOB
#define KEYPAD_INB_Pin GPIO_PIN_4
#define KEYPAD_INB_GPIO_Port GPIOB
#define KEYPAD_INC_Pin GPIO_PIN_5
#define KEYPAD_INC_GPIO_Port GPIOB
#define KEYPAD_IND_Pin GPIO_PIN_6
#define KEYPAD_IND_GPIO_Port GPIOB
#define LCDTC_INT_Pin GPIO_PIN_9
#define LCDTC_INT_GPIO_Port GPIOB
#define LCDTC_INT_EXTI_IRQn EXTI9_5_IRQn

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
