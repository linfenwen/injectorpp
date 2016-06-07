#include <vector>
#include <algorithm>

#include "InjectorCore.h"
#include "ClassResolver.h"
#include "FunctionResolver.h"
#include "Utility.h"

namespace InjectorPP
{
    bool InjectorCore::m_isSymInitialized = false;

    InjectorCore::InjectorCore()
        :m_behaviorChanger(NULL),
        m_classResolver(NULL),
        m_functionResolver(NULL),
        m_symbolInfoHelper(NULL),
        m_currentProcessHandler(NULL)
    {
        this->m_currentProcessHandler = GetCurrentProcess();
        this->m_behaviorChanger = new BehaviorChanger();
        this->m_functionResolver = new FunctionResolver(this->m_currentProcessHandler);
        this->m_classResolver = new ClassResolver(this->m_currentProcessHandler, this->m_functionResolver);
        this->m_symbolInfoHelper = new SymbolInfoHelper();
    }

    InjectorCore::~InjectorCore()
    {
        // Release all allocated type instances.
        std::vector<void*>::iterator it;
        for (it = this->m_allocatedTypeInstances.begin(); it != this->m_allocatedTypeInstances.end(); ++it)
        {
            if (*it != NULL)
            {
                delete *it;
                *it = NULL;
            }
        }

        // Release all fake classes.
        std::vector<Class*>::iterator classIt;
        for (classIt = this->m_resolvedClasses.begin(); classIt != this->m_resolvedClasses.end(); ++classIt)
        {
            if (*classIt != NULL)
            {
                delete *classIt;
                *classIt = NULL;
            }
        }

        // Release all original function asms.
        std::vector<OriginalFuncASM*>::iterator ofaIt;
        for (ofaIt = this->m_originalFunctionASMVector.begin(); ofaIt != this->m_originalFunctionASMVector.end(); ++ofaIt)
        {
            if (*ofaIt != NULL)
            {
                // Recover the original function behavior.
                this->m_behaviorChanger->DirectWriteToFunction((*ofaIt)->funcAddress, (*ofaIt)->asmCode, 6);

                delete *ofaIt;
                *ofaIt = NULL;
            }
        }

        if (this->m_symbolInfoHelper != NULL)
        {
            delete this->m_symbolInfoHelper;
            this->m_symbolInfoHelper = NULL;
        }

        if (this->m_classResolver != NULL)
        {
            delete this->m_classResolver;
            this->m_classResolver = NULL;
        }

        if (this->m_functionResolver != NULL)
        {
            delete this->m_functionResolver;
            this->m_functionResolver = NULL;
        }

        if (this->m_behaviorChanger != NULL)
        {
            delete m_behaviorChanger;
            m_behaviorChanger = NULL;
        }
    }

    void* InjectorCore::Fake(const char* typeName, size_t typeSize, bool autoFillDefaultValue)
    {
        if (!InjectorCore::m_isSymInitialized)
        {
            // Let's initialize the whole symbol system.
            if (SymInitialize(this->m_currentProcessHandler, NULL, TRUE) == FALSE)
            {
                throw;
            }

            InjectorCore::m_isSymInitialized = true;
        }

        void* typeInstance = nullptr;

        // Allocate a symbol entity to store top level symbol info.
        PSYMBOL_INFO sym = this->m_symbolInfoHelper->AllocSymbol();

        // Get current executing module handle.
        HMODULE module = GetModuleHandle(0);

        std::string typeNameString(typeName);
        
        std::string classSpecifier("class ");
        std::string startStr = typeNameString.substr(0, classSpecifier.length());
        if (startStr == classSpecifier)
        {
            Class* cls = new Class();

            // It's class type. Let's extract class name.
            cls->Name = typeNameString.substr(6);

            this->m_classResolver->ResolveMethods((ULONG64)module, cls->Name, cls->Methods);

            // Store the new class.
            this->m_resolvedClasses.push_back(cls);

            if (autoFillDefaultValue)
            {
                std::vector<Function>::iterator resolvedMethodIterator = cls->Methods.begin();
                for (; resolvedMethodIterator != cls->Methods.end(); ++resolvedMethodIterator)
                {
                    // Save the original asm code. Revert the function behavior when uninitialize.
                    OriginalFuncASM* originalFuncAsm = new OriginalFuncASM();

                    if ((*resolvedMethodIterator).ReturnType == "signed __int32")
                    {
                        this->m_behaviorChanger->ChangeFunctionReturnValue((*resolvedMethodIterator).Address, 0, originalFuncAsm);
                    }
                    else if ((*resolvedMethodIterator).ReturnType == "char*")
                    {
                        this->m_behaviorChanger->ChangeFunctionReturnValue((*resolvedMethodIterator).Address, "", originalFuncAsm);
                    }

                    if (!this->SaveOriginalFuncASM(originalFuncAsm))
                    {
                        delete originalFuncAsm;
                        originalFuncAsm = NULL;
                    }
                }
            }
        }
        else
        {
            // TODO: Add other type support.
        }

        // Allocate memory for the type.
        typeInstance = malloc(typeSize);
        memset(typeInstance, 0, typeSize);

        // Store the new instance. We'll delete it in the de-constructor.
        this->m_allocatedTypeInstances.push_back(typeInstance);

        return typeInstance;
    }

