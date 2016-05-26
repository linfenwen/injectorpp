#ifndef INJECTORPP_FAKEFUNCTIONS_H
#define INJECTORPP_FAKEFUNCTIONS_H

#include <string>
#include <iostream>

int fakeReturnIntFuncReturnValue = 0;

int FakeReturnIntFunc()
{
    return 2;
}

char* FakeReturnCCharFunc()
{
    char* tt = new char[4];
    tt[0] = 'h';
    tt[1] = 'j';
    tt[2] = 'k';
    tt[3] = '\0';

    return tt;
}

std::string FakeReturnStringFunc()
{
    return "";
}

#endif