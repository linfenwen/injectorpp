#include "SubClassTest.h"

SubClassTest::SubClassTest()
{
}

SubClassTest::~SubClassTest()
{
}

int SubClassTest::GetAnIntegerVirtual()
{
    return 1;
}

std::string SubClassTest::GetStringVirtual()
{
    return "Normal sub string";
}