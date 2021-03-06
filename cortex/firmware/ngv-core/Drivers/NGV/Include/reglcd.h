#ifndef __REGLCD_H_
#define __REGLCD_H_


#define LCD_INVON		0x20
#define LCD_INVOFF		0x21
#define LCD_CADDR		0x2A
#define LCD_PADDR		0x2B
#define LCD_MEMWR		0x2C
#define LCD_MADCTL		0x36

#define LCD_MADCTL_MY  0x80
#define LCD_MADCTL_MX  0x40
#define LCD_MADCTL_MV  0x20
#define LCD_MADCTL_ML  0x10
#define LCD_MADCTL_RGB 0x00
#define LCD_MADCTL_BGR 0x08
#define LCD_MADCTL_MH  0x04
#define LCD_MADCTL_HF  0x02
#define LCD_MADCTL_VF  0x01

#define LCD_DELAY 0xFFFF
const unsigned short _regValues[] = {
	0x11, 0,
	LCD_DELAY, 50,
	0xD0, 3, 0x07, 0x42, 0x18,
	0xD1, 3, 0x00, 0x07, 0x10,

	0xD2, 2, 0x01, 0x02,
	0xC0, 5, 0x10, 0x3B, 0x00,0x02,0x11,
	0xC5, 1, 0x00,
	0x36, 1, 0x0A,
	0x3A, 1, 0x55,
	0x2A, 4, 0x00, 0x00,0x01,0x3F,
	0x2B, 4, 0x00, 0x00,0x01,0xE0,
	LCD_DELAY, 50,
	0x29, 0,
	0x2C, 0,
	LCD_DELAY, 50,
};


#endif
