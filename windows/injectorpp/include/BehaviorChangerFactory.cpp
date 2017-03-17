#include <stdexcept>
#include "BehaviorChangerFactory.h"
#ifdef WINENVIRONMENT32
#include "X86WindowsBehaviorChanger.h"
#endif

namespace InjectorPP
{
    BehaviorChangerFactory::BehaviorChangerFactory()
    {
    }

    BehaviorChangerFactory::~BehaviorChangerFactory()
    {
    }

    BehaviorChanger* BehaviorChangerFactory::Create()
    {
#ifdef WINENVIRONMENT32
        return new X86WindowsBehaviorChanger();
#else
        throw std::logic_error("No supported BehaviorChanger found for current environment.");
#endif
    }
}