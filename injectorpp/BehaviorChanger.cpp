#include "BehaviorChanger.h"
#include <iostream>

namespace InjectorPP
{
    BehaviorChanger::BehaviorChanger()
    {
    }

    BehaviorChanger::~BehaviorChanger()
    {
        std::vector<char*>::iterator it;
        for (it = this->m_allocatedCharBuff.begin(); it != this->m_allocatedCharBuff.end(); ++it)
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
        // Allocate a new buff to store the expected return value.
        char* newCharBuff = new char[MAX_CHAR_BUFF_SIZE];
        strcpy_s(newCharBuff, MAX_CHAR_BUFF_SIZE, expectedReturnValue);
        
        // Store the new buff to a vector so that it can be released in the de-constructor.
        this->m_allocatedCharBuff.push_back(newCharBuff);

        // IMPORTANT: Why we need to store the address of the newly allocated buff
        // to a class data member?
        //
        // newCharBuff is a local variable, whose memory cannot be accessed outside
        // of BehaviorChanger::ChangeFunctionReturnValue. If we inject the address of
        // newCharBuff to the real function, a memory access violation will occur when
        // the real function is called.
        //
        // m_allocatedCharBuffAddress is the data member of BehaviorChanger, whose memory
        // address can be accessed outside of BehaviorChanger::ChangeFunctionReturnValue (of
        // cause, from language perspective, m_allocatedCharBuffAddress cannot be accessed
        // outside of class BehaviorChanger, but it is not from Operating System's perspective. 
        // There's no memory protection for class accesibility for the OS.
        //
        // When push_back called, m_allocatedCharBuffAddress allocate a new memory to store the
        // value of newCharBuff's address, and that new memory storage will be held and accessible
        // by any other function until BehaviorChanger class de-constructed. That's the whole purpose
        // of storing the newCharBuff's address to a vector data member of a class.
        this->m_allocatedCharBuffAddress.push_back((ULONG)newCharBuff);

        /*void* shellcodeStart;
        void* shellcodeEnd;
        __asm {
            mov shellcodeStart, offset shellcode_start
                mov shellcodeEnd, offset shellcode_end
                jmp shellcode_end
                shellcode_start :
            mov eax, [addr]
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

        byte asmCommand[6];

        // mov
        asmCommand[0] = 0xA1;

        // OK, we need to retreive the address of latest element, which holds the
        // address of newCharBuff.
        asmCommand[1] = ((ULONG)&this->m_allocatedCharBuffAddress.back()) & 0xFF;
        asmCommand[2] = (((ULONG)&this->m_allocatedCharBuffAddress.back()) >> 8) & 0xFF;
        asmCommand[3] = (((ULONG)&this->m_allocatedCharBuffAddress.back()) >> 16) & 0xFF;
        asmCommand[4] = (((ULONG)&this->m_allocatedCharBuffAddress.back()) >> 24) & 0xFF;

        // ret
        asmCommand[5] = 0xC3;

        WriteProcessMemory((HANDLE)-1, (void*)funcAddress, asmCommand, 6, 0);
    }
}