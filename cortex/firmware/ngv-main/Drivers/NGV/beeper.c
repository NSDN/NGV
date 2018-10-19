#include "./Include/beeper.h"

#include "main.h"

static short OUT_A_COUNT = 0;
static short OUT_B_COUNT = 0;

static short OUT_A_COUNT_POS = 0;
static short OUT_B_COUNT_POS = 0;

static short OUT_A_COUNT_SUM = 0;
static short OUT_B_COUNT_SUM = 0;

void beeperInerrupt() {
	if (OUT_A_COUNT <= OUT_A_COUNT_POS) HAL_GPIO_WritePin(OUT_A, GPIO_PIN_SET);
	else if (OUT_A_COUNT <= OUT_A_COUNT_SUM) HAL_GPIO_WritePin(OUT_A, GPIO_PIN_RESET);
	else OUT_A_COUNT = 0;
	OUT_A_COUNT += 1;

	if (OUT_B_COUNT <= OUT_B_COUNT_POS) HAL_GPIO_WritePin(OUT_B, GPIO_PIN_SET);
	else if (OUT_B_COUNT <= OUT_B_COUNT_SUM) HAL_GPIO_WritePin(OUT_B, GPIO_PIN_RESET);
	else OUT_B_COUNT = 0;
	OUT_B_COUNT += 1;
}

static uint8_t _state = 1;
void setState(uint8_t v) { _state = v; }
uint8_t getState() { return _state; }

static uint8_t _jump = 0;
void jumpOut() { _jump = 1; }

void setVolume(unsigned char vol) {
	OUT_A_COUNT_POS = OUT_B_COUNT_POS = vol > 5 ? 5 : vol;
}

void playMusicRaw(
		const unsigned short* symbol,
		const char* music, unsigned short length,
		unsigned short patternLength,
		uint8_t withControl) {
	unsigned short i;
	uint8_t step;
	for (i = 0; i < length; i++) {
		while (!_state && withControl);
		if (_jump && withControl) {
			_jump = 0;
			OUT_A_COUNT_SUM = 0;
			OUT_B_COUNT_SUM = 0;
			return;
		}
		step = music[i];
		if (step != 0xFF) {
			OUT_A_COUNT_SUM = symbol[step];
			OUT_B_COUNT_SUM = symbol[step];
		} else {
			OUT_A_COUNT_SUM = 0;
			OUT_B_COUNT_SUM = 0;
		}
		HAL_Delay(patternLength);
	}
}

void playMusicWithSpaceRaw(
		const unsigned short* symbol,
		const char* music, unsigned short length,
		unsigned short patternLength,
		unsigned short spaceLength,
		uint8_t withControl) {
	unsigned short i;
	uint8_t step;
	for (i = 0; i < length; i++) {
		while (!_state && withControl);
		if (_jump && withControl) {
			_jump = 0;
			OUT_A_COUNT_SUM = 0;
			OUT_B_COUNT_SUM = 0;
			return;
		}
		step = music[i];
		if (step != 0xFF) {
			OUT_A_COUNT_SUM = symbol[step];
			OUT_B_COUNT_SUM = symbol[step];
		} else {
			OUT_A_COUNT_SUM = 0;
			OUT_B_COUNT_SUM = 0;
		}
		HAL_Delay(patternLength);
		OUT_A_COUNT_SUM = 0;
		OUT_B_COUNT_SUM = 0;
		HAL_Delay(spaceLength);
	}
}

void playMusic(
		const unsigned short* symbol,
		Music* music,
		unsigned short patternLength,
		uint8_t withControl) {
	playMusicRaw(
			symbol,
			music->data, music->length,
			patternLength,
			withControl);
}

void playMusicWithSpace(
		const unsigned short* symbol,
		Music* music,
		unsigned short patternLength,
		unsigned short spaceLength,
		uint8_t withControl) {
	playMusicWithSpaceRaw(
			symbol,
			music->data, music->length,
			patternLength,
			spaceLength,
			withControl);
}

