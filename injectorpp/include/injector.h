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
        Injector()
            :m_injectorCore(NULL)
        {
            this->m_injectorCore = new InjectorCore();
            this->m_injectorCore->Initialize();
        }

        virtual ~Injector()
        {
            if (this->m_injectorCore != NULL)
            {
                delete this->m_injectorCore;
                this->m_injectorCore = NULL;
            }
        }

        Injector& WhenCalled(void* srcMockFunc)
        {
            this->m_whenCalledFunc.push(srcMockFunc);
            return *this;
        }

        Injector& WhenCalledVirtualMethod(void* classInstance, const std::string& virtualMethodName)
        {
            void* virtualFunction = this->m_injectorCore->GetVirtualMethodAddress(classInstance, virtualMethodName);
            this->m_whenCalledFunc.push(virtualFunction);

            return *this;
        }

        Injector& WillExecute(void* destMockFunc)
        {
            if (this->m_whenCalledFunc.empty())
            {
                throw;
            }

            if (this->m_whenCalledFunc.size() > 1)
            {
                throw;
            }

            void* srcMockFunc = this->m_whenCalledFunc.top();
            this->m_whenCalledFunc.pop();

            this->m_injectorCore->ReplaceFunction(srcMockFunc, destMockFunc);

            return *this;
        }

        template <typename T>
        T* Fake(bool autoFillDefaultValue = true)
        {
            return static_cast<T*>(this->m_injectorCore->Fake(typeid(T).name(), sizeof(T), autoFillDefaultValue));
        }

        /*void Return(int expectedReturnValue)
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
        }*/

        template<typename outtype, typename intype>
        static outtype ForceCast(intype in)
        {
            union
            {
                intype  in;
                outtype out;
            }
            u = { in };

            return u.out;
        }

    private:
        // Disable copy constructor.
        // To adapt C++0x, we do not specific "=delete" here.
        Injector(const Injector&);

        InjectorCore* m_injectorCore;

        std::stack<void*> m_whenCalledFunc;
    };

#define INJECTORPP_MEMBER_FUNCTION(fullyQualifiedFunctionName) \
InjectorPP::Injector::ForceCast<void*>(&fullyQualifiedFunctionName)
}

#endif
