#ifndef INJECTORPP_ICLASSRESOLVER_H
#define INJECTORPP_ICLASSRESOLVER_H

#include <Windows.h>
#include "function.h"

namespace InjectorPP
{
    class IClassResolver
    {
    public:
        virtual void ResolveMethods(const ULONG64& modBase, const std::string className, std::vector<Function>& resolvedMethods) = 0;
    };
}

#endif
