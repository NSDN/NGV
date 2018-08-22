#ifndef __BMP_H_
#define __BMP_H_


#include "halinc.h"

typedef struct {
	WORD	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
} BitmapFileHeader;

typedef struct {
	DWORD	biSize;
	LONG	biWidth;
	LONG	biHeight;
	WORD	biPlanes;
	WORD	biBitCount;
	DWORD	biCompression;
	DWORD	biSizeImage;
	LONG	biXPelsPerMeter;
	LONG	biYPelsPerMeter;
	DWORD	biClrUsed;
	DWORD	biClrImportant;
} BitmapInfoHeader;

typedef struct {
	BitmapFileHeader fileHeader;
	BitmapInfoHeader infoHeader;
} BitmapHeader;

int bmp_main(int argc, char* argv[]);


#endif
