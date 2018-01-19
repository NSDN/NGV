#ifndef __NSHEL_H_
#define __NSHEL_H_


#define NSHEL_VERSION 0.02

#define NSHEL_HED_LEN 32
#define NSHEL_ARG_LEN 64
#define NSHEL_ARG_MAX 8

int _nshel_fun_help(int argc, char* argv[]);
int _nshel_fun_exit(int argc, char* argv[]);
int _nshel_fun_print(int argc, char* argv[]);
int _nshel_fun_clear(int argc, char* argv[]);
int _nshel_fun_logo(int argc, char* argv[]);
int _nshel_fun_ver(int argc, char* argv[]);

int _nshel_fun_reset(int argc, char* argv[]);
int _nshel_fun_delay(int argc, char* argv[]);
int _nshel_fun_colorb(int argc, char* argv[]);
int _nshel_fun_colorf(int argc, char* argv[]);
int _nshel_fun_font(int argc, char* argv[]);
int _nshel_fun_style(int argc, char* argv[]);
int _nshel_fun_rotate(int argc, char* argv[]);
#ifdef USE_FLASH
int _nshel_fun_read(int argc, char* argv[]);
int _nshel_fun_reads(int argc, char* argv[]);
int _nshel_fun_write(int argc, char* argv[]);
int _nshel_fun_erase(int argc, char* argv[]);
#endif
#ifdef USE_GRAPH
int _nshel_fun_graph(int argc, char* argv[]);
#endif
int _nshel_fun_nshel(int argc, char* argv[]);
int _nshel_fun_nsasm(int argc, char* argv[]);

typedef struct {
	char name[NSHEL_HED_LEN];
	int (*fun)(int argc, char* argv[]);
} NSHEL_Function;

int NSHEL_getSymbolIndex(NSHEL_Function list[], char* var);
int NSHEL_getArgs(char* arg, char* argv[]);

int NSHEL_execute(char* var);
void NSHEL_console();
void NSHEL_run(char* var);

int nshel(int argc, char* argv[]);


#endif
