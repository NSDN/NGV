#include "ngp_bios.h"

#include "lcd.h"
#include "key.h"
#include "beeper.h"
#include "flash.h"
#include "symbol.h"
#include "logo.h"
#include "stg.h"

#define NGP_SYS_VERSION "dev181022"

extern LCD* lcd;
#define dev lcd
static uint8_t menu = 0;
static uint8_t music = 0;

const uint8_t cmusic = 7;

extern Flash* flash;
extern TIM_HandleTypeDef htim10;

int halfWidth() { return dev->p->width / 2 - 1; }
int halfHeight() { return dev->p->height / 2 - 1; }

extern void jumpOut();

void _tim_irq_callback() {
	if (menu == 13) {
		if (waitKeyUp(RPAD_RIGHT)) {
			jumpOut();
			menu -= 10;
			dev->colorb(dev->p, 0x000000);
			dev->clear(dev->p);
			HAL_TIM_Base_Stop_IT(&htim10);
			HAL_GPIO_WritePin(OUT_A, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(OUT_B, GPIO_PIN_RESET);
		}
		if (waitKeyUp(LPAD_UP)) {
			if (music > 0) music -= 1;
			else music = cmusic - 1;
			jumpOut();
			dev->colorb(dev->p, 0x000000);
			dev->clear(dev->p);
		} else if (waitKeyUp(LPAD_DOWN)) {
			if (music < cmusic - 1) music += 1;
			else music = 0;
			jumpOut();
			dev->colorb(dev->p, 0x000000);
			dev->clear(dev->p);
		}
	}
	beeperInerrupt();
}

static uint8_t init = 0;
void ngp_setup() {
	if (init) return;
	init = 1;

	dev->colorb(dev->p, 0x000000);
	dev->colorf(dev->p, 0xFFFFFF);
	dev->clear(dev->p);
}

void ngp_loop() {
	ngp_setup();

	if (menu < 10) {
		/* MENU BUILD BEGIN */
		dev->colorb(dev->p, 0x000000);
		dev->colorf(dev->p, 0xFF9800);
		dev->printfc(dev->p, 8, "MENU");
		dev->colorb(dev->p, 0x000000);
		dev->colorf(dev->p, 0xFFFFFF);
		dev->printfc(dev->p, 24, "W25Q64 Test");
		dev->printfc(dev->p, 32, "STG Sample");
		dev->printfc(dev->p, 40, "Flash Test");
		dev->printfc(dev->p, 48, "NSDN-Beeper");
		dev->printfc(dev->p, 56, "About this NGP");

		dev->draw(dev->p, (halfWidth() - 64) + 10, (menu + 2) * 8, ' ');
		dev->draw(dev->p, (halfWidth() - 64) + 10, (menu + 3) * 8, '>');
		dev->draw(dev->p, (halfWidth() - 64) + 10, (menu + 4) * 8, ' ');
		dev->draw(dev->p, (halfWidth() - 64) + 112, (menu + 2) * 8, ' ');
		dev->draw(dev->p, (halfWidth() - 64) + 112, (menu + 3) * 8, '<');
		dev->draw(dev->p, (halfWidth() - 64) + 112, (menu + 4) * 8, ' ');

		if (waitKeyUp(LPAD_UP)) menu = ((menu > 0) ? menu - 1 : 0);
		if (waitKeyUp(LPAD_DOWN)) menu = ((menu < 4) ? menu + 1 : 4);
		if (waitKeyUp(RPAD_UP)) {
			menu += 10;
			dev->clear(dev->p);

			switch (menu) {
				case 10:
					break;
				case 11:
					stg_main(STG_BACK);
					break;
				case 12:
					break;
				case 13:
					HAL_TIM_Base_Start_IT(&htim10);
					setVolume(5);
					music = 0;
					break;
				case 14:
					dev->colorb(dev->p, 0xFFFFFF);
					dev->colorf(dev->p, 0x000000);
					dev->clear(dev->p);
					break;
				default:
					break;
			}
		}
	} else if (menu < 20) {
		switch (menu) {
			case 10:
				dev->colorb(dev->p, 0x000000);
				dev->colorf(dev->p, 0xFF9800);
				dev->printfc(dev->p, 8, "W25Q64 Test");
				dev->colorb(dev->p, 0x000000);
				dev->colorf(dev->p, 0xFFFFFF);
				dev->printfc(dev->p, 48, "Init W25Q64...");
				uint8_t result = 0, manuf = 0, buf = 0;
				result = flash->begin(flash->p);
				manuf = flash->readManufacturer(flash->p);
				flash->read(flash->p, 0x00000000, &buf, 1);
				flash->end(flash->p);
				dev->printfc(dev->p, 64, "Result: %X", result);
				dev->printfc(dev->p, 72, "Manuf: %X", manuf);
				dev->printfc(dev->p, 80, "Buf: %X", buf);
				break;
			case 11:
				stg_main(STG_MAIN);
				break;
			case 12:
				dev->colorb(dev->p, 0x000000);
				dev->colorf(dev->p, 0xFF9800);
				dev->printfc(dev->p, 8, "Flash Test");
				dev->colorb(dev->p, 0x000000);
				dev->colorf(dev->p, 0x3F51B5);
				dev->printfc(dev->p, 24, "Flashing: 0x3F51B5");
				dev->rect(dev->p, 16, 48, dev->p->width - 16, dev->p->height - 16, 1);
				dev->colorf(dev->p, 0xB10000);
				dev->printfc(dev->p, 24, "Flashing: 0xB10000");
				dev->rect(dev->p, 16, 48, dev->p->width - 16, dev->p->height - 16, 1);
				dev->colorf(dev->p, 0xFFFFFF);
				break;
			case 13:
				dev->colorb(dev->p, 0x000000);
				dev->colorf(dev->p, 0xFF9800);
				dev->printfc(dev->p, 8, "NSDN-Beeper");
				dev->colorb(dev->p, 0x000000);
				dev->colorf(dev->p, 0xFFFFFF);
				dev->printfc(dev->p, 32, "NOW Playing...");
				dev->printfc(dev->p, 48, "%d of %d", cmusic, music + 1);
				switch (music) {
					case 0:
						dev->printfc(dev->p, 64, "Septet for");
						dev->printfc(dev->p, 72, "the Dead Princess");
						playMusicWithSpace(getSymbol(), getMusic(0), 233, 16, 1);
						break;
					case 1:
						dev->printfc(dev->p, 64, "The Way of Doom God");
						dev->printfc(dev->p, 72, "- Dark Road -");
						playMusicWithSpace(getSymbol(), getMusic(1), 233, 16, 1);
						break;
					case 2:
						dev->printfc(dev->p, 64, "Romantic Escape");
						dev->printfc(dev->p, 72, "Flight & Foul for");
						dev->printfc(dev->p, 80, "Impossible Bullets");
						playMusicWithSpace(getSymbol(), getMusic(2), 233, 16, 1);
						break;
					case 3:
						dev->printfc(dev->p, 64, "Lively and Innocent");
						dev->printfc(dev->p, 72, "Girl (9)");
						playMusicWithSpace(getSymbol(), getMusic(3), 233, 16, 1);
						break;
					case 4:
						dev->printfc(dev->p, 64, "Broken Moon");
						playMusicWithSpace(getSymbol(), getMusic(4), 233, 16, 1);
						break;
					case 5:
						dev->printfc(dev->p, 64, "U.N. Owen was her?");
						playMusicWithSpace(getSymbol(), getMusic(5), 128, 16, 1);
						break;
					case 6:
						dev->printfc(dev->p, 64, "Staking Your Life");
						dev->printfc(dev->p, 72, "on a Prank");
						playMusicWithSpace(getSymbol(), getMusic(6), 192, 8, 1);
						break;
				}
				break;
			case 14:
				dev->colorb(dev->p, 0xFFFFFF);
				dev->colorf(dev->p, 0x000000);
				dev->bitmapsc(dev->p, halfWidth() - 1, (halfHeight() - 64) + 48, 64, 64, getLogo());
				dev->printfc(dev->p, (halfHeight() - 64) + 86, "NyaGame Portable");
				dev->printfc(dev->p, (halfHeight() - 64) + 98, NGP_SYS_VERSION);
				break;
			default:
				break;
		}

		if (menu == 11) {
			if (waitKeyUp(RPAD_DOWN)) {
				menu -= 10;
				dev->colorb(dev->p, 0x000000);
				dev->colorf(dev->p, 0xFF9800);
				dev->clear(dev->p);
			}
		}
		if (menu != 13 && menu != 11) {
			if (waitKeyUp(RPAD_RIGHT)) {
				menu -= 10;
				dev->colorb(dev->p, 0x000000);
				dev->colorf(dev->p, 0xFF9800);
				dev->clear(dev->p);

				switch (menu) {
					case 10:
						break;
					case 11:
						break;
					case 12:
						break;
					case 13:
						HAL_TIM_Base_Start_IT(&htim10);
						setVolume(2);
						break;
					case 14:
						break;
					default:
						break;
				}
			}
		}
	} else {
		menu = 0;
		dev->clear(dev->p);
	}
}
