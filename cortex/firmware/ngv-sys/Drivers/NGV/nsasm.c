#include "./Include/nsio.h"
#include "./Include/nsasm.h"

#include <malloc.h>
#include <string.h>

int _mm_push(pMM* p, MMBlock* blk) {
	if (p->stackTop == 0) {
		p->stackTop = malloc(sizeof(MMType));
		p->stackTop->prev = 0;
	} else {
		p->stackTop->next = malloc(sizeof(MMType));
		p->stackTop->next->prev = p->stackTop;
		p->stackTop = p->stackTop->next;
	}
	if (p->stackCnt >= p->stackSiz) return ERR;
	memcpy(&(p->stackTop->var), blk, sizeof(MMBlock));
	p->stackCnt += 1;
	p->stackTop->next = 0;
	return OK;
}

int _mm_pop(pMM* p, MMBlock* blk) {
	if (p->stackTop->prev == 0) {
		memcpy(blk, &(p->stackTop->var), sizeof(MMBlock));
		free(p->stackTop);
		p->stackTop = 0;
		p->stackCnt = 0;
		return OK;
	}
	if (p->stackTop == 0) return ERR;
	memcpy(blk, &(p->stackTop->var), sizeof(MMBlock));
	p->stackCnt -= 1;
	p->stackTop = p->stackTop->prev;
	free(p->stackTop->next);
	return OK;
}

MMType* _mm_search(pMM* p, char* name) {
	MMType* tp = p->heapStart;
	if (p->heapCnt == 0) return OK;
	do {
		if (tp->name == 0) break;
		if (strcmp(tp->name, name) == 0) return tp;
		else tp = tp->next;
	} while (tp != 0);
	return OK;
}

int _mm_join(pMM* p, char* name, MMBlock* blk) {
	if (p->heapStart == 0) {
		p->heapStart = malloc(sizeof(MMType));
		p->heapStart->prev = 0;
		p->heapEnd = p->heapStart;
	} else {
		p->heapEnd->next = malloc(sizeof(MMType));
		p->heapEnd->next->prev = p->heapEnd;
		p->heapEnd = p->heapEnd->next;
	}
	p->heapEnd->name = 0;
	if (p->heapCnt >= p->heapSiz) return ERR;
	if (_mm_search(p, name) != 0) return ERR;
	p->heapEnd->name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(p->heapEnd->name, name);
	memcpy(&(p->heapEnd->var), blk, sizeof(MMBlock));
	p->heapCnt += 1;
	p->heapEnd->next = 0;
	return OK;
}

MMBlock* _mm_get(pMM* p, char* name) {
	MMType* sp = 0;
	sp = _mm_search(p, name);
	if (sp == 0) return OK;
	return &(sp->var);
}

int _mm_exit(pMM* p, char* name) {
	MMType* sp = 0;
	sp = _mm_search(p, name);
	if (sp == 0) return ERR;
	if (sp == p->heapStart) {
		p->heapStart = sp->next;
		p->heapStart->prev = 0;
	} else if (sp == p->heapEnd) {
		p->heapEnd = sp->prev;
		p->heapEnd->next = 0;
	} else {
		sp->prev->next = sp->next;
		sp->next->prev = sp->prev;
	}
	p->heapCnt -= 1;
	free(sp->name);
	free(sp);
	return OK;
}

