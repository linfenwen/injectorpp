#ifndef INJECTORPP_FUNCTIONRESOLVER_H
#define INJECTORPP_FUNCTIONRESOLVER_H

#include <vector>
#include "IFunctionResolver.h"

namespace InjectorPP
{
    enum BasicType;

    class FunctionResolver : public IFunctionResolver
    {
    public:
        // Pass process handle to constructor.
        FunctionResolver(HANDLE processHandle);

        virtual ~FunctionResolver();

        virtual void Resolve(const ULONG64& modBase, const ULONG& typeIndex, Function& resolvedFunction);

        virtual std::string GetMethodSymbolFromAddress(const ULONG64& funcAddress);

        virtual ResolvedType GetMethodReturnTypeFromAddress(const ULONG64& funcAddress);

    protected:
        // Resolve the function return type.
        virtual ResolvedType ResolveReturnType(const ULONG64& modBase, const ULONG& typeIndex);

        virtual void ResolveParameters(const ULONG64& functionAddress, const ULONG64& modBase, const ULONG& typeIndex, std::vector<FunctionParameter>& resolvedParameters);
    private:
        // Disable copy constructor.
        FunctionResolver(const FunctionResolver&);

        // Loads the name of the type of that symbol
        void LoadType(ULONG64 modBase, ULONG typeIndex, ResolvedType& resolvedType);

        // Loads a basic type (int, float, char, ...)
        void LoadBasicType(BasicType bt, ULONG64 byteSize, ResolvedType& resolvedType);

        // Loads the type pointed to
        void LoadPointerType(ULONG64 modBase, ULONG typeIndex, ULONG subType, ResolvedType& resolvedType);

        VARIANT m_value;
        std::string m_objName;
        //std::string m_typeName;
        HANDLE m_hProcess;
        std::string m_enumValue;
    };
}

#endif