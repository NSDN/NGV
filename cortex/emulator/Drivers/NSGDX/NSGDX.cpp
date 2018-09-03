#include "NSGDX.h"

#include "../NGV/Include/lcd.h"
extern LCD* lcd;

extern void delay(uint32_t ms);

namespace NSGDX {

    NSGDX::NSGDX(int heap, int stack, int reg, map<string, string> code) : NSASM(heap, stack, reg, code) {
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
            lcd->line(lcd->p, regGroup[0].n.i, regGroup[1].n.i, regGroup[2].n.i, regGroup[3].n.i);
            return Result::RES_OK;
        };
        funcList["tri"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            // TODO
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
            lcd->rect(lcd->p, regGroup[0].n.i, regGroup[1].n.i, regGroup[2].n.i, regGroup[3].n.i, dst->n.i);
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
    }

    NSGDX::~NSGDX() {

    }

}
