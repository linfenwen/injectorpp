#include <stdexcept>
#include "windows/behaviorchangerfactory.h"
#ifdef WINENVIRONMENT32
#include "windows/x86windowsbehaviorchanger.h"
#else
#include "windows/x64windowsbehaviorchanger.h"
#endif

namespace InjectorPP
{
    BehaviorChangerFactory::BehaviorChangerFactory()
    {
    }

    BehaviorChangerFactory::~BehaviorChangerFactory()
    {
    }

    BehaviorChanger* BehaviorChangerFactory::create()
    {
#ifdef WINENVIRONMENT32
        return new X86WindowsBehaviorChanger();
#else
        return new X64WindowsBehaviorChanger();
#endif
    }
}