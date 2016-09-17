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

        virtual ~InjectorCore();

        void Initialize();

        void* GetVirtualMethodAddress(void* classInstance, const std::string& virtualMethodName);

        std::string GetMethodReturnType(void* srcFunc);

        void ReplaceFunction(void* srcFunc, void* destFunc, bool isSourceMemberFunction);

        void* Fake(const char* typeName, size_t typeSize, bool autoFillDefaultValue);

        void ChangeFunctionReturnValue(const std::string& funcCallCode, const int& expectedReturnValue);

        void ChangeFunctionReturnValue(const std::string& funcCallCode, const char* expectedReturnValue);

        void ChangeFunctionReturnValue(const std::string& funcCallCode, const void* expectedReturnValue);

    private:
        InjectorCore(const InjectorCore&);

        void RecoverAllReplacedFunctions();

        void GetFunctionAddressByFunctionCallCode(const std::string& funcCallCode, ULONG64& funcAddress);

        bool SaveOriginalFuncASM(OriginalFuncASM* originalFuncASM);

        std::vector<void*> m_allocatedTypeInstances;

        BehaviorChanger* m_behaviorChanger;

        IClassResolver* m_classResolver;

        IFunctionResolver* m_functionResolver;

        SymbolInfoHelper* m_symbolInfoHelper;

        HANDLE m_currentProcessHandler;

        std::vector<Class*> m_resolvedClasses;

        std::vector<OriginalFuncASM*> m_originalFunctionASMVector;

        static bool m_isSymInitialized;
    };
}

#endif
