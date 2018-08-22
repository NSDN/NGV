#include "sdl.h"
#include "util.h"

#include "./Drivers/NGV/Include/nshel.h"
#include "./Drivers/NGV/Include/nsio.h"
#include "./Drivers/NGV/Include/logo.h"
#include "./Drivers/NGV/Include/lcd.h"
LCD* lcd;

void setup() {
    lcd = LCDInit();
    lcd->init(lcd->p);
    delay(1000);
    lcd->colorb(lcd->p, 0xFF9800);
    lcd->clear(lcd->p);
    delay(1000);
}

void loop() {
    
}
