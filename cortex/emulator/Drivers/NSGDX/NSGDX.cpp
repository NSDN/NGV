#include "NSGDX.h"

#include "../NGV/Include/nsio.h"

#include "../NGV/Include/lcd.h"
extern LCD* lcd;

#define NSGDX_IS_EMU

#include <algorithm>

#include <map>
static std::map<std::string, uint16_t> keymap = {
    { "lu", 0x0001 }, { "ld", 0x0002 },
    { "ll", 0x0004 }, { "lr", 0x0008 },
    { "ru", 0x0010 }, { "rd", 0x0020 },
    { "rl", 0x0040 }, { "rr", 0x0080 },
    { "f1", 0x0100 }, { "f2", 0x0200 },
    { "f3", 0x0400 }, { "f4", 0x0800 },
    { "f5", 0x1000 }, { "f6", 0x2000 },
    { "f7", 0x4000 }, { "f8", 0x8000 },
};

#include <cstring>
#include <cstdlib>

#ifdef NSGDX_IS_EMU
#include "../../util.h"
extern void delay(uint32_t ms);
extern void processEvent();
#else
#include "../NGV/Include/key.h"
#include "ngv_bios.h"
#endif

#define __u(v) ((unsigned int) (v >= 0 ? v : -v))

#ifndef NSGDX_IS_EMU

const uint32_t MEM_MAXSIZE = 32 * 1024; // 32KB
static uint8_t memory[MEM_MAXSIZE] = { 0 }; // Virtual addr: 0x000000 - 0x007FFF, 32KB

const uint32_t MEMEX_MAXSIZE = 512 * 1024; // 512KB
static uint8_t* memoryex = (uint8_t*) 0x08080000; // Virtual addr: 0x008000 - 0x087FFF, 512KB, Flash memory at 512KB / 1MB, Read-only

#else

const uint32_t MEM_MAXSIZE = 32 * 1024; // 32KB
static uint8_t memory[MEM_MAXSIZE] = { 0 }; // Virtual addr: 0x000000 - 0x007FFF, 32KB

const uint32_t MEMEX_MAXSIZE = 8 * 1024 * 1024; // 8MB
static uint8_t memoryex[MEMEX_MAXSIZE] = { 0 }; // Virtual addr: 0x008000 - 0x807FFF, 8MB, Read-only

#endif

namespace NSGDX {

    void NSGDX::dispose() { }

