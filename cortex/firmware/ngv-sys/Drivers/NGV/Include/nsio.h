#ifndef __NSIO_H_
#define __NSIO_H_


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


#endif
