#ifndef INJECTORPP_BEHAVIORCHANGER_H
#define INJECTORPP_BEHAVIORCHANGER_H

#include <Windows.h>
#include <vector>
#include "originalfuncasm.h"

namespace InjectorPP
{
    class BehaviorChanger
    {
    public:
        BehaviorChanger();
        virtual ~BehaviorChanger();

        // A magic function to change the function behavior at runtime
        virtual void ReplaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm, bool isComplexReturn, bool isSourceFuncVirtualMethod) = 0;

        virtual void DirectWriteToFunction(ULONG64 funcAddress, const byte* asmCode, size_t asmCodeSize);
    private:
        BehaviorChanger(const BehaviorChanger&);
    };
}

#endif
