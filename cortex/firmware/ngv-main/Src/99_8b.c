#include "99_8b.h"
#include "stm32f4xx_hal.h"
#include "main.h"

void _99_8b_reconf() {
	HAL_GPIO_WritePin(FPGA_CONF_GPIO_Port, FPGA_CONF_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(FPGA_CONF_GPIO_Port, FPGA_CONF_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
}

void _99_8b_reset() {
	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
}
