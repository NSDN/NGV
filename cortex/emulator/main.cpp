#include <cstdlib>
#include <cstdint>
#include <setjmp.h>

uint8_t initFlag = 0, endFlag = 0, reset = 0;
jmp_buf rstPos;
uint8_t FS_OK = 1;

extern void setup();
extern void loop();

extern void initSDL();
extern void deinitSDL();
extern void resizeWindow(uint16_t scale);
extern void processEvent();

extern void tTick();
extern void tReset();

int main(int argc, char* argv[]) {
	uint16_t scale = 1;
	if (argc > 1) scale = atoi(argv[1]);
	if (scale < 1) scale = 1;

	initSDL();

	setjmp(rstPos);
RST:
	initFlag = endFlag = reset = 0;
	tReset();
	resizeWindow(scale);

	setup();
	initFlag = 1;

	while (!endFlag) {
		processEvent();
		if (reset > 0) goto RST;

		loop();	

		tTick();
	}
	deinitSDL();

	return 0;
}
