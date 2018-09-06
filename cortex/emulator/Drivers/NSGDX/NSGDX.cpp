#include "NSGDX.h"

#include "../../util.h"

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

extern void delay(uint32_t ms);

namespace NSGDX {

    void NSGDX::gdxFunc() {
        funcList["wait"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            delay(dst->n.i);
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
    }

}
