#ifndef INJECTORPP_RETURNTYPE_H
#define INJECTORPP_RETURNTYPE_H

#include <string>
#include "SymbolEnums.h"

namespace InjectorPP
{
    struct ResolvedType
    {
        std::string Name;
        enum SymTagEnum SymbolTag;
    };
}

#endif