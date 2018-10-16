#include "./Include/lcd.h"
#include "./Include/fonts.h"
#include "./Include/reglcd.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#ifdef LCD_IS_EMU
#include "../../sdl.h"
static uint8_t cmdBuf;
extern void processEvent();
#endif

static uint32_t _buf[512] = { 0 };

static uint32_t _fmcAddr = 0x60000000;
static uint8_t _fmcData = 0;

const int LCD_BLK = 0x01;
const int LCD_CS = 0x02;
const int LCD_RS = 0x04;
const int LCD_RST = 0x08;
const int LCD_IOCTL = 0x80;

#ifndef LCD_IS_EMU
#define _data(v) *(__IO uint8_t*) (_fmcAddr + ((v) & 0xFFFFFF)) = _fmcData | LCD_IOCTL
#define _ioctl() *(__IO uint8_t*) (_fmcAddr + 0xFFFFFF) = _fmcData
#endif

void _lcd_writeCommand(pLCD* p, uint8_t cmd) {
#ifndef LCD_IS_EMU
	_fmcData &= ~LCD_RS; //CMD
	_fmcData &= ~LCD_CS; //NCS
	_ioctl();
	_data(cmd);
	_fmcData |= LCD_CS; //CS
	_ioctl();
#else
	cmdBuf = cmd;
#endif
}

void _lcd_writeData(pLCD* p, uint8_t data) {
#ifndef LCD_IS_EMU
	_fmcData |= LCD_RS; //DATA
	_fmcData &= ~LCD_CS; //NCS
	_ioctl();
	_data(data);
	_fmcData |= LCD_CS; //CS
	_ioctl();
#endif
}

void _lcd_writeReg8(pLCD* p, uint8_t cmd, uint8_t data) {
#ifndef LCD_IS_EMU
	_fmcData &= ~LCD_RS; //CMD
	_fmcData &= ~LCD_CS; //NCS
	_ioctl();
	_data(cmd);
	_fmcData |= LCD_CS; //CS
	_ioctl();

	_fmcData |= LCD_RS; //DATA
	_fmcData &= ~LCD_CS; //NCS
	_ioctl();
	_data(data);
	_fmcData |= LCD_CS; //CS
	_ioctl();
#endif
}

void _lcd_writeReg32(pLCD* p, uint8_t cmd, uint32_t data) {
#ifndef LCD_IS_EMU
	_fmcData &= ~LCD_RS; //CMD
	_fmcData &= ~LCD_CS; //NCS
	_ioctl();
	_data(cmd);
	_fmcData |= LCD_CS; //CS
	_ioctl();

	_fmcData |= LCD_RS; //DATA
	_fmcData &= ~LCD_CS; //NCS
	_ioctl();
	_data((data >> 24) & 0xFF);
	_data((data >> 16) & 0xFF);
	_data((data >> 8 ) & 0xFF);
	_data((data      ) & 0xFF);
	_fmcData |= LCD_CS; //CS
	_ioctl();
#endif
}

void _lcd_writeData32(pLCD* p, uint32_t data) {
#ifndef LCD_IS_EMU
	_fmcData |= LCD_RS; //DATA
	_fmcData &= ~LCD_CS; //NCS
	_ioctl();
	_data(data);
	_fmcData |= LCD_CS; //CS
	_ioctl();
#else
	if (cmdBuf == LCD_MEMWR) write(data);
#endif
}

void _lcd_writeData32s(pLCD* p, uint32_t* data, uint32_t length) {
#ifndef LCD_IS_EMU
	_fmcData |= LCD_RS; //DATA
	_fmcData &= ~LCD_CS; //NCS
	_ioctl();
	for (uint32_t i = 0; i < length; i++)
		_data(data[i]);
	_fmcData |= LCD_CS; //CS
	_ioctl();
#else
	if (cmdBuf == LCD_MEMWR) writes(data, length);
#endif
}

void _lcd_flashData32(pLCD* p, uint32_t data, uint32_t count) {
#ifndef LCD_IS_EMU
	_fmcData |= LCD_RS; //DATA
	_fmcData &= ~LCD_CS; //NCS
	_ioctl();
	for (uint32_t i = 0; i < count; i++)
		_data(data);
	_fmcData |= LCD_CS; //CS
	_ioctl();
#else
	if (cmdBuf == LCD_MEMWR) flash(data, count);
#endif
}

