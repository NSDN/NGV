#ifndef __SDL_H_
#define __SDL_H_


#include <cstdint>

void delay(uint32_t ms);
void progress();

void pos(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void write(uint32_t data);
void writes(uint32_t* data, uint32_t len);
void flash(uint32_t data, uint32_t n);


#endif
