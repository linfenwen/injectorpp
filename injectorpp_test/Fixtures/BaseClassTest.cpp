#include "BaseClassTest.h"

BaseClassTest::BaseClassTest()
{
}

BaseClassTest::~BaseClassTest()
{
}

int BaseClassTest::GetAnIntegerVirtual()
{
    return 0;
}

std::string BaseClassTest::GetStringVirtual()
{
    return "Normal base string";
}

int BaseClassTest::GetAnInteger()
{
    return -1;
}

std::string BaseClassTest::GetAString()
{
    return "Normal string";
}

std::string* BaseClassTest::GetAStringPointer()
{
    std::string* p = new std::string("Normal string pointer.");

    return p;
}