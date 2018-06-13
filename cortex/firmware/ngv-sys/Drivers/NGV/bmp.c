#include "./Include/bmp.h"

#include "nsio.h"

FIL bmp;
UINT* ptr = 0;
WORD buf;
uint16_t width, height;

#include "./Include/lcd.h"
extern LCD* lcd;

#define swap16(v) ((v & 0x00FF) << 8 | (v & 0xFF00) >> 8)
#define swap24(v) (v | (v & 0x0000FF) << 16 | (v & 0xFF0000) >> 16)

int bmp_main(int argc, char* argv[]) {
	if (argc != 2) {
		print("Usage: bmp [FILE]\n\n");
		return 0;
	}

	if (f_open(&bmp, argv[1], FA_READ) != FR_OK) {
		print("File read error.\n\n");
		return -1;
	}

	f_read(&bmp, &buf, 2, ptr);
	if (swap16(buf) != 0x1018) {
		print("File type error.\n\n");
		return -1;
	}

	f_read(&bmp, &buf, 2, ptr); width = swap16(buf);
	f_read(&bmp, &buf, 2, ptr); height = swap16(buf);
	print("Image size: %d x %d\n", width, height);
	HAL_Delay(1000);

	f_read(&bmp, &buf, 2, ptr);
	lcd->clear(lcd->p);

	uint32_t buff;
	for (uint16_t y = 0; y < height; y++)
		for (uint16_t x = 0; x < width; x++) {
			f_read(&bmp, &buff, 3, ptr);
			lcd->bitmap(lcd->p,
					x + (lcd->p->width - width) / 2,
					y + (lcd->p->height - height) / 2,
					1, 1, &buff);
		}

	while (HAL_UART_Receive(&HUART, &buf, 1, 1) != HAL_OK);
	lcd->clear(lcd->p);

	print("\n");
	return 0;
}
