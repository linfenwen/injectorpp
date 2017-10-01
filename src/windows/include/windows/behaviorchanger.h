#ifndef INJECTORPP_BEHAVIORCHANGER_H
#define INJECTORPP_BEHAVIORCHANGER_H

#include <Windows.h>
#include <vector>
#include "originalfuncasm.h"
#include "functiontype.h"
#include "functionreturntype.h"

namespace InjectorPP
{
    class BehaviorChanger
    {
    public:
        BehaviorChanger();
        virtual ~BehaviorChanger();

        // A magic function to change the function behavior at runtime.
        virtual void replaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm, FunctionType functionType, FunctionReturnType returnType) = 0;

        virtual void directWriteToFunction(ULONG64 funcAddress, const byte* asmCode, size_t asmCodeSize);
    private:
        BehaviorChanger(const BehaviorChanger&);
    };
}

#endif
