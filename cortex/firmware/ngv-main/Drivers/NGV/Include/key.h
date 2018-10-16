#ifndef __KEY_H_
#define __KEY_H_


#include "halinc.h"

#define LPAD_UP     0x0001
#define LPAD_DOWN   0x0002
#define LPAD_LEFT   0x0004
#define LPAD_RIGHT  0x0008
#define RPAD_UP     0x0010
#define RPAD_DOWN   0x0020
#define RPAD_LEFT   0x0040
#define RPAD_RIGHT  0x0080

#define KEY_F1      0x0100
#define KEY_F2      0x0200
#define KEY_F3      0x0400
#define KEY_F4      0x0800
#define KEY_F5      0x1000
#define KEY_F6      0x2000
#define KEY_F7      0x4000
#define KEY_F8      0x8000

#ifdef __cplusplus
extern "C" {
#endif

uint8_t checkKey(uint16_t key);
uint8_t checkKeyUp(uint16_t key);
uint8_t waitKey(uint16_t key);
uint8_t waitKeyUp(uint16_t key);

#ifdef __cplusplus
}
#endif


#endif
