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

        void ReplaceFunction(void* srcFunc, void* destFunc, bool isSourceMemberFunction);

    private:
        InjectorCore(const InjectorCore&);

        void RecoverAllReplacedFunctions();

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
