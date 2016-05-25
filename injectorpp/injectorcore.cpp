#include "InjectorCore.h"
#include <vector>
#include "FunctionResolver.h"

int FakeReturnIntFunc()
{
    return 0;
}

namespace InjectorPP
{
    InjectorCore::InjectorCore()
    {
    }

    InjectorCore::~InjectorCore()
    {
        std::vector<void*>::iterator it;
        for (it = this->m_allocatedTypeInstances.begin(); it != this->m_allocatedTypeInstances.end(); ++it)
        {
            if (*it != NULL)
            {
                delete *it;
                *it = NULL;
            }
        }
    }

    // A magic function to change the function behavior at runtime
    //
    // srcFunc - The address of the function to changed from.
    // targetFunc - The address of the destination function.
    // hookHandle - The inter-mid result during margic changing.
    void InjectorCore::HookFunc(ULONG64 srcFunc, ULONG64 targetFunc, HOOKHANDLE *hookHandle)
    {
        hookHandle->addr = srcFunc;

        // jmp
        hookHandle->jmp[0] = 0xE9;

        // ret
        hookHandle->jmp[5] = 0xC3;

        // Read original opcode from source function memory.
        ReadProcessMemory((HANDLE)-1, (void*)hookHandle->addr, hookHandle->origcode, 6, 0);

        // Calculate the offset address of target function.
        ULONG64 calc = targetFunc - hookHandle->addr - 5;

        // Fill the offset address for jmp command.
        memcpy(&hookHandle->jmp[1], &calc, 4);

        // Write the 'jmp targetFunctionAddress; ret' command to the original
        // function address.
        WriteProcessMemory((HANDLE)-1, (void*)hookHandle->addr, hookHandle->jmp, 6, 0);
    }

    void* InjectorCore::Fake(const char* typeName, size_t typeSize)
    {
        void* typeInstance = nullptr;

        HANDLE hProcess = GetCurrentProcess();
        BOOL ret = SymInitialize(hProcess, NULL, TRUE);

        PSYMBOL_INFO sym = this->AllocSymbol(MAX_SYM_NAME);

        std::vector<std::string> variables;

        HMODULE module = GetModuleHandle(0);

        if (SymGetTypeFromName(hProcess, (ULONG64)module, "Address", sym) == FALSE)
        {
            //std::string err = GetLastErrorStdStr();
            throw;
        }

        DWORD numChildren = 0;
        if (SymGetTypeInfo(hProcess, sym->ModBase, sym->TypeIndex, TI_GET_CHILDRENCOUNT, &numChildren) == FALSE)
        {
            //std::string err = GetLastErrorStdStr();
            throw;
        }

        // we are responsible for allocating enough space to hold numChildren values
        TI_FINDCHILDREN_PARAMS* methods = (TI_FINDCHILDREN_PARAMS*)new char[sizeof(TI_FINDCHILDREN_PARAMS) + numChildren * sizeof(ULONG)];
        methods->Count = numChildren;
        methods->Start = 0;

        if (SymGetTypeInfo(hProcess, sym->ModBase, sym->TypeIndex, TI_FINDCHILDREN, methods) == FALSE)
        {
            throw;
        }

        // Retrieve all methods.
        for (DWORD i = 0; i < numChildren; ++i)
        {
            ULONG curChild = methods->ChildId[i];

            LPWSTR methodSymName = NULL;

            // Let's get method's signature.
            if (SymGetTypeInfo(hProcess, sym->ModBase, curChild, TI_GET_SYMNAME, &methodSymName) == FALSE)
            {
                throw;
            }

            // Get method's address.
            ULONG64 methodAddress = 0;
            if (SymGetTypeInfo(hProcess, sym->ModBase, curChild, TI_GET_ADDRESS, &methodAddress) == FALSE)
            {
                throw;
            }

            FunctionResolver funcResolver(hProcess);
            std::string returnType = funcResolver.ResolveReturnType(sym->ModBase, curChild);

            if (returnType == "signed __int32")
            {
                PSYMBOL_INFO funcSym = this->AllocSymbol(MAX_SYM_NAME);
                if (SymGetTypeFromName(hProcess, (ULONG64)module, "FakeReturnIntFunc", funcSym) == FALSE)
                {
                    if (funcSym != NULL)
                    {
                        delete funcSym;
                        funcSym = NULL;
                    }

                    throw;
                }

                // intfunc's address.
                ULONG64 funcSymAddress = funcSym->Address;
                if (funcSym != NULL)
                {
                    delete funcSym;
                    funcSym = NULL;
                }

                HOOKHANDLE mhh;
                this->HookFunc(methodAddress, funcSymAddress, &mhh);
            }
        }

        typeInstance = malloc(typeSize);
        memset(typeInstance, 0, typeSize);

        // Store the new instance. We'll delete it in de-constructor.
        this->m_allocatedTypeInstances.push_back(typeInstance);

        return typeInstance;
    }

    PSYMBOL_INFO InjectorCore::AllocSymbol(int nameLen)
    {
        void* space = malloc(sizeof(SYMBOL_INFO) + nameLen);
        memset(space, 0, sizeof(SYMBOL_INFO) + nameLen);
        PSYMBOL_INFO sym = reinterpret_cast<PSYMBOL_INFO>(space);
        sym->NameLen = nameLen;
        // SizeOfStruct is ment to point at the actual size of the struct and not
        // of the whole memory allocated
        sym->SizeOfStruct = sizeof(SYMBOL_INFO);

        return sym;
    }
}
