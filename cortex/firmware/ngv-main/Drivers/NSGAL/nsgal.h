#ifndef __NSGAL_H_
#define __NSGAL_H_


#include "nsio.h"

typedef struct _GalIcon {
	uint16_t iconX;
	uint16_t iconY;
	uint32_t iconAddr;
	uint16_t iconWidth;
	uint16_t iconHeight;

	struct _GalIcon* next;
} GalIcon;

typedef struct _TextPart {
	uint32_t textBack;
	uint32_t textFore;
	uint8_t* textShow;
	uint8_t* textJump;

	struct _TextPart* next;
} TextPart;

typedef struct _GalText {
	uint16_t posX;
	uint16_t posY;
	uint8_t* ptrS;
	uint32_t ptrSback;
	uint32_t ptrSfore;

	TextPart* parts;
} GalText;

typedef struct _GalScene {
	uint8_t* name;
	int backAddr;
	uint16_t backWidth;
	uint16_t backHeight;

	GalIcon* icons;
	GalText text;
} GalScene;

int nsgal(int argc, char** argv);


#endif
