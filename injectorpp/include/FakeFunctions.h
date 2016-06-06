#ifndef INJECTORPP_FAKEFUNCTIONS_H
#define INJECTORPP_FAKEFUNCTIONS_H

#include <string>
#include <iostream>

int fakeReturnIntFuncReturnValue = 0;

std::string* t = new std::string("a");

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
    /*__asm 
    {
        push ""
        move ecx, [ebp+8]
        call string::basic_string
    }*/
}

void* FakeReturnPointerFunc(void* d)
{
    std::string* ss = new std::string();
    return t;
}

#endif