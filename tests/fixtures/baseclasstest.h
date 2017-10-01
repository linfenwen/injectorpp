#ifndef INJECTORPPTEST_FIXTURES_BASECLASSTEST_H
#define INJECTORPPTEST_FIXTURES_BASECLASSTEST_H

#include <string>
#include "address.h"

class BaseClassTest
{
public:
    BaseClassTest();

    virtual ~BaseClassTest();

    virtual int getAnIntegerVirtual();

    virtual std::string getStringVirtual();

    int getAnInteger();

    std::string getAString();

    std::string* getAStringPointer();

    Address getAnAddress();

    static Address getAnAddressStatic();
};

#endif