void _mm_clear(pMM* p) {
	MMType* ptr = p->stackTop;
	MMType* tmp = 0;

	while (ptr > 0) {
		tmp = ptr;
		ptr = ptr->prev;
		free(tmp);
	}

	ptr = p->heapStart;
	while (ptr > 0) {
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
}

MemoryManager* InitMemoryManager(int stackSize, int heapSize) {
	pMM* p = malloc(sizeof(pMM));
	p->stackSiz = stackSize;
	p->heapSiz = heapSize;
	p->stackCnt = 0;
	p->heapCnt = 0;
	p->stackTop = 0;
	p->heapStart = 0;
	p->heapEnd = 0;

	MemoryManager* c = malloc(sizeof(MemoryManager));
	c->p = p;
	c->push = &_mm_push;
	c->pop = &_mm_pop;
	c->join = &_mm_join;
	c->get = &_mm_get;
	c->exit = &_mm_exit;

	return c;
}

void DisposeMemoryManager(MemoryManager* ptr) {
	_mm_clear(ptr->p);
	free(ptr->p);
	free(ptr);
}

/* -------------------------------- */

#define NSASM_FUN_NO_OPER_CNT 4
static NSASM_Function NSASM_funList[] = {
	{ "rem", &_nsasm_rem_rem },
	{ "end", &_nsasm_fun_end },
	{ "nop", &_nsasm_fun_nop },
	{ "rst", &_nsasm_fun_rst },

	{ "var", &_nsasm_dat_var },
	{ "int", &_nsasm_dat_int },
	{ "char", &_nsasm_dat_char },
	{ "float", &_nsasm_dat_float },
	{ "str", &_nsasm_dat_str },

	{ "mov", &_nsasm_fun_mov },
	{ "push", &_nsasm_fun_push },
	{ "pop", &_nsasm_fun_pop },
	{ "in", &_nsasm_fun_in },
	{ "out", &_nsasm_fun_out },
	{ "add", &_nsasm_fun_add },
	{ "inc", &_nsasm_fun_inc },
	{ "sub", &_nsasm_fun_sub },
	{ "dec", &_nsasm_fun_dec },
	{ "mul", &_nsasm_fun_mul },
	{ "div", &_nsasm_fun_div },
	{ "cmp", &_nsasm_fun_cmp },
	{ "jmp", &_nsasm_fun_jmp },
	{ "jz", &_nsasm_fun_jz },
	{ "jnz", &_nsasm_fun_jnz },
	{ "jg", &_nsasm_fun_jg },
	{ "jl", &_nsasm_fun_jl },
	{ "and", &_nsasm_fun_and },
	{ "or", &_nsasm_fun_or },
	{ "xor", &_nsasm_fun_xor },
	{ "not", &_nsasm_fun_not },
	{ "shl", &_nsasm_fun_shl },
	{ "shr", &_nsasm_fun_shr },
	{ "run", &_nsasm_fun_run },
	{ "call", &_nsasm_fun_call },

	{ "prt", &_nsasm_fun_prt },

	{ "\0", 0 }
};

NSASM_Instance* NSASM_NewInstance(int stackSiz, int heapSiz) {
	NSASM_Instance* ptr = malloc(sizeof(NSASM_Instance));
	ptr->mm = InitMemoryManager(stackSiz, heapSiz);
	memset(ptr->reg, 0, sizeof(Register) * REG_CNT);
	memset(&ptr->state, 0, sizeof(Register));
	ptr->tag[0] = '\0';
	ptr->cnt = 0;
	return ptr;
}

void NSASM_FreeInstance(NSASM_Instance* ptr) {
	DisposeMemoryManager(ptr->mm);
	free(ptr);
}

int nsasm(int argc, char* argv[]) {
	print("NyaSama Assembly Script Module\n");
	print("Version: %1.2f\n\n", NSASM_VERSION);
	if (argc < 2) {
		print("Usage: nsasm [c/r] [FILE]\n\n");
		return OK;
	} else {
		if (argc == 3) {
			if (strchr(argv[1], 'r') > 0) {
				NSASM_run(read(argv[2]));
				return OK;
			}
		}
		if (strchr(argv[1], 'c') > 0) {
			NSASM_console();
			return OK;
		}
		NSASM_run(read(argv[1]));
		return OK;
	}
}

int _nsasm_rem_rem(NSASM_Instance* inst, Register* dst, Register* src) {
	return OK;
}

int _nsasm_dat_var(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	return inst->mm->join(inst->mm->p, dst->data.vPtr, src);
}
int _nsasm_dat_int(NSASM_Instance* inst, Register* dst, Register* src) {
	return OK;
}
int _nsasm_dat_char(NSASM_Instance* inst, Register* dst, Register* src) {
	return OK;
}
int _nsasm_dat_float(NSASM_Instance* inst, Register* dst, Register* src) {
	return OK;
}
int _nsasm_dat_str(NSASM_Instance* inst, Register* dst, Register* src) {
	return OK;
}

int _nsasm_fun_mov(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (dst->readOnly) return ERR;
	if (dst->type == RegChar && src->type == RegPtr) {
		dst->data.vChar = src->data.vPtr[0];
	} else if (dst->type == RegPtr && src->type == RegChar) {
		dst->data.vPtr[0] = src->data.vChar;
	} else {
		memcpy(dst, src, sizeof(Register));
		if (dst->readOnly) dst->readOnly = 0;
	}
	return OK;
}
int _nsasm_fun_push(NSASM_Instance* inst, Register* dst, Register* src) {
	return inst->mm->push(inst->mm->p, dst);
}
int _nsasm_fun_pop(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->readOnly) return ERR;
	return inst->mm->pop(inst->mm->p, dst);
}
int _nsasm_fun_in(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->readOnly) return ERR;
	char buf[IOBUF] = "";
	switch (dst->type) {
		case RegChar:
			fscan(buf, "%c", &(dst->data.vChar));
			break;
		case RegFloat:
			fscan(buf, "%f", &(dst->data.vFloat));
			break;
		case RegInt:
			fscan(buf, "%d", &(dst->data.vInt));
			break;
		case RegPtr:
			fscan(buf, "%s", dst->data.vPtr);
			break;
	}
	return OK;
}
int _nsasm_fun_out(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	switch (dst->data.vInt) {
		case 0x00:
			switch (src->type) {
				case RegChar:
					print("%c", src->data.vChar);
					break;
				case RegFloat:
					print("%f", src->data.vFloat);
					break;
				case RegInt:
					print("%d", src->data.vInt);
					break;
				case RegPtr:
					print("%s", src->data.vPtr);
					break;
			}
			break;
		case 0xFF:
			print("[DEBUG] ");
			switch (src->type) {
				case RegChar:
					print("%c", src->data.vChar);
					break;
				case RegFloat:
					print("%f", src->data.vFloat);
					break;
				case RegInt:
					print("%d", src->data.vInt);
					break;
				case RegPtr:
					print("%s", src->data.vPtr);
					break;
			}
			break;
		default:
			return ERR;
	}
	return OK;
}
int __calc_char__(char* dst, char src, char fun) {
	switch (fun) {
		case '+': *dst += src; break;
		case '-': *dst -= src; break;
		case '*': *dst *= src; break;
		case '/': *dst /= src; break;
		case '&': *dst &= src; break;
		case '|': *dst |= src; break;
		case '~': *dst = ~(*dst); break;
		case '^': *dst ^= src; break;
		case '<': *dst = *dst << src; break;
		case '>': *dst = *dst >> src; break;
		default: return ERR;
	}
	return OK;
}
int __calc_int__(int* dst, int src, char fun) {
	switch (fun) {
		case '+': *dst += src; break;
		case '-': *dst -= src; break;
		case '*': *dst *= src; break;
		case '/': *dst /= src; break;
		case '&': *dst &= src; break;
		case '|': *dst |= src; break;
		case '~': *dst = ~(*dst); break;
		case '^': *dst ^= src; break;
		case '<': *dst = *dst << src; break;
		case '>': *dst = *dst >> src; break;
		default: return ERR;
	}
	return OK;
}
int __calc_float__(float* dst, float src, char fun) {
	switch (fun) {
		case '+': *dst += src; break;
		case '-': *dst -= src; break;
		case '*': *dst *= src; break;
		case '/': *dst /= src; break;
		case '&': return ERR;
		case '|': return ERR;
		case '~': return ERR;
		case '^': return ERR;
		case '<': return ERR;
		case '>': return ERR;
		default: return ERR;
	}
	return OK;
}
int __calc_ptr__(char** dst, int src, char fun) {
	switch (fun) {
		case '+': *dst += src; break;
		case '-': *dst -= src; break;
		case '*': return ERR;
		case '/': return ERR;
		case '&': return ERR;
		case '|': return ERR;
		case '~': return ERR;
		case '^': return ERR;
		case '<': return ERR;
		case '>': return ERR;
		default: return ERR;
	}
	return OK;
}
int __calc__(Register* dst, Register* src, char fun) {
	switch (dst->type) {
		case RegChar:
			switch (src->type) {
				case RegChar:
					return __calc_char__(&(dst->data.vChar), src->data.vChar, fun);
				case RegFloat:
					return __calc_char__(&(dst->data.vChar), src->data.vFloat, fun);
				case RegInt:
					return __calc_char__(&(dst->data.vChar), src->data.vInt, fun);
				case RegPtr:
					return ERR;
			}
			break;
		case RegFloat:
			switch (src->type) {
				case RegChar:
					return __calc_float__(&(dst->data.vFloat), src->data.vChar, fun);
				case RegFloat:
					return __calc_float__(&(dst->data.vFloat), src->data.vFloat, fun);
				case RegInt:
					return __calc_float__(&(dst->data.vFloat), src->data.vInt, fun);
				case RegPtr:
					return ERR;
			}
			break;
		case RegInt:
			switch (src->type) {
				case RegChar:
					return __calc_int__(&(dst->data.vInt), src->data.vChar, fun);
				case RegFloat:
					return __calc_int__(&(dst->data.vInt), src->data.vFloat, fun);
				case RegInt:
					return __calc_int__(&(dst->data.vInt), src->data.vInt, fun);
				case RegPtr:
					return ERR;
			}
			break;
		case RegPtr:
			switch (src->type) {
				case RegChar:
					return __calc_ptr__(&(dst->data.vPtr), src->data.vChar, fun);
				case RegFloat:
					return ERR;
				case RegInt:
					return __calc_ptr__(&(dst->data.vPtr), src->data.vInt, fun);
				case RegPtr:
					return ERR;
			}
	}
	return OK;
}
int _nsasm_fun_add(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (dst->readOnly) return ERR;
	return __calc__(dst, src, '+');
}
int _nsasm_fun_inc(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->readOnly) return ERR;
	Register r;
	r.readOnly = 0;
	r.type = RegChar;
	r.data.vChar = 1;
	return __calc__(dst, &r, '+');
}
int _nsasm_fun_sub(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (dst->readOnly) return ERR;
	return __calc__(dst, src, '-');
}
int _nsasm_fun_dec(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->readOnly) return ERR;
	Register r;
	r.readOnly = 0;
	r.type = RegChar;
	r.data.vChar = 1;
	return __calc__(dst, &r, '-');
}
int _nsasm_fun_mul(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (dst->readOnly) return ERR;
	return __calc__(dst, src, '*');
}
int _nsasm_fun_div(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (dst->readOnly) return ERR;
	return __calc__(dst, src, '/');
}
int _nsasm_fun_cmp(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (_nsasm_fun_mov(inst, &(inst->state), dst) == ERR) return ERR;
	if (_nsasm_fun_sub(inst, &(inst->state), src) == ERR) return ERR;
	switch (inst->state.type) {
		case RegChar:
			inst->state.data.vInt = inst->state.data.vChar;
			break;
		case RegFloat:
			inst->state.data.vInt = (int) inst->state.data.vFloat;
			break;
		case RegInt:
			break;
		case RegPtr:
			return ERR;
	}
	inst->state.type = RegInt;
	return OK;
}
int _nsasm_fun_jmp(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->type != RegPtr) return ERR;
	if (NSASM_verifyTag(dst->data.vPtr) == ERR) return ERR;
	strcpy(inst->tag, dst->data.vPtr);
	return OK;
}
int _nsasm_fun_jz(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->type != RegPtr) return ERR;
	if (NSASM_verifyTag(dst->data.vPtr) == ERR) return ERR;
	if (inst->state.data.vInt == 0) strcpy(inst->tag, dst->data.vPtr);
	return OK;
}
int _nsasm_fun_jnz(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->type != RegPtr) return ERR;
	if (NSASM_verifyTag(dst->data.vPtr) == ERR) return ERR;
	if (inst->state.data.vInt != 0) strcpy(inst->tag, dst->data.vPtr);
	return OK;
}
int _nsasm_fun_jg(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->type != RegPtr) return ERR;
	if (NSASM_verifyTag(dst->data.vPtr) == ERR) return ERR;
	if (inst->state.data.vInt > 0) strcpy(inst->tag, dst->data.vPtr);
	return OK;
}
int _nsasm_fun_jl(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->type != RegPtr) return ERR;
	if (NSASM_verifyTag(dst->data.vPtr) == ERR) return ERR;
	if (inst->state.data.vInt < 0) strcpy(inst->tag, dst->data.vPtr);
	return OK;
}
int _nsasm_fun_and(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (dst->readOnly) return ERR;
	return __calc__(dst, src, '&');
}
int _nsasm_fun_or(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (dst->readOnly) return ERR;
	return __calc__(dst, src, '|');
}
int _nsasm_fun_xor(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (dst->readOnly) return ERR;
	return __calc__(dst, src, '^');
}
int _nsasm_fun_not(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->readOnly) return ERR;
	Register r;
	r.readOnly = 0;
	r.type = RegChar;
	r.data.vChar = 0;
	return __calc__(dst, &r, '~');
}
int _nsasm_fun_shl(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (dst->readOnly) return ERR;
	return __calc__(dst, src, '<');
}
int _nsasm_fun_shr(NSASM_Instance* inst, Register* dst, Register* src) {
	if (src == 0) return ERR;
	if (dst->readOnly) return ERR;
	return __calc__(dst, src, '>');
}
int _nsasm_fun_run(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->type != RegPtr) return ERR;
	print("[+] %s\n\n", dst->data.vPtr);
	NSASM_run(read(dst->data.vPtr));
	print("[-] %s\n\n", dst->data.vPtr);
	return OK;
}
int _nsasm_fun_call(NSASM_Instance* inst, Register* dst, Register* src) {
	if (dst->type != RegPtr) return ERR;
	NSASM_call(dst->data.vPtr, inst);
	return OK;
}
int _nsasm_fun_end(NSASM_Instance* inst, Register* dst, Register* src) {
	return ETC;
}
int _nsasm_fun_nop(NSASM_Instance* inst, Register* dst, Register* src) {
	#ifdef WINDOWS
		__asm {
			nop;
		}
	#else
		__asm__("nop");
	#endif
	return OK;
}
int _nsasm_fun_rst(NSASM_Instance* inst, Register* dst, Register* src) {
	DisposeMemoryManager(inst->mm);
	longjmp(rstPos, 0);
 	return OK;
}

