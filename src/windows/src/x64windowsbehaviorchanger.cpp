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

        if (returnType == FunctionReturnType::ComplexReturnType
            && (functionType == FunctionType::NonVirtualMemberFunction
                || functionType == FunctionType::VirtualMemberFunction))
        {
            // 48 89 54 24 10       mov         qword ptr [rsp+10h],rdx
            asmCommand.push_back(0x48);
            asmCommand.push_back(0x89);
            asmCommand.push_back(0x54);
            asmCommand.push_back(0x24);
            asmCommand.push_back(0x10);
        }

        // 48 89 4C 24 08       mov         qword ptr [rsp+8],rcx
        asmCommand.push_back(0x48);
        asmCommand.push_back(0x89);
        asmCommand.push_back(0x4C);
        asmCommand.push_back(0x24);
        asmCommand.push_back(0x08);

        // 57                   push        rdi 
        asmCommand.push_back(0x57);

        // 48 83 EC             sub         rsp,
        asmCommand.push_back(0x48);
        asmCommand.push_back(0x83);
        asmCommand.push_back(0xEC);

        if (returnType == FunctionReturnType::ComplexReturnType)
        {
            // 48 83 EC 30            sub         rsp,30h
            asmCommand.push_back(0x30);
        }
        else
        {
            // 48 83 EC 20            sub         rsp,20h
            asmCommand.push_back(0x20);
        }

        // 48 8B FC             mov         rdi,rsp
        asmCommand.push_back(0x48);
        asmCommand.push_back(0x8B);
        asmCommand.push_back(0xFC);

        asmCommand.push_back(0xB9);
        if (returnType == FunctionReturnType::ComplexReturnType)
        {
            // B9 0C 00 00 00       mov         ecx,0Ch
            asmCommand.push_back(0x0C);
        }
        else
        {
            // B9 08 00 00 00       mov         ecx,8
            asmCommand.push_back(0x08);
        }

        asmCommand.push_back(0x00);
        asmCommand.push_back(0x00);
        asmCommand.push_back(0x00);

        // B8 CC CC CC CC       mov         eax,0CCCCCCCCh
        asmCommand.push_back(0xB8);
        asmCommand.push_back(0xCC);
        asmCommand.push_back(0xCC);
        asmCommand.push_back(0xCC);
        asmCommand.push_back(0xCC);

        // F3 AB                rep stos    dword ptr [rdi]
        asmCommand.push_back(0xF3);
        asmCommand.push_back(0xAB);

        asmCommand.push_back(0x48);
        asmCommand.push_back(0x8B);
        asmCommand.push_back(0x4C);
        asmCommand.push_back(0x24);
        if (returnType == FunctionReturnType::ComplexReturnType)
        {
            // 48 8B 4C 24 40       mov         rcx,qword ptr [rsp+40h]
            asmCommand.push_back(0x40);

            // C7 44 24 20 00 00 00 00 mov         dword ptr [rsp+20h],0
            asmCommand.push_back(0xC7);
            asmCommand.push_back(0x44);
            asmCommand.push_back(0x24);
            asmCommand.push_back(0x20);
            asmCommand.push_back(0x00);
            asmCommand.push_back(0x00);
            asmCommand.push_back(0x00);
            asmCommand.push_back(0x00);

            asmCommand.push_back(0x48);
            asmCommand.push_back(0x8B);
            asmCommand.push_back(0x4C);
            asmCommand.push_back(0x24);

            if (functionType == FunctionType::NonVirtualMemberFunction
                || functionType == FunctionType::VirtualMemberFunction)
            {
                // 48 8B 4C 24 48       mov         rcx,qword ptr [rsp+48h]
                asmCommand.push_back(0x48);
            }
            else
            {
                // 48 8B 4C 24 40       mov         rcx,qword ptr [rsp+40h]
                asmCommand.push_back(0x40);
            }
        }
        else
        {
            // 48 8B 4C 24 30       mov         rcx,qword ptr [this]
            asmCommand.push_back(0x30);
        }

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

        if (returnType == FunctionReturnType::ComplexReturnType)
        {
            // 8B 44 24 20          mov         eax,dword ptr [rsp+20h]
            asmCommand.push_back(0x8B);
            asmCommand.push_back(0x44);
            asmCommand.push_back(0x24);
            asmCommand.push_back(0x20);

            // 83 C8 01             or          eax,1
            asmCommand.push_back(0x83);
            asmCommand.push_back(0xC8);
            asmCommand.push_back(0x01);

            // 89 44 24 20          mov         dword ptr [rsp+20h],eax
            asmCommand.push_back(0x89);
            asmCommand.push_back(0x44);
            asmCommand.push_back(0x24);
            asmCommand.push_back(0x20);

            asmCommand.push_back(0x48);
            asmCommand.push_back(0x8B);
            asmCommand.push_back(0x44);
            asmCommand.push_back(0x24);
            if (functionType == FunctionType::NonVirtualMemberFunction
                || functionType == FunctionType::VirtualMemberFunction)
            {
                // 48 8B 44 24 48       mov         rax,qword ptr [rsp+48h]
                asmCommand.push_back(0x48);
            }
            else
            {
                // 48 8B 44 24 40       mov         rax,qword ptr [rsp+40h]
                asmCommand.push_back(0x40);
            }
        }

        asmCommand.push_back(0x48);
        asmCommand.push_back(0x83);
        asmCommand.push_back(0xC4);
        if (returnType == FunctionReturnType::ComplexReturnType)
        {
            // 48 83 C4 30          add         rsp,30h
            asmCommand.push_back(0x30);
        }
        else
        {
            // 48 83 C4 20          add         rsp,20h
            asmCommand.push_back(0x20);
        }

        // 5F                   pop         rdi
        asmCommand.push_back(0x5F);

        // C3                   ret
        asmCommand.push_back(0xC3);

        asmCode = asmCommand;
    }
}