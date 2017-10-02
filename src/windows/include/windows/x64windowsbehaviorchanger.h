#ifndef INJECTORPP_X64BEHAVIORCHANGER_H
#define INJECTORPP_X64BEHAVIORCHANGER_H

#include "behaviorchanger.h"

namespace InjectorPP
{
    class X64WindowsBehaviorChanger : public BehaviorChanger
    {
    public:
        X64WindowsBehaviorChanger();
        virtual ~X64WindowsBehaviorChanger();

    protected:
        virtual void generateToInjectAsmCode(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, FunctionType functionType, FunctionReturnType returnType, std::vector<byte>& asmCode);

    private:
        X64WindowsBehaviorChanger(const X64WindowsBehaviorChanger&);
        X64WindowsBehaviorChanger& operator=(const X64WindowsBehaviorChanger&);
    };
}

#endif