#ifndef INJECTORPP_X86BEHAVIORCHANGER_H
#define INJECTORPP_X86BEHAVIORCHANGER_H

#include "behaviorchanger.h"

namespace InjectorPP
{
    class X86WindowsBehaviorChanger : public BehaviorChanger
    {
    public:
        X86WindowsBehaviorChanger();
        virtual ~X86WindowsBehaviorChanger();

        virtual void replaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm, FunctionType functionType, FunctionReturnType returnType);

    private:
        X86WindowsBehaviorChanger(const X86WindowsBehaviorChanger&);
    };
}

#endif