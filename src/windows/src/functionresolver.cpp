#include <Windows.h>
#include <DbgHelp.h>
#include <sstream>

#include "windows/symbolinfohelper.h"
#include "windows/functionresolver.h"
#include "windows/utility.h"

namespace InjectorPP
{
    BOOL __stdcall EnumParamsCallback(PSYMBOL_INFO inf, ULONG size, PVOID param)
    {
        // Transform the param back to the one it was before.
        std::vector<FunctionParameter>* params = (std::vector<FunctionParameter>*)param;
        if (inf == NULL)
        {
            return true;
        }

        // The flags contain various information on what type of symbol we have here
        // SYMFLAG_PARAMETER says that its a function parameter.
        if (inf->Flags & SYMFLAG_PARAMETER)
        {
            FunctionParameter newParam;
            newParam.Name = inf->Name;

            // TODO: How about complex type and pointers?
            LPWSTR functionSymName = 0;
            SymGetTypeInfo(GetCurrentProcess(), inf->ModBase, inf->TypeIndex, TI_GET_SYMNAME, &functionSymName);

            if (functionSymName != NULL)
            {
                newParam.Type = Utility::W2M(functionSymName);
            }

            params->push_back(newParam);
        }

        return true;
    }

    FunctionResolver::FunctionResolver(HANDLE processHandle)
        : m_hProcess(processHandle)
    {
        SymSetOptions(SYMOPT_DEBUG | SYMOPT_LOAD_ANYTHING);
    }

    FunctionResolver::~FunctionResolver()
    {
    }

    ResolvedType FunctionResolver::GetMethodReturnTypeFromAddress(const ULONG64& funcAddress)
    {
        SymbolInfoHelper* pSymbolInfoHelper = new SymbolInfoHelper();
        //PSYMBOL_INFO symbol = pSymbolInfoHelper->AllocSymbol(256);

        const size_t array_size = 256;
        const size_t size = sizeof(SYMBOL_INFO) + (array_size - 1) * sizeof(TCHAR);
        SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(1, size);
        if (!symbol)
        {
            //deal with it
        }
        symbol->SizeOfStruct = sizeof(*symbol);  //both values must
        symbol->MaxNameLen = array_size;           //be set by user

        ResolvedType resolvedType;

        DWORD64  dwDisplacement = 0;
        if (SymFromAddr(this->m_hProcess, funcAddress, &dwDisplacement, symbol) == FALSE)
        {
            return resolvedType;
        }

        resolvedType = this->ResolveReturnType(symbol->ModBase, symbol->TypeIndex);

        delete pSymbolInfoHelper;
        pSymbolInfoHelper = NULL;

        return resolvedType;
    }

    std::string FunctionResolver::GetMethodSymbolFromAddress(const ULONG64& funcAddress)
    {
        SymbolInfoHelper* pSymbolInfoHelper = new SymbolInfoHelper();
        //PSYMBOL_INFO symbol = pSymbolInfoHelper->AllocSymbol(256);

        const size_t array_size = 256;
        const size_t size = sizeof(SYMBOL_INFO) + (array_size - 1) * sizeof(TCHAR);
        SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(1, size);
        if (!symbol)
        {
            //deal with it
        }
        symbol->SizeOfStruct = sizeof(*symbol);  //both values must
        symbol->MaxNameLen = array_size;           //be set by user

        DWORD64  dwDisplacement = 0;
        if (SymFromAddr(this->m_hProcess, funcAddress, &dwDisplacement, symbol) == FALSE)
        {
            return "";
        }

        std::string functionSymName(symbol->Name, symbol->NameLen);

        delete pSymbolInfoHelper;
        pSymbolInfoHelper = NULL;

        return functionSymName;
    }

    void FunctionResolver::Resolve(const ULONG64& modBase, const ULONG& typeIndex, Function& resolvedFunction)
    {
        // Get function's memory address.
        ULONG64 functionAddress = 0;
        if (SymGetTypeInfo(this->m_hProcess, modBase, typeIndex, TI_GET_ADDRESS, &functionAddress) == FALSE)
        {
            // Failed to get function address.
            return;
        }

        // Get function's symbol name.
        LPWSTR functionSymName;
        if (SymGetTypeInfo(this->m_hProcess, modBase, typeIndex, TI_GET_SYMNAME, &functionSymName) == FALSE)
        {
            // Failed to get symbol name;
            return;
        }

        // Get function's return type.
        ResolvedType returnType = this->ResolveReturnType(modBase, typeIndex);

        // Get function's parameters.
        std::vector<FunctionParameter> parameters;
        this->ResolveParameters(functionAddress, modBase, typeIndex, parameters);

        resolvedFunction.Name = Utility::W2M(functionSymName);

        size_t scopeIdentifierIndex = resolvedFunction.Name.find("::");
        if (scopeIdentifierIndex != std::string::npos)
        {
            resolvedFunction.RawName = resolvedFunction.Name.substr(scopeIdentifierIndex + 2);
        }
        else
        {
            resolvedFunction.RawName = resolvedFunction.Name;
        }

        resolvedFunction.Parameters = parameters;
        resolvedFunction.ReturnType = returnType;
        resolvedFunction.Address = functionAddress;
    }

