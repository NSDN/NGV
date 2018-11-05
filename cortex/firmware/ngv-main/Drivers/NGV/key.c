#include "./Include/key.h"

#include "main.h"

uint16_t tmpState;

uint16_t scanKeys() {
	uint16_t value = 0;

	HAL_GPIO_WritePin(KEYPAD_OUTA_GPIO_Port, KEYPAD_OUTA_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEYPAD_OUTB_GPIO_Port, KEYPAD_OUTB_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(KEYPAD_INA_GPIO_Port, KEYPAD_INA_Pin) == GPIO_PIN_RESET) value |= LPAD_UP;
	if (HAL_GPIO_ReadPin(KEYPAD_INB_GPIO_Port, KEYPAD_INB_Pin) == GPIO_PIN_RESET) value |= LPAD_DOWN;
	if (HAL_GPIO_ReadPin(KEYPAD_INC_GPIO_Port, KEYPAD_INC_Pin) == GPIO_PIN_RESET) value |= LPAD_LEFT;
	if (HAL_GPIO_ReadPin(KEYPAD_IND_GPIO_Port, KEYPAD_IND_Pin) == GPIO_PIN_RESET) value |= LPAD_RIGHT;

	HAL_GPIO_WritePin(KEYPAD_OUTB_GPIO_Port, KEYPAD_OUTB_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEYPAD_OUTA_GPIO_Port, KEYPAD_OUTA_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(KEYPAD_INA_GPIO_Port, KEYPAD_INA_Pin) == GPIO_PIN_RESET) value |= RPAD_UP;
	if (HAL_GPIO_ReadPin(KEYPAD_INB_GPIO_Port, KEYPAD_INB_Pin) == GPIO_PIN_RESET) value |= RPAD_DOWN;
	if (HAL_GPIO_ReadPin(KEYPAD_INC_GPIO_Port, KEYPAD_INC_Pin) == GPIO_PIN_RESET) value |= RPAD_LEFT;
	if (HAL_GPIO_ReadPin(KEYPAD_IND_GPIO_Port, KEYPAD_IND_Pin) == GPIO_PIN_RESET) value |= RPAD_RIGHT;

	if (HAL_GPIO_ReadPin(KEYBRD_F1_GPIO_Port, KEYBRD_F1_Pin) == GPIO_PIN_RESET) value |= KEY_F1;
	if (HAL_GPIO_ReadPin(KEYBRD_F2_GPIO_Port, KEYBRD_F2_Pin) == GPIO_PIN_RESET) value |= KEY_F2;
	if (HAL_GPIO_ReadPin(KEYBRD_F3_GPIO_Port, KEYBRD_F3_Pin) == GPIO_PIN_RESET) value |= KEY_F3;
	if (HAL_GPIO_ReadPin(KEYBRD_F4_GPIO_Port, KEYBRD_F4_Pin) == GPIO_PIN_RESET) value |= KEY_F4;
	if (HAL_GPIO_ReadPin(KEYBRD_F5_GPIO_Port, KEYBRD_F5_Pin) == GPIO_PIN_RESET) value |= KEY_F5;
	if (HAL_GPIO_ReadPin(KEYBRD_F6_GPIO_Port, KEYBRD_F6_Pin) == GPIO_PIN_RESET) value |= KEY_F6;
	if (HAL_GPIO_ReadPin(KEYBRD_F7_GPIO_Port, KEYBRD_F7_Pin) == GPIO_PIN_RESET) value |= KEY_F7;
	if (HAL_GPIO_ReadPin(KEYBRD_F8_GPIO_Port, KEYBRD_F8_Pin) == GPIO_PIN_RESET) value |= KEY_F8;

	return value;
}

uint8_t checkKey(uint16_t key) {
	return (scanKeys() & key) != 0;
}

uint8_t checkKeyUp(uint16_t key) {
	if (checkKey(key)) {
		tmpState |= key;
		return 0;
	} else if (tmpState & key) {
		tmpState &= ~key;
		return 1;
	}
	return 0;
}

uint8_t waitKey(uint16_t key) {
	if (checkKey(key)) return 0;
	while (!checkKey(key));
	return 1;
}

uint8_t waitKeyUp(uint16_t key) {
	if (!checkKey(key)) return 0;
	while (checkKey(key));
	return 1;
}
