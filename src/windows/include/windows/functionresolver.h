#ifndef INJECTORPP_FUNCTIONRESOLVER_H
#define INJECTORPP_FUNCTIONRESOLVER_H

#include <vector>
#include "ifunctionresolver.h"

namespace InjectorPP
{
    enum BasicType;

    class FunctionResolver : public IFunctionResolver
    {
    public:
        // Pass process handle to constructor.
        FunctionResolver(HANDLE processHandle);

        virtual ~FunctionResolver();

        virtual void resolve(const ULONG64& modBase, const ULONG& typeIndex, Function& resolvedFunction);

        virtual std::string getMethodSymbolFromAddress(const ULONG64& funcAddress);

        virtual ResolvedType getMethodReturnTypeFromAddress(const ULONG64& funcAddress);

    protected:
        // Resolve the function return type.
        virtual ResolvedType resolveReturnType(const ULONG64& modBase, const ULONG& typeIndex);

        virtual void resolveParameters(const ULONG64& functionAddress, const ULONG64& modBase, const ULONG& typeIndex, std::vector<FunctionParameter>& resolvedParameters);
    private:
        // Disable copy constructor.
        FunctionResolver(const FunctionResolver&);

        // Loads the name of the type of that symbol
        void loadType(ULONG64 modBase, ULONG typeIndex, ResolvedType& resolvedType);

        // Loads a basic type (int, float, char, ...)
        void loadBasicType(BasicType bt, ULONG64 byteSize, ResolvedType& resolvedType);

        // Loads the type pointed to
        void loadPointerType(ULONG64 modBase, ULONG typeIndex, ULONG subType, ResolvedType& resolvedType);

        HANDLE m_hProcess;
        VARIANT m_value;
        std::string m_objName;
        std::string m_enumValue;
    };
}

#endif