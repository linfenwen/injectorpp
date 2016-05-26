#include "InjectorCore.h"
#include <vector>
#include "FunctionResolver.h"
#include "FakeFunctions.h"

namespace InjectorPP
{
    InjectorCore::InjectorCore()
        :m_behaviorChanger(NULL)
    {
        this->m_behaviorChanger = new BehaviorChanger();
    }

    InjectorCore::~InjectorCore()
    {
        // Release all allocated type instances.
        std::vector<void*>::iterator it;
        for (it = this->m_allocatedTypeInstances.begin(); it != this->m_allocatedTypeInstances.end(); ++it)
        {
            if (*it != NULL)
            {
                delete *it;
                *it = NULL;
            }
        }

        // Release all allocated symbol info.
        std::vector<PSYMBOL_INFO>::iterator symbolInfoIterator;
        for (symbolInfoIterator = this->m_allocatedSymbolInfos.begin();
        symbolInfoIterator != this->m_allocatedSymbolInfos.end(); ++symbolInfoIterator)
        {
            if (*symbolInfoIterator != NULL)
            {
                delete *symbolInfoIterator;
                *symbolInfoIterator = NULL;
            }
        }

        if (this->m_behaviorChanger != NULL)
        {
            delete m_behaviorChanger;
            m_behaviorChanger = NULL;
        }
    }

    void* InjectorCore::Fake(const char* typeName, size_t typeSize)
    {
        void* typeInstance = nullptr;

        HANDLE hProcess = GetCurrentProcess();
        BOOL ret = SymInitialize(hProcess, NULL, TRUE);

        PSYMBOL_INFO sym = this->AllocSymbol(MAX_SYM_NAME);

        HMODULE module = GetModuleHandle(0);

        std::string typeNameString(typeName);
        std::string classSpecifier("class ");
        std::string startStr = typeNameString.substr(0, classSpecifier.length());
        if (startStr == classSpecifier)
        {
            // It's class type. Let's extract class name.
            std::string className = typeNameString.substr(6);

            if (SymGetTypeFromName(hProcess, (ULONG64)module, className.c_str(), sym) == FALSE)
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

                std::string fakeFuncName;
                if (returnType == "std::basic_string<char,std::char_traits<char>,std::allocator<char> >")
                {
                    fakeFuncName = "FakeReturnStringFunc";
                }
                else if (returnType == "signed __int32")
                {
                    fakeFuncName = "FakeReturnIntFunc";
                }
                else if (returnType == "char*")
                {
                    fakeFuncName = "FakeReturnCCharFunc";
                }

                if (fakeFuncName.empty())
                {
                    continue;
                }

                char* ddd = FakeReturnCCharFunc();

                if (fakeFuncName == "FakeReturnIntFunc")
                {
                    this->m_behaviorChanger->ChangeFunctionReturnValue(methodAddress, 0);
                }
                else if (fakeFuncName == "FakeReturnCCharFunc")
                {
                    this->m_behaviorChanger->ChangeFunctionReturnValue(methodAddress, "");
                }
                else if (fakeFuncName == "FakeReturnStringFunc")
                {
                    this->m_behaviorChanger->ChangeFunctionReturnValue(methodAddress, "");
                }
            }
        }
        else
        {
            // TODO: Add other type support.
        }

        typeInstance = malloc(typeSize);
        memset(typeInstance, 0, typeSize);

        // Store the new instance. We'll delete it in the de-constructor.
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

        // Store the new symbol info poitner. We'll delete it in the de-constructor.
        this->m_allocatedSymbolInfos.push_back(sym);

        return sym;
    }
}
