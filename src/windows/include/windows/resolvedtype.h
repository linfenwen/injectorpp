#ifndef INJECTORPP_RETURNTYPE_H
#define INJECTORPP_RETURNTYPE_H

#include <string>
#include "symbolenums.h"

namespace InjectorPP
{
    struct ResolvedType
    {
        std::string Name;
        enum SymTagEnum SymbolTag;
    };
}

#endif