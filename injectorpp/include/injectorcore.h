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
#include "Class.h"

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

        void ChangeFunctionReturnValue(const std::string& funcCallCode, const void* expectedReturnValue);

        //void ChangeFunctionReturnValue(const std::string& funcCallCode, const std::string& expectedReturnValue);
    private:
        InjectorCore(const InjectorCore&);

        void GetFunctionAddressByFunctionCallCode(const std::string& funcCallCode, ULONG64& funcAddress);

        std::vector<void*> m_allocatedTypeInstances;

        BehaviorChanger* m_behaviorChanger;

        IClassResolver* m_classResolver;

        IFunctionResolver* m_functionResolver;

        SymbolInfoHelper* m_symbolInfoHelper;

        HANDLE m_currentProcessHandler;

        std::vector<Class*> m_resolvedClasses;

        static bool m_isSymInitialized;
    };
}

#endif
