#ifndef __NGV_BIOS_H_
#define __NGV_BIOS_H_


void setup();
void loop();

#ifdef __cplusplus
extern "C" {
#endif

void delay(int t);
void processEvent();

#ifdef __cplusplus
}
#endif


#endif
