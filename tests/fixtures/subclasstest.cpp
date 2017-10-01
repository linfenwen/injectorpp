#include "subclasstest.h"

SubClassTest::SubClassTest()
{
}

SubClassTest::~SubClassTest()
{
}

int SubClassTest::getAnIntegerVirtual()
{
    return 1;
}

std::string SubClassTest::getStringVirtual()
{
    return "Normal sub string";
}