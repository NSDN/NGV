#ifndef __LCD_H_
#define __LCD_H_


#include "halinc.h"

#define IOBUF_SIZE 128
#define IOBUF_WIDTH 80
#define IOBUF_HEIGHT 60

#define LCD_PORTRAIT 2
#define LCD_PORTRAIT_ANTI 0
#define LCD_LANDSCAPE 3
#define LCD_LANDSCAPE_ANTI 1

//#define LCD_USE_PRIVATE_FUN
#define LCD_USE_REG_ACCESS

typedef enum {
	Small = 1,
	Big = 2
} LCDFont;

typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
} pIO;

typedef struct {
	pIO rd, wr, rs, cs, rst;
	pIO data[8];
	uint32_t cdata[8];
	uint16_t width;
	uint16_t height;
	LCDFont Font;
	uint16_t backColor;
	uint16_t foreColor;
	uint8_t rotate;
	uint16_t ptrX;
	uint16_t ptrY;
	char buffer[IOBUF_HEIGHT][IOBUF_WIDTH];
} pLCD;

typedef struct {
	pLCD* p;
	#ifdef LCD_USE_PRIVATE_FUN
	void (*writeCommand)(pLCD* p, uint8_t cmd);
	void (*writeData)(pLCD* p, uint8_t data);
	void (*writeData16)(pLCD* p, uint16_t data);
	void (*setPosition)(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	#endif
	void (*init)(pLCD* p);
	void (*reset)(pLCD* p);
	void (*colorb)(pLCD* p, uint32_t color);
	void (*colorf)(pLCD* p, uint32_t color);
	void (*font)(pLCD* p, LCDFont f);
	void (*clear)(pLCD* p);
	void (*scroll)(pLCD* p, uint16_t pos);
	void (*rotate)(pLCD* p, uint8_t r);
	void (*pixel)(pLCD* p, uint16_t x, uint16_t y);
	void (*line)(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void (*tri)(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t fill);
	void (*rect)(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t fill);
	void (*bitmap)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data);
	void (*bitmapc)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* data);
	void (*bitmapt)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, uint16_t* data);
	void (*bitmaptc)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, uint16_t* data);
	void (*bitmaps)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data);
	void (*bitmapsc)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data);
	void (*draw)(pLCD* p, uint16_t x, uint16_t y, char character);
	void (*print)(pLCD* p, uint16_t x, uint16_t y, char* string);
	int (*printf)(pLCD* p, uint16_t x, uint16_t y, const char* format, ...);
	int (*printfc)(pLCD* p, uint16_t y, const char* format, ...);
	int (*printfa)(pLCD* p, const char* format, ...);
} LCD;

LCD* LCDInit(pIO* RD, pIO* WR, pIO* RS, pIO* CS, pIO* RST, pIO data[]);


#endif
