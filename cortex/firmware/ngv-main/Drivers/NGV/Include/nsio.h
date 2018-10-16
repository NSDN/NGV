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

void pause();

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
#define FIL_READ    1
#define FIL_WRITE   2
#define FIL_BIN     4

uint8_t filopen(FILTYPE* file, char* name, uint8_t mode);
void filclose(FILTYPE* file);
void filread(FILTYPE* file, uint8_t* buf, uint32_t len, uint32_t* ptr);
void filwrite(FILTYPE* file, uint8_t* buf, uint32_t len);
void filgets(FILTYPE* file, char* buf, uint32_t len);
int filprint(FILTYPE* file, const char* format, ...);
uint8_t fileof(FILTYPE* file);
uint32_t filsiz(FILTYPE* file);

#ifdef __cplusplus
}
#endif


#endif
