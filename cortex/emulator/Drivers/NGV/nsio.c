#include "./Include/nsio.h"

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>

#include "./Include/lcd.h"

extern LCD* lcd;
extern uint8_t FS_OK;

char* strlwr(char* s) {
	char* str;
	str = s;
	while(*str != '\0') {
		if(*str >= 'A' && *str <= 'Z') {
			*str += 'a'-'A';
		}
		str++;
	}
	return s;
}

uint8_t filopen(FILTYPE* file, char* name, uint8_t mode) {
	FILTYPE f = NULL;
	const char* base = "Assets/";
	char* path = (char*) malloc(sizeof(char) * (strlen(base) + strlen(name)));
	strcpy(path, base); strcat(path, name);
	if ((mode & FIL_READ) != 0) {
		f = fopen(path, (mode & FIL_BIN) != 0 ? "rb" : "r");
	} else if ((mode & FIL_WRITE) != 0) {
		f = fopen(path, (mode & FIL_BIN) != 0 ? "wb" : "w");
	}
	free(path);
	if (f != NULL) *file = f;
	return f != NULL ? FIL_OK : FIL_ERR;
}

void filclose(FILTYPE* file) {
	if (*file != NULL) {
		fclose(*file);
		*file = NULL;
	}
}

void filread(FILTYPE* file, uint8_t* buf, uint32_t len, uint32_t* ptr) {
	fread(buf + *ptr, 1, len, *file);
	*ptr += len;
}

void filwrite(FILTYPE* file, uint8_t* buf, uint32_t len) {
	fwrite(buf, 1, len, *file);
}

void filgets(FILTYPE* file, char* buf, uint32_t len) {
	char* ptr = fgets(buf, len, *file);
	if (ptr == NULL) buf[0] = '\r';
}

uint8_t fileof(FILTYPE* file) {
	return feof(*file) != 0;
}

uint32_t filsiz(FILTYPE* file) {
	uint32_t tmp = ftell(*file);
	fseek(*file, 0, SEEK_END);
	uint32_t size = ftell(*file);
	fseek(*file, tmp, SEEK_SET);
	return size;
}

int filprint(FILTYPE* file, const char* format, ...) {
	char* iobuf = (char*) malloc(sizeof(char) * IOBUF);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);

	fputs(iobuf, *file);

	free(iobuf);
	return result;
}

int print(const char* format, ...) {
	char* iobuf = (char*) malloc(sizeof(char) * IOBUF);
	va_list args;
	va_start(args, format);
	int result = vsprintf(iobuf, format, args);
	va_end(args);

	printf("%s", iobuf);
	lcd->printfa(lcd->p, iobuf);

	free(iobuf);
	return result;
}

extern void processEvent();
extern void progress();

void pause() {
	while (1) {
		processEvent();
		if (kbhit() != 0) {
			getch();
			break;
		}
	}
}

int scan(char* buffer) {
	unsigned char count = 0, tmp = '\0';
	while (kbhit() != 0) getch();
	while (1) {
		processEvent();
		if (kbhit() != 0) {
			tmp = getch();
			if (tmp == 0x1B || tmp == 0x03)
				exit(0);

			if (tmp == '\r') {
				lcd->draw(lcd->p, lcd->p->ptrX, lcd->p->ptrY, ' ');
				printf(" ");
				break;
			}
			buffer[count] = tmp;
			if (buffer[count] == 0x08 && count > 0) {
				count -= 1;
				lcd->printfa(lcd->p, "%c", 0x08);
				printf("\b  \b\b");
				continue;
			} else if (buffer[count] != 0x08) {
				lcd->printfa(lcd->p, "%c", buffer[count]);
				putch(buffer[count]);
				count += 1;
			}
		}
		lcd->draw(lcd->p, lcd->p->ptrX, lcd->p->ptrY, '_');
		lcd->draw(lcd->p, lcd->p->ptrX + (lcd->p->Font == Big ? 8 : 6), lcd->p->ptrY, ' ');
		progress();
	}
	buffer[count] = '\0';
	print("\n");
	return count;
}

int fscan(char* buffer, const char* format, ...) {
	scan(buffer);
	va_list args;
	va_start(args, format);
	int result = vsscanf(buffer, format, args);
	va_end(args);
	return result;
}

