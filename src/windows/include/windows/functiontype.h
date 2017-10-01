#ifndef INJECTORPP_FUNCTIONTYPE_H
#define INJECTORPP_FUNCTIONTYPE_H

namespace InjectorPP
{
    enum FunctionType
    {
        GlobalFunction,
        NonVirtualMemberFunction,
        VirtualMemberFunction,
        StaticMemberFunction
    };
}

#endif