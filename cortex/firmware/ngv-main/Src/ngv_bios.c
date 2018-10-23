#include "ngv_bios.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"

#include <setjmp.h>

#include "usbd_core.h"

#include "nshel.h"
#include "flash.h"
#include "nsio.h"
#include "logo.h"
#include "lcd.h"
#include "key.h"

#include "99_8b.h"

#define NGV_SYS_VERSION "181023"

LCD* lcd;
Flash* flash;
FILTYPE file;
jmp_buf rstPos;
uint8_t FS_OK = 0;

//HAL_SD_CardInfoTypeDef cardInfo;

extern USBD_HandleTypeDef hUsbDeviceFS;
//extern SD_HandleTypeDef hsd;
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

void ngv_setup() {
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
	print("Init flash...\n");
	result = flash->begin(flash->p);
	print("  State: %d\n", result);
	print("  PID: 0x%X\n", flash->readPartID(flash->p));
	print("  UID: 0x%X\n", flash->readUniqueID(flash->p));
	print("\n");
	delay(1000);

	print("Press F8 to erase flash.\n");
	delay(500);
	if (waitKeyUp(KEY_F8)) {
		print("Erase flash...\n");
		flash->eraseAll(flash->p);
		while (flash->busy(flash->p));
		print("Erase OK\n");
	}
	delay(500);
	print("\n");
	/*
	if (HAL_SD_Init(&hsd) == HAL_OK) {
		print("Init SD card... OK\n");
		HAL_SD_GetCardInfo(&hsd, &cardInfo);
		print("Card size: %dMB\n", (cardInfo.BlockNbr * cardInfo.BlockSize) >> 20);
	} else {
		print("Init SD card... ERR\n");
	}
	print("\n");
	delay(1000);
	*/
	print("Mount file system...\n");
	result = f_mount(&USERFatFS, USERPath, 1);
	delay(1000);
	if(result == FR_OK) {
		char path[] = "NGV_INFO.TXT";
		f_open(&file, path, FA_WRITE | FA_CREATE_ALWAYS);
		f_printf(&file, "NyaGame Vita v1.0 with STM32F429ZGT6 and AL3S10LG144\n");
		f_printf(&file, "by NyaSama Developer Network\n");
		f_printf(&file, "Firmware Version: %s\n", NGV_SYS_VERSION);
		f_close(&file);
		print("Test file system... OK\n");
		FS_OK = 1;
	} else {
		print("Test file system... ERR: %02X\n", result);
		FS_OK = 0;
	}
	print("\n");
	delay(1000);

	print("Press F4 to mount USB Mass Storage.\n");
	delay(500);
	if (waitKeyUp(KEY_F4)) {
		print("Init USB Mass Storage...\n");
		USBD_Start(&hUsbDeviceFS);
	}
	delay(500);
	print("\n");

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
void ngv_loop() {
	lcd->colorb(lcd->p, 0xFF0000);
	lcd->clear(lcd->p);
	lcd->colorb(lcd->p, 0x00FF00);
	lcd->clear(lcd->p);
	lcd->colorb(lcd->p, 0x0000FF);
	lcd->clear(lcd->p);
	if (cnt < 128) cnt += 1;
	else {
		cnt = 0;
		greenScreen("END OF SYSTEM");
		delay(3000);
		longjmp(rstPos, 0);
	}
}