void _lcd_setPosition(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
#ifndef LCD_IS_EMU
	uint32_t t;

    t = x1;
    t <<= 16;
    t |= x2;
    _lcd_writeReg32(p, LCD_CADDR, t);
    t = y1;
    t <<= 16;
    t |= y2;
    _lcd_writeReg32(p, LCD_PADDR, t);
#else
	pos(x1, y1, x2, y2);
#endif
}

void _lcd_reset(pLCD* p) {
#ifndef LCD_IS_EMU
	_fmcData |= LCD_CS; //CS
	_fmcData &= ~LCD_RST; //NRST
	_ioctl();
	HAL_Delay(10);
	_fmcData |= LCD_RST; //RST
	_ioctl();
	HAL_Delay(100);

	_fmcData |= LCD_BLK;
	_ioctl();
#else
	p->backColor = 0x000000;
	p->foreColor = 0xFFFFFF;
	_lcd_setPosition(p, 0, 0, p->width - 1, p->height - 1);
	_lcd_writeCommand(p, LCD_MEMWR);
	_lcd_flashData32(p, p->backColor, p->width * p->height);
	p->ptrX = p->ptrY = 0;
#endif
}

void _lcd_rotate(pLCD* p, uint8_t r) {
#ifndef LCD_IS_EMU
	uint8_t t = 0; uint16_t tmp;
	p->rotate = r;
	switch (r) {
		case LCD_PORTRAIT:
			t = 0;
			break;
		case LCD_LANDSCAPE:
			t = LCD_MADCTL_MV | LCD_MADCTL_HF;
			tmp = p->width; p->width = p->height; p->height = tmp;
			break;
		case LCD_PORTRAIT_ANTI:
			t = LCD_MADCTL_MY | LCD_MADCTL_HF;
			break;
		case LCD_LANDSCAPE_ANTI:
			t = LCD_MADCTL_MV | LCD_MADCTL_MY ;
			tmp = p->width; p->width = p->height; p->height = tmp;
			break;
	}
	_lcd_writeReg8(p, LCD_MADCTL, t | LCD_MADCTL_BGR);
	_lcd_setPosition(p, 0, 0, p->width - 1, p->height - 1);
#else
	p->width = 854; p->height = 480;
#endif
}

void _lcd_init(pLCD* p) {
	_lcd_reset(p);
	
	uint32_t i = 0;
	uint16_t r = 0, len = 0, x = 0;
	uint16_t size = sizeof(_regValues) / sizeof(unsigned short);
	while(i < size) {
		r = _regValues[i++];
		len = _regValues[i++];
	#ifndef LCD_IS_EMU
		if(r == LCD_DELAY) {
			HAL_Delay(len);
		} else {
			_fmcData &= ~LCD_RS; //CMD
			_fmcData &= ~LCD_CS; //NCS
			_ioctl();
			_data(r & 0xFF);
			_fmcData |= LCD_CS; //CS
			_ioctl();

			_fmcData |= LCD_RS; //DATA
			_fmcData &= ~LCD_CS; //NCS
			_ioctl();
			for (uint16_t d = 0; d < len; d++) {
				x = _regValues[i++];
				_data(x & 0xFF);
			}
			_fmcData |= LCD_CS; //CS
			_ioctl();
		}
	#endif
    }
	
	_lcd_rotate(p, p->rotate);
}

void _lcd_back_color(pLCD* p, uint32_t color) { p->backColor = color & 0xFFFFFF; }

void _lcd_fore_color(pLCD* p, uint32_t color) { p->foreColor = color & 0xFFFFFF; }

void _lcd_font(pLCD* p, LCDFont f) { p->Font = f; }

void _lcd_scale(pLCD* p, uint8_t scale) { p->scale = scale > 1 ? 2 : 1; }

void _lcd_clear(pLCD* p) {
	_lcd_setPosition(p, 0, 0, p->width - 1, p->height - 1);
	_lcd_writeCommand(p, LCD_MEMWR);
	_lcd_flashData32(p, p->backColor, p->width * p->height);
	p->ptrX = p->ptrY = 0;
}

