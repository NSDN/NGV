#include "ngv_bios.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"

#include <setjmp.h>

#include "usbd_core.h"

#include "nshel.h"
#include "nsio.h"
#include "logo.h"
#include "lcd.h"

#include "99_8b.h"

#define NGV_SYS_VERSION "181016"

LCD* lcd;
FILTYPE file;
jmp_buf rstPos;
uint8_t FS_OK = 0;
HAL_SD_CardInfoTypeDef cardInfo;

extern USBD_HandleTypeDef hUsbDeviceFS;
extern SD_HandleTypeDef hsd;
extern SRAM_HandleTypeDef hsram1;

void greenScreen(const char* head) {
	lcd->colorb(lcd->p, 0x007F00);
	lcd->colorf(lcd->p, 0xFFFFFF);
	lcd->font(lcd->p, Small);
	lcd->clear(lcd->p);
	lcd->printfc(lcd->p, 64, "%s", head);
	lcd->printfc(lcd->p, 100, "WE DO NOT KNOW");
	lcd->printfc(lcd->p, 110, "WHAT HAD HAPPENED");
}

extern void tick();

void delay(int t) { HAL_Delay(t); }

void processEvent() {  }

void setup() {
	lcd = LCDInit();
	setjmp(rstPos);
	USBD_Stop(&hUsbDeviceFS);
	HAL_SRAM_WriteOperation_Enable(&hsram1);
	_99_8b_reconf();
	_99_8b_reset();

	lcd->init(lcd->p);
	delay(100);
	lcd->rotate(lcd->p, LCD_LANDSCAPE);
	lcd->font(lcd->p, Small);
	lcd->colorb(lcd->p, 0xFFFFFF);
	lcd->colorf(lcd->p, 0x000000);
	lcd->clear(lcd->p);
	delay(1000);

	lcd->bitmapsc(lcd->p, lcd->p->width / 2, 140, 64, 64, getLogo());
	lcd->printfc(lcd->p, 180, "nyagame vita");
	delay(1000);
	lcd->clear(lcd->p);

	print("NyaGame Vita Factory System\n");
	print("Version: %s\n\n", NGV_SYS_VERSION);
	delay(1000);

	/* Initialize device */
	print("Init ARM(R) Avalon(R) external bus...\n");
	print("\n");
	delay(1000);

	print("Re-configure NSDN(C) 99-8B(R) chip...\n");
	print("Reset NSDN(C) 99-8B(R) chip...\n");
	print("\n");
	delay(1000);

	uint8_t result = 0;
	if (HAL_SD_Init(&hsd) == HAL_OK)
		print("Init SD card... OK\n");
	else
		print("Init SD card... ERR\n");
	print("\n");
	delay(1000);

	print("Mount SD card...\n");
	result = f_mount(&SDFatFS, SDPath, 1);
	delay(1000);
	if(result == FR_OK) {
		char path[] = "NGV_INFO.TXT";
		f_open(&file, path, FA_WRITE | FA_CREATE_ALWAYS);
		f_printf(&file, "NyaGame Vita v1.0 with STM32F429ZGT6 and AL3S10LG144\n");
		f_printf(&file, "by NyaSama Developer Network\n");
		f_printf(&file, "Firmware Version: %s\n", NGV_SYS_VERSION);
		f_close(&file);
		print("Test SD card... OK\n");
		FS_OK = 1;
	} else {
		print("Test SD card... ERR: %02X\n", result);
		FS_OK = 0;
	}
	print("\n");
	delay(1000);

	if (FS_OK) {
		HAL_SD_GetCardInfo(&hsd, &cardInfo);
		print("Init USB Mass Storage...\n");
		USBD_Start(&hUsbDeviceFS);
	}
	print("\n");
	/* Initialize end */

	delay(1000);

	print("Loading NSHEL...\n");

	print("\n");
	const char* args[] = { "nshel", "init.d" };
	nshel(2, (char**) args);
	print("\n");
	delay(5000);
}

volatile uint8_t cnt = 0;
void loop() {
	lcd->colorb(lcd->p, 0xFF0000);
	lcd->clear(lcd->p);
	lcd->colorb(lcd->p, 0x00FF00);
	lcd->clear(lcd->p);
	lcd->colorb(lcd->p, 0x0000FF);
	lcd->clear(lcd->p);
	if (cnt < 128) cnt += 1;
	else {
		greenScreen("END OF SYSTEM");
		delay(3000);
		longjmp(rstPos, 0);
	}
}