#include "NSGDX.h"

#include "../NSASM/Util.h"

#define __GREG_NRO(n) (regGroup[n].readOnly = false)

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

                //Draw icon
                reg.s = "icons";
                if (nowScene.m.count(reg) > 0) {
                    if (nowScene.m[reg].m.size() > 0) {
                        if (nsgal.m.count(_icon) == 0) break;
                        for (
                            auto it = nowScene.m[reg].m.begin();
                            it != nowScene.m[reg].m.end(); it++
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
                if (nowScene.m.count(reg) > 0) {
                    if (nowScene.m[reg].m.size() > 0) {
                        Register txt = nowScene.m[reg];
                        int posX = 64, posY = 64, textCnt = 0;

                        if (nsgal.m.count(_font) == 0) break;
                        if (nsgal.m.count(_char) == 0) break;

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
                            funcList["eval"](&nsgal.m[_char], nullptr);
                            textCnt += 1;
                        }

                        //Handle input
                        reg.s = "ptrS"; int ptrPos = 0;
                        Register ptrS; ptrS.s = "->";
                        ptrS.type = RegType::REG_STR;
                        if (txt.m.count(reg) > 0) ptrS = txt.m[reg];
                        while (1) {
                            funcList["pmq"](nullptr, nullptr);
                            reg.s = "f1"; funcList["key"](&tmp, &reg);
                            val = tmp.n.i;
                            reg.s = "f8"; funcList["key"](&tmp, &reg);
                            val += tmp.n.i;
                            if (val > 1) return Result::RES_OK;

                            regGroup[0] = nsgal.m[_font];
                            regGroup[1].type = RegType::REG_INT;
                            regGroup[1].n.i = posX - 32;
                            regGroup[2].type = RegType::REG_INT;
                            regGroup[2].n.i = posY + ptrPos * 16;
                            regGroup[3] = ptrS;
                            __GREG_NRO(0); __GREG_NRO(1);
                            __GREG_NRO(2); __GREG_NRO(3);
                            funcList["eval"](&nsgal.m[_char], nullptr);

                            reg.s = "lu"; funcList["kup"](&tmp, &reg);
                            if (tmp.n.i > 0) ptrPos += 1;
                            reg.s = "ld"; funcList["kup"](&tmp, &reg);
                            if (tmp.n.i > 0) ptrPos -= 1;
                            if (ptrPos >= textCnt) ptrPos = 0;
                            if (ptrPos < 0) ptrPos = textCnt - 1;
                            
                            reg.s = "ru"; funcList["kup"](&tmp, &reg);
                            if (tmp.n.i > 0) {
                                tmp.n.i = ptrPos;
                                reg.s = "textJump";
                                nowScene = txt.m[tmp].m[reg];
                                if (nowScene.s == "__exit")
                                    return Result::RES_OK;
                                else break;
                            }
                        }
                    }
                }
            }

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
