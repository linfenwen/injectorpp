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

    void X64WindowsBehaviorChanger::ReplaceFunction(ULONG64 sourceFuncAddress, ULONG64 targetFuncAddress, OriginalFuncASM* originalFuncAsm, FunctionType functionType, FunctionReturnType returnType)
    {
        throw std::logic_error("Not supported yet.");
    }
}