int _nsasm_fun_prt(NSASM_Instance* inst, Register* dst, Register* src) {
	switch (dst->type) {
		case RegChar:
			print("%c", dst->data.vChar);
			break;
		case RegFloat:
			print("%f", dst->data.vFloat);
			break;
		case RegInt:
			print("%d", dst->data.vInt);
			break;
		case RegPtr:
			print("%s", dst->data.vPtr);
			break;
	}
	print("\n");
	return OK;
}

/* -------------------------------- */

int NSASM_getSymbolIndex(NSASM_Function list[], char* var) {
	for (int i = 0; list[i].name[0] != '\0'; i++) {
		if (strcmp(strlwr(var), list[i].name) == 0) {
			return i;
		}
	}
	return ETC;
}

int NSASM_verifyVarName(char* var) {
	if (
		(var[0] >= '0' && var[0] <= '9') ||
		var[0] == 'r' || var[0] == 'R' ||
		var[0] == '-' || var[0] == '+' ||
		var[0] == '.' || var[0] == '['
	) return ERR;
	return OK;
}

int NSASM_verifyTag(char* var) {
	if (var[0] == '[' && var[strlen(var) - 1] == ']') {
		return OK;
	}
	return ERR;
}

int NSASM_getRegister(NSASM_Instance* inst, char* var, Register** ptr) {
	if (var[0] == 'r' || var[0] == 'R') {
		int srn = -1;
		sscanf(var, "%*[rR]%d", &srn);
		if (srn >= 0 && srn < REG_CNT) {
			*ptr = &(inst->reg[srn]);
			return OK;
		} else return ERR;
	} else {
		if (var[0] == '\'') {
			if (var[strlen(var) - 1] != '\'') return ERR;
			char tmp = 0;
			if (sscanf(var, "%*[\'\\]%[^\']c", &tmp)) {
				*ptr = malloc(sizeof(Register));
				switch (tmp) {
					case 'n': (*ptr)->data.vChar = '\n'; break;
					case 'r': (*ptr)->data.vChar = '\r'; break;
					case 't': (*ptr)->data.vChar = '\t'; break;
					case '\\': (*ptr)->data.vChar = '\\'; break;
					default: (*ptr)->data.vChar = tmp; break;
				}
				(*ptr)->type = RegChar;
				(*ptr)->readOnly = 0;
				return ETC;
			} else if (sscanf(var, "%*[\']%[^\']c", &tmp)) {
				*ptr = malloc(sizeof(Register));
				(*ptr)->data.vChar = tmp;
				(*ptr)->type = RegChar;
				(*ptr)->readOnly = 0;
				return ETC;
			} else return ERR;
		} else if (var[0] == '\"') {
			int len = strlen(var), repeat = 0;
			if (var[len - 1] != '\"') {
				char buf[4];
				if (sscanf(var, "%*[^*]%*[* ]%[^\n]s", &buf) == 0) {
					return ERR;
				}
				if (sscanf(buf, "%d", &repeat) == 0) {
					return ERR;
				}
			}
			*ptr = malloc(sizeof(Register));
			if (repeat == 0) {
				(*ptr)->data.vPtr = malloc(sizeof(char) * (len - 1));
				memcpy((*ptr)->data.vPtr, var + 1, len - 2);
				(*ptr)->data.vPtr[len - 2] = '\0';
			} else {
				char* buf = malloc(sizeof(char) * len);
				if(sscanf(var, "%*[\"]%[^\"]s", buf)) {
					int bufLen = strlen(buf);
					(*ptr)->data.vPtr = malloc(sizeof(char) * (bufLen * (repeat + 1)));
					(*ptr)->data.vPtr[0] = '\0';
					for (int i = 0; i < repeat; i++) {
						strcat((*ptr)->data.vPtr, buf);
					}
				} else return ERR;
				free(buf);
			}
			(*ptr)->type = RegPtr;
			(*ptr)->readOnly = 1;
			return ETC;
		} else if (!(var[0] < '0' || var[0] > '9') || var[0] == '-' || var[0] == '+' ||
					var[strlen(var) - 1] == 'h' || var[strlen(var) - 1] == 'H') {
			if ((
					(var[strlen(var) - 1] == 'F' || var[strlen(var) - 1] == 'f') &&
					(var[1] != 'x' && var[1] != 'X')
				) || strchr(var, '.') > 0) {
				float tmp = 0;
				if (sscanf(var, "%f", &tmp)) {
					*ptr = malloc(sizeof(Register));
					(*ptr)->data.vFloat = tmp;
					(*ptr)->type = RegFloat;
					(*ptr)->readOnly = 0;
					return ETC;
				} else return ERR;
			} else {
				int tmp = 0;
				if (var[1] == 'x' || var[1] == 'X' ||
				var[strlen(var) - 1] == 'h' || var[strlen(var) - 1] == 'H') {
					sscanf(var, "%x", &tmp);
				} else {
					sscanf(var, "%d", &tmp);
				}
				*ptr = malloc(sizeof(Register));
				(*ptr)->data.vInt = tmp;
				(*ptr)->type = RegInt;
				(*ptr)->readOnly = 0;
				return ETC;
			}
		} else {
			char tmp[strlen(var)];
			sscanf(var, "%[^ \t]s", tmp);
			Register* r = inst->mm->get(inst->mm->p, tmp);
			if (r == 0) return ERR;
			*ptr = r;
			return OK;
		}
	}
}