float _lcd_abs(float v) {
	return v > 0 ? v : -v;
}

int _lcd_absi(int v) {
	return v > 0 ? v : -v;
}

float _lcd_round(float v) {
	if (v - (float) ((int) v) > 0)
		return (float) ((int) v + 1);
	if (v - (float) ((int) v) < 0)
		return (float) ((int) v - 1);
	return v;
}

void _lcd_pixel(pLCD* p, uint16_t x, uint16_t y) {  
	_lcd_setPosition(p, x, y, x, y);
	_lcd_writeCommand(p, LCD_MEMWR);
	_lcd_writeData32(p, p->foreColor);
}

void _lcd_line(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	float tx = x2 - x1, ty = y2 - y1;
	if (tx == 0) {
		_lcd_setPosition(p, x1, ty > 0 ? y1 : y2, x1, ty > 0 ? y2 : y1);
		_lcd_writeCommand(p, LCD_MEMWR);
		_lcd_flashData32(p, p->foreColor, _lcd_absi(ty));
	} else if (ty == 0) {
		_lcd_setPosition(p, tx > 0 ? x1 : x2, y1, tx > 0 ? x2 : x1, y1);
		_lcd_writeCommand(p, LCD_MEMWR);
		_lcd_flashData32(p, p->foreColor, _lcd_absi(tx));
	} else {
		if (_lcd_abs(tx) > _lcd_abs(ty)) {
			float dx = tx / _lcd_abs(ty), dy = ty / _lcd_abs(ty);
			for (float x = x1, y = y1; y != y2; x += dx, y += dy) {
				_lcd_setPosition(p, dx > 0 ? x : x + dx, y, dx > 0 ? x + dx : x, y);
				_lcd_writeCommand(p, LCD_MEMWR);
				_lcd_flashData32(p, p->foreColor, _lcd_absi(_lcd_round(dx)));
			}
		} else {
			float dx = tx / _lcd_abs(tx), dy = ty / _lcd_abs(tx);
			for (float x = x1, y = y1; x != x2; x += dx, y += dy) {
				_lcd_setPosition(p, x, dy > 0 ? y : y + dy, x, dy > 0 ? y + dy : y);
				_lcd_writeCommand(p, LCD_MEMWR);
				_lcd_flashData32(p, p->foreColor, _lcd_absi(_lcd_round(dy)));
			}
		}
	}
}

float _lcd_line_func(char v, float i, float x1, float y1, float x2, float y2) {
	if (v == 'x') 
		return (y2 - y1) / (x2 - x1) * (i - x1) + y1;
	else if (v == 'y')
		return (x2 - x1) / (y2 - y1) * (i - y1) + x1;
	return i;
}

int _lcd_tri_min(int a, int b, int c) {
	return a > b ? (b > c ? c : b) : (a > c ? c : a);
}

char _lcd_tri_judge(int x1, int y1, int x2, int y2, int x3, int y3) {
	int xs[] = {
		_lcd_absi(x2 - x1) + _lcd_absi(x3 - x1),
		_lcd_absi(x1 - x2) + _lcd_absi(x3 - x2),
		_lcd_absi(x1 - x3) + _lcd_absi(x2 - x3)
	};
	int ys[] = {
		_lcd_absi(y2 - y1) + _lcd_absi(y3 - y1),
		_lcd_absi(y1 - y2) + _lcd_absi(y3 - y2),
		_lcd_absi(y1 - y3) + _lcd_absi(y2 - y3)
	};
	int xm = _lcd_tri_min(xs[0], xs[1], xs[2]);
	int ym = _lcd_tri_min(ys[0], ys[1], ys[2]);

	if (xm >= ym) return 'x';
	else return 'y';
}

