#include "nsgal.h"

#include <string.h>
#include <malloc.h>

#include "lcd.h"
extern LCD* lcd;

#include "key.h"
#include "ngv_bios.h"

#define FONT_ADDR 0x008000 // Virtual addr

extern uint8_t* __getmem_C_(uint32_t addr);

int conv(uint8_t* str, uint32_t ptr) {
    uint8_t end = (ptr == (strlen(str) - 1)) ? 1 : 0;
    uint8_t a = str[ptr], b = (end != 0) ? 0 : str[ptr + 1];
    int res = (a < 0xA1 || b < 0xA1) ? -1 : ((a - 0xA1) * 0x5E + (b - 0xA1));
    return res;
}

void draw(uint32_t font, int x, int y, uint8_t* str) {
    int siz = strlen(str), cnt = 0, addr = 0;
    int _x = x, _y = y; char chr = '\0';
HEAD:
    addr = conv(str, cnt);
    if (addr < 0) goto ASCII;
    goto GBK;
ASCII:
    chr = str[cnt];
    if (chr == '\n') goto NLIN;
    lcd->font(lcd->p, (LCDFont) 2);
    lcd->scale(lcd->p, 1);
    lcd->draw(lcd->p, _x, _y, chr);
    lcd->font(lcd->p, (LCDFont) 1);
    _x += 8; cnt += 1;
    if (x > 838) goto ORIX;
    goto NEXT;
GBK:
    addr *= 32;
    addr += font;
    lcd->icon(lcd->p, _x, _y, 16, 16, __getmem_C_(addr));
    _x += 16; cnt += 2;
    if (x > 830) goto ORIX;
    goto NEXT;
NLIN:
    cnt += 1;
ORIX:
    _x = x; _y += 16;
NEXT:
    if (cnt != siz) goto HEAD;
}

extern GalScene* galScenes[];
extern void loadGalScenes();

int nsgal(int argc, char** argv) {
	GalScene* nowScene; uint32_t scenePtr;
	uint32_t defBack, defFore;

	loadGalScenes();

	nowScene = galScenes[0];
	defBack = lcd->p->backColor;
	defFore = lcd->p->foreColor;
	scenePtr = 0;

	while (1) {
		if (checkKey(KEY_F1) && checkKey(KEY_F8)) break;

		if (nowScene == NULL) break;

		//Draw back
		int addr = nowScene->backAddr;
		uint16_t width = nowScene->backWidth;
		uint16_t height = nowScene->backHeight;
		if (addr < 0) {
			lcd->colorb(lcd->p, -addr);
			lcd->clear(lcd->p);
		} else {
			for (uint16_t y = 0; y < lcd->p->height; y += height)
				for (uint16_t x = 0; x < lcd->p->width; x += width)
					lcd->bitmap(
						lcd->p, x, y, width, height,
						(uint32_t*) __getmem_C_(addr)
					);
		}

		//Draw icon
		if (nowScene->icons != NULL) {
			uint32_t addr;
			uint16_t x, y, w, h;
			GalIcon* now = nowScene->icons;
			while (now != NULL) {
				addr = now->iconAddr;
				x = now->iconX;
				y = now->iconY;
				w = now->iconWidth;
				h = now->iconHeight;
				lcd->iconc(lcd->p, x, y, w, h, __getmem_C_(addr));
				now = now->next;
			}
		}

		//Draw text
		if (nowScene->text.parts != NULL) {
			GalText* txt = &(nowScene->text);
			uint32_t posX = 64, posY = 64, textCnt = 0;

			posX = txt->posX; posY = txt->posY;

			uint32_t font = FONT_ADDR, color;
			uint16_t x, y; uint8_t* str;
			TextPart* part = txt->parts;
			while (part != NULL) {
				x = posX;
				y = posY + textCnt * 16;
				str = part->textShow;

				color = part->textBack;
				lcd->colorb(lcd->p, color);
				color = part->textFore;
				lcd->colorf(lcd->p, color);

				draw(font, x, y, str);

				color = defBack;
				lcd->colorb(lcd->p, color);
				color = defFore;
				lcd->colorf(lcd->p, color);

				textCnt += 1;
				part = part->next;
			}

			//Handle input
			uint8_t* ptrS = txt->ptrS; int ptrPos = 0;
			while (1) {
				if (checkKey(KEY_F1) && checkKey(KEY_F8))
					return 0;

				x = posX - 32;
				y = posY + ptrPos * 16;
				str = ptrS;

				color = txt->ptrSback;
				lcd->colorb(lcd->p, color);
				color = txt->ptrSfore;
				lcd->colorf(lcd->p, color);

				draw(font, x, y, str);

				uint32_t tmp = strlen(str);
				str = (uint8_t*) malloc(tmp + 1);
				memset(str, ' ', tmp); str[tmp] = '\0';
				y = posY + (ptrPos - 1) * 16;
				draw(font, x, y, str);
				y = posY + (ptrPos + 1) * 16;
				draw(font, x, y, str);
				free(str);

				color = defBack;
				lcd->colorb(lcd->p, color);
				color = defFore;
				lcd->colorf(lcd->p, color);

				if (checkKeyUp(LPAD_UP)) ptrPos -= 1;
				if (checkKeyUp(LPAD_DOWN)) ptrPos += 1;
				if (ptrPos > textCnt - 1) ptrPos = textCnt - 1;
				if (ptrPos < 0) ptrPos = 0;

				if (checkKeyUp(RPAD_UP)) {
					tmp = 0; part = txt->parts; uint8_t* jump = NULL;
					while (part != NULL) {
						if (tmp == ptrPos) {
							jump = part->textJump;
							break;
						} else tmp += 1;
						part = part->next;
					}
					if (jump != NULL) {
						uint8_t* name = NULL;
						if (strcmp(jump, "__exit") == 0)
							return 0;
						else if (strcmp(jump, "__this") == 0)
							continue;
						else {
							uint32_t i = 0; uint8_t state = 0;
							while (galScenes[i] != NULL) {
								name = galScenes[i]->name;
								if (strcmp(name, jump) == 0) {
									state = 1; break;
								}
								i += 1;
							}
							if (state != 0) {
								scenePtr = i;
								nowScene = galScenes[scenePtr];
								break;
							}
						}
					}
				}
			}
		}
	}

	lcd->colorb(lcd->p, defBack);
	lcd->colorf(lcd->p, defFore);
	lcd->clear(lcd->p);

	return 0;
}
