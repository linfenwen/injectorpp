#ifndef INJECTORPP_BEHAVIORCHANGER_H
#define INJECTORPP_BEHAVIORCHANGER_H

#include <Windows.h>

namespace InjectorPP
{
    class BehaviorChanger
    {
    public:
        BehaviorChanger();
        ~BehaviorChanger();

        // A magic function to change the function behavior at runtime
        //
        // funcAddress - The address of the function to be changed from.
        // expectedReturnValue - The return value should be changed to.
        void ChangeFunctionReturnValue(ULONG64 funcAddress, int expectedReturnValue);

        void ChangeFunctionReturnValue(ULONG64 funcAddress, const char* expectedReturnValue);
    private:
        BehaviorChanger(const BehaviorChanger&);
    };
}

#endif
