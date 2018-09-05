#include "sdl.h"
#include "util.h"

#include "./Drivers/NGV/Include/nshel.h"
#include "./Drivers/NGV/Include/nsio.h"
#include "./Drivers/NGV/Include/logo.h"
#include "./Drivers/NGV/Include/lcd.h"
LCD* lcd;
FILTYPE file;

#define NGV_SYS_VERSION "180905"

void greenScreen(const char* head) {
	lcd->colorb(lcd->p, 0x007F00);
	lcd->colorf(lcd->p, 0xFFFFFF);
	lcd->font(lcd->p, Small);
	lcd->clear(lcd->p);
	lcd->printfc(lcd->p, 64, "%s", head);
	lcd->printfc(lcd->p, 100, "WE DO NOT KNOW");
	lcd->printfc(lcd->p, 110, "WHAT HAD HAPPENED");
	delay(3000);
	longjmp(rstPos, 0);
}

void setup() {
    lcd = LCDInit();
    lcd->init(lcd->p);
    delay(1000);

    lcd->init(lcd->p);
	lcd->rotate(lcd->p, LCD_LANDSCAPE);
	lcd->font(lcd->p, Small);
	lcd->colorb(lcd->p, 0xFFFFFF);
	lcd->colorf(lcd->p, 0x000000);
	lcd->clear(lcd->p);

	lcd->bitmapsc(lcd->p, lcd->p->width / 2, 140, 64, 64, getLogo());
	lcd->printfc(lcd->p, 180, "nyagame vita");
    delay(1000);
    lcd->clear(lcd->p);
    
    print("NyaGame Vita Factory System\n");
	print("Version: %s\n\n", NGV_SYS_VERSION);
	delay(1000);

    print("Init SD card... OK\n");

	char path[] = "NGV_INFO.TXT";
    uint8_t res = filopen(&file, path, FIL_WRITE);
    print("Mount SD card... %s\n", (res == FIL_OK) ? "OK" : "ERR");
    filprint(&file, "NyaGame Vita Factory Edition with NGV-EMU\n");
    filprint(&file, "by NyaSama Developer Network\n");
    filprint(&file, "Firmware Version: %s\n", NGV_SYS_VERSION);
    filclose(&file);
    print("Test SD card... OK\n");

	print("Loading NSHEL...\n");

    print("\n");
	const char* args[] = { "nshel", "init.d" };
	nshel(2, (char**) args);
	print("\n");
    delay(3000);
}

void loop() {
    greenScreen("END OF SYSTEM");
}
