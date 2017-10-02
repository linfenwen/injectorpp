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
        virtual void replaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm, FunctionType functionType, FunctionReturnType returnType);

        virtual void directWriteToFunction(ULONG64 funcAddress, const byte* asmCode, size_t asmCodeSize);
    protected:
        virtual void generateToInjectAsmCode(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, FunctionType functionType, FunctionReturnType returnType, std::vector<byte>& asmCode) = 0;
    private:
        BehaviorChanger(const BehaviorChanger&);
        BehaviorChanger& operator=(const BehaviorChanger&);
    };
}

#endif
