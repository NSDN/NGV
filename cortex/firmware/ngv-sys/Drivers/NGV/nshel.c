#include "./Include/nsio.h"
#include "./Include/nshel.h"
#include "./Include/nsasm.h"

#include "./Include/lcd.h"
#include "./Include/logo.h"
#include "./Include/flash.h"

#include <malloc.h>
#include <string.h>

extern LCD* lcd;
#ifdef USE_FLASH
extern Flash* flash;
#endif
#ifdef USE_GRAPH
extern ADC_HandleTypeDef hadc3;
#endif

static NSHEL_Function NSHEL_funList[] = {
	{ "help", &_nshel_fun_help },
	{ "exit", &_nshel_fun_exit },
	{ "print", &_nshel_fun_print },
	{ "clear", &_nshel_fun_clear },
	{ "logo", &_nshel_fun_logo },
	{ "ver", &_nshel_fun_ver },

	{ "reset", &_nshel_fun_reset },
	{ "delay", &_nshel_fun_delay },
	{ "colorb", &_nshel_fun_colorb },
	{ "colorf", &_nshel_fun_colorf },
	{ "font", &_nshel_fun_font },
	{ "style", &_nshel_fun_style },
	{ "rotate", &_nshel_fun_rotate },
#ifdef USE_FLASH
	{ "read", &_nshel_fun_read },
	{ "reads", &_nshel_fun_reads },
	{ "write", &_nshel_fun_write },
	{ "erase", &_nshel_fun_erase },
#endif
#ifdef USE_GRAPH
	{ "graph", &_nshel_fun_graph },
#endif
	{ "nshel", &_nshel_fun_nshel },
	{ "nsasm", &_nshel_fun_nsasm },

	{ "\0", 0 }
};

int nshel(int argc, char* argv[]) {
	print("NyaSama Hardware Environment Language\n");
	print("Version: %1.2f\n\n", NSHEL_VERSION);
	if (argc < 2) {
		print("Usage: nshel [FILE]\n\n");
		NSHEL_console();
		return OK;
	} else {
		NSHEL_run(read(argv[1]));
		return OK;
	}
}

int _nshel_fun_help(int argc, char* argv[]) {
	print("You can use these commands:\n");
	for (int i = 0; NSHEL_funList[i].name[0] != '\0'; i++) {
		print("%s ", NSHEL_funList[i].name);
	}
	print("\n");
	return OK;
}
int _nshel_fun_exit(int argc, char* argv[]) {
	return ETC;
}
int _nshel_fun_print(int argc, char* argv[]) {
	if (argc > 1) {
		//print("%d args\n", argc);
		for (int i = 1; i < argc; i++)
			print("%s ", argv[i]);
	}
	print("\n");
	return OK;
}
int _nshel_fun_clear(int argc, char* argv[]) {
	lcd->clear(lcd->p);
	return OK;
}
int _nshel_fun_logo(int argc, char* argv[]) {
	lcd->colorb(lcd->p, 0xFFFFFF);
	lcd->colorf(lcd->p, 0x000000);
	lcd->clear(lcd->p);

	lcd->bitmapsc(lcd->p, 240, 140, 64, 64, getLogo());
	lcd->printfc(lcd->p, 180, "nyagame vita");
	lcd->printfc(lcd->p, 200, "this is a factory system");
	HAL_Delay(1000);
	uint8_t buf = 0;
	while (HAL_UART_Receive(&HUART, &buf, 1, 1) != HAL_OK);
	lcd->clear(lcd->p);
	return OK;
}
int _nshel_fun_ver(int argc, char* argv[]) {
	print("NSHEL %1.2f\n", NSHEL_VERSION);
	return OK;
}

