#ifndef __LCD_H_
#define __LCD_H_


#include "halinc.h"

#define IOBUF_SIZE 128
#define IOBUF_WIDTH 160
#define IOBUF_HEIGHT 128

#define LCD_PORTRAIT 2
#define LCD_PORTRAIT_ANTI 0
#define LCD_LANDSCAPE 3
#define LCD_LANDSCAPE_ANTI 1

#define LCD_IS_EMU

//#define LCD_USE_PRIVATE_FUN

typedef enum {
	Small = 1,
	Big = 2
} LCDFont;

#ifndef LCD_IS_EMU
typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
} pIO;
#endif

typedef struct {
	uint16_t width;
	uint16_t height;
	LCDFont Font;
	uint8_t scale;
	uint32_t backColor;
	uint32_t foreColor;
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
	void (*writeData32)(pLCD* p, uint32_t data);
	void (*setPosition)(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	#endif
	void (*init)(pLCD* p);
	void (*reset)(pLCD* p);
	void (*colorb)(pLCD* p, uint32_t color);
	void (*colorf)(pLCD* p, uint32_t color);
	void (*font)(pLCD* p, LCDFont f);
	void (*scale)(pLCD* p, uint8_t scale);
	void (*clear)(pLCD* p);
	void (*scroll)(pLCD* p, uint16_t pos);
	void (*rotate)(pLCD* p, uint8_t r);
	void (*pixel)(pLCD* p, uint16_t x, uint16_t y);
	void (*line)(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void (*tri)(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t fill);
	void (*rect)(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t fill);
	void (*bitmap)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t* data);
	void (*bitmapc)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t* data);
	void (*bitmapt)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, const unsigned char* data);
	void (*bitmaptc)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, const unsigned char* data);
	void (*bitmaps)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data);
	void (*bitmapsc)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data);
	void (*icon)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data);
	void (*iconc)(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data);
	void (*draw)(pLCD* p, uint16_t x, uint16_t y, char character);
	void (*print)(pLCD* p, uint16_t x, uint16_t y, char* string);
	int (*printf)(pLCD* p, uint16_t x, uint16_t y, const char* format, ...);
	int (*printfc)(pLCD* p, uint16_t y, const char* format, ...);
	int (*printfcp)(pLCD* p, uint16_t x, uint16_t y, const char* format, ...);
	int (*printfa)(pLCD* p, const char* format, ...);
} LCD;

LCD* LCDInit();


#endif
