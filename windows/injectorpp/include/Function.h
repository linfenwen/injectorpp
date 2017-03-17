#ifndef INJECTORPP_FUNCTION_H
#define INJECTORPP_FUNCTION_H

#include <Windows.h>
#include <string>
#include <vector>
#include "ResolvedType.h"

namespace InjectorPP
{
    struct FunctionParameter
    {
        std::string Name;
        std::string Type;
    };

    struct Function
    {
        std::string Name;

        // Without scope or class identifier.
        // E.G:
        // Function.Name may be Address::GetMyCode, whereas 
        // Function.RawName is GetMyCode.
        std::string RawName;
        ResolvedType ReturnType;
        std::vector<FunctionParameter> Parameters;
        ULONG64 Address;
    };
}

#endif