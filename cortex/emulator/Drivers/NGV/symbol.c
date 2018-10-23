#include "./Include/symbol.h"

// Time base: 10us

#define SYMBOL_LENGTH 60

const unsigned short SYMBOL[] = {
	381, 361, 340, 321, 303, 286, 270, 255, 240, 227, 214, 202, // C4 - B4
	190, 180, 170, 160, 151, 143, 135, 127, 120, 113, 107, 101, // C5 - B5
	 95,  90,  85,  80,  75,  71,  67,  63,  60,  56,  53,  50, // C6 - B6
	 47,  45,  42,  40,  37,  35,  33,  31,  30,  28,  26,  25, // C7 - B7
	 23,  22,  21,  20,  18,  17,  16,  15,  15,  14,  13,  12, // C8 - B8
};

const unsigned short SYMBOL_HALF[] = {
	190, 180, 170, 160, 151, 143, 135, 127, 120, 113, 107, 101, // C4 - B4
	 95,  90,  85,  80,  75,  71,  67,  63,  60,  56,  53,  50, // C5 - B5
	 47,  45,  42,  40,  37,  35,  33,  31,  30,  28,  26,  25, // C6 - B6
	 23,  22,  21,  20,  18,  17,  16,  15,  15,  14,  13,  12, // C7 - B7
	 11,  11,  10,  10,   9,   8,   8,   7,   7,   7,   6,   6, // C8 - B8
};

const unsigned short* getSymbol() { return SYMBOL; }
const unsigned short* getSymbolHalf() { return SYMBOL_HALF; }
