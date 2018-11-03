#include "ngv_bios.h"
#include "stm32f7xx_hal.h"
#include "fatfs.h"

#include <setjmp.h>

#include "usbd_core.h"

#include "nshel.h"
#include "flash.h"
#include "nsio.h"
#include "logo.h"
#include "lcd.h"
#include "sd.h"

#define NGV_SYS_VERSION "181026"

FILTYPE file;

extern USBD_HandleTypeDef hUsbDeviceFS;
extern jmp_buf rstPos;
extern uint8_t FS_OK;
extern LCD* lcd;
extern SD* sd;

extern ADC_HandleTypeDef hadc1;

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
void work();

void delay(int t) { HAL_Delay(t); }

void processEvent() {  }

void ngv_setup() {
	setjmp(rstPos);
	USBD_Stop(&hUsbDeviceFS);

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
	print("-- Initialize device --\n\n");
	uint8_t result = 0;
	result = sd->init(sd->p);
	uint32_t size = sd->size(sd->p);
	if (result == 0) {
		print("Init SD card... OK, %d MB\n", size / 2048);
	} else {
		print("Init SD card... ERR: %d\n", result);
	}
	print("\n");
	delay(1000);

	print("Mount file system...\n");
	result = f_mount(&USERFatFS, USERPath, 1);
	delay(1000);
	if(result == FR_OK) {
		char path[] = "NGV_INFO.TXT";
		f_open(&file, path, FA_WRITE | FA_CREATE_ALWAYS);
		f_printf(&file, "NyaGame Vita Core Edition with STM32F767ZIT6\n");
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

	if (FS_OK) {
		print("Init USB Mass Storage...\n");
		USBD_Start(&hUsbDeviceFS);
		delay(1000);
		print("\n");
	}

	print("-- Initialize end --\n\n");
	delay(1000);
	/* Initialize end */

	work();

	print("\n");
	delay(5000);
}

__IO uint32_t counter = 0; float t = 0;
__IO uint16_t buffer[1024] = { 0 };

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	counter += 1;
}

void work() {
	print("Start ADC with DMA, length: 1024...\n");
	HAL_ADC_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) buffer, 1024);

	uint32_t now = HAL_GetTick();

	while (1) {
		if (counter > 1000) {
			now = HAL_GetTick() - now;
			t = (float) now / 1000.0F;
			print("ADC with DMA finished. It tooks %f ms\n", t);
			t = 1000.0F / t * 1024.0F / 1000.0F / 1000.0F;
			print("ADC with DMA speed: %f MSPS\n", t);
			break;
		}
	}
}

void ngv_loop() {

}
