#ifndef INJECTORPP_CORE_H
#define INJECTORPP_CORE_H

#include "injectorcore.h"
#include <iostream>
#include <intrin.h>
#include <stack>

namespace InjectorPP
{
    struct FunctionWrapper
    {
        void* functionAddress;
        bool isMemberFunction;
    };

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

            // Prevent memory leak.
            while (this->m_whenCalledFunc.size() > 0)
            {
                FunctionWrapper* item = this->m_whenCalledFunc.top();
                this->m_whenCalledFunc.pop();

                delete item;
                item = NULL;
            }
        }

        Injector& WhenCalled(void* srcMockFunc, bool isMemberFunction = false)
        {
            FunctionWrapper* functionWrapper = new FunctionWrapper();
            functionWrapper->functionAddress = srcMockFunc;
            functionWrapper->isMemberFunction = isMemberFunction;
            this->m_whenCalledFunc.push(functionWrapper);
            return *this;
        }

        Injector& WhenCalledVirtualMethod(void* classInstance, const std::string& virtualMethodName)
        {
            void* virtualFunction = this->m_injectorCore->GetVirtualMethodAddress(classInstance, virtualMethodName);

            FunctionWrapper* functionWrapper = new FunctionWrapper();
            functionWrapper->functionAddress = virtualFunction;
            functionWrapper->isMemberFunction = true;

            this->m_whenCalledFunc.push(functionWrapper);

            return *this;
        }

        Injector& WillExecute(void* destMockFunc, bool isMemberFunction = false)
        {
            if (this->m_whenCalledFunc.empty())
            {
                throw;
            }

            if (this->m_whenCalledFunc.size() > 1)
            {
                throw;
            }

            FunctionWrapper* srcFunctionWrapper = this->m_whenCalledFunc.top();
            this->m_whenCalledFunc.pop();

            this->m_injectorCore->ReplaceFunction(srcFunctionWrapper->functionAddress, destMockFunc, srcFunctionWrapper->isMemberFunction);

            delete srcFunctionWrapper;
            srcFunctionWrapper = NULL;

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

        template<typename OUTTYPE, typename INTYPE>
        static OUTTYPE ForceCast(INTYPE in)
        {
            union
            {
                INTYPE  in;
                OUTTYPE out;
            }
            u = { in };

            return u.out;
        }

    private:
        // Disable copy constructor.
        // To adapt C++0x, we do not specific "=delete" here.
        Injector(const Injector&);

        InjectorCore* m_injectorCore;

        std::stack<FunctionWrapper*> m_whenCalledFunc;
    };

#define INJECTORPP_MEMBER_FUNCTION(fullyQualifiedFunctionName) \
InjectorPP::Injector::ForceCast<void*>(&fullyQualifiedFunctionName), true
}

#endif
