#define NSHEL_VERSION 0.01

#define NSHEL_HED_LEN 32
#define NSHEL_ARG_LEN 64
#define NSHEL_ARG_MAX 8

/* -------------------------------- */

int _nshel_fun_exit(int argc, char* argv[]);
int _nshel_fun_print(int argc, char* argv[]);
int _nshel_fun_clear(int argc, char* argv[]);
int _nshel_fun_logo(int argc, char* argv[]);
int _nshel_fun_ver(int argc, char* argv[]);

int _nshel_fun_reset(int argc, char* argv[]);
int _nshel_fun_colorb(int argc, char* argv[]);
int _nshel_fun_colorf(int argc, char* argv[]);
int _nshel_fun_font(int argc, char* argv[]);
int _nshel_fun_style(int argc, char* argv[]);

int _nshel_fun_read(int argc, char* argv[]);
int _nshel_fun_reads(int argc, char* argv[]);
int _nshel_fun_write(int argc, char* argv[]);
int _nshel_fun_erase(int argc, char* argv[]);

int _nshel_fun_nshel(int argc, char* argv[]);
int _nshel_fun_nsasm(int argc, char* argv[]);

typedef struct {
	char name[NSHEL_HED_LEN];
	int (*fun)(int argc, char* argv[]);
} NSHEL_Function;

static NSHEL_Function NSHEL_funList[] = {
	{ "exit", &_nshel_fun_exit },
	{ "print", &_nshel_fun_print },
	{ "clear", &_nshel_fun_clear },
	{ "logo", &_nshel_fun_logo },
	{ "ver", &_nshel_fun_ver },

	{ "reset", &_nshel_fun_reset },
	{ "colorb", &_nshel_fun_colorb },
	{ "colorf", &_nshel_fun_colorf },
	{ "font", &_nshel_fun_font },
	{ "style", &_nshel_fun_style },

	{ "read", &_nshel_fun_read },
	{ "reads", &_nshel_fun_reads },
	{ "write", &_nshel_fun_write },
	{ "erase", &_nshel_fun_erase },

	{ "nshel", &_nshel_fun_nshel },
	{ "nsasm", &_nshel_fun_nsasm },

	{ "\0", 0 }
};

int NSHEL_getSymbolIndex(NSHEL_Function list[], char* var);
int NSHEL_getArgs(char* arg, char* argv[]);

/* -------------------------------- */

int NSHEL_execute(char* var);
void NSHEL_console();
void NSHEL_run(char* var);

/* -------------------------------- */

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

/* -------------------------------- */

int _nshel_fun_exit(int argc, char* argv[]) {
	return ETC;
}
int _nshel_fun_print(int argc, char* argv[]) {
	if (argc > 1) {
		print("%d args\n", argc);
		for (int i = 0; i < argc; i++)
			print("%s ", argv[i]);
	}
	print("\n");
	return OK;
}
int _nshel_fun_clear(int argc, char* argv[]) {
	clearScreen();
	return OK;
}
int _nshel_fun_logo(int argc, char* argv[]) {
	lcd->colorb(lcd->p, 0xFFFFFF);
	lcd->colorf(lcd->p, 0x000000);
	lcd->clear(lcd->p);

	lcd->bitmapsc(lcd->p, 120, 140, 64, 64, __NYAGAME_LOGO_);
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

int _nshel_fun_read(int argc, char* argv[]) {
	if (argc == 2) {
		int addr = 0;
		if (__getvar__(argv[1], &addr) == ERR) return ERR;
		uint8_t buf = 0;
		//flash->read(flash->p, addr, &buf, 1);
		print("Read: %02X\n", buf);
	} else if (argc == 3) {
		int addr = 0;
		if (__getvar__(argv[1], &addr) == ERR) return ERR;
		int size = 1;
		if (__getvar__(argv[2], &size) == ERR) return ERR;
		uint8_t buf[size]; memset(buf, 0, sizeof(uint8_t) * size);
		//flash->read(flash->p, addr, buf, size);
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
		//flash->read(flash->p, addr, &buf, 1);
		print("Read: %02X\n", buf);
	} else if (argc == 3) {
		int addr = 0;
		if (__getvar__(argv[1], &addr) == ERR) return ERR;
		int size = 1;
		if (__getvar__(argv[2], &size) == ERR) return ERR;
		uint8_t buf[size]; memset(buf, 0, sizeof(uint8_t) * size);
		//flash->read(flash->p, addr, buf, size);
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
		//flash->writePage(flash->p, addr, (uint8_t*)buf);
		print("Write finished.\n");
	} else return ERR;
	return OK;
}
int _nshel_fun_erase(int argc, char* argv[]) {
	if (argc == 2) {
		int addr = 0;
		if (__getvar__(argv[1], &addr) == ERR) return ERR;
		//flash->eraseSector(flash->p, addr);
		print("Erase finished.\n");
	} else return ERR;
	return OK;
}

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
	return NSHEL_funList[index].fun(argc, argv);
}

void NSHEL_console() {
	char buf[IOBUF];
	int result = 0;
	while (1) {
		print("NGV:/> ");
		scan(buf);
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

	int varLines = lines(var);
	print("NSHEL: %d line(s), running...\n", varLines);
	for (int i = 0; i < varLines; i++)
		if (NSHEL_execute(line(var, i))) {
			print("\nNSHEL running error!\n");
			print("At line %d: %s\n\n", i, line(var, i));
			return;
		}

	print("\nNSHEL running finished.\n\n");
}
