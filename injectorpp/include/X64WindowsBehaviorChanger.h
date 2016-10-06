#ifndef INJECTORPP_X64BEHAVIORCHANGER_H
#define INJECTORPP_X64BEHAVIORCHANGER_H

#include "BehaviorChanger.h"

namespace InjectorPP
{
    class X64WindowsBehaviorChanger : public BehaviorChanger
    {
    public:
        X64WindowsBehaviorChanger();
        virtual ~X64WindowsBehaviorChanger();

        virtual void ReplaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm, bool isComplexReturn, bool isSourceFuncVirtualMethod);

    private:
        X64WindowsBehaviorChanger(const X64WindowsBehaviorChanger&);
    };
}

#endif