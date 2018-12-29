#include "NSGDX.h"

#include "../NSASM/Util.h"

#ifndef NSGDX_IS_EMU
#include <string>
using namespace std;
#include "../NGV/Include/lcd.h"
extern LCD* lcd;
#include "../NGV/Include/key.h"
#include "ngv_bios.h"
extern uint8_t* __getmem(uint32_t addr);

int convGBK(string str, size_t ptr) {
    uint8_t end = (ptr == (str.length() - 1)) ? 1 : 0;
    uint8_t a = str[ptr], b = (end != 0) ? 0 : str[ptr + 1];
    int res = (a < 0xA1 || b < 0xA1) ? -1 : ((a - 0xA1) * 0x5E + (b - 0xA1));
    return res;
}

void drawGBK(int font, int x, int y, string str) {
    int siz = str.length(), cnt = 0, addr = 0;
    int _x = x, _y = y; char chr = '\0';
HEAD:
    addr = convGBK(str, cnt);
    if (addr < 0) goto ASCII;
    goto GBK;
ASCII:
    chr = str[cnt];
    if (chr == '\n') goto NLIN;
    lcd->font(lcd->p, (LCDFont) 2);
    lcd->scale(lcd->p, 1);
    lcd->draw(lcd->p, _x, _y, chr);
    lcd->font(lcd->p, (LCDFont) 1);
    _x += 8; cnt += 1;
    if (x > 838) goto ORIX;
    goto NEXT;
GBK:
    addr *= 32;
    addr += font;
    lcd->icon(lcd->p, _x, _y, 16, 16, __getmem(addr));
    _x += 16; cnt += 2;
    if (x > 830) goto ORIX;
    goto NEXT;
NLIN:
    cnt += 1;
ORIX:
    _x = x; _y += 16;
NEXT:
    if (cnt != siz) goto HEAD;
}
#endif

#define __GREG_NRO(n) (regGroup[n].readOnly = false)

#ifdef NSGDX_IS_EMU
extern void* loadBGM(std::string path);
extern void unloadBGM(void* bgm);
extern void stopBGM();
extern playBGM(void* bgm);
#endif

namespace NSGDX {

