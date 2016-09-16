#ifndef INJECTORPP_BEHAVIORCHANGER_H
#define INJECTORPP_BEHAVIORCHANGER_H

#include <Windows.h>
#include <vector>

namespace InjectorPP
{
    static const int INJECTED_ASM_CODE_SIZE = 34;

    struct OriginalFuncASM
    {
        ULONG64 funcAddress;
        byte asmCode[INJECTED_ASM_CODE_SIZE];
    };

    class BehaviorChanger
    {
    public:
        BehaviorChanger();
        ~BehaviorChanger();

        // A magic function to change the function behavior at runtime
        //
        // funcAddress - The address of the function to be changed from.
        // expectedReturnValue - The return value should be changed to.
        void ChangeFunctionReturnValue(ULONG64 funcAddress, const int& expectedReturnValue, OriginalFuncASM* originalFuncAsm);

        void ChangeFunctionReturnValue(ULONG64 funcAddress, const char* expectedReturnValue, OriginalFuncASM* originalFuncAsm);

        void ChangeFunctionReturnValue(ULONG64 funcAddress, const void* expectedReturnValue, OriginalFuncASM* originalFuncAsm);

        void ReplaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm);

        void DirectWriteToFunction(ULONG64 funcAddress, const byte* asmCode, size_t asmCodeSize);
    private:
        BehaviorChanger(const BehaviorChanger&);

        std::vector<char*> m_allocatedCharBuff;
        std::vector<ULONG> m_allocatedCharBuffAddress;

        static const int MAX_CHAR_BUFF_SIZE = 1024 * 1024;
    };
}

#endif
