#ifndef INJECTORPP_CLASSRESOLVER_H
#define INJECTORPP_CLASSRESOLVER_H

#include "utility.h"
#include "iclassresolver.h"
#include "symbolinfohelper.h"
#include "ifunctionresolver.h"

namespace InjectorPP
{
    class ClassResolver : public IClassResolver
    {
    public:
        ClassResolver(HANDLE processHandle, IFunctionResolver* functionResolver);

        virtual ~ClassResolver();

        virtual void resolveMethods(const ULONG64& modBase, const std::string className, std::vector<Function>& resolvedMethods);

    private:
        HANDLE m_hProcess;

        SymbolInfoHelper* m_symbolInfoHelper;

        IFunctionResolver* m_functionResolver;
    };
}

#endif