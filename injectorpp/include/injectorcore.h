#ifndef INJECTORPP_INJECTORCORE_H
#define INJECTORPP_INJECTORCORE_H

#include <Windows.h>
#include <DbgHelp.h>
#include <string>
#include <vector>
#include <map>

#include "BehaviorChanger.h"

namespace InjectorPP
{
    class InjectorCore
    {
    public:
        InjectorCore();

        ~InjectorCore();

        void* Fake(const char* typeName, size_t typeSize);

        void ChangeFunctionReturnValue(const std::string& funcCallCode, const int& expectedReturnValue);

        void ChangeFunctionReturnValue(const std::string& funcCallCode, const char* expectedReturnValue);
    private:
        InjectorCore(const InjectorCore&);

        PSYMBOL_INFO AllocSymbol(int nameLen);

        void AddFunctionSymbolAddressMapping(const wchar_t* funcSymbol, const ULONG64& address);

        std::vector<void*> m_allocatedTypeInstances;

        std::vector<PSYMBOL_INFO> m_allocatedSymbolInfos;

        std::map<std::string, ULONG64> m_funcSymAddressMapping;

        BehaviorChanger* m_behaviorChanger;
    };
}

#endif
