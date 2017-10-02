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

    protected:
        virtual void generateToInjectAsmCode(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, FunctionType functionType, FunctionReturnType returnType, std::vector<byte>& asmCode);

    private:
        X86WindowsBehaviorChanger(const X86WindowsBehaviorChanger&);
        X86WindowsBehaviorChanger& operator=(const X86WindowsBehaviorChanger&);
    };
}

#endif