void _lcd_tri(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t fill) {
	if (fill) {
		int xs = 0, ys = _lcd_tri_min(y1, y2, y3), xe = 0, ye = 0;
		int xa, ya, xb, yb;
		if (ys == y1) { xs = x1; xa = x2; ya = y2; xb = x3; yb = y3; }
		else if (ys == y2) { xs = x2; xa = x1; ya = y1; xb = x3; yb = y3; }
		else { xs = x3; xa = x1; ya = y1; xb = x2; yb = y2; }
		
		if (_lcd_tri_judge(x1, y1, x2, y2, x3, y3) == 'x') {
			int y = ys; float xl, xr;
			for (; y != ya && y != yb; y++) {
				xl = _lcd_line_func('y', y, xs, ys, xa, ya);
				xr = _lcd_line_func('y', y, xs, ys, xb, yb);
				_lcd_setPosition(p, xl > xr ? xr : xl, y, xl > xr ? xl : xr, y);
				_lcd_writeCommand(p, LCD_MEMWR);
				_lcd_flashData32(p, p->foreColor, _lcd_absi(xl - xr));
			}
			if (y == ya) { xe = xb; ye = yb; }
			else if (y == yb) { xe = xa; ye = ya; }
			for (; y != ye; y++) {
				xl = _lcd_line_func('y', y, xs, ys, xe, ye);
				xr = _lcd_line_func('y', y, xa, ya, xb, yb);
				_lcd_setPosition(p, xl > xr ? xr : xl, y, xl > xr ? xl : xr, y);
				_lcd_writeCommand(p, LCD_MEMWR);
				_lcd_flashData32(p, p->foreColor, _lcd_absi(xl - xr));
			}
		} else {
			int x = xs; float yl, yr;
			for (; x != xa && x != xb; x++) {
				yl = _lcd_line_func('x', x, xs, ys, xa, ya);
				yr = _lcd_line_func('x', x, xs, ys, xb, yb);
				_lcd_setPosition(p, x, yl > yr ? yr : yl, x, yl > yr ? yl : yr);
				_lcd_writeCommand(p, LCD_MEMWR);
				_lcd_flashData32(p, p->foreColor, _lcd_absi(yl - yr));
			}
			if (x == xa) { xe = xb; ye = yb; }
			else if (x == xb) { xe = xa; ye = ya; }
			for (; x != xe; x++) {
				yl = _lcd_line_func('x', x, xs, ys, xe, ye);
				yr = _lcd_line_func('x', x, xa, ya, xb, yb);
				_lcd_setPosition(p, x, yl > yr ? yr : yl, x, yl > yr ? yl : yr);
				_lcd_writeCommand(p, LCD_MEMWR);
				_lcd_flashData32(p, p->foreColor, _lcd_absi(yl - yr));
			}
		}
	} else {
		_lcd_line(p, x1, y1, x2, y2);
		_lcd_line(p, x2, y2, x3, y3);
		_lcd_line(p, x3, y3, x1, y1);
	}
}

void _lcd_rect(pLCD* p, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t fill) {
	if (fill) {
		_lcd_setPosition(p, x1, y1, x2, y2);
		_lcd_writeCommand(p, LCD_MEMWR);
		_lcd_flashData32(
			p, p->foreColor,
			(_lcd_absi((int) x2 - (int) x1) + 1) * (_lcd_absi((int) y2 - (int) y1) + 1)
		);
	} else {
		_lcd_line(p, x1, y1, x2, y1);
		_lcd_line(p, x2, y1, x2, y2);
		_lcd_line(p, x2, y2, x1, y2);
		_lcd_line(p, x1, y2, x1, y1);
	}
}

void _lcd_bitmap(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t* data) {
	_lcd_setPosition(p, x, y, x + w - 1, y + h - 1);
	_lcd_writeCommand(p, LCD_MEMWR);
	_lcd_writeData32s(p, data, w * h);
}

void _lcd_bitmapc(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t* data) {
	_lcd_bitmap(p, x - w / 2, y - h / 2, w, h, data);
}

void _lcd_bitmapt(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, const unsigned char* data) {
	uint32_t c = 0; uint32_t colort = trans & 0xFFFFFF, color = 0;
	for (uint16_t j = 0; j < h; j++) {
		for (uint16_t i = 0; i < w; i++) {
			color = data[c + 2] << 16;
			color |= data[c + 1] << 8;
			color |= data[c];
			if (color != colort) {
				_lcd_setPosition(p, x + i, y + j, x + i, y + j);
				_lcd_writeCommand(p, LCD_MEMWR);
				_lcd_writeData32(p, color);
			}
			c += 3;
		#ifdef LCD_IS_EMU
			if (((c / 3) % 16) == 0) processEvent();
		#endif
		}
	}
}

