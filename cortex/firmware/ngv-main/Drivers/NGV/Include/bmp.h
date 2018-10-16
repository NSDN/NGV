#ifndef __BMP_H_
#define __BMP_H_


#include "halinc.h"

typedef struct {
	uint16_t	bfType;
	uint32_t	bfSize;
	uint16_t	bfReserved1;
	uint16_t	bfReserved2;
	uint32_t	bfOffBits;
} BitmapFileHeader;

typedef struct {
	uint32_t	biSize;
	uint32_t	biWidth;
	uint32_t	biHeight;
	uint16_t	biPlanes;
	uint16_t	biBitCount;
	uint32_t	biCompression;
	uint32_t	biSizeImage;
	uint32_t	biXPelsPerMeter;
	uint32_t	biYPelsPerMeter;
	uint32_t	biClrUsed;
	uint32_t	biClrImportant;
} BitmapInfoHeader;

typedef struct {
	BitmapFileHeader fileHeader;
	BitmapInfoHeader infoHeader;
} BitmapHeader;

int bmp_main(int argc, char* argv[]);


#endif
