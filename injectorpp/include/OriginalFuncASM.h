#ifndef INJECTORPP_ORIGINALFUNCASM_H
#define INJECTORPP_ORIGINALFUNCASM_H

#include <Windows.h>

struct OriginalFuncASM
{
    OriginalFuncASM()
        :funcAddress(0),
        asmCode(NULL)
    {
    }

    ~OriginalFuncASM()
    {
        if (asmCode != NULL)
        {
            delete[] asmCode;
            asmCode = NULL;
        }
    }

    ULONG64 funcAddress;
    byte* asmCode;
};

#endif
