#ifndef __NSASM_H_
#define __NSASM_H_


#define NSASM_VERSION 0.2

typedef enum {
	RegInt,
	RegFloat,
	RegChar,
	RegPtr
} RegType;

typedef struct {
	char readOnly;
	RegType type;
	union {
		char vChar;
		int vInt;
		float vFloat;
		char* vPtr;
	} data;
} Register;

typedef Register MMBlock;
typedef struct _mmtype {
	MMBlock var;
	char* name;
	struct _mmtype* prev;
	struct _mmtype* next;
} MMType;

typedef struct {
	int stackSiz;
	int heapSiz;
	int stackCnt;
	int heapCnt;
	MMType* stackTop;
	MMType* heapStart;
	MMType* heapEnd;
} pMM;

typedef struct {
	pMM* p;
	int (*push)(pMM* p, MMBlock* blk);
	int (*pop)(pMM* p, MMBlock* blk);
	int (*join)(pMM* p, char* name, MMBlock* blk);
	MMBlock* (*get)(pMM* p, char* name);
	int (*exit)(pMM* p, char* name);
} MemoryManager;

int _mm_push(pMM* p, MMBlock* blk);
int _mm_pop(pMM* p, MMBlock* blk);
MMType* _mm_search(pMM* p, char* name);
int _mm_join(pMM* p, char* name, MMBlock* blk);
MMBlock* _mm_get(pMM* p, char* name);
int _mm_exit(pMM* p, char* name);
void _mm_clear(pMM* p);
MemoryManager* InitMemoryManager(int stackSize, int heapSize);
void DisposeMemoryManager(MemoryManager* ptr);

/* -------------------------------- */

#define REG_CNT 8
typedef struct {
	MemoryManager* mm;
	Register reg[REG_CNT];
	Register state;
	char tag[32];
	int cnt;
} NSASM_Instance;

NSASM_Instance* NSASM_NewInstance(int stackSiz, int heapSiz);
void NSASM_FreeInstance(NSASM_Instance* ptr);

int _nsasm_rem_rem(NSASM_Instance* inst, Register* dst, Register* src);

int _nsasm_dat_var(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_dat_int(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_dat_char(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_dat_float(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_dat_str(NSASM_Instance* inst, Register* dst, Register* src);

int _nsasm_fun_mov(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_push(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_pop(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_in(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_out(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_add(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_inc(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_sub(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_dec(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_mul(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_div(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_cmp(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_jmp(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_jz(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_jnz(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_jg(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_jl(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_and(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_or(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_xor(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_not(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_shl(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_shr(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_run(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_call(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_end(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_nop(NSASM_Instance* inst, Register* dst, Register* src);
int _nsasm_fun_rst(NSASM_Instance* inst, Register* dst, Register* src);

int _nsasm_fun_prt(NSASM_Instance* inst, Register* dst, Register* src);

typedef struct {
	char name[8];
	int (*fun)(NSASM_Instance* inst, Register* dst, Register* src);
} NSASM_Function;

int NSASM_getSymbolIndex(NSASM_Function list[], char* var);
int NSASM_verifyVarName(char* var);
int NSASM_verifyTag(char* var);
int NSASM_getRegister(NSASM_Instance* inst, char* var, Register** ptr);

int NSASM_execute(NSASM_Instance* inst, char* var, char type);
void NSASM_console();
void NSASM_run(char* var);
void NSASM_call(char* var, NSASM_Instance* prev);

int nsasm(int argc, char* argv[]);


#endif