    void NSGDX::loadNSGAL() {
        funcList["gal.init"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            nsgal.type = RegType::REG_MAP;
            nsgal.m.clear();

            _back.type = RegType::REG_STR;
            _back.s = "__back_renderer";
            _icon.type = RegType::REG_STR;
            _icon.s = "__icon_renderer";
            _char.type = RegType::REG_STR;
            _char.s = "__char_renderer";
            _font.type = RegType::REG_STR;
            _font.s = "__gbk_font_addr";

            _defback.type = RegType::REG_STR;
            _defback.s = "__default_back";
            _deffore.type = RegType::REG_STR;
            _deffore.s = "__default_fore";

            return Result::RES_OK;
        };
        funcList["gal.run"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_STR) return Result::RES_ERR;
            if (nsgal.m.count(*dst) == 0) return Result::RES_ERR;
            
            Register nowScene = *dst;
            Register reg, tmp; int val;
            reg.type = RegType::REG_STR;
            tmp.type = RegType::REG_INT;
        #ifdef NSGDX_IS_EMU
            void* bgm = 0;

            while (1) {
                funcList["pmq"](nullptr, nullptr);
                reg.s = "f1"; funcList["key"](&tmp, &reg);
                val = tmp.n.i;
                reg.s = "f8"; funcList["key"](&tmp, &reg);
                val += tmp.n.i;
                if (val > 1) break;

                nowScene = nsgal.m[nowScene];
                if (nowScene.m.size() == 0) break;

                //Draw back
                if (nsgal.m.count(_back) != 0) {
                    reg.s = "backAddr";
                    regGroup[0] = nowScene.m[reg];
                    reg.s = "backWidth";
                    regGroup[1] = nowScene.m[reg];
                    reg.s = "backHeight";
                    regGroup[2] = nowScene.m[reg];
                    __GREG_NRO(0); __GREG_NRO(1); __GREG_NRO(2);
                    funcList["eval"](&nsgal.m[_back], nullptr);
                }

                //Play BGM
                reg.s = "bgmPath";
                if (nowScene.m.count(reg) != 0) {
                    Register r = nowScene.m[reg];
                    string path = r.s.substr(r.sp);
                    stopBGM();
                    if (bgm != 0) {
                        unloadBGM(bgm);
                        bgm = 0;
                    }
                    bgm = loadBGM(path);
                    playBGM(bgm);
                } else {
                    stopBGM();
                    if (bgm != 0) {
                        unloadBGM(bgm);
                        bgm = 0;
                    }
                }

                //Draw icon
                reg.s = "icons";
                if (
                    nowScene.m.count(reg) != 0 &&
                    nsgal.m.count(_icon) != 0
                ) {
                    if (nowScene.m[reg].m.size() > 0) {
                        Register ico = nowScene.m[reg];
                        for (
                            auto it = ico.m.begin();
                            it != ico.m.end(); it++
                        ) {
                            if (it->second.m.empty()) continue;
                            reg.s = "iconAddr";
                            regGroup[0] = it->second.m[reg];
                            reg.s = "iconX";
                            regGroup[1] = it->second.m[reg];
                            reg.s = "iconY";
                            regGroup[2] = it->second.m[reg];
                            reg.s = "iconWidth";
                            regGroup[3] = it->second.m[reg];
                            reg.s = "iconHeight";
                            regGroup[4] = it->second.m[reg];
                            __GREG_NRO(0); __GREG_NRO(1); __GREG_NRO(2);
                            __GREG_NRO(3); __GREG_NRO(4);
                            funcList["eval"](&nsgal.m[_icon], nullptr);
                        }
                    }
                }

                //Draw text
                reg.s = "texts";
                if (
                    nowScene.m.count(reg) != 0 &&
                    nsgal.m.count(_font) != 0 &&
                    nsgal.m.count(_char) != 0
                ) {
                    if (nowScene.m[reg].m.size() > 0) {
                        Register txt = nowScene.m[reg], color;
                        int posX = 64, posY = 64, textCnt = 0;

                        reg.s = "posX";
                        if (txt.m.count(reg) > 0) posX = txt.m[reg].n.i;
                        reg.s = "posY";
                        if (txt.m.count(reg) > 0) posY = txt.m[reg].n.i;
                        for (
                            auto it = txt.m.begin();
                            it != txt.m.end(); it++
                        ) {
                            if (it->first.type != RegType::REG_INT)
                                continue;
                            if (it->second.m.empty()) continue;
                            regGroup[0] = nsgal.m[_font];
                            regGroup[1].type = RegType::REG_INT;
                            regGroup[1].n.i = posX;
                            regGroup[2].type = RegType::REG_INT;
                            regGroup[2].n.i = posY + textCnt * 16;
                            reg.s = "textShow";
                            regGroup[3] = it->second.m[reg];
                            __GREG_NRO(0); __GREG_NRO(1);
                            __GREG_NRO(2); __GREG_NRO(3);

                            reg.s = "textBack";
                            if (it->second.m.count(reg) != 0) {
                                color = it->second.m[reg];
                                funcList["back"](&color, nullptr);
                            }
                            reg.s = "textFore";
                            if (it->second.m.count(reg) != 0) {
                                color = it->second.m[reg];
                                funcList["fore"](&color, nullptr);
                            }

                            funcList["eval"](&nsgal.m[_char], nullptr);

                            if (nsgal.m.count(_defback) != 0)
                                funcList["back"](&nsgal.m[_defback], nullptr);
                            if (nsgal.m.count(_deffore) != 0)
                                funcList["fore"](&nsgal.m[_deffore], nullptr);

                            textCnt += 1;
                        }

                        //Handle input
                        reg.s = "ptrS"; int ptrPos = 0;
                        Register ptrS; ptrS.s = "->";
                        ptrS.type = RegType::REG_STR;
                        if (txt.m.count(reg) != 0) ptrS = txt.m[reg];
                        while (1) {
                            funcList["pmq"](nullptr, nullptr);
                            reg.s = "f1"; funcList["key"](&tmp, &reg);
                            val = tmp.n.i;
                            reg.s = "f8"; funcList["key"](&tmp, &reg);
                            val += tmp.n.i;
                            if (val > 1) {
                                stopBGM();
                                if (bgm != 0) {
                                    unloadBGM(bgm);
                                    bgm = 0;
                                }
                                return Result::RES_OK;
                            }

                            regGroup[0] = nsgal.m[_font];
                            regGroup[1].type = RegType::REG_INT;
                            regGroup[1].n.i = posX - 32;
                            regGroup[2].type = RegType::REG_INT;
                            regGroup[2].n.i = posY + ptrPos * 16;
                            regGroup[3] = ptrS;
                            __GREG_NRO(0); __GREG_NRO(1);
                            __GREG_NRO(2); __GREG_NRO(3);

                            reg.s = "ptrSback";
                            if (txt.m.count(reg) != 0) {
                                color = txt.m[reg];
                                funcList["back"](&color, nullptr);
                            }
                            reg.s = "ptrSfore";
                            if (txt.m.count(reg) != 0) {
                                color = txt.m[reg];
                                funcList["fore"](&color, nullptr);
                            }

                            funcList["eval"](&nsgal.m[_char], nullptr);

                            reg.s = " ";
                            for (int i = 0; i < ptrS.s.length() - 1; i++)
                                reg.s += " ";
                            regGroup[2].n.i = posY + (ptrPos - 1) * 16;
                            regGroup[3] = reg; __GREG_NRO(3);
                            funcList["eval"](&nsgal.m[_char], nullptr);
                            regGroup[2].n.i = posY + (ptrPos + 1) * 16;
                            funcList["eval"](&nsgal.m[_char], nullptr);

                            if (nsgal.m.count(_defback) != 0)
                                funcList["back"](&nsgal.m[_defback], nullptr);
                            if (nsgal.m.count(_deffore) != 0)
                                funcList["fore"](&nsgal.m[_deffore], nullptr);

                            reg.s = "lu"; funcList["kup"](&tmp, &reg);
                            if (tmp.n.i > 0) ptrPos -= 1;
                            reg.s = "ld"; funcList["kup"](&tmp, &reg);
                            if (tmp.n.i > 0) ptrPos += 1;
                            if (ptrPos > textCnt - 1) ptrPos = textCnt - 1;
                            if (ptrPos < 0) ptrPos = 0;
                            
                            reg.s = "ru"; funcList["kup"](&tmp, &reg);
                            if (tmp.n.i > 0) {
                                tmp.n.i = ptrPos;
                                reg.s = "textJump";
                                nowScene = txt.m[tmp].m[reg];
                                if (nowScene.s == "__exit") {
                                    stopBGM();
                                    if (bgm != 0) {
                                        unloadBGM(bgm);
                                        bgm = 0;
                                    }
                                    return Result::RES_OK;
                                } else if (nowScene.s == "__this")
                                    continue;
                                else break;
                            }
                        }
                    }
                }
            }
        #else
            while (1) {
                if (checkKey(KEY_F1) && checkKey(KEY_F8)) break;

                nowScene = nsgal.m[nowScene];
                if (nowScene.m.size() == 0) break;

                //Draw back
                if (nsgal.m.count(_back) != 0) {
                    reg.s = "backAddr";
                    int addr = nowScene.m[reg].n.i;
                    reg.s = "backWidth";
                    int width = nowScene.m[reg].n.i;
                    reg.s = "backHeight";
                    int height = nowScene.m[reg].n.i;
                    if (addr < 0) {
                        lcd->colorb(lcd->p, -addr);
                        lcd->clear(lcd->p);
                    } else {
                        for (uint16_t y = 0; y < lcd->p->height; y += height)
                            for (uint16_t x = 0; x < lcd->p->width; x += width)
                                lcd->bitmap(
                                    lcd->p, x, y, width, height,
                                    (uint32_t*) __getmem(addr)
                                );
                    }
                }

                //Draw icon
                reg.s = "icons";
                if (
                    nowScene.m.count(reg) != 0 &&
                    nsgal.m.count(_icon) != 0
                ) {
                    if (nowScene.m[reg].m.size() > 0) {
                        Register ico = nowScene.m[reg];
                        int addr, x, y, w, h;
                        for (
                            auto it = ico.m.begin();
                            it != ico.m.end(); it++
                        ) {
                            if (it->second.m.empty()) continue;
                            reg.s = "iconAddr"; addr = it->second.m[reg].n.i;
                            reg.s = "iconX"; x = it->second.m[reg].n.i;
                            reg.s = "iconY"; y = it->second.m[reg].n.i;
                            reg.s = "iconWidth"; w = it->second.m[reg].n.i;
                            reg.s = "iconHeight"; h = it->second.m[reg].n.i;
                            lcd->iconc(lcd->p, x, y, w, h, __getmem(addr));
                        }
                    }
                }

                //Draw text
                reg.s = "texts";
                if (
                    nowScene.m.count(reg) != 0 &&
                    nsgal.m.count(_font) != 0 &&
                    nsgal.m.count(_char) != 0
                ) {
                    if (nowScene.m[reg].m.size() > 0) {
                        Register txt = nowScene.m[reg];
                        int posX = 64, posY = 64, textCnt = 0;

                        reg.s = "posX";
                        if (txt.m.count(reg) > 0) posX = txt.m[reg].n.i;
                        reg.s = "posY";
                        if (txt.m.count(reg) > 0) posY = txt.m[reg].n.i;

                        int font = nsgal.m[_font].n.i, x, y, color; string str;
                        for (
                            auto it = txt.m.begin();
                            it != txt.m.end(); it++
                        ) {
                            if (it->first.type != RegType::REG_INT)
                                continue;
                            if (it->second.m.empty()) continue;
                            x = posX;
                            y = posY + textCnt * 16;
                            reg.s = "textShow"; str = it->second.m[reg].s;

                            reg.s = "textBack";
                            if (it->second.m.count(reg) != 0) {
                                color = it->second.m[reg].n.i;
                                lcd->colorb(lcd->p, color);
                            }
                            reg.s = "textFore";
                            if (it->second.m.count(reg) != 0) {
                                color = it->second.m[reg].n.i;
                                lcd->colorf(lcd->p, color);
                            }

                            drawGBK(font, x, y, str);

                            if (nsgal.m.count(_defback) != 0) {
                                color = nsgal.m[_defback].n.i;
                                lcd->colorb(lcd->p, color);
                            }
                            if (nsgal.m.count(_deffore) != 0) {
                                color = nsgal.m[_deffore].n.i;
                                lcd->colorf(lcd->p, color);
                            }

                            textCnt += 1;
                        }

                        //Handle input
                        reg.s = "ptrS"; int ptrPos = 0;
                        Register ptrS; ptrS.s = "->";
                        ptrS.type = RegType::REG_STR;
                        if (txt.m.count(reg) != 0) ptrS = txt.m[reg];
                        while (1) {
                            if (checkKey(KEY_F1) && checkKey(KEY_F8))
                                return Result::RES_OK;

                            x = posX - 32;
                            y = posY + ptrPos * 16;
                            str = ptrS.s;

                            reg.s = "ptrSback";
                            if (txt.m.count(reg) != 0) {
                                color = txt.m[reg].n.i;
                                lcd->colorb(lcd->p, color);
                            }
                            reg.s = "ptrSfore";
                            if (txt.m.count(reg) != 0) {
                                color = txt.m[reg].n.i;
                                lcd->colorf(lcd->p, color);
                            }

                            drawGBK(font, x, y, str);

                            str = " ";
                            for (int i = 0; i < ptrS.s.length() - 1; i++)
                                str += " ";
                            y = posY + (ptrPos - 1) * 16;
                            drawGBK(font, x, y, str);
                            y = posY + (ptrPos + 1) * 16;
                            drawGBK(font, x, y, str);

                            if (nsgal.m.count(_defback) != 0) {
                                color = nsgal.m[_defback].n.i;
                                lcd->colorb(lcd->p, color);
                            }
                            if (nsgal.m.count(_deffore) != 0) {
                                color = nsgal.m[_deffore].n.i;
                                lcd->colorf(lcd->p, color);
                            }

                            if (checkKeyUp(LPAD_UP)) ptrPos -= 1;
                            if (checkKeyUp(LPAD_DOWN)) ptrPos += 1;
                            if (ptrPos > textCnt - 1) ptrPos = textCnt - 1;
                            if (ptrPos < 0) ptrPos = 0;

                            if (checkKeyUp(RPAD_UP)) {
                                tmp.n.i = ptrPos;
                                reg.s = "textJump";
                                nowScene = txt.m[tmp].m[reg];
                                if (nowScene.s == "__exit")
                                    return Result::RES_OK;
                                else if (nowScene.s == "__this")
                                    continue;
                                else break;
                            }
                        }
                    }
                }
            }
        #endif

            return Result::RES_OK;
        };