void _lcd_bitmaptc(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t trans, const unsigned char* data) {
	_lcd_bitmapt(p, x - w / 2, y - h / 2, w, h, trans, data);
}

void _lcd_bitmaps(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data) {
	uint32_t c = 0;
	for (uint16_t m = 0; m < h; m += 8) {
		for (uint16_t n = 0; n < w; n += 8) {
			for (uint8_t i = 0; i < 8; i++) {
				for (uint8_t j = 0; j < 8; j++)
					if (data[c] & (1 << j))
						_buf[i + j * 8] = p->foreColor;
					else 
						_buf[i + j * 8] = p->backColor;
				c++;
			}
			_lcd_setPosition(p, x + n, y + m, x + n + 7, y + m + 7);
			_lcd_writeCommand(p, LCD_MEMWR);
			_lcd_writeData32s(p, _buf, 64);
		}
	}
}

void _lcd_bitmapsc(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data) {
	_lcd_bitmaps(p, x - w / 2, y - h / 2, w, h, data);
}

void _lcd_icon(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data) {
	if (h % 8 != 0) return;

	for (uint16_t i = 0; i < w; i++) {
		for (uint16_t j = 0; j < h; j++) {
			if (data[i * h / 8 + j / 8] & (1 << (j % 8)))
				_buf[j] = p->foreColor;
			else
				_buf[j] = p->backColor;
		}
		_lcd_setPosition(p, x + i, y, x + i, y + h - 1);
		_lcd_writeCommand(p, LCD_MEMWR);
		_lcd_writeData32s(p, _buf, h);
	}
}

void _lcd_iconc(pLCD* p, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* data) {
	_lcd_icon(p, x - w / 2, y - h / 2, w, h, data);
}

