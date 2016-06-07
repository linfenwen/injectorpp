#ifndef INJECTORPP_BEHAVIORCHANGER_H
#define INJECTORPP_BEHAVIORCHANGER_H

#include <Windows.h>
#include <vector>

namespace InjectorPP
{
    struct OriginalFuncASM
    {
        ULONG64 funcAddress;
        byte asmCode[6];
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

        void DirectWriteToFunction(ULONG64 funcAddress, const byte* asmCode, size_t asmCodeSize);
    private:
        BehaviorChanger(const BehaviorChanger&);

        std::vector<char*> m_allocatedCharBuff;
        std::vector<ULONG> m_allocatedCharBuffAddress;

        const int MAX_CHAR_BUFF_SIZE = 1024 * 1024;
    };
}

#endif