int _nshel_fun_reset(int argc, char* argv[]) {
	longjmp(rstPos, 0);
	return OK;
}
int __get_color__(uint16_t color) {
	uint16_t tmp = color & 0x00FF;
	tmp = (tmp << 8) | (color >> 8);
	return  ((tmp & 0xF800) << 8) |
			((tmp & 0x07E0) << 5) |
			((tmp & 0x001F) << 3);
}
int __getvar__(char* var, int* num) {
	if (strchr(strlwr(var), 'x') > 0 || strchr(strlwr(var), 'h') > 0) {
		if (sscanf(var, "%x", num) <= 0) {
			return ERR;
		}
	} else {
		if (sscanf(var, "%d", num) <= 0) {
			return ERR;
		}
	}
	return OK;
}
int _nshel_fun_delay(int argc, char* argv[]) {
	if (argc < 2) {
		print("delay [ms]\n");
	} else {
		int time = 0;
		if (__getvar__(argv[1], &time) == ERR) return ERR;
		HAL_Delay(time);
	}
	return OK;
}
int _nshel_fun_colorb(int argc, char* argv[]) {
	if (argc == 1) {
		print("Back color: %06X\n", __get_color__(lcd->p->backColor));
	} else {
		int color = 0;
		if (__getvar__(argv[1], &color) == ERR) return ERR;
		lcd->colorb(lcd->p, color);
	}
	return OK;
}
int _nshel_fun_colorf(int argc, char* argv[]) {
	if (argc == 1) {
		print("Fore color: %06X\n", __get_color__(lcd->p->foreColor));
	} else {
		int color = 0;
		if (__getvar__(argv[1], &color) == ERR) return ERR;
		lcd->colorf(lcd->p, color);
	}
	return OK;
}
int _nshel_fun_font(int argc, char* argv[]) {
	if (argc == 1) {
		print("Font size: %s\n", lcd->p->Font == Big ? "Big" : "Small");
	} else {
		if (strcmp(strlwr(argv[1]), "big") == 0) {
			lcd->font(lcd->p, Big);
			lcd->clear(lcd->p);
		} else if (strcmp(strlwr(argv[1]), "small") == 0) {
			lcd->font(lcd->p, Small);
			lcd->clear(lcd->p);
		} else {
			return ERR;
		}
	}
	return OK;
}
int _nshel_fun_style(int argc, char* argv[]) {
	if (argc == 1) {
		print("You can choose: black, white, orange, green, amber, bonus\n");
	} else if (argc == 2) {
		if (strcmp(strlwr(argv[1]), "black") == 0) {
			lcd->colorf(lcd->p, 0xFFFFFF);
			lcd->colorb(lcd->p, 0x000000);
			lcd->clear(lcd->p);
		} else if (strcmp(strlwr(argv[1]), "white") == 0) {
			lcd->colorf(lcd->p, 0x000000);
			lcd->colorb(lcd->p, 0xFFFFFF);
			lcd->clear(lcd->p);
		} else if (strcmp(strlwr(argv[1]), "orange") == 0) {
			lcd->colorf(lcd->p, 0xFFFFFF);
			lcd->colorb(lcd->p, 0xE65100);
			lcd->clear(lcd->p);
		} else if (strcmp(strlwr(argv[1]), "green") == 0) {
			lcd->colorf(lcd->p, 0x259B24);
			lcd->colorb(lcd->p, 0x000000);
			lcd->clear(lcd->p);
		} else if (strcmp(strlwr(argv[1]), "amber") == 0) {
			lcd->colorf(lcd->p, 0xFCB326);
			lcd->colorb(lcd->p, 0x000000);
			lcd->clear(lcd->p);
		} else if (strcmp(strlwr(argv[1]), "bonus") == 0) {
			lcd->colorf(lcd->p, 0x455A64);
			lcd->colorb(lcd->p, 0x78909C);
			lcd->clear(lcd->p);
		} else {
			print("You can choose: black, white, orange, green, amber, bonus\n");
		}
	} else return ERR;
	return OK;
}
int _nshel_fun_rotate(int argc, char* argv[]) {
	if (argc == 1) {
		print("You can choose:\n");
		print("    p (as portrait)\n");
		print("    l (as landscape)\n");
		print("    pa (as portrait-anti)\n");
		print("    la (as landscape-anti)\n");
	} else if (argc == 2) {
		if (strcmp(strlwr(argv[1]), "p") == 0) {
			lcd->rotate(lcd->p, LCD_PORTRAIT);
			lcd->clear(lcd->p);
		} else if (strcmp(strlwr(argv[1]), "l") == 0) {
			lcd->rotate(lcd->p, LCD_LANDSCAPE);
			lcd->clear(lcd->p);
		} else if (strcmp(strlwr(argv[1]), "pa") == 0) {
			lcd->rotate(lcd->p, LCD_PORTRAIT_ANTI);
			lcd->clear(lcd->p);
		} else if (strcmp(strlwr(argv[1]), "la") == 0) {
			lcd->rotate(lcd->p, LCD_LANDSCAPE_ANTI);
			lcd->clear(lcd->p);
		} else {
			print("You can choose:\n");
			print("    p (as portrait)\n");
			print("    l (as landscape)\n");
			print("    pa (as portrait-anti)\n");
			print("    la (as landscape-anti)\n");
		}
	} else return ERR;
	return OK;
}
#ifdef USE_FLASH
int _nshel_fun_read(int argc, char* argv[]) {
	if (argc == 2) {
		int addr = 0;
		if (__getvar__(argv[1], &addr) == ERR) return ERR;
		uint8_t buf = 0;
		flash->read(flash->p, addr, &buf, 1);
		print("Read: %02X\n", buf);
	} else if (argc == 3) {
		int addr = 0;
		if (__getvar__(argv[1], &addr) == ERR) return ERR;
		int size = 1;
		if (__getvar__(argv[2], &size) == ERR) return ERR;
		uint8_t buf[size]; memset(buf, 0, sizeof(uint8_t) * size);
		flash->read(flash->p, addr, buf, size);
		print("Read: ");
		for (uint16_t i = 0; i < size; i++)
			print("%02X ", buf[i]);
		print("\n");
	} else return ERR;
	return OK;
}
int _nshel_fun_reads(int argc, char* argv[]) {
	if (argc == 2) {
		int addr = 0;
		if (__getvar__(argv[1], &addr) == ERR) return ERR;
		uint8_t buf = 0;
		flash->read(flash->p, addr, &buf, 1);
		print("Read: %02X\n", buf);
	} else if (argc == 3) {
		int addr = 0;
		if (__getvar__(argv[1], &addr) == ERR) return ERR;
		int size = 1;
		if (__getvar__(argv[2], &size) == ERR) return ERR;
		uint8_t buf[size]; memset(buf, 0, sizeof(uint8_t) * size);
		flash->read(flash->p, addr, buf, size);
		print("Read: ");
		for (uint16_t i = 0; i < size; i++)
			print("%c", buf[i]);
		print("\n");
	} else return ERR;
	return OK;
}
int _nshel_fun_write(int argc, char* argv[]) {
	if (argc == 3) {
		int addr = 0;
		if (__getvar__(argv[1], &addr) == ERR) return ERR;
		char buf[256];
		memset(buf, 0, 256);
		strcpy(buf, argv[2]);
		flash->writePage(flash->p, addr, (uint8_t*)buf);
		print("Write finished.\n");
	} else return ERR;
	return OK;
}
int _nshel_fun_erase(int argc, char* argv[]) {
	if (argc == 2) {
		int addr = 0;
		if (__getvar__(argv[1], &addr) == ERR) return ERR;
		flash->eraseSector(flash->p, addr);
		print("Erase finished.\n");
	} else return ERR;
	return OK;
}
#endif
#ifdef USE_GRAPH
int _nshel_fun_graph(int argc, char* argv[]) {
	lcd->clear(lcd->p);
	HAL_Delay(1000);
	uint8_t buf = 0; uint16_t data = 0, prev = 0;
	uint16_t time = 1;
	while (HAL_UART_Receive(&HUART, &buf, 1, 1) != HAL_OK) {
		prev = data;
		HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3, 5);
		data = HAL_ADC_GetValue(&hadc3);
		HAL_ADC_Stop(&hadc3);
		data = data * lcd->p->height / 4095;
		lcd->line(lcd->p, time - 1, prev, time, data);
		time += 1;
		if (time > lcd->p->width - 1) {
			time = 1;
			lcd->clear(lcd->p);
		}
		HAL_Delay(5);
	}
	lcd->clear(lcd->p);
	return OK;
}
#endif
int _nshel_fun_nshel(int argc, char* argv[]) {
	return nshel(argc, argv);
}
int _nshel_fun_nsasm(int argc, char* argv[]) {
	return nsasm(argc, argv);
}