    ResolvedType FunctionResolver::ResolveReturnType(const ULONG64& modBase, const ULONG& typeIndex)
    {
        // Let's resolve return type.

        // First, let's find the function return type index.
        ULONG functionReturnTypeIndex = 0;
        SymGetTypeInfo(this->m_hProcess, modBase, typeIndex, TI_GET_TYPEID, &functionReturnTypeIndex);

        // We got the return type index, now resolve it as string.
        ResolvedType returnType;
        this->LoadType(modBase, functionReturnTypeIndex, returnType);

        return returnType;
    }

    void FunctionResolver::ResolveParameters(const ULONG64& functionAddress, const ULONG64& modBase, const ULONG& typeIndex, std::vector<FunctionParameter>& resolvedParameters)
    {
        IMAGEHLP_STACK_FRAME sf;

        sf.InstructionOffset = functionAddress;

        if (!SymSetContext(this->m_hProcess, &sf, 0))
        {
            throw;
        }

        // Retrieve all parameters of this function.
        SymEnumSymbols(this->m_hProcess, NULL, NULL, EnumParamsCallback, &resolvedParameters);
    }

    void FunctionResolver::LoadBasicType(BasicType bt, ULONG64 byteSize, ResolvedType& resolvedType)
    {
        // To represent what kind of object we have i use a VARIANT which
        // can hold a lot of different types.
        switch (bt)
        {
        case btNoType:
            resolvedType.Name = "Unknown";
            m_value.vt = VT_UNKNOWN;
            break;
        case btVoid:
            resolvedType.Name = "void";
            m_value.vt = VT_UNKNOWN; // void cannot be evaluated so we set it as VT_UNKNOWN
            break;
        case btChar:
            m_value.vt = VT_I1 | 0x1000; // its a 'special' 1 byte integer, so we add 0x1000
            resolvedType.Name = "char";
            break;
        case btWChar:
            m_value.vt = VT_I1 | 0x8000; // its actually not a 1 byte integer, so we add 0x8000
            resolvedType.Name = "wchar_t";
            break;
        case btInt:
        {
            resolvedType.Name = "signed __int32";
            m_value.vt = VT_I4; // btInt is set for every type of integer. We need to determine the real type by the byte size
            switch (byteSize)
            {
            case sizeof(__int8) :
                resolvedType.Name = "signed __int8";
                m_value.vt = VT_I1;
                break;
            case sizeof(__int16) :
                resolvedType.Name = "signed __int16";
                m_value.vt = VT_I2;
                break;
            case sizeof(__int32) :
                resolvedType.Name = "signed __int32";
                m_value.vt = VT_I4;
                break;
            case sizeof(__int64) :
                resolvedType.Name = "signed __int64";
                m_value.vt = VT_I8;
                break;
            }
            break;
        }
        case btUInt:
        {
            resolvedType.Name = "unsigned __int32";
            m_value.vt = VT_UI4;
            switch (byteSize)
            {
            case sizeof(__int8) :
                resolvedType.Name = "unsigned __int8";
                m_value.vt = VT_UI1;
                break;
            case sizeof(__int16) :
                resolvedType.Name = "unsigned __int16";
                m_value.vt = VT_UI2;
                break;
            case sizeof(__int32) :
                resolvedType.Name = "unsigned __int32";
                m_value.vt = VT_UI4;
                break;
            case sizeof(__int64) :
                resolvedType.Name = "unsigned __int64";
                m_value.vt = VT_UI8;
                break;
            }
            break;
        }
        case btFloat:
            resolvedType.Name = "float";
            m_value.vt = VT_R4;
            break;
        case btBCD:
            resolvedType.Name = "BCD";
            m_value.vt = VT_UNKNOWN;
            break;
        case btBool:
            resolvedType.Name = "bool";
            m_value.vt = VT_BOOL;
            break;
        case btLong:
            resolvedType.Name = "long int";
            m_value.vt = VT_I4;
            break;
        case btULong:
            resolvedType.Name = "unsigned long int";
            m_value.vt = VT_UI4;
            break;
        case btCurrency:
            resolvedType.Name = "currency";
            m_value.vt = VT_UNKNOWN;
            break;
        case btDate:
            resolvedType.Name = "DATE";
            m_value.vt = VT_UNKNOWN;
            break;
        case btComplex:
            resolvedType.Name = "complex";
            m_value.vt = VT_UNKNOWN;
            break;
        case btVariant:
            resolvedType.Name = "VARIANT";
            m_value.vt = VT_UNKNOWN;
            break;
        case btBit:
            resolvedType.Name = "bit";
            m_value.vt = VT_UNKNOWN;
            break;
        case btBSTR:
            resolvedType.Name = "OLESTR";
            m_value.vt = VT_BSTR;
            break;
        case btHresult:
            resolvedType.Name = "HRESULT";
            m_value.vt = VT_BLOB;
            break;

        default:
            resolvedType.Name = "Unknown";
            m_value.vt = VT_UNKNOWN;
            break;
        }
    }

