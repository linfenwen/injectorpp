#ifndef INJECTORPP_CORE_H
#define INJECTORPP_CORE_H

#include "injectorcore.h"

namespace injectorpp
{
    class Injector
    {
    public:
        ~Injector()
        {
            if (this->m_injectorCore != NULL)
            {
                delete this->m_injectorCore;
                this->m_injectorCore = NULL;
            }
        }

        // Get the single instance of Injector.
        // This method is non-thread safe.
        static Injector* getInstance()
        {
            if (m_instance == 0)
            {
                m_instance = new Injector();
            }

            return m_instance;
        }

        static void initialize()
        {
            if (m_instance == NULL)
            {
                m_instance = new Injector();
            }
        }

        static void uninitialize()
        {
            if (m_instance != NULL)
            {
                delete m_instance;
                m_instance = NULL;
            }
        }

        template <typename T>
        T* fake()
        {
            return static_cast<T*>(this->m_injectorCore->fake(typeid(T).name(), sizeof(T)));
        }
    private:
        Injector()
        {
            if (this->m_injectorCore == NULL)
            {
                this->m_injectorCore = new InjectorCore();
            }
        }

        // Disable copy constructor.
        // To adapt C++0x, we do not specific "=delete" here.
        Injector(const Injector&);

        static Injector* m_instance;

        InjectorCore* m_injectorCore;
    };

    Injector* Injector::m_instance = NULL;
}

#define INJECTORPP_SETUP() injectorpp::Injector::initialize()
#define INJECTORPP_CLEANUP() injectorpp::Injector::uninitialize()
#define INJECTORPP_FAKE injectorpp::Injector::getInstance()->fake

#endif