/* -------------------------------- */

int NSHEL_getSymbolIndex(NSHEL_Function list[], char* var) {
	for (int i = 0; list[i].name[0] != '\0'; i++) {
		if (strcmp(strlwr(var), list[i].name) == 0) {
			return i;
		}
	}
	return ETC;
}

int NSHEL_getArgs(char* arg, char* argv[]) {
	char tmp[64]; char buf[64];
	int argc; int result = 0;
	strcpy(tmp, arg);
	for (argc = 1; argc < NSHEL_ARG_MAX; argc++) {
		result = sscanf(tmp, "%s %[^\n]", buf, tmp);
		if (result <= 0) break;
		else if (result == 1) {
			argv[argc] = malloc(sizeof(char) * (strlen(buf) + 1));
			strcpy(argv[argc], buf);
			argc += 1;
			if (argc < NSHEL_ARG_MAX) {
				result = sscanf(tmp, "%s %[^\n]", buf, tmp);
				if (result <= 0) break;
				argv[argc] = malloc(sizeof(char) * (strlen(buf) + 1));
				strcpy(argv[argc], buf);
			}
			break;
		} else {
			argv[argc] = malloc(sizeof(char) * (strlen(buf) + 1));
			strcpy(argv[argc], buf);
		}
	}
	return argc;
}

