#ifndef __BEEPER_H_
#define __BEEPER_H_


#include "halinc.h"

#include "music.h"

#define OUT_A LED_R_GPIO_Port,LED_R_Pin
#define OUT_B LED_B_GPIO_Port,LED_B_Pin

void beeperInerrupt();

void setVolume(unsigned char vol);

#ifdef USE_RAW
void playMusicRaw(
		const unsigned short* symbol,
		const char* music, unsigned short length,
		unsigned short patternLength,
		uint8_t withControl);
void playMusicWithSpaceRaw(
		const unsigned short* symbol,
		const char* music, unsigned short length,
		unsigned short patternLength,
		unsigned short spaceLength,
		uint8_t withControl);
#endif
void playMusic(
		const unsigned short* symbol,
		Music* music,
		unsigned short patternLength,
		uint8_t withControl);
void playMusicWithSpace(
		const unsigned short* symbol,
		Music* music,
		unsigned short patternLength,
		unsigned short spaceLength,
		uint8_t withControl);


#endif
