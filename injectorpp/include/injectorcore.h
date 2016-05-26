#ifndef INJECTORPP_INJECTORCORE_H
#define INJECTORPP_INJECTORCORE_H

#include <Windows.h>
#include <DbgHelp.h>
#include <string>
#include <vector>

#include "BehaviorChanger.h"

namespace InjectorPP
{
    class InjectorCore
    {
    public:
        InjectorCore();

        ~InjectorCore();

        void* Fake(const char* typeName, size_t typeSize);

    private:
        InjectorCore(const InjectorCore&);

        PSYMBOL_INFO AllocSymbol(int nameLen);

        std::vector<void*> m_allocatedTypeInstances;

        std::vector<PSYMBOL_INFO> m_allocatedSymbolInfos;

        BehaviorChanger* m_behaviorChanger;
    };
}

#endif