    void InjectorCore::ChangeFunctionReturnValue(const std::string& funcCallCode, const int& expectedReturnValue)
    {
        ULONG64 funcAddress = 0;
        this->GetFunctionAddressByFunctionCallCode(funcCallCode, funcAddress);

        // Save the original asm code. Revert the function behavior when uninitialize.
        OriginalFuncASM* originalFuncAsm = new OriginalFuncASM();

        this->m_behaviorChanger->ChangeFunctionReturnValue(funcAddress, expectedReturnValue, originalFuncAsm);

        if (!this->SaveOriginalFuncASM(originalFuncAsm))
        {
            delete originalFuncAsm;
            originalFuncAsm = NULL;
        }
    }

    bool InjectorCore::SaveOriginalFuncASM(OriginalFuncASM* originalFuncASM)
    {
        if (originalFuncASM->funcAddress == 0)
        {
            return false;
        }

        std::vector<OriginalFuncASM*>::iterator it = this->m_originalFunctionASMVector.begin();
        for (; it != this->m_originalFunctionASMVector.end(); ++it)
        {
            if ((*it)->funcAddress == originalFuncASM->funcAddress)
            {
                // The original asm already been stored.
                return false;
            }
        }

        this->m_originalFunctionASMVector.push_back(originalFuncASM);

        return true;
    }

    void InjectorCore::ChangeFunctionReturnValue(const std::string& funcCallCode, const void* expectedReturnValue)
    {
        ULONG64 funcAddress = 0;
        this->GetFunctionAddressByFunctionCallCode(funcCallCode, funcAddress);

        // Save the original asm code. Revert the function behavior when uninitialize.
        OriginalFuncASM* originalFuncAsm = new OriginalFuncASM();

        this->m_behaviorChanger->ChangeFunctionReturnValue(funcAddress, expectedReturnValue, originalFuncAsm);

        if (!this->SaveOriginalFuncASM(originalFuncAsm))
        {
            delete originalFuncAsm;
            originalFuncAsm = NULL;
        }
    }

    void InjectorCore::ChangeFunctionReturnValue(const std::string& funcCallCode, const char* expectedReturnValue)
    {
        ULONG64 funcAddress = 0;
        this->GetFunctionAddressByFunctionCallCode(funcCallCode, funcAddress);

        // Save the original asm code. Revert the function behavior when uninitialize.
        OriginalFuncASM* originalFuncAsm = new OriginalFuncASM();

        this->m_behaviorChanger->ChangeFunctionReturnValue(funcAddress, expectedReturnValue, originalFuncAsm);

        if (!this->SaveOriginalFuncASM(originalFuncAsm))
        {
            delete originalFuncAsm;
            originalFuncAsm = NULL;
        }
    }

    void InjectorCore::GetFunctionAddressByFunctionCallCode(const std::string& funcCallCode, ULONG64& funcAddress)
    {
        // Extract function name.
        std::string functionSignature = funcCallCode.substr(funcCallCode.find("->") + 2);
        std::string functionName = functionSignature.substr(0, functionSignature.find("("));

        size_t leftBrackeIndex = functionSignature.find("(");
        size_t paramterStringCount = functionSignature.find(")") - leftBrackeIndex - 1;
        
        size_t parameterCount = 0;
        if (paramterStringCount != 0)
        {
            std::string functionParameters = functionSignature.substr(leftBrackeIndex + 1, functionSignature.find(")") - leftBrackeIndex - 1);

            // Find parameter count.
            std::vector<std::string> splittedParameters = Utility::Split(functionParameters, ',');

            parameterCount = splittedParameters.size();
        }

        funcAddress = 0;

        Class* cls = this->m_resolvedClasses[0];

        std::vector<Function>::const_iterator it = cls->Methods.begin();
        for (; it != cls->Methods.end(); ++it)
        {
            if (it->RawName != functionName)
            {
                continue;
            }

            if (it->Parameters.size() != parameterCount)
            {
                continue;
            }

            // TODO: parameter identify.

            funcAddress = it->Address;

            break;
        }
    }
}