    void FunctionResolver::LoadType(ULONG64 modBase, ULONG typeIndex, ResolvedType& resolvedType)
    {
        // Sadly loading the actual name of the type must be done in several different
        // ways for different types. To see which one we need to follow we first take
        // the tag of the symbol.

        std::string typeName = "Unknown";
        enum SymTagEnum tag = (enum SymTagEnum)0;
        SymGetTypeInfo(m_hProcess, modBase, typeIndex, TI_GET_SYMTAG, &tag);
        if (tag == SymTagFunctionType)
        {
            ULONG subType1;
            SymGetTypeInfo(this->m_hProcess, modBase, typeIndex, TI_GET_TYPE, &subType1);

            SymGetTypeInfo(m_hProcess, modBase, subType1, TI_GET_SYMTAG, &tag);
        }

        switch (tag)
        {
            // Base types are types like int, float, void, char, ...
            // They are not stored directly in the PDB because there aren't any big information
            // that need to be stored for them. We can use TI_GET_BASETYPE to fill a BasicType
            // which will describe the basic type and were done!
        case SymTagBaseType:
        {
            BasicType bt = (BasicType)0;
            SymGetTypeInfo(m_hProcess, modBase, typeIndex, TI_GET_BASETYPE, &bt);
            ULONG64 length = 0;
            SymGetTypeInfo(m_hProcess, modBase, typeIndex, TI_GET_LENGTH, &length);

            resolvedType.SymbolTag = SymTagBaseType;
            LoadBasicType(bt, length, resolvedType);

            return;
        }

        // For pointer types we just need to take the type it points to and then load that type
        case SymTagPointerType:
        {
            ULONG subType;
            SymGetTypeInfo(m_hProcess, modBase, typeIndex, TI_GET_TYPE, &subType);

            resolvedType.SymbolTag = SymTagPointerType;
            LoadPointerType(modBase, typeIndex, subType, resolvedType);
            // We need to override the possible type set by LoadPointerType because it should
            // only take an address when we evaluate it.
            m_value.vt = VT_BLOB;
            return;
        }

        // Now most of the rest can be simply parsed by getting the SYMNAME because they are complex
        // types and therefore have an entry in the PDB.
        default:
        {
            // we don't want to evaluate complex types
            m_value.vt = VT_UNKNOWN;
            LPWSTR symName = NULL;
            BOOL ret = SymGetTypeInfo(m_hProcess, modBase, typeIndex, TI_GET_SYMNAME, &symName);
            if (ret == TRUE)
            {
                std::vector<char> ansi(wcslen(symName) + 1);
                WideCharToMultiByte(CP_ACP, 0, symName, ansi.size(), &ansi[0], ansi.size(), NULL, NULL);
                resolvedType.Name = &ansi[0];
                resolvedType.SymbolTag = tag;
                LocalFree(symName);
            }
            break;
        }
        }
    }

    void FunctionResolver::LoadPointerType(ULONG64 modBase, ULONG typeIndex, ULONG type, ResolvedType& resolvedType)
    {
        // This function does exactly the same as LoadType does except
        // that it adds a * to the type name
        enum SymTagEnum tag = (enum SymTagEnum)0;
        SymGetTypeInfo(m_hProcess, modBase, type, TI_GET_SYMTAG, &tag);
        switch (tag)
        {
        case SymTagBaseType:
        {
            BasicType bt = (BasicType)0;
            SymGetTypeInfo(m_hProcess, modBase, type, TI_GET_BASETYPE, &bt);
            ULONG64 length = 0;
            SymGetTypeInfo(m_hProcess, modBase, type, TI_GET_LENGTH, &length);
            LoadBasicType(bt, length, resolvedType);
            resolvedType.Name += '*';

            return;
        }
        case SymTagPointerType:
        {
            ULONG subType;
            SymGetTypeInfo(m_hProcess, modBase, type, TI_GET_TYPE, &subType);
            // We recursively call ourselfs until we dont have a PointerType anymore
            // for example in char*** we need to call it 3 times
            LoadPointerType(modBase, typeIndex, subType, resolvedType);
            resolvedType.Name += '*';

            return;
        }
        case SymTagUDT:
        {
            // TODO: Resolve user defined type.

            return;
        }

        default:
        {
            LPWSTR symName = NULL;
            BOOL ret = SymGetTypeInfo(m_hProcess, modBase, typeIndex, TI_GET_SYMNAME, &symName);
            if (ret == TRUE)
            {
                std::vector<char> ansi(wcslen(symName) + 1);
                WideCharToMultiByte(CP_ACP, 0, symName, ansi.size(), &ansi[0], ansi.size(), NULL, NULL);
                resolvedType.Name = &ansi[0];
                resolvedType.Name += '*';

                LocalFree(symName);
            }
            break;
        }
        }
    }
}