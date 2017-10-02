#include <iostream>
#include <vector>

#include "windows/behaviorchanger.h"

using std::vector;

namespace InjectorPP
{
    BehaviorChanger::BehaviorChanger()
    {
    }

    BehaviorChanger::~BehaviorChanger()
    {
    }

    void BehaviorChanger::replaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm, FunctionType functionType, FunctionReturnType returnType)
    {
        vector<byte> asmCode;
        generateToInjectAsmCode(sourceFuncAddress, targetFuncAddress, functionType, returnType, asmCode);

        size_t injectedCodeSize = asmCode.size();

        // First important thing, backup the original asm code.
        originalFuncAsm->asmCode = new byte[injectedCodeSize];
        originalFuncAsm->funcAddress = sourceFuncAddress;
        ReadProcessMemory((HANDLE)-1, (void *)originalFuncAsm->funcAddress, originalFuncAsm->asmCode, injectedCodeSize, 0);

        // Then inject the code to source function.
        this->directWriteToFunction(sourceFuncAddress, &asmCode[0], injectedCodeSize);
    }

    void BehaviorChanger::directWriteToFunction(ULONG64 funcAddress, const byte* asmCode, size_t asmCodeSize)
    {
        WriteProcessMemory((HANDLE)-1, (void*)funcAddress, asmCode, asmCodeSize, 0);
    }
}