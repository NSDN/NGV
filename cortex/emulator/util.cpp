#include "util.h"

long time = 0;

long millis() { return time; }
void tTick() { time += 1; }
void tReset() { time = 0; }

uint16_t keyValue = 0;
uint16_t tmpState;

extern void processEvent();

bool checkKey(uint16_t key) {
    return (keyValue & key) > 0;
}

bool checkKeyUp(uint16_t key) {
	if (checkKey(key)) {
		tmpState |= key;
		return false;
	} else if (tmpState & key) {
		tmpState &= ~key;
		return true;
	}
	return false;
}

bool waitKey(uint16_t key) {
	if (!checkKey(key)) {
		while (!checkKey(key)) 
            processEvent();
		return true;
	}
	return false;
}

bool waitKeyUp(uint16_t key) {
	if (checkKey(key)) {
		while (checkKey(key)) 
            processEvent();
		return true;
	}
	return false;
}