int NSASM_execute(NSASM_Instance* inst, char* var, char type) {
	char head[32] = "\0", dst[32] = "\0", src[64] = "\0";
	if (type == 'd') {
		sscanf(var, "%s %[^ \t=] %*[= \t]%[^\n]", head, dst, src);
		int index = NSASM_getSymbolIndex(NSASM_funList, head);
		if (index == ETC) return ERR;
		if (NSASM_verifyVarName(dst)) return ERR;
		Register dr;
		dr.data.vPtr = malloc(sizeof(char) * (strlen(dst) + 1));
		dr.type = RegPtr;
		strcpy(dr.data.vPtr, dst);
		Register* sr;
		if (NSASM_getRegister(inst, src, &sr) == ETC) {
			if (NSASM_funList[index].fun(inst, &dr, sr)) {
				return ERR;
			}
			free(dr.data.vPtr);
			free(sr);
		} else return ERR;
	} else if (type == 'c') {
		sscanf(var, "%s %[^,] %*[, \t]%[^\n]", head, dst, src);
		int index = NSASM_getSymbolIndex(NSASM_funList, head);
		if (index == ETC) {
			return NSASM_verifyTag(head);
		}
		Register* dr = 0; Register* sr = 0; int dresult = 0, sresult = 0;
		dresult = NSASM_getRegister(inst, dst, &dr);
		if (dresult != OK) {
			if (dresult == ETC) {
				dr->readOnly = 1;
			} else if (index < NSASM_FUN_NO_OPER_CNT) {
				dr = 0;
			} else {
				if (NSASM_verifyTag(dst) == OK) {
					dr = malloc(sizeof(Register));
					dr->data.vPtr = malloc(sizeof(char) * (strlen(dst) + 1));
					dr->type = RegPtr;
					dr->readOnly = 1;
					strcpy(dr->data.vPtr, dst);
				} else return ERR;
			}
		}
		sresult = NSASM_getRegister(inst, src, &sr);
		//if (sresult) return ERR;
		int result = NSASM_funList[index].fun(inst, dr, sr);
		if (result == ERR) return ERR;
		if (result == ETC) return ETC;
		if (dr != 0) {
			if (dr->type == RegPtr) {
				if (NSASM_verifyTag(dr->data.vPtr) == OK) {
					free(dr->data.vPtr);
					free(dr);
				}
			}
			if (dresult == ETC) {
				if (dr->type == RegPtr) free(dr->data.vPtr);
				free(dr);
			}
		}
		if (sresult == ETC) {
			if (sr->type == RegPtr) free(sr->data.vPtr);
			free(sr);
		}
	}
	return OK;
}

