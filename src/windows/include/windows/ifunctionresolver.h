#ifndef INJECTORPP_IFUNCTIONRESOLVER_H
#define INJECTORPP_IFUNCTIONRESOLVER_H

#include <Windows.h>
#include <string>
#include "function.h"
#include "resolvedtype.h"

namespace InjectorPP
{
    class IFunctionResolver
    {
    public:
        virtual std::string GetMethodSymbolFromAddress(const ULONG64& funcAddress) = 0;
        virtual ResolvedType GetMethodReturnTypeFromAddress(const ULONG64& funcAddress) = 0;
        virtual void Resolve(const ULONG64& modBase, const ULONG& typeIndex, Function& resolvedFunction) = 0;
    };
}

#endif