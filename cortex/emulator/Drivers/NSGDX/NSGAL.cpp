#include "NSGDX.h"

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

            _font.type = RegType::REG_INT;
            _font.n.i = 0x000000;

            return Result::RES_OK;
        };
        funcList["gal.debug"] = $OP_{
            if (dst == nullptr) return Result::RES_ERR;
            if (src != nullptr) return Result::RES_ERR;
            if (dst->readOnly) return Result::RES_ERR;
            *dst = nsgal;
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