void NSASM_console() {
	print("Now in console mode.\n\n");
	char buf[IOBUF]; int lines = 1, result = 0;
	NSASM_Instance* instance = NSASM_NewInstance(16, 32);

	while (1) {
		print("%d >>> ", lines);
		scan(buf);
		if (strlen(buf) == 0) {
			lines += 1;
			continue;
		}
		if (NSASM_execute(instance, buf, 'd') == ERR) {
			result = NSASM_execute(instance, buf, 'c');
			if (result == ERR) {
				print("\nNSASM running error!\n");
				print("At line %d: %s\n\n", lines, buf);
			} else if (result == ETC) {
				break;
			}
		}
		lines += 1;
	}
	NSASM_FreeInstance(instance);
}

void NSASM_run(char* var) {
	if (var == 0) return;
	char* conf = cut(var, ".conf");
	char* data = cut(var, ".data");
	char* code = cut(var, ".code");
	char* _gcvar = 0;
	free(var);

	int stackSiz = 0, heapSiz = 0;
	if (conf != 0) {
		int confLines = lines(conf);
		char type[8] = "", value[8] = "";
		print("CONF: %d line(s), init...\n", confLines);
		for (int i = 0; i < confLines; i++) {
			sscanf(_gcvar = line(conf, i), "%s %[^\n]", type, value);
			free(_gcvar);
			if (strcmp(strlwr(type), "stack") == 0) {
				if (sscanf(value, "%d", &stackSiz) == 0) {
					print("\nNSASM init error!\n");
					print("At [CONF] line %d: %s\n\n", i + 1, _gcvar = line(data, i));
					free(_gcvar);
					return;
				}
			} else if (strcmp(strlwr(type), "heap") == 0) {
				if (sscanf(value, "%d", &heapSiz) == 0) {
					print("\nNSASM init error!\n");
					print("At [CONF] line %d: %s\n\n", i + 1, _gcvar = line(data, i));
					free(_gcvar);
					return;
				}
			} else {
				print("\nNSASM init error!\n");
				print("At [CONF] line %d: %s\n\n", i + 1, _gcvar = line(data, i));
				free(_gcvar);
				return;
			}
		}
	} else {
		stackSiz = 32;
		heapSiz = 96;
	}

	NSASM_Instance* instance = NSASM_NewInstance(stackSiz, heapSiz);

	if (data != 0) {
		int dataLines = lines(data);
		print("DATA: %d line(s), loading...\n", dataLines);
		for (int i = 0; i < dataLines; i++)
			if (NSASM_execute(instance, _gcvar = line(data, i), 'd')) {
				free(_gcvar);
				print("\nNSASM running error!\n");
				print("At [DATA] line %d: %s\n\n", i + 1, _gcvar = line(data, i));
				free(_gcvar);
				return;
			} else free(_gcvar);
	}

	if (code != 0) {
		int prev = 0, result = 0, codeLines = lines(code);
		print("CODE: %d line(s), running...\n\n", codeLines);
		for (; instance->cnt < codeLines; instance->cnt++) {
			prev = instance->cnt;
			result = NSASM_execute(instance, _gcvar = line(code, instance->cnt), 'c');
			free(_gcvar);
			if (result == ETC) break;
			if (result) {
				print("\nNSASM running error!\n");
				print("At line %d: %s\n\n", prev + 1, _gcvar = line(code, prev));
				free(_gcvar);
				return;
			}
			for (int i = 0; i < codeLines; i++) {
				if (strcmp(_gcvar = line(code, i), instance->tag) == 0) {
					free(_gcvar);
					for (int j = i + 1; j < codeLines; j++) {
						if (strcmp(_gcvar = line(code, j), instance->tag) == 0) {
							free(_gcvar);
							print("\nNSASM running error!\n");
							print("At  [CODE] line %d: %s\n", i, _gcvar = line(code, i));
							free(_gcvar);
							print("And [CODE] line %d: %s\n\n", j, _gcvar = line(code, j));
							free(_gcvar);
							return;
						} else free(_gcvar);
					}
					instance->cnt = i;
					instance->tag[0] = '\0';
				} else free(_gcvar);
			}
			if (instance->cnt >= codeLines) {
				print("\nNSASM running error!\n");
				print("At [CODE] line %d: %s\n\n", prev + 1, _gcvar = line(code, prev));
				free(_gcvar);
				return;
			}
		}
	}

	NSASM_FreeInstance(instance);
	free(conf); free(data); free(code);
	print("\nNSASM running finished.\n\n");
}

