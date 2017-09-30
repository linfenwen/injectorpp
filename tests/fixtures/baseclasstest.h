#ifndef INJECTORPPTEST_FIXTURES_BASECLASSTEST_H
#define INJECTORPPTEST_FIXTURES_BASECLASSTEST_H

#include <string>
#include "address.h"

class BaseClassTest
{
public:
    BaseClassTest();

    virtual ~BaseClassTest();

    virtual int GetAnIntegerVirtual();

    virtual std::string GetStringVirtual();

    int GetAnInteger();

    std::string GetAString();

    std::string* GetAStringPointer();

    Address GetAnAddress();

    static Address GetAnAddressStatic();
};

#endif