#ifndef INJECTORPP_INJECTORCORE_H
#define INJECTORPP_INJECTORCORE_H

#include <Windows.h>
#include <DbgHelp.h>
#include <string>
#include <vector>

namespace InjectorPP
{
    class InjectorCore
    {
    public:
        InjectorCore();

        ~InjectorCore();

        struct HOOKHANDLE
        {
            ULONG64 addr;
            char origcode[6];
            BYTE jmp[6];
        };

        // A magic function to change the function behavior at runtime
        //
        // srcFunc - The address of the function to changed from.
        // targetFunc - The address of the destination function.
        // hookHandle - The inter-mid result during margic changing.
        void HookFunc(ULONG64 srcFunc, ULONG64 targetFunc, HOOKHANDLE *hookHandle);

        void* Fake(const char* typeName, size_t typeSize);

    private:
        InjectorCore(const InjectorCore&);

        PSYMBOL_INFO AllocSymbol(int nameLen);

        std::vector<void*> m_allocatedTypeInstances;
    };
}

#endif
