#ifndef INJECTORPP_CORE_H
#define INJECTORPP_CORE_H

#include "injectorcore.h"
#include <iostream>
#include <intrin.h>
#include <stack>

namespace InjectorPP
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
        static Injector* GetInstance()
        {
            if (m_instance == 0)
            {
                m_instance = new Injector();
            }

            return m_instance;
        }

        static void Initialize()
        {
            if (m_instance == NULL)
            {
                m_instance = new Injector();
            }
        }

        static void Uninitialize()
        {
            if (m_instance != NULL)
            {
                delete m_instance;
                m_instance = NULL;
            }
        }

        template <typename T>
        T* Fake(bool autoFillDefaultValue = true)
        {
            return static_cast<T*>(this->m_injectorCore->Fake(typeid(T).name(), sizeof(T), autoFillDefaultValue));
        }

        void PushWhenCalledFunction(const std::string& funcCallCode)
        {
            this->m_whenCalledFuncCode.push(funcCallCode);
        }
        
        void Return(int expectedReturnValue)
        {
            if (this->m_whenCalledFuncCode.empty())
            {
                throw;
            }

            std::string funcCallCode = this->m_whenCalledFuncCode.top();
            this->m_whenCalledFuncCode.pop();

            this->m_injectorCore->ChangeFunctionReturnValue(funcCallCode, expectedReturnValue);
        }

        void Return(char* expectedReturnValue)
        {
            if (this->m_whenCalledFuncCode.empty())
            {
                throw;
            }

            std::string funcCallCode = this->m_whenCalledFuncCode.top();
            this->m_whenCalledFuncCode.pop();

            this->m_injectorCore->ChangeFunctionReturnValue(funcCallCode, expectedReturnValue);
        }

        void Return(void* expectedReturnValue)
        {
            if (this->m_whenCalledFuncCode.empty())
            {
                throw;
            }

            std::string funcCallCode = this->m_whenCalledFuncCode.top();
            this->m_whenCalledFuncCode.pop();

            this->m_injectorCore->ChangeFunctionReturnValue(funcCallCode, expectedReturnValue);
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

        std::stack<std::string> m_whenCalledFuncCode;
    };

    Injector* Injector::m_instance = NULL;
}

#define INJECTORPP_SETUP() InjectorPP::Injector::Initialize()
#define INJECTORPP_CLEANUP() InjectorPP::Injector::Uninitialize()
#define INJECTORPP_FAKE InjectorPP::Injector::GetInstance()->Fake

#define WHEN_CALLED(...) \
	__INTERNAL_WHEN_CALLED(__VA_ARGS__, #__VA_ARGS__)

#define __INTERNAL_WHEN_CALLED(function, function_call_name) \
{\
__nop(); \
InjectorPP::Injector::GetInstance()->PushWhenCalledFunction(function_call_name); \
}\
(*InjectorPP::Injector::GetInstance())

#endif
