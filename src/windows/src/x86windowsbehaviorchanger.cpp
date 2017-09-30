#include <vector>

#include "windows/x86windowsbehaviorchanger.h"

using std::vector;

namespace InjectorPP
{
    X86WindowsBehaviorChanger::X86WindowsBehaviorChanger()
    {
    }

    X86WindowsBehaviorChanger::~X86WindowsBehaviorChanger()
    {
    }

    void X86WindowsBehaviorChanger::ReplaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm, bool isComplexReturn, bool isSourceFuncVirtualMethod)
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

        size_t preparationCodeLength = asmCommand.size();

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

        size_t injectedCodeSize = asmCommand.size();

        // First important thing, backup the original asm code.
        originalFuncAsm->asmCode = new byte[injectedCodeSize];
        originalFuncAsm->funcAddress = sourceFuncAddress;
        ReadProcessMemory((HANDLE)-1, (void*)originalFuncAsm->funcAddress, originalFuncAsm->asmCode, injectedCodeSize, 0);

        // Then inject the code to source function.
        this->DirectWriteToFunction(sourceFuncAddress, &asmCommand[0], injectedCodeSize);
    }
}