void _lcd_draw(pLCD* p, uint16_t x, uint16_t y, char character) {
	char c = character - ' ';
	if (p->scale == 1) {
		if (p->Font == Big) {
			if (x >= p->width) { x = 0; y = y + 16; }
			for (uint8_t i = 0; i < 8; i++) {
				for (uint8_t j = 0; j < 8; j++) {
					if (getFont(1)[c * 16 + i] & (1 << j))
						_buf[i + j * 8] = p->foreColor;
					else
						_buf[i + j * 8] = p->backColor;
				}
			}
			for (uint8_t i = 0; i < 8; i++) {
				for (uint8_t j = 0; j < 8; j++) {
					if (getFont(1)[c * 16 + i + 8] & (1 << j))
						_buf[i + j * 8 + 64] = p->foreColor;
					else
						_buf[i + j * 8 + 64] = p->backColor;
				}
			}
			_lcd_setPosition(p, x, y, x + 7, y + 15);
			_lcd_writeCommand(p, LCD_MEMWR);
			_lcd_writeData32s(p, _buf, 128);
		} else {
			if (x >= p->width) { x = 0; y = y + 8; }
			for (uint8_t i = 0; i < 6; i++) {
				for (uint8_t j = 0; j < 8; j++) {
					if (getFont(0)[c * 6 + i] & (1 << j))
						_buf[i + j * 6] = p->foreColor;
					else
						_buf[i + j * 6] = p->backColor;
				}
			}
			_lcd_setPosition(p, x, y, x + 5, y + 7);
			_lcd_writeCommand(p, LCD_MEMWR);
			_lcd_writeData32s(p, _buf, 48);
		}
	} else if (p->scale == 2) {
		if (p->Font == Big) {
			if (x >= p->width) { x = 0; y = y + 32; }
			for (uint8_t i = 0; i < 8; i++) {
				for (uint8_t j = 0; j < 8; j++) {
					if (getFont(1)[c * 16 + i] & (1 << j)) {
						_buf[i * 2 + j * 2 * 16] = p->foreColor;
						_buf[i * 2 + 1 + j * 2 * 16] = p->foreColor;
						_buf[i * 2 + (j * 2 + 1) * 16] = p->foreColor;
						_buf[i * 2 + 1 + (j * 2 + 1) * 16] = p->foreColor;
					} else {
						_buf[i * 2 + j * 2 * 16] = p->backColor;
						_buf[i * 2 + 1 + j * 2 * 16] = p->backColor;
						_buf[i * 2 + (j * 2 + 1) * 16] = p->backColor;
						_buf[i * 2 + 1 + (j * 2 + 1) * 16] = p->backColor;
					}
				}
			}
			for (uint8_t i = 0; i < 8; i++) {
				for (uint8_t j = 0; j < 8; j++) {
					if (getFont(1)[c * 16 + i + 8] & (1 << j)) {
						_buf[i * 2 + j * 2 * 16 + 256] = p->foreColor;
						_buf[i * 2 + 1 + j * 2 * 16 + 256] = p->foreColor;
						_buf[i * 2 + (j * 2 + 1) * 16 + 256] = p->foreColor;
						_buf[i * 2 + 1 + (j * 2 + 1) * 16 + 256] = p->foreColor;
					} else {
						_buf[i * 2 + j * 2 * 16 + 256] = p->backColor;
						_buf[i * 2 + 1 + j * 2 * 16 + 256] = p->backColor;
						_buf[i * 2 + (j * 2 + 1) * 16 + 256] = p->backColor;
						_buf[i * 2 + 1 + (j * 2 + 1) * 16 + 256] = p->backColor;
					}
				}
			}
			_lcd_setPosition(p, x, y, x + 15, y + 31);
			_lcd_writeCommand(p, LCD_MEMWR);
			_lcd_writeData32s(p, _buf, 512);
		} else {
			if (x >= p->width) { x = 0; y = y + 16; }
			for (uint8_t i = 0; i < 6; i++) {
				for (uint8_t j = 0; j < 8; j++) {
					if (getFont(0)[c * 6 + i] & (1 << j)) {
						_buf[i * 2 + j * 2 * 12] = p->foreColor;
						_buf[i * 2 + 1 + j * 2 * 12] = p->foreColor;
						_buf[i * 2 + (j * 2 + 1) * 12] = p->foreColor;
						_buf[i * 2 + 1 + (j * 2 + 1) * 12] = p->foreColor;
					} else {
						_buf[i * 2 + j * 2 * 12] = p->backColor;
						_buf[i * 2 + 1 + j * 2 * 12] = p->backColor;
						_buf[i * 2 + (j * 2 + 1) * 12] = p->backColor;
						_buf[i * 2 + 1 + (j * 2 + 1) * 12] = p->backColor;
					}
				}
			}
			_lcd_setPosition(p, x, y, x + 11, y + 15);
			_lcd_writeCommand(p, LCD_MEMWR);
			_lcd_writeData32s(p, _buf, 192);
		}
	}
}

void _lcd_scroll(pLCD* p, uint16_t pos) {
	_lcd_setPosition(p, 0, 0, p->width - 1, p->height - 1);
	_lcd_writeCommand(p, LCD_MEMWR);
	_lcd_flashData32(p, p->backColor, p->width * p->height);
	if (pos > IOBUF_HEIGHT) pos = IOBUF_HEIGHT;
	if (p->Font == Big) {
		for (uint16_t i = 0; i < p->height / 16 / p->scale - pos; i++) {
			memcpy(p->buffer[i], p->buffer[i + pos], IOBUF_WIDTH);
			for (uint16_t j = 0; p->buffer[i][j] != '\0'; j++) {
				if (j >= IOBUF_WIDTH) break;
				_lcd_draw(p, j * 8 * p->scale, i * 16 * p->scale, p->buffer[i][j]);
			}
		#ifdef LCD_IS_EMU
			processEvent();
		#endif
		}
		memset(p->buffer[p->height / 16 / p->scale - pos], 0, IOBUF_WIDTH * pos);
	} else {
		for (uint16_t i = 0; i < p->height / 8 / p->scale - pos; i++) {
			memcpy(p->buffer[i], p->buffer[i + pos], IOBUF_WIDTH);
			for (uint16_t j = 0; p->buffer[i][j] != '\0'; j++) {
				if (j >= IOBUF_WIDTH) break;
				_lcd_draw(p, j * 6 * p->scale, i * 8 * p->scale, p->buffer[i][j]);
			}
		#ifdef LCD_IS_EMU
			processEvent();
		#endif
		}
		memset(p->buffer[p->height / 8 / p->scale - pos], 0, IOBUF_WIDTH * pos);
	}
}