        funcList["gal.debug.get"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->readOnly) return Result::RES_ERR;
            *dst = nsgal;
            return Result::RES_OK;
        };
        funcList["gal.debug.prt"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            string s = ""; *dst >> s;
            for (int i = 0; i < s.length(); i++) {
                Util::print(s[i]);
                funcList["pmq"](nullptr, nullptr);
            }
            Util::print("\n");
            return Result::RES_OK;
        };

        funcList["gal.render.back"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_CODE) return Result::RES_ERR;
            nsgal.m[_back] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.render.icon"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_CODE) return Result::RES_ERR;
            nsgal.m[_icon] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.render.char"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_CODE) return Result::RES_ERR;
            nsgal.m[_char] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.render.font"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            nsgal.m[_font] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.default.back"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            nsgal.m[_defback] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.default.fore"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            nsgal.m[_deffore] = *dst;
            return Result::RES_OK;
        };

        funcList["gal.scene.begin"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_STR) return Result::RES_ERR;
            _sceneName = *dst;
            _scene.type = RegType::REG_MAP;
            _scene.m.clear();
            return Result::RES_OK;
        };
        funcList["gal.scene.end"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            nsgal.m[_sceneName] = _scene;
            return Result::RES_OK;
        };

        funcList["gal.scene.back"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[0].type != RegType::REG_INT) return Result::RES_ERR;
            if (regGroup[1].type != RegType::REG_INT) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "backAddr"; _scene.m[reg] = *dst;
            reg.s = "backWidth"; _scene.m[reg] = regGroup[0];
            reg.s = "backHeight"; _scene.m[reg] = regGroup[1];
            return Result::RES_OK;
        };

    #ifdef NSGDX_IS_EMU
        funcList["gal.scene.bgm"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_STR) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "bgmPath"; _scene.m[reg] = *dst;
            return Result::RES_OK;
        };
    #endif

        funcList["gal.scene.icon.begin"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            _icons.type = RegType::REG_MAP;
            _icons.m.clear(); _iconCnt = 0;
            return Result::RES_OK;
        };
        funcList["gal.scene.icon.end"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "icons"; _scene.m[reg] = _icons;
            return Result::RES_OK;
        };

        funcList["gal.scene.icon.new"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            _icont.type = RegType::REG_MAP;
            _icont.m.clear();
            return Result::RES_OK;
        };
        funcList["gal.scene.icon.put"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_INT;
            reg.n.i = _iconCnt; _icons.m[reg] = _icont;
            _iconCnt += 1;
            return Result::RES_OK;
        };
        funcList["gal.scene.icon.addr"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "iconAddr"; _icont.m[reg] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.scene.icon.pos"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "iconX"; _icont.m[reg] = *dst;
            reg.s = "iconY"; _icont.m[reg] = *src;
            return Result::RES_OK;
        };
        funcList["gal.scene.icon.size"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "iconWidth"; _icont.m[reg] = *dst;
            reg.s = "iconHeight"; _icont.m[reg] = *src;
            return Result::RES_OK;
        };

        funcList["gal.scene.text.begin"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            _texts.type = RegType::REG_MAP;
            _texts.m.clear(); _textCnt = 0;
            return Result::RES_OK;
        };
        funcList["gal.scene.text.end"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "texts"; _scene.m[reg] = _texts;
            return Result::RES_OK;
        };
        funcList["gal.scene.text.pos"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src == nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            if (src->type != RegType::REG_INT) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "posX"; _texts.m[reg] = *dst;
            reg.s = "posY"; _texts.m[reg] = *src;
            return Result::RES_OK;
        };
        funcList["gal.scene.text.ptr"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_STR) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "ptrS"; _texts.m[reg] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.scene.text.ptr.back"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "ptrSback"; _texts.m[reg] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.scene.text.ptr.fore"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "ptrSfore"; _texts.m[reg] = *dst;
            return Result::RES_OK;
        };

        funcList["gal.scene.text.new"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            _textt.type = RegType::REG_MAP;
            _textt.m.clear();
            return Result::RES_OK;
        };
        funcList["gal.scene.text.put"] = $OP_{
            if (dst != nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_INT;
            reg.n.i = _textCnt; _texts.m[reg] = _textt;
            _textCnt += 1;
            return Result::RES_OK;
        };
        funcList["gal.scene.text.back"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "textBack"; _textt.m[reg] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.scene.text.fore"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_INT) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "textFore"; _textt.m[reg] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.scene.text.show"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_STR) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "textShow"; _textt.m[reg] = *dst;
            return Result::RES_OK;
        };
        funcList["gal.scene.text.jump"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->type != RegType::REG_STR) return Result::RES_ERR;
            Register reg; reg.type = RegType::REG_STR;
            reg.s = "textJump"; _textt.m[reg] = *dst;
            return Result::RES_OK;
        };
    }

}
