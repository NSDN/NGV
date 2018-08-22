#ifndef __NSIO_H_
#define __NSIO_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "halinc.h"
#include "conf.h"

#include <setjmp.h>

#define OK 0
#define ERR 1
#define ETC -1

#define IOBUF 128

extern jmp_buf rstPos;
extern UART_HandleTypeDef HUART;

char* strlwr(char* s);

int scan(char* buffer);
int fscan(char* buffer, const char* format, ...);
int print(const char* format, ...);
char* read(char* path);

int lines(char* src);
char* line(char* src, int index);
char* cut(char* src, const char* head);
char* get(char* src, int start, char* buf, int size);

#include "fatfs.h"

#define FILTYPE FIL

#define FIL_ERR     0
#define FIL_OK      1
#define FIL_READ    0
#define FIL_WRITE   1

uint8_t filopen(FILTYPE* file, char* name, uint8_t mode);
void filclose(FILTYPE* file);
void filread(FILTYPE* file, uint8_t* buf, uint32_t len, uint32_t* ptr);
void filgets(FILTYPE* file, uint8_t* buf, uint32_t len);
uint8_t fileof(FILTYPE* file);

#ifdef __cplusplus
}
#endif


#endif