void _lcd_printa_(pLCD* p, char* string) {
	int i = 0;
	if (p->Font == Big) {
		while (string[i] != '\0') {
			if (p->ptrY > p->height - 16 * p->scale) {
				p->ptrX = 0;
				p->ptrY = p->height - 16 * p->scale;
				_lcd_scroll(p, 1);
			}
			if (string[i] == '\n') {
				i++;
				p->ptrX = 0;
				p->ptrY += 16 * p->scale;
				continue;
			}
			if (string[i] == 0x08) {
				i++;
				if (p->ptrX >= 8 * p->scale) p->ptrX -= 8 * p->scale;
				else {
					p->ptrX = p->width - 8 * p->scale;
					p->ptrY -= 16 * p->scale;
				}
				_lcd_draw(p, p->ptrX, p->ptrY, ' ');
				continue;
			}
			_lcd_draw(p, p->ptrX, p->ptrY, string[i]);
			p->buffer[p->ptrY / 16 / p->scale][p->ptrX / 8 / p->scale] = string[i];
			p->ptrX += 8 * p->scale;
			if (p->ptrX > p->width - 8 * p->scale) {
				p->ptrX = 0;
				p->ptrY += 16 * p->scale;
			}
			i++;
		}
		if (string[i] == '\0') p->buffer[p->ptrY / 16 / p->scale][p->ptrX / 8 / p->scale] = string[i];
	} else {
		while (string[i] != '\0') {
			if (p->ptrY > p->height - 8 * p->scale) {
				p->ptrX = 0;
				p->ptrY = p->height - 8 * p->scale;
				_lcd_scroll(p, 1);
			}
			if (string[i] == '\n') {
				i++;
				p->ptrX = 0;
				p->ptrY += 8 * p->scale;
				continue;
			}
			if (string[i] == 0x08) {
				i++;
				if (p->ptrX >= 6 * p->scale) p->ptrX -= 6 * p->scale;
				else {
					p->ptrX = p->width - 6 * p->scale;
					p->ptrY -= 8 * p->scale;
				}
				_lcd_draw(p, p->ptrX, p->ptrY, ' ');
				continue;
			}
			_lcd_draw(p, p->ptrX, p->ptrY, string[i]);
			p->buffer[p->ptrY / 8 / p->scale][p->ptrX / 6 / p->scale] = string[i];
			p->ptrX += 6 * p->scale;
			if (p->ptrX >= p->width - 6 * p->scale) {
				p->ptrX = 0;
				p->ptrY += 8 * p->scale;
			}
			i++;
		}
		if (string[i] == '\0') p->buffer[p->ptrY / 8 / p->scale][p->ptrX / 6 / p->scale] = string[i];
	}
}

void _lcd_print(pLCD* p, uint16_t x, uint16_t y, char* string) {
	int i = 0;
	if (p->Font == Big) {
		while (string[i] != '\0') {
			if (y > p->height - 16 * p->scale) {
				x = 0;
				y = 0;
				_lcd_clear(p);
			}
			if (string[i] == '\n') {
				i++;
				x = 0;
				y += 16 * p->scale;
				continue;
			}
			if (string[i] == 0x08) {
				i++;
				if (x >= 8 * p->scale) x -= 8 * p->scale;
				else {
					x = p->width - 8 * p->scale;
					y -= 16 * p->scale;
				}
				_lcd_draw(p, x, y, ' ');
				continue;
			}
			_lcd_draw(p, x, y, string[i]);
			x += 8 * p->scale;
			if (x > p->width - 8 * p->scale) {
				x = 0;
				y += 16 * p->scale;
			}
			i++;
		}
	} else {
		while (string[i] != '\0') {
			if (y > p->height - 8 * p->scale) {
				x = 0;
				y = 0;
				_lcd_clear(p);
			}
			if (string[i] == '\n') {
				i++;
				x = 0;
				y += 8 * p->scale;
				continue;
			}
			if (string[i] == 0x08) {
				i++;
				if (x >= 6 * p->scale) x -= 6 * p->scale;
				else {
					x = p->width - 6 * p->scale;
					y -= 8 * p->scale;
				}
				_lcd_draw(p, x, y, ' ');
				continue;
			}
			_lcd_draw(p, x, y, string[i]);
			x += 6 * p->scale;
			if (x >= p->width - 6 * p->scale) {
				x = 0;
				y += 8 * p->scale;
			}
			i++;
		}
	}
}

