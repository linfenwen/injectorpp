#include <iostream>
#include <vector>

#include "BehaviorChanger.h"
#include "FakeFunctions.h"

using std::vector;

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
    void BehaviorChanger::ChangeFunctionReturnValue(ULONG64 funcAddress, const int& expectedReturnValue, OriginalFuncASM* originalFuncAsm)
    {
        // First important thing, backup the original asm code.
        originalFuncAsm->funcAddress = funcAddress;
        ReadProcessMemory((HANDLE)-1, (void*)originalFuncAsm->funcAddress, originalFuncAsm->asmCode, 6, 0);

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

        this->DirectWriteToFunction(funcAddress, asmCommand, 6);
    }

    void BehaviorChanger::ChangeFunctionReturnValue(ULONG64 funcAddress, const void* expectedReturnValue, OriginalFuncASM* originalFuncAsm)
    {
        // First important thing, backup the original asm code.
        originalFuncAsm->funcAddress = funcAddress;
        ReadProcessMemory((HANDLE)-1, (void*)originalFuncAsm->funcAddress, originalFuncAsm->asmCode, 6, 0);

        if (expectedReturnValue == NULL)
        {
            return;
        }

        ULONG expectedReturnValueAddress = (ULONG)expectedReturnValue;

        byte asmCommand[6];

        // mov
        asmCommand[0] = 0xB8;

        // The value.
        asmCommand[1] = expectedReturnValueAddress & 0xFF;
        asmCommand[2] = (expectedReturnValueAddress >> 8) & 0xFF;
        asmCommand[3] = (expectedReturnValueAddress >> 16) & 0xFF;
        asmCommand[4] = (expectedReturnValueAddress >> 24) & 0xFF;

        // ret
        asmCommand[5] = 0xC3;

        this->DirectWriteToFunction(funcAddress, asmCommand, 6);
    }

    //void BehaviorChanger::ChangeFunctionReturnValue(ULONG64 funcAddress, std::string expectedReturnValue)
    //{
        // Allocate a new buff to store the expected return value.
        //std::string* newString = new std::string(expectedReturnValue);

        // Store the new buff to a vector so that it can be released in the de-constructor.
        //this->m_allocatedStringBuff.push_back(newString);

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
        //this->m_allocatedStringBuffAddress.push_back((ULONG)newString);

        /*ULONG a = (ULONG)FakeReturnStringFunc;
        void* shellcodeStart;
        void* shellcodeEnd;
        __asm {
        mov shellcodeStart, offset shellcode_start
        mov shellcodeEnd, offset shellcode_end
        jmp shellcode_end
        shellcode_start :
        jmp [a]
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

        //byte asmCommand[6];

        // call
        //asmCommand[0] = 0xE9;

        // OK, we need to retreive the address of latest element, which holds the
        // address of newCharBuff.
        //ULONG fakeReturnStringAddress = (ULONG)FakeReturnStringFunc;

        //DWORD offset = fakeReturnStringAddress - (ULONG)funcAddress - 5;
        //memcpy(&asmCommand[1], &offset, 4);

        /*asmCommand[1] = ((ULONG64)&FakeReturnStringFunc) & 0xFF;
        asmCommand[2] = (((ULONG64)&FakeReturnStringFunc) >> 8) & 0xFF;
        asmCommand[3] = (((ULONG64)&FakeReturnStringFunc) >> 16) & 0xFF;
        asmCommand[4] = (((ULONG64)&FakeReturnStringFunc) >> 24) & 0xFF;*/

        // ret
        //asmCommand[5] = 0xC3;

        //WriteProcessMemory((HANDLE)-1, (void*)funcAddress, asmCommand, 6, 0);
    //}

    void BehaviorChanger::ReplaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm, bool isComplexReturn, bool isSourceFuncVirtualMethod)
    {
        //byte* asmCommand = new byte[injectedCodeSize];
        vector<byte> asmCommand;

        // push ebp
        asmCommand.push_back(0x55);

        // mov         ebp,esp
        // sub         esp,0CCh
        asmCommand.push_back(0x8B);

        asmCommand.push_back(0xEC);
        asmCommand.push_back(0x81);
        asmCommand.push_back(0xEC);
        asmCommand.push_back(0xCC);
        asmCommand.push_back(0x00);
        asmCommand.push_back(0x00);
        asmCommand.push_back(0x00);

        // push        ebx
        // push        esi
        // push        edi
        asmCommand.push_back(0x53);
        asmCommand.push_back(0x56);
        asmCommand.push_back(0x57);

        if (isComplexReturn && isSourceFuncVirtualMethod)
        {
            // mov         eax,dword ptr [ebp+8]
            asmCommand.push_back(0x8B);
            asmCommand.push_back(0x45);
            asmCommand.push_back(0x08);
        }

        // push eax
        asmCommand.push_back(0x50);

        int preparationCodeLength = asmCommand.size();

        // Calculate the offset.
        // We will inject preparation code to the begining of the source function.
        // 'E8 <offset>' requires 5 bytes (1 byte for opcode, 4 bytes for address) 
        // so the next instruction's address is the source function address + preparationCodeLength + 5.
        // The <offset> value is targetFuncAddress - <next instruction's address>.
        ULONG64 offset = targetFuncAddress - (sourceFuncAddress + preparationCodeLength + 5);


        // call <relative target function address>
        asmCommand.push_back(0xE8);
        asmCommand.push_back((offset) & 0xFF);
        asmCommand.push_back((offset >> 8) & 0xFF);
        asmCommand.push_back((offset >> 16) & 0xFF);
        asmCommand.push_back((offset >> 24) & 0xFF);

        // add esp, 4  
        // because we pushed eax before calling target function address, 
        // the esp should be pulled back for 4 bytes.
        asmCommand.push_back(0x83);
        asmCommand.push_back(0xC4);
        asmCommand.push_back(0x04);

        // pop         edi
        // pop         esi
        // pop         ebx
        // add         esp,0CCh
        // mov         esp,ebp
        // pop         ebp
        asmCommand.push_back(0x5F);
        asmCommand.push_back(0x5E);
        asmCommand.push_back(0x5B);
        asmCommand.push_back(0x81);
        asmCommand.push_back(0xC4);
        asmCommand.push_back(0xCC);
        asmCommand.push_back(0x00);
        asmCommand.push_back(0x00);
        asmCommand.push_back(0x00);
        asmCommand.push_back(0x8B);
        asmCommand.push_back(0xE5);
        asmCommand.push_back(0x5D);

        // ret
        if (isComplexReturn)
        {
            asmCommand.push_back(0xC2);
            asmCommand.push_back(0x04);
            asmCommand.push_back(0x00);
        }
        else
        {
            asmCommand.push_back(0xC3);
        }

        int injectedCodeSize = asmCommand.size();

        // First important thing, backup the original asm code.
        originalFuncAsm->asmCode = new byte[injectedCodeSize];
        originalFuncAsm->funcAddress = sourceFuncAddress;
        ReadProcessMemory((HANDLE)-1, (void*)originalFuncAsm->funcAddress, originalFuncAsm->asmCode, injectedCodeSize, 0);

        // Then inject the code to source function.
        this->DirectWriteToFunction(sourceFuncAddress, &asmCommand[0], injectedCodeSize);
    }

    void BehaviorChanger::ChangeFunctionReturnValue(ULONG64 funcAddress, const char* expectedReturnValue, OriginalFuncASM* originalFuncAsm)
    {
        // First important thing, backup the original asm code.
        originalFuncAsm->funcAddress = funcAddress;
        ReadProcessMemory((HANDLE)-1, (void*)originalFuncAsm->funcAddress, originalFuncAsm->asmCode, 6, 0);

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
        asmCommand[0] = 0xE8;

        // OK, we need to retreive the address of latest element, which holds the
        // address of newCharBuff.
        asmCommand[1] = ((ULONG)&this->m_allocatedCharBuffAddress.back()) & 0xFF;
        asmCommand[2] = (((ULONG)&this->m_allocatedCharBuffAddress.back()) >> 8) & 0xFF;
        asmCommand[3] = (((ULONG)&this->m_allocatedCharBuffAddress.back()) >> 16) & 0xFF;
        asmCommand[4] = (((ULONG)&this->m_allocatedCharBuffAddress.back()) >> 24) & 0xFF;

        // ret
        asmCommand[5] = 0xC3;

        this->DirectWriteToFunction(funcAddress, asmCommand, 6);
    }

    void BehaviorChanger::DirectWriteToFunction(ULONG64 funcAddress, const byte* asmCode, size_t asmCodeSize)
    {
        WriteProcessMemory((HANDLE)-1, (void*)funcAddress, asmCode, asmCodeSize, 0);
    }
}