int NSHEL_execute(char* var) {
	char head[NSHEL_HED_LEN] = "\0", arg[NSHEL_ARG_LEN] = "\0";
	sscanf(var, "%s %[^\n]", head, arg);
	int index = NSHEL_getSymbolIndex(NSHEL_funList, head);
	if (index == ETC) return ERR;
	int argc = 0; char* argv[NSHEL_ARG_MAX];
	argv[0] = malloc(sizeof(char) * (strlen(head) + 1));
	strcpy(argv[0], head);
	argc =  NSHEL_getArgs(arg, argv);
	int result = NSHEL_funList[index].fun(argc, argv);
	for (int i = 0; i < argc; i++) free(argv[i]);
	return result;
}

void NSHEL_console() {
	char buf[IOBUF];
	int result = 0;
	while (1) {
		print("NGV:/> ");
		scan(buf);
		if (strlen(buf) == 0) continue;
		result = NSHEL_execute(buf);
		if (result == ERR) {
			print("\nNSHEL running error!\n");
			print("At line: %s\n\n", buf);
		}
		if (result == ETC) break;
	}
}

void NSHEL_run(char* var) {
	if (var == 0) return;
	char* _gcvar = 0;

	int varLines = lines(var);
	print("NSHEL: %d line(s), running...\n", varLines);
	for (int i = 0; i < varLines; i++)
		if (NSHEL_execute(_gcvar = line(var, i))) {
			free(_gcvar);
			print("\nNSHEL running error!\n");
			print("At line %d: %s\n\n", i, _gcvar = line(var, i));
			free(_gcvar);
			return;
		} else free(_gcvar);
	free(_gcvar);

	print("\nNSHEL running finished.\n\n");
}