void NSASM_call(char* var, NSASM_Instance* prev) {
	char* raw = 0;
	raw = read(var);
	if (raw == 0) return;
	char* conf = cut(raw, ".conf");
	char* data = cut(raw, ".data");
	char* code = cut(raw, ".code");
	char* _gcvar = 0;
	free(raw);

	int stackSiz = 0, heapSiz = 0;
	if (conf != 0) {
		int confLines = lines(conf);
		char type[8] = "", value[8] = "";
		for (int i = 0; i < confLines; i++) {
			sscanf(_gcvar = line(conf, i), "%s %[^\n]", type, value);
			free(_gcvar);
			if (strcmp(strlwr(type), "stack") == 0) {
				if (sscanf(value, "%d", &stackSiz) == 0) {
					print("\nNSASM init error in \"%s\"!\n", var);
					print("At [CONF] line %d: %s\n\n", i + 1, _gcvar = line(data, i));
					free(_gcvar);
					return;
				}
			} else if (strcmp(strlwr(type), "heap") == 0) {
				if (sscanf(value, "%d", &heapSiz) == 0) {
					print("\nNSASM init error in \"%s\"!\n", var);
					print("At [CONF] line %d: %s\n\n", i + 1, _gcvar = line(data, i));
					free(_gcvar);
					return;
				}
			} else {
				print("\nNSASM init error in \"%s\"!\n", var);
				print("At [CONF] line %d: %s\n\n", i + 1, _gcvar = line(data, i));
				free(_gcvar);
				return;
			}
		}
	} else {
		stackSiz = 32;
		heapSiz = 96;
	}

	NSASM_Instance* instance = NSASM_NewInstance(stackSiz, heapSiz);
	memcpy(instance->reg, prev->reg, sizeof(Register) * REG_CNT);
	memcpy(&instance->state, &prev->state, sizeof(Register));

	if (data != 0) {
		int dataLines = lines(data);
		for (int i = 0; i < dataLines; i++)
			if (NSASM_execute(instance, _gcvar = line(data, i), 'd')) {
				free(_gcvar);
				print("\nNSASM running error in \"%s\"!\n", var);
				print("At [DATA] line %d: %s\n\n", i + 1, _gcvar = line(data, i));
				free(_gcvar);
				return;
			} else free(_gcvar);
	}

	if (code != 0) {
		int prev = 0, result = 0, codeLines = lines(code);
		for (; instance->cnt < codeLines; instance->cnt++) {
			prev = instance->cnt;
			result = NSASM_execute(instance, _gcvar = line(code, instance->cnt), 'c');
			free(_gcvar);
			if (result == ETC) break;
			if (result) {
				print("\nNSASM running error in \"%s\"!\n", var);
				print("At line %d: %s\n\n", prev + 1, _gcvar = line(code, prev));
				free(_gcvar);
				return;
			}
			for (int i = 0; i < codeLines; i++) {
				if (strcmp(_gcvar = line(code, i), instance->tag) == 0) {
					free(_gcvar);
					for (int j = i + 1; j < codeLines; j++) {
						if (strcmp(_gcvar = line(code, j), instance->tag) == 0) {
							free(_gcvar);
							print("\nNSASM running error!\n");
							print("At  [CODE] line %d: %s\n", i, _gcvar = line(code, i));
							free(_gcvar);
							print("And [CODE] line %d: %s\n\n", j, _gcvar = line(code, j));
							free(_gcvar);
							return;
						} else free(_gcvar);
					}
					instance->cnt = i;
					instance->tag[0] = '\0';
				} else free(_gcvar);
			}
			if (instance->cnt >= codeLines) {
				print("\nNSASM running error in \"%s\"!\n", var);
				print("At [CODE] line %d: %s\n\n", prev + 1, _gcvar = line(code, prev));
				free(_gcvar);
				return;
			}
		}
	}

	NSASM_FreeInstance(instance);
	free(conf); free(data); free(code);
}

