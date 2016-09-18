#ifndef INJECTORPP_BEHAVIORCHANGER_H
#define INJECTORPP_BEHAVIORCHANGER_H

#include <Windows.h>
#include <vector>

namespace InjectorPP
{
    struct OriginalFuncASM
    {
        OriginalFuncASM()
            :funcAddress(0),
            asmCode(NULL)
        {
        }

        ~OriginalFuncASM()
        {
            if (asmCode != NULL)
            {
                delete[] asmCode;
                asmCode = NULL;
            }
        }

        ULONG64 funcAddress;
        byte* asmCode;
    };

    class BehaviorChanger
    {
    public:
        BehaviorChanger();
        virtual ~BehaviorChanger();

        // A magic function to change the function behavior at runtime
        virtual void ReplaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm, bool isComplexReturn, bool isSourceFuncVirtualMethod);

        virtual void DirectWriteToFunction(ULONG64 funcAddress, const byte* asmCode, size_t asmCodeSize);
    private:
        BehaviorChanger(const BehaviorChanger&);

        std::vector<char*> m_allocatedCharBuff;
        std::vector<ULONG> m_allocatedCharBuffAddress;
    };
}

#endif
