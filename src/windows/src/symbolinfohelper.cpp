#include "windows/symbolinfohelper.h"

namespace InjectorPP
{
    SymbolInfoHelper::SymbolInfoHelper()
    {
    }

    SymbolInfoHelper::~SymbolInfoHelper()
    {
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
    }

    PSYMBOL_INFO SymbolInfoHelper::AllocSymbol(int nameLen)
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