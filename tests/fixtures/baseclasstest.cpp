#include "baseclasstest.h"

BaseClassTest::BaseClassTest()
{
}

BaseClassTest::~BaseClassTest()
{
}

int BaseClassTest::getAnIntegerVirtual()
{
    return 0;
}

std::string BaseClassTest::getStringVirtual()
{
    return "Normal base string";
}

int BaseClassTest::getAnInteger()
{
    return -1;
}

std::string BaseClassTest::getAString()
{
    return "Get string.";
}

std::string* BaseClassTest::getAStringPointer()
{
    std::string* p = new std::string("Normal string pointer.");

    return p;
}

Address BaseClassTest::getAnAddress()
{
    Address addr;
    addr.setAddressLine("normalAddressLine");
    addr.setZipCode("normalZipCode");

    return addr;
}

Address BaseClassTest::getAnAddressStatic()
{
    Address addr;
    addr.setAddressLine("normalAddressLine");
    addr.setZipCode("normalZipCode");

    return addr;
}