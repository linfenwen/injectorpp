#include <vector>
#include <stdexcept>

#include "windows/x64windowsbehaviorchanger.h"

using std::vector;

namespace InjectorPP
{
    X64WindowsBehaviorChanger::X64WindowsBehaviorChanger()
    {
    }

    X64WindowsBehaviorChanger::~X64WindowsBehaviorChanger()
    {
    }

    void X64WindowsBehaviorChanger::generateToInjectAsmCode(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, FunctionType functionType, FunctionReturnType returnType, std::vector<byte>& asmCode)
    {
        // 1. Function return basic type:
        // 000000013FBC79E0 48 89 4C 24 08       mov         qword ptr [rsp+8],rcx  
        // 000000013FBC79E5 57                   push        rdi  
        // 000000013FBC79E6 48 83 EC 20          sub         rsp,20h  
        // 000000013FBC79EA 48 8B FC             mov         rdi,rsp  
        // 000000013FBC79ED B9 08 00 00 00       mov         ecx,8  
        // 000000013FBC79F2 B8 CC CC CC CC       mov         eax,0CCCCCCCCh  
        // 000000013FBC79F7 F3 AB                rep stos    dword ptr [rdi]  
        // 000000013FBC79F9 48 8B 4C 24 30       mov         rcx,qword ptr [this]  
        //    return targetFunction();
        // 000000013FBC79FE E8 5D 03 00 00       call        targetFunction (013FBC7D60h)  
        // }
        // 000000013FBC7A03 48 83 C4 20          add         rsp,20h  
        // 000000013FBC7A07 5F                   pop         rdi  
        // 000000013FBC7A08 C3   		         ret

        vector<byte> asmCommand;

        // 48 89 4C 24 08       mov         qword ptr [rsp+8],rcx
        asmCommand.push_back(0x48);
        asmCommand.push_back(0x89);
        asmCommand.push_back(0x4C);
        asmCommand.push_back(0x24);
        asmCommand.push_back(0x08);

        // 57                   push        rdi 
        asmCommand.push_back(0x57);

        // 48 83 EC 20          sub         rsp,20h
        asmCommand.push_back(0x48);
        asmCommand.push_back(0x83);
        asmCommand.push_back(0xEC);
        asmCommand.push_back(0x20);

        // 48 8B FC             mov         rdi,rsp
        asmCommand.push_back(0x48);
        asmCommand.push_back(0x8B);
        asmCommand.push_back(0xFC);

        // B9 08 00 00 00       mov         ecx,8
        asmCommand.push_back(0xB9);
        asmCommand.push_back(0x08);
        asmCommand.push_back(0x00);
        asmCommand.push_back(0x00);
        asmCommand.push_back(0x00);

        // F3 AB                rep stos    dword ptr [rdi]
        asmCommand.push_back(0xF3);
        asmCommand.push_back(0xAB);

        // 48 8B 4C 24 30       mov         rcx,qword ptr [this]
        asmCommand.push_back(0x48);
        asmCommand.push_back(0x8B);
        asmCommand.push_back(0x4C);
        asmCommand.push_back(0x24);
        asmCommand.push_back(0x30);

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

        // 48 83 C4 20          add         rsp,20h
        asmCommand.push_back(0x48);
        asmCommand.push_back(0x83);
        asmCommand.push_back(0xC4);
        asmCommand.push_back(0x20);

        // 5F                   pop         rdi
        asmCommand.push_back(0x5F);

        // C3                   ret
        asmCommand.push_back(0xC3);

        asmCode = asmCommand;
    }
}