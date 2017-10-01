#ifndef INJECTORPP_BEHAVIORCHANGERFACTORY_H
#define INJECTORPP_BEHAVIORCHANGERFACTORY_H

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define WINENVIRONMENT64
#else
#define WINENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define GCCENVIRONMENT64
#else
#define GCCENVIRONMENT32
#endif
#endif

#include "behaviorchanger.h"

namespace InjectorPP
{
    class BehaviorChangerFactory
    {
    public:
        static BehaviorChanger* create();

    private:
        BehaviorChangerFactory();

        ~BehaviorChangerFactory();

        BehaviorChangerFactory(const BehaviorChangerFactory&);
    };
}

#endif
