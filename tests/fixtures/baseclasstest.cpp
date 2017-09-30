#include "baseclasstest.h"

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

std::string fakefunc()
{
	return "fake";
}

std::string BaseClassTest::GetAString()
{
	return fakefunc();
}

std::string* BaseClassTest::GetAStringPointer()
{
    std::string* p = new std::string("Normal string pointer.");

    return p;
}

Address BaseClassTest::GetAnAddress()
{
    Address addr;
    addr.SetAddressLine("normalAddressLine");
    addr.SetZipCode("normalZipCode");

    return addr;
}

Address BaseClassTest::GetAnAddressStatic()
{
    Address addr;
    addr.SetAddressLine("normalAddressLine");
    addr.SetZipCode("normalZipCode");

    return addr;
}