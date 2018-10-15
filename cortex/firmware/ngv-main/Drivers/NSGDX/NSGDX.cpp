#include "NSGDX.h"

#include "../../util.h"

#include "../NGV/Include/nsio.h"

#include "../NGV/Include/lcd.h"
extern LCD* lcd;

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
const uint32_t MEM_DEFAULT = 64 * 1024; // 64KB
const uint32_t MEM_MAXSIZE = 8 * 1024 * 1024; // 8MB
static uint32_t memsize = MEM_DEFAULT;
#ifdef USE_MEM_MALLOC
static uint8_t* memory = (uint8_t*) malloc(memsize);
#else
static uint8_t memory[MEM_MAXSIZE] = { 0 };
#endif

extern void delay(uint32_t ms);
extern void processEvent();

namespace NSGDX {

    void NSGDX::dispose() {
    #ifdef USE_MEM_MALLOC
        free(memory);
    #endif
    }

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

        funcList["msiz"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            memsize = dst->n.i;
            if (memsize < MEM_DEFAULT) memsize = MEM_DEFAULT;
            if (memsize > MEM_MAXSIZE) memsize = MEM_MAXSIZE;
        #ifdef USE_MEM_MALLOC
            free(memory); memory = NULL;
            memory = (uint8_t*) malloc(memsize);
        #endif
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
            if (dst->n.i + regGroup[0].n.i > memsize) return Result::RES_ERR;
            if (src->n.i + regGroup[0].n.i > memsize) return Result::RES_ERR;
            memmove(memory + dst->n.i, memory + src->n.i, regGroup[0].n.i);
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
            if (dst->n.i + regGroup[0].n.i > memsize) return Result::RES_ERR;
            if (src->n.i + regGroup[0].n.i > memsize) return Result::RES_ERR;
            memcpy(memory + dst->n.i, memory + src->n.i, regGroup[0].n.i);
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
            if (dst->n.i + regGroup[0].n.i > memsize) return Result::RES_ERR;
            memset(memory + dst->n.i, src->type == RegType::REG_INT ? src->n.i : src->n.c, regGroup[0].n.i);
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
            if (dst->n.i + len > memsize) return Result::RES_ERR;
            uint32_t ptr = dst->n.i;
            filread(&file, memory, len, &ptr);
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
            if (dst->n.i + regGroup[0].n.i > memsize) return Result::RES_ERR;
            FILTYPE file;
            if (filopen(&file, (char*) src->s.substr(src->sp).c_str(), FIL_WRITE | FIL_BIN) != FIL_OK)
                return Result::RES_ERR;
            filwrite(&file, memory + dst->n.i, regGroup[0].n.i);
            filclose(&file);
            return Result::RES_OK;
        };
        funcList["mgc"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            if (src->n.i < 0) return Result::RES_ERR;
            if (src->n.i >= memsize) return Result::RES_ERR;
            if (dst->readOnly) return Result::RES_ERR;
            dst->type = RegType::REG_CHAR; dst->n.c = memory[src->n.i];
            return Result::RES_OK;
        };
        funcList["msc"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            if (dst->n.i >= memsize) return Result::RES_ERR;
            if (src->type != RegType::REG_CHAR) return Result::RES_ERR;
            memory[dst->n.i] = src->n.c;
            return Result::RES_OK;
        };
        funcList["mgb"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            if (src->n.i < 0) return Result::RES_ERR;
            if (src->n.i >= memsize) return Result::RES_ERR;
            if (dst->readOnly) return Result::RES_ERR;
            dst->type = RegType::REG_INT; dst->n.i = memory[src->n.i];
            return Result::RES_OK;
        };
        funcList["msb"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (dst->n.i < 0) return Result::RES_ERR;
            if (dst->n.i >= memsize) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            memory[dst->n.i] = src->n.i & 0xFF;
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
            if (dst->n.i + siz / 8 >= memsize) return Result::RES_ERR;
            lcd->bitmaps(lcd->p,
                regGroup[0].n.i, regGroup[1].n.i,
                regGroup[2].n.i, regGroup[3].n.i,
                memory + dst->n.i
            );
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
            if (dst->n.i + siz / 8 >= memsize) return Result::RES_ERR;
            lcd->bitmapsc(lcd->p,
                regGroup[0].n.i, regGroup[1].n.i,
                regGroup[2].n.i, regGroup[3].n.i,
                memory + dst->n.i
            );
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
            if (dst->n.i + siz / 8 >= memsize) return Result::RES_ERR;
            lcd->icon(lcd->p,
                regGroup[0].n.i, regGroup[1].n.i,
                regGroup[2].n.i, regGroup[3].n.i,
                memory + dst->n.i
            );
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
            if (dst->n.i + siz / 8 >= memsize) return Result::RES_ERR;
            lcd->iconc(lcd->p,
                regGroup[0].n.i, regGroup[1].n.i,
                regGroup[2].n.i, regGroup[3].n.i,
                memory + dst->n.i
            );
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
            if (dst->n.i + siz * 3 >= memsize) return Result::RES_ERR;
            if (src != nullptr) {
                if (src->type != RegType::REG_INT) return Result::RES_ERR;
                if (src->n.i < 0) return Result::RES_ERR;
                lcd->bitmapt(lcd->p,
                    regGroup[0].n.i, regGroup[1].n.i,
                    regGroup[2].n.i, regGroup[3].n.i,
                    src->n.i, memory + dst->n.i
                );
            } else {
                lcd->bitmap(lcd->p,
                    regGroup[0].n.i, regGroup[1].n.i,
                    regGroup[2].n.i, regGroup[3].n.i,
                    (uint32_t*) (memory + dst->n.i)
                );
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
            if (dst->n.i + siz * 3 >= memsize) return Result::RES_ERR;
            if (src != nullptr) {
                if (src->type != RegType::REG_INT) return Result::RES_ERR;
                if (src->n.i < 0) return Result::RES_ERR;
                lcd->bitmaptc(lcd->p,
                    regGroup[0].n.i, regGroup[1].n.i,
                    regGroup[2].n.i, regGroup[3].n.i,
                    src->n.i, memory + dst->n.i
                );
            } else {
                lcd->bitmapc(lcd->p,
                    regGroup[0].n.i, regGroup[1].n.i,
                    regGroup[2].n.i, regGroup[3].n.i,
                    (uint32_t*) (memory + dst->n.i)
                );
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
            if (src->sp >= src->s.length()) return Result::RES_ERR;
            uint8_t end = (src->sp == (src->s.length() - 1)) ? 1 : 0;
            uint8_t a = src->s[src->sp], b = (end != 0) ? 0 : src->s[src->sp + 1];
            int res = (a < 0xA1 || b < 0xA1) ? -1 : ((a - 0xA1) * 0x5E + (b - 0xA1));
            dst->type = RegType::REG_INT; dst->n.i = res;
            return Result::RES_OK;
        };
    }

}