int _lcd_printf(pLCD* p, uint16_t x, uint16_t y, const char* format, ...) {
	char* iobuf = (char*) malloc(sizeof(char) * IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	_lcd_print(p, x, y, iobuf);
	free(iobuf);
	return result;
}

int _lcd_printfc(pLCD* p, uint16_t y, const char* format, ...) {
	char* iobuf = (char*) malloc(sizeof(char) * IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	uint16_t x = (p->width - strlen(iobuf) * (((p->Font == Big) ? 8 : 6 ) * p->scale)) / 2;
	_lcd_print(p, x, y, iobuf);
	free(iobuf);
	return result;
}

int _lcd_printfcp(pLCD* p, uint16_t x, uint16_t y, const char* format, ...) {
	char* iobuf = (char*) malloc(sizeof(char) * IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	x = x - (strlen(iobuf) * (((p->Font == Big) ? 8 : 6 ) * p->scale)) / 2;
	y = y - (((p->Font == Big) ? 16 : 8) * p->scale);
	_lcd_print(p, x, y, iobuf);
	free(iobuf);
	return result;
}

int _lcd_printfa(pLCD* p, const char* format, ...) {
	char* iobuf = (char*) malloc(sizeof(char) * IOBUF_SIZE);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);
	_lcd_printa_(p, iobuf);
	free(iobuf);
	return result;
}

LCD* LCDInit() {
	pLCD* p = (pLCD*) malloc(sizeof(pLCD));

	p->width = 480;
	p->height = 854;
	p->Font = Small;
	p->scale = 1;
	p->backColor = 0x000000;
	p->foreColor = 0xFFFFFF;
	p->rotate = LCD_PORTRAIT;
	p->ptrX = p->ptrY = 0;
	memset(p->buffer, 0, IOBUF_WIDTH * IOBUF_HEIGHT);
			
	LCD* c = (LCD*) malloc(sizeof(LCD));
	c->p = p;
	#ifdef LCD_USE_PRIVATE_FUN
	c->writeCommand = &_lcd_writeCommand;
	c->writeData = &_lcd_writeData;
	c->writeData32 = &_lcd_writeData32;
	c->setPosition = &_lcd_setPosition;
	#endif
	c->init = &_lcd_init;
	c->reset = &_lcd_reset;
	c->colorb = &_lcd_back_color;
	c->colorf = &_lcd_fore_color;
	c->font = &_lcd_font;
	c->scale = &_lcd_scale;
	c->clear = &_lcd_clear;
	c->scroll = *_lcd_scroll;
	c->rotate = &_lcd_rotate;
	c->pixel = &_lcd_pixel;
	c->line = &_lcd_line;
	c->tri = &_lcd_tri;
	c->rect = &_lcd_rect;
	c->bitmap = &_lcd_bitmap;
	c->bitmapc = &_lcd_bitmapc;
	c->bitmapt = &_lcd_bitmapt;
	c->bitmaptc = &_lcd_bitmaptc;
	c->bitmaps = &_lcd_bitmaps;
	c->bitmapsc = &_lcd_bitmapsc;
	c->icon = &_lcd_icon;
	c->iconc = &_lcd_iconc;
	c->draw = &_lcd_draw;
	c->print = &_lcd_print;
	c->printf = &_lcd_printf;
	c->printfc = &_lcd_printfc;
	c->printfcp = &_lcd_printfcp;
	c->printfa = &_lcd_printfa;
	
	return c;
}
