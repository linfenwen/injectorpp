#ifndef INJECTORPP_FUNCTIONPARAMETER_H
#define INJECTORPP_FUNCTIONPARAMETER_H

#include <Windows.h>
#include <string>
#include <vector>

struct FunctionParameter
{
    std::string Name;
    std::string Type;
};

struct Function
{
    std::string Name;
    std::string ReturnType;
    std::vector<FunctionParameter> Parameters;
    ULONG64 Address;
};

#endif