#ifndef __NSGDX_H_
#define __NSGDX_H_


#include "../NSASM/NSASM.h"

//#define NSGDX_IS_EMU

namespace NSGDX {

    using namespace NSASM;

    class NSGDX : public NSASM {

    public:
        NSGDX(int heap, int stack, int reg, map<string, string> code) : NSASM(heap, stack, reg, code) { gdxFunc(); }
        ~NSGDX() { dispose(); }
        void dispose();

    private:
        Register nsgal;

        Register _back, _icon, _char;
        Register _font;

        Register _defback, _deffore;

        Register _sceneName, _scene;
        Register _icons, _texts;
        Register _icont, _textt;
        size_t _iconCnt, _textCnt;

        void loadNSGAL();
    
    protected:
        void gdxFunc();

        NSASM* instance(NSASM& super, map<string, string>& code) override {
            return new NSGDX(super, code);
        }
        NSGDX(NSASM& super, map<string, string>& code) : NSASM(super, code) { gdxFunc(); }
        
    };

}


#endif
