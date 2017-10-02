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

    void X86WindowsBehaviorChanger::generateToInjectAsmCode(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, FunctionType functionType, FunctionReturnType returnType, std::vector<byte>& asmCode)
    {
        // The idea is hijack the original function, inject op codes to let it 
        // jump to the target function.
        // Here we need to handle several kind of functions.
        //
        // 1. Function returns basic type:
        // For instance: int BaseClassTest::GetAnInteger(); 
        // We want to change the body of this function to:
        // {
        //      return targetFunction();   
        // }
        // MSVC will generate following asm code. We just need to inject the same
        // code to original function. Note we can minimize the code from eliminating
        // 00B811D4~00B811DB and _RTC_CheckEsp.
        // 
        // 00B811D0 55                   push        ebp  
        // 00B811D1 8B EC                mov         ebp,esp  
        // 00B811D3 51                   push        ecx  
        // 00B811D4 C7 45 FC CC CC CC CC mov         dword ptr [this],0CCCCCCCCh  
        // 00B811DB 89 4D FC             mov         dword ptr [this],ecx  
        //    return targetFunction();
        // 00B811DE E8 B0 C4 FD FF       call        targetFunction (0B5D693h)  
        // }
        // 00B811E3 83 C4 04             add         esp,4  
        // 00B811E6 3B EC                cmp         ebp,esp  
        // 00B811E8 E8 6C 40 FD FF       call        __RTC_CheckEsp (0B55259h)  
        // 00B811ED 8B E5                mov         esp,ebp  
        // 00B811EF 5D                   pop         ebp  
        // 00B811F0 C3                   ret
        //
        // 2. Non-static member function return complex type.
        // 002C0220 55                   push        ebp  
        // 002C0221 8B EC                mov         ebp,esp  
        // 002C0223 83 EC 08             sub         esp,8  
        // 002C0226 C7 45 F8 CC CC CC CC mov         dword ptr [ebp-8],0CCCCCCCCh  
        // 002C022D C7 45 FC CC CC CC CC mov         dword ptr [this],0CCCCCCCCh  
        // 002C0234 89 4D FC             mov         dword ptr [this],ecx  
        // 002C0237 C7 45 F8 00 00 00 00 mov         dword ptr [ebp-8],0  
        //    return targetFunction();
        // 002C023E 8B 45 08             mov         eax,dword ptr [ebp+8]  
        // 002C0241 50                   push        eax  
        // 002C0242 E8 D4 D3 FD FF       call        targetFunction (029D61Bh)  
        // 002C0247 83 C4 04             add         esp,4  
        // 002C024A 8B 4D F8             mov         ecx,dword ptr [ebp-8]  
        // 002C024D 83 C9 01             or          ecx,1  
        // 002C0250 89 4D F8             mov         dword ptr [ebp-8],ecx  
        // 002C0253 8B 45 08             mov         eax,dword ptr [ebp+8]  
        // }
        // 002C0256 83 C4 08             add         esp,8  
        // 002C0259 3B EC                cmp         ebp,esp  
        // 002C025B E8 26 50 FD FF       call        __RTC_CheckEsp (0295286h)  
        // 002C0260 8B E5                mov         esp,ebp  
        // 002C0262 5D                   pop         ebp  
        // 002C0263 C2 04 00             ret         4  
        //
        // 3. Static member function return complex type.
        // 011B31F0 55                   push        ebp  
        // 011B31F1 8B EC                mov         ebp,esp  
        // 011B31F3 51                   push        ecx  
        // 011B31F4 C7 45 FC CC CC CC CC mov         dword ptr [ebp-4],0CCCCCCCCh  
        // 011B31FB C7 45 FC 00 00 00 00 mov         dword ptr [ebp-4],0  
        //   return targetFunction();
        // 011B3202 8B 45 08             mov         eax,dword ptr [ebp+8]  
        // 011B3205 50                   push        eax  
        // 011B3206 E8 65 00 00 00       call        targetFunction (011B3270h)  
        // 011B320B 83 C4 04             add         esp,4  
        // 011B320E 8B 4D FC             mov         ecx,dword ptr [ebp-4]  
        // 011B3211 83 C9 01             or          ecx,1  
        // 011B3214 89 4D FC             mov         dword ptr [ebp-4],ecx  
        // 011B3217 8B 45 08             mov         eax,dword ptr [ebp+8]  
        // }
        // 011B321A 83 C4 04             add         esp,4  
        // 011B321D 3B EC                cmp         ebp,esp  
        // 011B321F E8 4C 8F 0C 00       call        _RTC_CheckEsp (0127C170h)  
        // 011B3224 8B E5                mov         esp,ebp  
        // 011B3226 5D                   pop         ebp  
        // 011B3227 C3                   ret  

        vector<byte> asmCommand;

        // push ebp
        asmCommand.push_back(0x55);

        // 8B EC                mov         ebp,esp
        asmCommand.push_back(0x8B);
        asmCommand.push_back(0xEC);

        if (functionType != FunctionType::GlobalFunction && returnType == FunctionReturnType::ComplexReturnType)
        {
            // 83 EC 08             sub         esp,8
            asmCommand.push_back(0x83);
            asmCommand.push_back(0xEC);
            asmCommand.push_back(0x08);
        }
        else
        {
            // 51                   push        ecx
            asmCommand.push_back(0x51);
        }

        if (returnType == FunctionReturnType::ComplexReturnType)
        {
            // 8B 45 08	            mov eax,dword ptr [ebp+8]
            asmCommand.push_back(0x8B);
            asmCommand.push_back(0x45);
            asmCommand.push_back(0x08);

            // 0x50	                push eax
            asmCommand.push_back(0x50);
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
            // 83 C4 04				add esp,4
            asmCommand.push_back(0x83);
            asmCommand.push_back(0xC4);
            asmCommand.push_back(0x04);

            if (functionType == FunctionType::NonVirtualMemberFunction || functionType == FunctionType::VirtualMemberFunction)
            {
                // 8B 4D F8             mov         ecx, dword ptr[ebp - 8]
                asmCommand.push_back(0x8B);
                asmCommand.push_back(0x4D);
                asmCommand.push_back(0xF8);
            }
            else
            {
                // 8B 4D FC             mov         ecx,dword ptr [ebp-4]
                asmCommand.push_back(0x8B);
                asmCommand.push_back(0x4D);
                asmCommand.push_back(0xFC);
            }

            // 83 C9 01             or          ecx,1
            asmCommand.push_back(0x83);
            asmCommand.push_back(0xC9);
            asmCommand.push_back(0x01);

            if (functionType == FunctionType::NonVirtualMemberFunction || functionType == FunctionType::VirtualMemberFunction)
            {
                // 89 4D F8             mov         dword ptr [ebp-8],ecx
                asmCommand.push_back(0x89);
                asmCommand.push_back(0x4D);
                asmCommand.push_back(0xF8);
            }
            else
            {
                // 89 4D FC             mov         dword ptr [ebp-4],ecx
                asmCommand.push_back(0x89);
                asmCommand.push_back(0x4D);
                asmCommand.push_back(0xFC);
            }

            // 8B 45 08             mov         eax,dword ptr [ebp+8]
            asmCommand.push_back(0x8B);
            asmCommand.push_back(0x45);
            asmCommand.push_back(0x08);
        }

        if (returnType == FunctionReturnType::ComplexReturnType
            && (functionType == FunctionType::NonVirtualMemberFunction
                || functionType == FunctionType::VirtualMemberFunction))
        {
            // 83 C4 08             add         esp,8
            asmCommand.push_back(0x83);
            asmCommand.push_back(0xC4);
            asmCommand.push_back(0x08);
        }
        else
        {
            // 83 C4 04             add         esp,4 
            asmCommand.push_back(0x83);
            asmCommand.push_back(0xC4);
            asmCommand.push_back(0x04);
        }

        // 8B E5                mov         esp,ebp
        asmCommand.push_back(0x8B);
        asmCommand.push_back(0xE5);

        // 5D                   pop         ebp
        asmCommand.push_back(0x5D);

        // ret
        if (returnType == FunctionReturnType::ComplexReturnType
            && (functionType == FunctionType::NonVirtualMemberFunction
                || functionType == FunctionType::VirtualMemberFunction))
        {
            // C2 04 00             ret         4
            asmCommand.push_back(0xC2);
            asmCommand.push_back(0x04);
            asmCommand.push_back(0x00);
        }
        else
        {
            // C3                   ret
            asmCommand.push_back(0xC3);
        }

        asmCode = asmCommand;
    }
}