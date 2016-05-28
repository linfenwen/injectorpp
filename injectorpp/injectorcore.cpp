#include "InjectorCore.h"
#include <vector>
#include "ClassResolver.h"
#include "FunctionResolver.h"
#include "FakeFunctions.h"
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

    void* InjectorCore::Fake(const char* typeName, size_t typeSize)
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
            // It's class type. Let's extract class name.
            std::string className = typeNameString.substr(6);

            std::vector<Function> resolvedMethods;
            this->m_classResolver->ResolveMethods((ULONG64)module, className, resolvedMethods);

            std::vector<Function>::iterator resolvedMethodIterator = resolvedMethods.begin();
            for (; resolvedMethodIterator != resolvedMethods.end(); ++resolvedMethodIterator)
            {
                // Store function symbol and address mapping.
                this->AddFunctionSymbolAddressMapping((*resolvedMethodIterator).Name, (*resolvedMethodIterator).Address);

                if ((*resolvedMethodIterator).ReturnType == "std::basic_string<char,std::char_traits<char>,std::allocator<char> >")
                {
                    // TODO: std::string won't work here.
                    this->m_behaviorChanger->ChangeFunctionReturnValue((*resolvedMethodIterator).Address, "");
                }
                else if ((*resolvedMethodIterator).ReturnType == "signed __int32")
                {
                    this->m_behaviorChanger->ChangeFunctionReturnValue((*resolvedMethodIterator).Address, 0);
                }
                else if ((*resolvedMethodIterator).ReturnType == "char*")
                {
                    this->m_behaviorChanger->ChangeFunctionReturnValue((*resolvedMethodIterator).Address, "");
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

    void InjectorCore::AddFunctionSymbolAddressMapping(const std::string& funcSymbol, const ULONG64& address)
    {
        if (this->m_funcSymAddressMapping.find(funcSymbol) != this->m_funcSymAddressMapping.end())
        {
            // The same symbol name already added.
            return;
        }

        this->m_funcSymAddressMapping.insert(std::make_pair(funcSymbol, address));
    }

    void InjectorCore::ChangeFunctionReturnValue(const std::string& funcCallCode, const int& expectedReturnValue)
    {
        // Extract function name.
        std::string functionSignature = funcCallCode.substr(funcCallCode.find("->") + 2);
        std::string functionName = functionSignature.substr(0, functionSignature.find("("));

        ULONG64 funcAddress = 0;
        std::map<std::string, ULONG64>::const_iterator it = this->m_funcSymAddressMapping.begin();
        for (; it != this->m_funcSymAddressMapping.end(); ++it)
        {
            // OK, OK. I know this logic is silly.
            // We need to opimize the whole method for querying function address.
            //
            // TODO: We may believe in the full symbol of funciton.
            if (it->first.find(functionName) != std::string::npos)
            {
                funcAddress = it->second;

                break;
            }
        }

        if (funcAddress == 0)
        {
            throw;
        }

        this->m_behaviorChanger->ChangeFunctionReturnValue(funcAddress, expectedReturnValue);
    }

    void InjectorCore::ChangeFunctionReturnValue(const std::string& funcCallCode, const char* expectedReturnValue)
    {
        // Extract function name.
        std::string functionSignature = funcCallCode.substr(funcCallCode.find("->") + 2);
        std::string functionName = functionSignature.substr(0, functionSignature.find("("));

        ULONG64 funcAddress = 0;
        std::map<std::string, ULONG64>::const_iterator it = this->m_funcSymAddressMapping.begin();
        for (; it != this->m_funcSymAddressMapping.end(); ++it)
        {
            // OK, OK. I know this logic is silly.
            // We need to opimize the whole method for querying function address.
            //
            // TODO: We may believe in the full symbol of funciton.
            if (it->first.find(functionName))
            {
                funcAddress = it->second;
            }
        }

        if (funcAddress == 0)
        {
            throw;
        }

        this->m_behaviorChanger->ChangeFunctionReturnValue(funcAddress, expectedReturnValue);
    }
}
