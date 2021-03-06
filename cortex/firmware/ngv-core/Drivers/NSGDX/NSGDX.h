#ifndef __NSGDX_H_
#define __NSGDX_H_


#include "../NSASM/NSASM.h"

namespace NSGDX {

    using namespace NSASM;

    class NSGDX : public NSASM {

    public:
        NSGDX(int heap, int stack, int reg, map<string, string> code) : NSASM(heap, stack, reg, code) { gdxFunc(); }
        ~NSGDX() { dispose(); }
        void dispose();
    
    protected:
        void gdxFunc();

        NSASM* instance(NSASM& super, map<string, string>& code) override {
            return new NSGDX(super, code);
        }
        NSGDX(NSASM& super, map<string, string>& code) : NSASM(super, code) { gdxFunc(); }
        
    };

}


#endif
