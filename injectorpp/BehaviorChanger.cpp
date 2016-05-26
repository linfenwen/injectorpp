#include "BehaviorChanger.h"
#include <iostream>

namespace InjectorPP
{
    BehaviorChanger::BehaviorChanger()
    {
    }

    BehaviorChanger::~BehaviorChanger()
    {
    }

    // A magic function to change the function behavior at runtime
    //
    // funcAddress - The address of the function to be changed from.
    // expectedReturnValue - The return value should be changed to.
    void BehaviorChanger::ChangeFunctionReturnValue(ULONG64 funcAddress, int expectedReturnValue)
    {
        // The purpose of this method is to change the return value
        // to what ever int value we expected.

        // Therefore, we just need to inject below asm to the header of specific function:
        //
        // mov eax, expectedValue
        // ret
        //
        // Above asm code tells the function to return expectedValue immediately.

        // Now let's prepare the asm command.
        byte asmCommand[6];
        
        // mov
        asmCommand[0] = 0xB8;

        // The value.
        asmCommand[1] = expectedReturnValue & 0xFF;
        asmCommand[2] = (expectedReturnValue >> 8) & 0xFF;
        asmCommand[3] = (expectedReturnValue >> 16) & 0xFF;
        asmCommand[4] = (expectedReturnValue >> 24) & 0xFF;

        // ret
        asmCommand[5] = 0xC3;

        WriteProcessMemory((HANDLE)-1, (void*)funcAddress, asmCommand, 6, 0);
    }

    void BehaviorChanger::ChangeFunctionReturnValue(ULONG64 funcAddress, const char* expectedReturnValue)
    {
        // TODO:

        /*char* tt = new char[4];
        tt[0] = 'h';
        tt[1] = 'j';
        tt[2] = 'k';
        tt[3] = '\0';*/

        /*void* shellcodeStart;
        void* shellcodeEnd;
        __asm {
            mov shellcodeStart, offset shellcode_start
                mov shellcodeEnd, offset shellcode_end
                jmp shellcode_end
                shellcode_start :
            mov eax, dword ptr[tt]
                ret
                shellcode_end :
        }
        size_t cb = (ULONG)shellcodeEnd - (ULONG)shellcodeStart;

        byte* start = (byte*)shellcodeStart;
        byte* end = (byte*)shellcodeEnd;

        for (; start <= shellcodeEnd; start++)
        {
            printf("%02x \n", *start);
        }*/

        /*byte asmCommand[5];
        asmCommand[0] = 0x8B;
        asmCommand[1] = 0x86;
        asmCommand[2] = 0x43;
        asmCommand[3] = 0x08;
        asmCommand[4] = 0x8B;

        WriteProcessMemory((HANDLE)-1, (void*)funcAddress, asmCommand, 5, 0);*/
    }
}