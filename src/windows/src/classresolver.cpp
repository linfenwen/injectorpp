#include "windows/classresolver.h"

namespace InjectorPP
{
    ClassResolver::ClassResolver(HANDLE processHandle, IFunctionResolver* functionResolver)
        :m_hProcess(processHandle),
        m_symbolInfoHelper(NULL),
        m_functionResolver(NULL)
    {
        this->m_symbolInfoHelper = new SymbolInfoHelper();
        this->m_functionResolver = functionResolver;
    }

    ClassResolver::~ClassResolver()
    {
        if (this->m_symbolInfoHelper != NULL)
        {
            delete this->m_symbolInfoHelper;
            this->m_symbolInfoHelper = NULL;
        }

        if (this->m_functionResolver != NULL)
        {
            delete this->m_functionResolver;
            this->m_functionResolver = NULL;
        }
    }

    void ClassResolver::ResolveMethods(const ULONG64& modBase, const std::string className, std::vector<Function>& resolvedMethods)
    {
        PSYMBOL_INFO classSymbol = this->m_symbolInfoHelper->AllocSymbol();

        // Retrieve class symbol.
        if (SymGetTypeFromName(this->m_hProcess, modBase, className.c_str(), classSymbol) == FALSE)
        {
            throw;
        }

        // Get children of class - which are methods.
        DWORD numChildren = 0;
        if (SymGetTypeInfo(this->m_hProcess, classSymbol->ModBase, classSymbol->TypeIndex, TI_GET_CHILDRENCOUNT, &numChildren) == FALSE)
        {
            throw;
        }

        // We are responsible for allocating enough space to hold numChildren values.
        TI_FINDCHILDREN_PARAMS* methods = (TI_FINDCHILDREN_PARAMS*)new char[sizeof(TI_FINDCHILDREN_PARAMS) + numChildren * sizeof(ULONG)];
        methods->Count = numChildren;
        methods->Start = 0;

        // Get methods info.
        if (SymGetTypeInfo(this->m_hProcess, classSymbol->ModBase, classSymbol->TypeIndex, TI_FINDCHILDREN, methods) == FALSE)
        {
            throw;
        }

        // Retrieve all methods.
        for (DWORD i = 0; i < numChildren; ++i)
        {
            ULONG curChild = methods->ChildId[i];

            DWORD tag = 0;
            if (SymGetTypeInfo(this->m_hProcess, classSymbol->ModBase, curChild, TI_GET_SYMTAG, &tag) == FALSE)
            {
                throw;
            }

            if (tag != SymTagFunction)
            {
                // We do not process member varaibles.
                continue;
            }

            // Resolve function.
            Function resolvedFunction;
            this->m_functionResolver->Resolve(classSymbol->ModBase, curChild, resolvedFunction);

            // Add the resolved function to the output.
            resolvedMethods.push_back(resolvedFunction);
        }

        // Now we are responsible for releasing the memory.
        if (methods != NULL)
        {
            delete methods;
            methods = NULL;
        }
    }
}