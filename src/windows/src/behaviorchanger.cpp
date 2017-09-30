#include <iostream>
#include <vector>

#include "windows/behaviorchanger.h"

namespace InjectorPP
{
    BehaviorChanger::BehaviorChanger()
    {
    }

    BehaviorChanger::~BehaviorChanger()
    {
    }

    void BehaviorChanger::DirectWriteToFunction(ULONG64 funcAddress, const byte* asmCode, size_t asmCodeSize)
    {
        WriteProcessMemory((HANDLE)-1, (void*)funcAddress, asmCode, asmCodeSize, 0);
    }
}