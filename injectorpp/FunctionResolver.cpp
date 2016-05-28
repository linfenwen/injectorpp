#include <Windows.h>
#include <DbgHelp.h>
#include <sstream>

#include "FunctionResolver.h"
#include "Utility.h"

namespace InjectorPP
{
    enum SymTagEnum
    {
        SymTagNull,
        SymTagExe,
        SymTagCompiland,
        SymTagCompilandDetails,
        SymTagCompilandEnv,
        SymTagFunction,
        SymTagBlock,
        SymTagData,
        SymTagAnnotation,
        SymTagLabel,
        SymTagPublicSymbol,
        SymTagUDT,
        SymTagEnum,
        SymTagFunctionType,
        SymTagPointerType,
        SymTagArrayType,
        SymTagBaseType,
        SymTagTypedef,
        SymTagBaseClass,
        SymTagFriend,
        SymTagFunctionArgType,
        SymTagFuncDebugStart,
        SymTagFuncDebugEnd,
        SymTagUsingNamespace,
        SymTagVTableShape,
        SymTagVTable,
        SymTagCustom,
        SymTagThunk,
        SymTagCustomType,
        SymTagManagedType,
        SymTagDimension,
        SymTagCallSite,
        SymTagMax
    };

    /* Extracted from cvconst.h */
    enum BasicType
    {
        btNoType = 0,
        btVoid = 1,
        btChar = 2,
        btWChar = 3,
        btInt = 6,
        btUInt = 7,
        btFloat = 8,
        btBCD = 9,
        btBool = 10,
        btLong = 13,
        btULong = 14,
        btCurrency = 25,
        btDate = 26,
        btVariant = 27,
        btComplex = 28,
        btBit = 29,
        btBSTR = 30,
        btHresult = 31,
    };

    BOOL __stdcall EnumParamsCallback(PSYMBOL_INFO inf, ULONG size, PVOID param)
    {
        // Transform the param back to the one it was before.
        std::vector<std::string>* params = (std::vector<std::string>*)param;
        if (inf == NULL)
        {
            return true;
        }

        // The flags contain various information on what type of symbol we have here
        // SYMFLAG_PARAMETER says that its a function parameter.
        if (inf->Flags & SYMFLAG_PARAMETER)
        {
            params->push_back(inf->Name);
        }

        return true;
    }

    FunctionResolver::FunctionResolver(HANDLE processHandle)
        : m_hProcess(processHandle)
    {
    }

    FunctionResolver::~FunctionResolver()
    {
    }

    void FunctionResolver::Resolve(const ULONG64& modBase, const ULONG& typeIndex, Function& resolvedFunction)
    {
        // Get function's memory address.
        ULONG64 functionAddress = 0;
        if (SymGetTypeInfo(this->m_hProcess, modBase, typeIndex, TI_GET_ADDRESS, &functionAddress) == FALSE)
        {
            throw;
        }

        // Get function's symbol name.
        LPWSTR functionSymName;
        if (SymGetTypeInfo(this->m_hProcess, modBase, typeIndex, TI_GET_SYMNAME, &functionSymName) == FALSE)
        {
            throw;
        }

        // Get function's return type.
        std::string returnType = this->ResolveReturnType(modBase, typeIndex);
        
        // Get function's parameters.
        std::vector<FunctionParameter> parameters;
        this->ResolveParameters(modBase, typeIndex, parameters);

        resolvedFunction.Name = Utility::W2M(functionSymName);
        resolvedFunction.Parameters = parameters;
        resolvedFunction.ReturnType = returnType;
        resolvedFunction.Address = functionAddress;
    }

    std::string FunctionResolver::ResolveReturnType(const ULONG64& modBase, const ULONG& typeIndex)
    {
        // Let's resolve return type.

        // First, let's find the function type index.
        ULONG functionTypeIndex = 0;
        SymGetTypeInfo(this->m_hProcess, modBase, typeIndex, TI_GET_TYPE, &functionTypeIndex);

        // Function return type is the sub type of the function.
        ULONG returnTypeIndex = 0;
        SymGetTypeInfo(this->m_hProcess, modBase, functionTypeIndex, TI_GET_TYPE, &returnTypeIndex);

        // We got the return type index, now resolve it as string.
        std::string returnTypeString;
        this->LoadType(modBase, returnTypeIndex, returnTypeString);

        return returnTypeString;
    }

    void FunctionResolver::ResolveParameters(const ULONG64& modBase, const ULONG& typeIndex, std::vector<FunctionParameter>& resolvedParameters)
    {
        // TODO:

        /*if (SymGetTypeFromName(hProcess, (ULONG64)module, "?GetCountry@Address@@QAE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ", sym) == FALSE)
        {
        std::string err = GetLastErrorStdStr();
        return p;
        }*/

        /*GetLocalVariables(variables, sym->Address);

        BasicType bt = (BasicType)1;
        SymGetTypeInfo(hProcess, sym->ModBase, sym->TypeIndex, TI_GET_BASETYPE, &bt);*/
    }