    void NSGDX::gdxFunc() {
        funcList["pmq"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            processEvent();
            return Result::RES_OK;
        };
        funcList["wait"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            delay(dst->n.i);
            return Result::RES_OK;
        };

        funcList["mmov"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0 || src->n.i < 0) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].n.i <= 0) return Result::RES_ERR;

            if (__u(dst->n.i) + __u(regGroup[0].n.i) > MEM_MAXSIZE) return Result::RES_ERR;
            if (__u(src->n.i) >= MEM_MAXSIZE) {
				if (__u(src->n.i) + __u(regGroup[0].n.i) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				memmove(memory + __u(dst->n.i), memoryex + __u(src->n.i) - MEM_MAXSIZE, __u(regGroup[0].n.i));
            } else {
            	if (__u(src->n.i) + __u(regGroup[0].n.i) > MEM_MAXSIZE)
					return Result::RES_ERR;
            	memmove(memory + __u(dst->n.i), memory + __u(src->n.i), __u(regGroup[0].n.i));
            }

            return Result::RES_OK;
        };
        funcList["mcpy"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0 || src->n.i < 0) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].n.i <= 0) return Result::RES_ERR;

            if (__u(dst->n.i) + __u(regGroup[0].n.i) > MEM_MAXSIZE) return Result::RES_ERR;
			if (__u(src->n.i) >= MEM_MAXSIZE) {
				if (__u(src->n.i) + __u(regGroup[0].n.i) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				memcpy(memory + __u(dst->n.i), memoryex + __u(src->n.i) - MEM_MAXSIZE, __u(regGroup[0].n.i));
			} else {
				if (__u(src->n.i) + __u(regGroup[0].n.i) > MEM_MAXSIZE)
					return Result::RES_ERR;
				memcpy(memory + __u(dst->n.i), memory + __u(src->n.i), __u(regGroup[0].n.i));
			}

            return Result::RES_OK;
        };
        funcList["mset"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            if (src->type != RegType::REG_INT && src->type != RegType::REG_CHAR) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].n.i <= 0) return Result::RES_ERR;

            if (__u(dst->n.i) + __u(regGroup[0].n.i) > MEM_MAXSIZE) return Result::RES_ERR;
            memset(memory + __u(dst->n.i), src->type == RegType::REG_INT ? src->n.i : src->n.c, __u(regGroup[0].n.i));

            return Result::RES_OK;
        };
        funcList["mld"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            if (src->type != RegType::REG_STR) return Result::RES_ERR;
            if (regGroup[0].readOnly) return Result::RES_ERR;

            uint32_t len = 0; FILTYPE file;
			if (filopen(&file, (char*) src->s.substr(src->sp).c_str(), FIL_READ | FIL_BIN) != FIL_OK)
				return Result::RES_ERR;
			len = filsiz(&file);
			uint32_t ptr = dst->n.i;

		#ifndef NSGDX_IS_EMU
			if (__u(dst->n.i) >= MEM_MAXSIZE) return Result::RES_OK; // Do not load when using flash
			if (__u(dst->n.i) + len > MEM_MAXSIZE) return Result::RES_ERR;
			filread(&file, memory, len, &ptr);
		#else
			if (__u(dst->n.i) >= MEM_MAXSIZE) {
				if (__u(dst->n.i) + len > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				ptr -= MEM_MAXSIZE;
				filread(&file, memoryex, len, &ptr);
			} else {
				if (__u(dst->n.i) + len > MEM_MAXSIZE)
					return Result::RES_ERR;
				filread(&file, memory, len, &ptr);
			}
		#endif
			filclose(&file);
			regGroup[0].type = RegType::REG_INT; regGroup[0].n.i = len;

            return Result::RES_OK;
        };
        funcList["msv"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            if (src->type != RegType::REG_STR) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].n.i <= 0) return Result::RES_ERR;

            FILTYPE file;
			if (filopen(&file, (char*) src->s.substr(src->sp).c_str(), FIL_WRITE | FIL_BIN) != FIL_OK)
				return Result::RES_ERR;
            if (__u(dst->n.i) >= MEM_MAXSIZE) {
				if (__u(dst->n.i) + __u(regGroup[0].n.i) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				filwrite(&file, memoryex + __u(dst->n.i) - MEM_MAXSIZE, regGroup[0].n.i);
			} else {
				if (__u(dst->n.i) + __u(regGroup[0].n.i) > MEM_MAXSIZE)
					return Result::RES_ERR;
				filwrite(&file, memory + __u(dst->n.i), regGroup[0].n.i);
			}
            filclose(&file);

            return Result::RES_OK;
        };
        funcList["mgc"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            if (src->n.i < 0) return Result::RES_ERR;
            if (dst->readOnly) return Result::RES_ERR;

            if (__u(src->n.i) >= MEM_MAXSIZE) {
				if (__u(src->n.i) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				dst->type = RegType::REG_CHAR; dst->n.c = memoryex[__u(src->n.i) - MEM_MAXSIZE];
			} else {
				dst->type = RegType::REG_CHAR; dst->n.c = memory[__u(src->n.i)];
			}

            return Result::RES_OK;
        };
        funcList["msc"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            if (src->type != RegType::REG_CHAR) return Result::RES_ERR;

            if (__u(dst->n.i) >= MEM_MAXSIZE) {
				if (__u(dst->n.i) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				memoryex[__u(dst->n.i) - MEM_MAXSIZE] = src->n.c;
			} else {
				memory[__u(dst->n.i)] = src->n.c;
			}

            return Result::RES_OK;
        };
        funcList["mgb"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            if (src->n.i < 0) return Result::RES_ERR;
            if (dst->readOnly) return Result::RES_ERR;

            if (__u(src->n.i) >= MEM_MAXSIZE) {
				if (__u(src->n.i) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				dst->type = RegType::REG_INT; dst->n.i = memoryex[__u(src->n.i) - MEM_MAXSIZE];
			} else {
				dst->type = RegType::REG_INT; dst->n.i = memory[__u(src->n.i)];
			}

            return Result::RES_OK;
        };
        funcList["msb"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;

			if (__u(dst->n.i) >= MEM_MAXSIZE) {
				if (__u(dst->n.i) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				memoryex[__u(dst->n.i) - MEM_MAXSIZE] = src->n.i & 0xFF;
			} else {
				memory[__u(dst->n.i)] = src->n.i & 0xFF;
			}

            return Result::RES_OK;
        };

        funcList["cls"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            lcd->clear(lcd->p);
            return Result::RES_OK;
        };
        funcList["back"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            lcd->colorb(lcd->p, dst->n.i);
            return Result::RES_OK;
        };
        funcList["fore"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            lcd->colorf(lcd->p, dst->n.i);
            return Result::RES_OK;
        };
        funcList["font"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            lcd->font(lcd->p, (LCDFont) (dst->n.i > 1 ? 2 : 1));
            return Result::RES_OK;
        };
        funcList["scl"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            lcd->scale(lcd->p, dst->n.i);
            return Result::RES_OK;
        };
        funcList["pix"] = $OP_{
            if (dst == nullptr && src == nullptr) {
                if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
                if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
                lcd->pixel(lcd->p, regGroup[0].n.i, regGroup[1].n.i);
                return Result::RES_OK;
            }

            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            lcd->pixel(lcd->p, dst->n.i, src->n.i);
            return Result::RES_OK;
        };
        funcList["line"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[2].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[3].type != RegType::REG_INT) return Result::RES_ERR;
            lcd->line(lcd->p,
                regGroup[0].n.i, regGroup[1].n.i,
                regGroup[2].n.i, regGroup[3].n.i
            );
            return Result::RES_OK;
        };
        funcList["tri"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[2].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[3].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[4].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[5].type != RegType::REG_INT) return Result::RES_ERR;
            lcd->tri(lcd->p,
                regGroup[0].n.i, regGroup[1].n.i,
                regGroup[2].n.i, regGroup[3].n.i,
                regGroup[4].n.i, regGroup[5].n.i,
                dst->n.i
            );
            return Result::RES_OK;
        };
        funcList["rect"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[2].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[3].type != RegType::REG_INT) return Result::RES_ERR;
            lcd->rect(lcd->p,
                regGroup[0].n.i, regGroup[1].n.i,
                regGroup[2].n.i, regGroup[3].n.i,
                dst->n.i
            );
            return Result::RES_OK;
        };
        funcList["draw"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_CHAR) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            lcd->draw(lcd->p, regGroup[0].n.i, regGroup[1].n.i, dst->n.c);
            return Result::RES_OK;
        };
        funcList["show"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_STR) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            lcd->print(lcd->p, regGroup[0].n.i, regGroup[1].n.i, (char*) dst->s.substr(dst->sp).c_str());
            return Result::RES_OK;
        };
        funcList["bmp"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[2].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[3].type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            int siz = regGroup[2].n.i * regGroup[3].n.i;

            if (__u(dst->n.i) >= MEM_MAXSIZE) {
				if (__u(dst->n.i) + __u(siz / 8) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				lcd->bitmaps(lcd->p,
					regGroup[0].n.i, regGroup[1].n.i,
					regGroup[2].n.i, regGroup[3].n.i,
					memoryex + __u(dst->n.i) - MEM_MAXSIZE
				);
			} else {
				if (__u(dst->n.i) + __u(siz / 8) > MEM_MAXSIZE)
					return Result::RES_ERR;
				lcd->bitmaps(lcd->p,
					regGroup[0].n.i, regGroup[1].n.i,
					regGroup[2].n.i, regGroup[3].n.i,
					memory + __u(dst->n.i)
				);
			}

            return Result::RES_OK;
        };
        funcList["bmpc"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[2].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[3].type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            int siz = regGroup[2].n.i * regGroup[3].n.i;

            if (__u(dst->n.i) >= MEM_MAXSIZE) {
				if (__u(dst->n.i) + __u(siz / 8) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				lcd->bitmapsc(lcd->p,
					regGroup[0].n.i, regGroup[1].n.i,
					regGroup[2].n.i, regGroup[3].n.i,
					memoryex + __u(dst->n.i) - MEM_MAXSIZE
				);
			} else {
				if (__u(dst->n.i) + __u(siz / 8) > MEM_MAXSIZE)
					return Result::RES_ERR;
				lcd->bitmapsc(lcd->p,
					regGroup[0].n.i, regGroup[1].n.i,
					regGroup[2].n.i, regGroup[3].n.i,
					memory + __u(dst->n.i)
				);
			}

            return Result::RES_OK;
        };
        funcList["ico"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[2].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[3].type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            int siz = regGroup[2].n.i * regGroup[3].n.i;

            if (__u(dst->n.i) >= MEM_MAXSIZE) {
				if (__u(dst->n.i) + __u(siz / 8) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				lcd->icon(lcd->p,
					regGroup[0].n.i, regGroup[1].n.i,
					regGroup[2].n.i, regGroup[3].n.i,
					memoryex + __u(dst->n.i) - MEM_MAXSIZE
				);
			} else {
				if (__u(dst->n.i) + __u(siz / 8) > MEM_MAXSIZE)
					return Result::RES_ERR;
				lcd->icon(lcd->p,
					regGroup[0].n.i, regGroup[1].n.i,
					regGroup[2].n.i, regGroup[3].n.i,
					memory + __u(dst->n.i)
				);
			}

            return Result::RES_OK;
        };
        funcList["icoc"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[2].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[3].type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            int siz = regGroup[2].n.i * regGroup[3].n.i;

            if (__u(dst->n.i) >= MEM_MAXSIZE) {
				if (__u(dst->n.i) + __u(siz / 8) > MEM_MAXSIZE + MEMEX_MAXSIZE)
					return Result::RES_ERR;
				lcd->iconc(lcd->p,
					regGroup[0].n.i, regGroup[1].n.i,
					regGroup[2].n.i, regGroup[3].n.i,
					memoryex + __u(dst->n.i) - MEM_MAXSIZE
				);
			} else {
				if (__u(dst->n.i) + __u(siz / 8) > MEM_MAXSIZE)
					return Result::RES_ERR;
				lcd->iconc(lcd->p,
					regGroup[0].n.i, regGroup[1].n.i,
					regGroup[2].n.i, regGroup[3].n.i,
					memory + __u(dst->n.i)
				);
			}

            return Result::RES_OK;
        };
        funcList["img"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[2].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[3].type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            int siz = regGroup[2].n.i * regGroup[3].n.i;

            if (src != nullptr) {
            	if (src->type != RegType::REG_INT) return Result::RES_ERR;
				if (src->n.i < 0) return Result::RES_ERR;

            	if (__u(dst->n.i) >= MEM_MAXSIZE) {
					if (__u(dst->n.i) + __u(siz * 3) > MEM_MAXSIZE + MEMEX_MAXSIZE)
						return Result::RES_ERR;
					lcd->bitmapt(lcd->p,
						regGroup[0].n.i, regGroup[1].n.i,
						regGroup[2].n.i, regGroup[3].n.i,
						__u(src->n.i), memoryex + __u(dst->n.i) - MEM_MAXSIZE
					);
				} else {
					if (__u(dst->n.i) + __u(siz * 3) > MEM_MAXSIZE)
						return Result::RES_ERR;
					lcd->bitmapt(lcd->p,
						regGroup[0].n.i, regGroup[1].n.i,
						regGroup[2].n.i, regGroup[3].n.i,
						__u(src->n.i), memory + __u(dst->n.i)
					);
				}
			} else {
				if (__u(dst->n.i) >= MEM_MAXSIZE) {
					if (__u(dst->n.i) + __u(siz * 4) > MEM_MAXSIZE + MEMEX_MAXSIZE)
						return Result::RES_ERR;
					lcd->bitmap(lcd->p,
						regGroup[0].n.i, regGroup[1].n.i,
						regGroup[2].n.i, regGroup[3].n.i,
						(uint32_t*) (memoryex + __u(dst->n.i) - MEM_MAXSIZE)
					);
				} else {
					if (__u(dst->n.i) + __u(siz * 4) > MEM_MAXSIZE)
						return Result::RES_ERR;
					lcd->bitmap(lcd->p,
						regGroup[0].n.i, regGroup[1].n.i,
						regGroup[2].n.i, regGroup[3].n.i,
						(uint32_t*) (memory + __u(dst->n.i))
					);
				}
			}

            return Result::RES_OK;
        };
        funcList["imgc"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[2].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[3].type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            int siz = regGroup[2].n.i * regGroup[3].n.i;

            if (src != nullptr) {
				if (src->type != RegType::REG_INT) return Result::RES_ERR;
				if (src->n.i < 0) return Result::RES_ERR;

				if (__u(dst->n.i) >= MEM_MAXSIZE) {
					if (__u(dst->n.i) + __u(siz * 3) > MEM_MAXSIZE + MEMEX_MAXSIZE)
						return Result::RES_ERR;
					lcd->bitmaptc(lcd->p,
						regGroup[0].n.i, regGroup[1].n.i,
						regGroup[2].n.i, regGroup[3].n.i,
						__u(src->n.i), memoryex + __u(dst->n.i) - MEM_MAXSIZE
					);
				} else {
					if (__u(dst->n.i) + __u(siz * 3) > MEM_MAXSIZE)
						return Result::RES_ERR;
					lcd->bitmaptc(lcd->p,
						regGroup[0].n.i, regGroup[1].n.i,
						regGroup[2].n.i, regGroup[3].n.i,
						__u(src->n.i), memory + __u(dst->n.i)
					);
				}
			} else {
				if (__u(dst->n.i) >= MEM_MAXSIZE) {
					if (__u(dst->n.i) + __u(siz * 4) > MEM_MAXSIZE + MEMEX_MAXSIZE)
						return Result::RES_ERR;
					lcd->bitmapc(lcd->p,
						regGroup[0].n.i, regGroup[1].n.i,
						regGroup[2].n.i, regGroup[3].n.i,
						(uint32_t*) (memoryex + __u(dst->n.i) - MEM_MAXSIZE)
					);
				} else {
					if (__u(dst->n.i) + __u(siz * 4) > MEM_MAXSIZE)
						return Result::RES_ERR;
					lcd->bitmapc(lcd->p,
						regGroup[0].n.i, regGroup[1].n.i,
						regGroup[2].n.i, regGroup[3].n.i,
						(uint32_t*) (memory + __u(dst->n.i))
					);
				}
			}

            return Result::RES_OK;
        };

        funcList["key"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) {
                if (dst->type != RegType::REG_STR) return Result::RES_ERR;
                string kcode = dst->s.substr(dst->sp);
                transform(kcode.begin(), kcode.end(), kcode.begin(), ::tolower);
                waitKey(keymap[kcode]);
            } else {
                if (dst->readOnly) return Result::RES_ERR;
                if (src->type != RegType::REG_STR) return Result::RES_ERR;
                string kcode = src->s.substr(src->sp);
                transform(kcode.begin(), kcode.end(), kcode.begin(), ::tolower);
                bool res = checkKey(keymap[kcode]);
                dst->type = RegType::REG_INT;
                dst->n.i = res ? 1 : 0;
            }
            return Result::RES_OK;
        };
        funcList["kup"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) {
                if (dst->type != RegType::REG_STR) return Result::RES_ERR;
                string kcode = dst->s.substr(dst->sp);
                transform(kcode.begin(), kcode.end(), kcode.begin(), ::tolower);
                waitKeyUp(keymap[kcode]);
            } else {
                if (dst->readOnly) return Result::RES_ERR;
                if (src->type != RegType::REG_STR) return Result::RES_ERR;
                string kcode = src->s.substr(src->sp);
                transform(kcode.begin(), kcode.end(), kcode.begin(), ::tolower);
                bool res = checkKeyUp(keymap[kcode]);
                dst->type = RegType::REG_INT;
                dst->n.i = res ? 1 : 0;
            }
            return Result::RES_OK;
        };

        funcList["gbk"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->readOnly) return Result::RES_ERR;
            if (src->type != RegType::REG_STR) return Result::RES_ERR;
            if ((size_t) src->sp >= src->s.length()) return Result::RES_ERR;
            uint8_t end = ((size_t) src->sp == (src->s.length() - 1)) ? 1 : 0;
            uint8_t a = src->s[src->sp], b = (end != 0) ? 0 : src->s[src->sp + 1];
            int res = (a < 0xA1 || b < 0xA1) ? -1 : ((a - 0xA1) * 0x5E + (b - 0xA1));
            dst->type = RegType::REG_INT; dst->n.i = res;
            return Result::RES_OK;
        };
    }

}
