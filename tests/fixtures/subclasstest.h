#ifndef INJECTORPPTEST_FIXTURES_SUBCLASSTEST_H
#define INJECTORPPTEST_FIXTURES_SUBCLASSTEST_H

#include "baseclasstest.h"

class SubClassTest : public BaseClassTest
{
public:
    SubClassTest();
    virtual ~SubClassTest();

    virtual int GetAnIntegerVirtual();
    virtual std::string GetStringVirtual();
};

#endif