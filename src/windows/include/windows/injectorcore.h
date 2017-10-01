#ifndef INJECTORPP_INJECTORCORE_H
#define INJECTORPP_INJECTORCORE_H

#include <Windows.h>
#include <DbgHelp.h>
#include <string>
#include <vector>
#include <map>

#include "behaviorchanger.h"
#include "ifunctionresolver.h"
#include "iclassresolver.h"
#include "symbolinfohelper.h"
#include "class.h"

namespace InjectorPP
{
    class InjectorCore
    {
    public:
        InjectorCore();

        virtual ~InjectorCore();

        void initialize();

        void* getVirtualMethodAddress(void* classInstance, const std::string& virtualMethodName);

        void replaceFunction(void* srcFunc, void* destFunc, bool isSourceMemberFunction, bool isSourceStaticMemberFunction, bool isSourceVirtualMemberFunction);

    private:
        InjectorCore(const InjectorCore&);

        void recoverAllReplacedFunctions();

        bool saveOriginalFuncASM(OriginalFuncASM* originalFuncASM);

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
