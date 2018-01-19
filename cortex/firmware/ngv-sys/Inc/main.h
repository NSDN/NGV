/**
  ******************************************************************************
  * File Name          : main.hpp
  * Description        : This file contains the common defines of the application
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
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define LCD_RD_Pin GPIO_PIN_2
#define LCD_RD_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_3
#define LCD_RST_GPIO_Port GPIOE
#define LCD_D0_Pin GPIO_PIN_4
#define LCD_D0_GPIO_Port GPIOE
#define LCD_D1_Pin GPIO_PIN_5
#define LCD_D1_GPIO_Port GPIOE
#define LCD_D2_Pin GPIO_PIN_6
#define LCD_D2_GPIO_Port GPIOE
#define LAN_MOSI_Pin GPIO_PIN_7
#define LAN_MOSI_GPIO_Port GPIOA
#define LCD_D3_Pin GPIO_PIN_7
#define LCD_D3_GPIO_Port GPIOE
#define LCD_D4_Pin GPIO_PIN_8
#define LCD_D4_GPIO_Port GPIOE
#define LCD_D5_Pin GPIO_PIN_9
#define LCD_D5_GPIO_Port GPIOE
#define LCD_D6_Pin GPIO_PIN_10
#define LCD_D6_GPIO_Port GPIOE
#define LCD_D7_Pin GPIO_PIN_11
#define LCD_D7_GPIO_Port GPIOE
#define LCD_D8_Pin GPIO_PIN_12
#define LCD_D8_GPIO_Port GPIOE
#define LCD_D9_Pin GPIO_PIN_13
#define LCD_D9_GPIO_Port GPIOE
#define LCD_D10_Pin GPIO_PIN_14
#define LCD_D10_GPIO_Port GPIOE
#define LCD_D11_Pin GPIO_PIN_15
#define LCD_D11_GPIO_Port GPIOE
#define LCD_BL_Pin GPIO_PIN_14
#define LCD_BL_GPIO_Port GPIOB
#define LCD_D16_Pin GPIO_PIN_8
#define LCD_D16_GPIO_Port GPIOD
#define LCD_D17_Pin GPIO_PIN_9
#define LCD_D17_GPIO_Port GPIOD
#define LCD_D18_Pin GPIO_PIN_10
#define LCD_D18_GPIO_Port GPIOD
#define LCD_D19_Pin GPIO_PIN_11
#define LCD_D19_GPIO_Port GPIOD
#define LCD_D20_Pin GPIO_PIN_12
#define LCD_D20_GPIO_Port GPIOD
#define LCD_D21_Pin GPIO_PIN_13
#define LCD_D21_GPIO_Port GPIOD
#define LCD_D22_Pin GPIO_PIN_14
#define LCD_D22_GPIO_Port GPIOD
#define LCD_D23_Pin GPIO_PIN_15
#define LCD_D23_GPIO_Port GPIOD
#define LAN_INT_Pin GPIO_PIN_2
#define LAN_INT_GPIO_Port GPIOG
#define KBD_TX_Pin GPIO_PIN_6
#define KBD_TX_GPIO_Port GPIOC
#define KBD_RX_Pin GPIO_PIN_7
#define KBD_RX_GPIO_Port GPIOC
#define LAN_CS_Pin GPIO_PIN_15
#define LAN_CS_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_0
#define LCD_CS_GPIO_Port GPIOD
#define LCD_RS_Pin GPIO_PIN_1
#define LCD_RS_GPIO_Port GPIOD
#define LCD_WR_Pin GPIO_PIN_3
#define LCD_WR_GPIO_Port GPIOD
#define LCD_D12_Pin GPIO_PIN_4
#define LCD_D12_GPIO_Port GPIOD
#define LCD_D13_Pin GPIO_PIN_5
#define LCD_D13_GPIO_Port GPIOD
#define LCD_D14_Pin GPIO_PIN_6
#define LCD_D14_GPIO_Port GPIOD
#define LCD_D15_Pin GPIO_PIN_7
#define LCD_D15_GPIO_Port GPIOD
#define LCD_NULL1_Pin GPIO_PIN_13
#define LCD_NULL1_GPIO_Port GPIOG
#define LCD_NULL2_Pin GPIO_PIN_14
#define LCD_NULL2_GPIO_Port GPIOG
#define LAN_SCK_Pin GPIO_PIN_3
#define LAN_SCK_GPIO_Port GPIOB
#define LAN_MISO_Pin GPIO_PIN_4
#define LAN_MISO_GPIO_Port GPIOB
#define LCD_BL_NULL_Pin GPIO_PIN_7
#define LCD_BL_NULL_GPIO_Port GPIOB
#define LD1_Pin GPIO_PIN_5
#define LD1_GPIO_Port GPIOI
#define LD2_Pin GPIO_PIN_6
#define LD2_GPIO_Port GPIOI
#define LD3_Pin GPIO_PIN_7
#define LD3_GPIO_Port GPIOI

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
void greenScreen(char* head);
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