char* read(char* path) {
	if (FS_OK == 0) {
		print("File system error.\n");
		return OK;
	}
	uint8_t res; FILTYPE f;
	res = filopen(&f, path, FIL_READ);
	if (res != FIL_OK) {
		print("File open failed.\n");
		print("At file: %s\n\n", path);
		return OK;
	}
	int length = filsiz(&f); char tmp[2];
	char* data = (char*) malloc(sizeof(char) * (length + 1));
	length = 0;
	while (fileof(&f) != FIL_OK) {
		filgets(&f, tmp, 2);
		if (tmp[0] != '\r') {
			data[length] = tmp[0];
			length += 1;
		}
	}
	filclose(&f);
	data[length] = '\0';
	return data;
}

int lines(char* src) {
	if(src == 0) return OK;
	int cnt = 0, length = strlen(src);
	for (int i = 0; i < length; i++)
		if (src[i] == '\n') cnt += 1;
	return cnt;
}

char* line(char* src, int index) {
	if (index >= lines(src)) return OK;
	int srcLen = strlen(src), cnt = 0, pos = 0;
	char* buf = (char*) malloc(sizeof(char) * srcLen);
	char* result = 0;
	for (int i = 0; i < srcLen; i++) {
		if (index == 0) {
			for (i = 0; src[i] != '\n'; i++)
				buf[i] = src[i];
			pos = i + 1;
			result = (char*) malloc(sizeof(char) * (pos));
			for (i = 0; i < pos; i++) {
				if (i == pos - 1) {
					result[i] = '\0';
					break;
				}
				result[i] = buf[i];
			}
			free(buf);
			return result;
		}
		if (index == cnt) {
			pos = i;
			for (; src[i] != '\n'; i++)
				buf[i - pos] = src[i];
			pos = i - pos + 1;
			result = (char*) malloc(sizeof(char) * pos);
			for (i = 0; i < pos; i++) {
				if (i == pos - 1) {
					result[i] = '\0';
					break;
				}
				result[i] = buf[i];
			}
			free(buf);
			return result;
		}
		if (src[i] == '\n') cnt += 1;
	}
	return OK;
}

char* get(char* src, int start, char* buf, int size) {
	for (int i = 0; i < size; i++) {
		if (i == size - 1) {
			buf[i] = '\0';
			break;
		}
		buf[i] = src[start + i];
	}
	return buf;
}

char* cut(char* src, const char* head) {
	int srcLen = strlen(src), headLen = strlen(head) + 1;
	char* headBuf = (char*) malloc(sizeof(char) * headLen);
	char* bodyBuf = (char*) malloc(sizeof(char) * srcLen);
	int start = 0, size, cnt; char* buf = 0; char* blk = 0;
	for (int i = 0; i < srcLen - headLen; i++) {
		if (strcmp(get(src, i, headBuf, headLen), head) == 0) {
			for (; i < srcLen; i++) {
				if (src[i] == '{') {
					start = i += 1;
					for (; i < srcLen; i++) {
						if (src[i] == '}') break;
						if (i == srcLen) return OK;
						bodyBuf[i - start] = src[i];
					}
					break;
				}
			}
			size = i - start + 1;
			buf = (char*) malloc(sizeof(char) * size);
			for (cnt = i = 0; i < size; i++) {
				if (i == size - 1) {
					buf[i] = '\0';
					break;
				}
				if (i == 0 && bodyBuf[i] == '\n') {
					cnt += 1;
					continue;
				}
				if (bodyBuf[i] == ' ' && bodyBuf[i - 1] == ' ') {
					cnt += 1;
					continue;
				}
				if (bodyBuf[i] == '\n' && bodyBuf[i - 1] == '\n') {
					cnt += 1;
					continue;
				}
				if (bodyBuf[i] == '\n' && bodyBuf[i - 1] == '\t') {
					cnt += 1;
					continue;
				}
				if (bodyBuf[i] == '\t' && bodyBuf[i - 1] == '\t') {
					cnt += 1;
					continue;
				}
				if (bodyBuf[i] == '\t' && bodyBuf[i - 1] == '\n') {
					cnt += 1;
					continue;
				}
				buf[i - cnt] = bodyBuf[i];
			}
			size -= cnt;
			blk = (char*) malloc(sizeof(char) * size);
			for (i = 0; i < size; i++) {
				if (i == size - 1) {
					blk[i] = '\0';
					break;
				}
				blk[i] = buf[i];
			}
			free(buf);
			free(bodyBuf);
			free(headBuf);
			return blk;
		}
	}
	return OK;
}
