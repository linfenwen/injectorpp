# Injector++

Injector++ is a library to help you fake non-virtual methods, static methods and global functions.

## How to use
It's extremely easy to mock class methods via Injector++. Just one line code:

```cpp
Address* fakeAddress = INJECTORPP_FAKE<Address>();
```

*Address* is the class to mock. Calling INJECTORPP_FAKE will make all methods of Address as fake functions. That is, a function return int will return 0, a function return char* or std::string will return "", and etc.

Below example will demonstrate the magic (This example leveraging googletest as the test runner):

```cpp

#include "gmock/gmock.h"
#include "Injector.h"
#include "Address.h"

class AddressTestFixture : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        INJECTORPP_SETUP();
    }

    virtual void TearDown()
    {
        INJECTORPP_CLEANUP();
    }
};

TEST_F(AddressTestFixture, MyTest)
{
    Address* fakeAddress = INJECTORPP_FAKE<Address>();

    // Will return 0 instead of the real implementation!
    std::cout << fakeAddress->GetZipCode() << std::endl;

    // Will return "" instead of the real implementation!
    std::cout << fakeAddress->GetCity() << std::endl;

    // Will return "" instead of the real implementation!
    std::cout << fakeAddress->GetCapitalCityByCountry("") << std::endl;
}

```
Just put INJECTORPP_SETUP() into Setup() and put INJECTORPP_CLEANUP() into TearDown(). You're ready!

## Prerequisites
1. Injector++ only supports Windows by now. Linux support will be added soon.
2. Injector++ needs to retrieve information from pdb, therefore please:

> In Visual Studio, open project property dialog, select Configuration Properties -> Linker -> Debuggin, change Generate Debug Info to "true"

> Add dbgHelp.lib as dependency in your test project setting.

## Roadmap
There's a headache of C++ unit testing - No way to abstract the legacy code. As there's no reflection in C++, it is not that easy to change the behavior of non-virtual and static methods, which makes C++ unit testing extremely hard.

Injector++ intends to resolve the headache. It's goal is to make static method, non-virtual method and global functions testable without changing your production code. To accomplish this we still have to:

1. Add rich mock helper functions (e.g, return value expectation, input parameter mock, exception expectation...).
2. Add Linux support.
3. Adapt to mainstream unit test runners.

## Contribute
Welcome to help Injector++. You can submit bugs, suggestions and feature requests to the issue tracker, or send me pull request (to dev branch please!). Happy coding & testing!