    void FunctionResolver::LoadBasicType(BasicType bt, ULONG64 byteSize, std::string& resolvedType)
    {
        // To represent what kind of object we have i use a VARIANT which
        // can hold a lot of different types.
        switch (bt)
        {
        case btNoType:
            resolvedType = "Unknown";
            m_value.vt = VT_UNKNOWN;
            break;
        case btVoid:
            resolvedType = "void";
            m_value.vt = VT_UNKNOWN; // void cannot be evaluated so we set it as VT_UNKNOWN
            break;
        case btChar:
            m_value.vt = VT_I1 | 0x1000; // its a 'special' 1 byte integer, so we add 0x1000
            resolvedType = "char";
            break;
        case btWChar:
            m_value.vt = VT_I1 | 0x8000; // its actually not a 1 byte integer, so we add 0x8000
            resolvedType = "wchar_t";
            break;
        case btInt:
        {
            resolvedType = "signed __int32";
            m_value.vt = VT_I4; // btInt is set for every type of integer. We need to determine the real type by the byte size
            switch (byteSize)
            {
            case sizeof(__int8) :
                resolvedType = "signed __int8";
                m_value.vt = VT_I1;
                break;
            case sizeof(__int16) :
                resolvedType = "signed __int16";
                m_value.vt = VT_I2;
                break;
            case sizeof(__int32) :
                resolvedType = "signed __int32";
                m_value.vt = VT_I4;
                break;
            case sizeof(__int64) :
                resolvedType = "signed __int64";
                m_value.vt = VT_I8;
                break;
            }
            break;
        }
        case btUInt:
        {
            resolvedType = "unsigned __int32";
            m_value.vt = VT_UI4;
            switch (byteSize)
            {
            case sizeof(__int8) :
                resolvedType = "unsigned __int8";
                m_value.vt = VT_UI1;
                break;
            case sizeof(__int16) :
                resolvedType = "unsigned __int16";
                m_value.vt = VT_UI2;
                break;
            case sizeof(__int32) :
                resolvedType = "unsigned __int32";
                m_value.vt = VT_UI4;
                break;
            case sizeof(__int64) :
                resolvedType = "unsigned __int64";
                m_value.vt = VT_UI8;
                break;
            }
            break;
        }
        case btFloat:
            resolvedType = "float";
            m_value.vt = VT_R4;
            break;
        case btBCD:
            resolvedType = "BCD";
            m_value.vt = VT_UNKNOWN;
            break;
        case btBool:
            resolvedType = "bool";
            m_value.vt = VT_BOOL;
            break;
        case btLong:
            resolvedType = "long int";
            m_value.vt = VT_I4;
            break;
        case btULong:
            resolvedType = "unsigned long int";
            m_value.vt = VT_UI4;
            break;
        case btCurrency:
            resolvedType = "currency";
            m_value.vt = VT_UNKNOWN;
            break;
        case btDate:
            resolvedType = "DATE";
            m_value.vt = VT_UNKNOWN;
            break;
        case btComplex:
            resolvedType = "complex";
            m_value.vt = VT_UNKNOWN;
            break;
        case btVariant:
            resolvedType = "VARIANT";
            m_value.vt = VT_UNKNOWN;
            break;
        case btBit:
            resolvedType = "bit";
            m_value.vt = VT_UNKNOWN;
            break;
        case btBSTR:
            resolvedType = "OLESTR";
            m_value.vt = VT_BSTR;
            break;
        case btHresult:
            resolvedType = "HRESULT";
            m_value.vt = VT_BLOB;
            break;

        default:
            resolvedType = "Unknown";
            m_value.vt = VT_UNKNOWN;
            break;
        }
    }

    void FunctionResolver::LoadType(ULONG64 modBase, ULONG typeIndex, std::string& resolvedType)
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
            LoadBasicType(bt, length, resolvedType);

            return;
        }

        // For pointer types we just need to take the type it points to and then load that type
        case SymTagPointerType:
        {
            ULONG subType;
            SymGetTypeInfo(m_hProcess, modBase, typeIndex, TI_GET_TYPE, &subType);
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
                resolvedType = &ansi[0];
                LocalFree(symName);
            }
            break;
        }
        }
    }

    void FunctionResolver::LoadPointerType(ULONG64 modBase, ULONG typeIndex, ULONG type, std::string& resolvedType)
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
            resolvedType += '*';
            return;
        }
        case SymTagPointerType:
        {
            ULONG subType;
            SymGetTypeInfo(m_hProcess, modBase, type, TI_GET_TYPE, &subType);
            // We recursively call ourselfs until we dont have a PointerType anymore
            // for example in char*** we need to call it 3 times
            LoadPointerType(modBase, typeIndex, subType, resolvedType);
            resolvedType += '*';

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
                resolvedType = &ansi[0];
                resolvedType += '*';
                LocalFree(symName);
            }
            break;
        }
        }
    }
}