#ifndef INJECTORPP_INJECTORCORE_H
#define INJECTORPP_INJECTORCORE_H

#include <Windows.h>
#include <DbgHelp.h>
#include <string>
#include <vector>
#include <map>

#include "BehaviorChanger.h"
#include "IFunctionResolver.h"
#include "IClassResolver.h"
#include "SymbolInfoHelper.h"

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

        void AddFunctionSymbolAddressMapping(const std::string& funcSymbol, const ULONG64& address);

        std::vector<void*> m_allocatedTypeInstances;

        std::map<std::string, ULONG64> m_funcSymAddressMapping;

        BehaviorChanger* m_behaviorChanger;

        IClassResolver* m_classResolver;

        IFunctionResolver* m_functionResolver;

        SymbolInfoHelper* m_symbolInfoHelper;

        HANDLE m_currentProcessHandler;

        static bool m_isSymInitialized;
    };
}

#endif
