# Injector++

[![Build status](https://ci.appveyor.com/api/projects/status/gd9380fs0wnnx05e?svg=true)](https://ci.appveyor.com/project/mazong1123/injectorpp)

Injector++ is a library to help you fake virtual/non-virtual methods, static methods and global functions **WITHOUT** changing existing code.

## How to use

### Prerequisites

#### Windows
- Injector++ needs to retrieve information from pdb, therefore please:
- In Visual Studio, open project property dialog, select Configuration Properties -> Linker -> Debugging, change `Generate Debug Info` to `Debug`.
- Add `dbgHelp.lib` as dependency in your test project setting.
- Project property dialog -> Linker -> General -> Enable Incremental Linking -> `No (/INCREMENTAL:NO)`.

#### Linux
In progress.

#### Mac OS
In progress.

### Mock global functions

Below example fakes `fooReturnString` by using `fakeFooReturnString`:

```cpp

std::string fooReturnString()
{
    return "FooReturnString";
}

std::string fakeFooReturnString()
{
    return "FakeFooReturnString";
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeGlobalStringFunctionWhenCalled)
{
    // Prepare
    std::string expected = "FakeFooReturnString";
    InjectorPP::Injector injector;

    injector.whenCalled(fooReturnString)
        .willExecute(fakeFooReturnString);

    // Act
    // fakeFooReturnString will be executued!
    std::string actual = fooReturnString();

    // Assert
    EXPECT_EQ(expected, actual);
}

```

### Mock non-virtual methods

Below example fakes `BaseClassTest::getAnInteger()` by using `fakeFunc()`:

```cpp

class FakeClassNonVirtualMethodTestFixture : public ::testing::Test
{
public:
    int fakeFunc()
    {
        return 6;
    }
};

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeIntFunctionWhenCalled)
{
    // Prepare
    int expected = 6;
    InjectorPP::Injector injector;

    injector.whenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::getAnInteger))
        .willExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::fakeFunc));

    BaseClassTest b = BaseClassTest();

    // Act
    // fakeFunc will be executed!
    int actual = b.getAnInteger();

    // Assert
    EXPECT_EQ(expected, actual);
}

```

### Mock virtual methods
Injector++ supports virtual method mocking (Amazing, huh?). Below is a simple example:

```cpp

int fakeIntFuncForDerived()
{
    return 2;
}

TEST_F(FakeClassVirtualMethodTestFixture, MockDerivedClassVirtualMemberFunctionWhenCalled)
{
    // Prepare
    int expected = 2;
    BaseClassTest* derived = new SubClassTest();

    InjectorPP::Injector injector;
    injector.whenCalledVirtualMethod(derived, "getAnIntegerVirtual")
        .willExecute(fakeIntFuncForDerived);

    // Act
    // fakeIntFuncForDerived() will be exectued!
    int actual = derived->getAnIntegerVirtual();

    // Assert
    EXPECT_EQ(expected, actual);

    delete derived;
    derived = NULL;
}

```

### Mock static methods
Injector++ supports static method mocking. Below is a simple example:

```cpp

static Address fakeGetAnAddressStatic()
{
    Address addr;
    addr.setAddressLine("fakeAddressLine");
    addr.setZipCode("fakeZipCode");

    return addr;
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeStaticFunctionReturnUserDefinedClassWhenCalled)
{
    // Prepare
    Address expected;
    expected.setAddressLine("fakeAddressLine");
    expected.setZipCode("fakeZipCode");

    InjectorPP::Injector injector;

    injector.whenCalled(INJECTORPP_STATIC_MEMBER_FUNCTION(BaseClassTest::getAnAddressStatic))
        .willExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::fakeGetAnAddressStatic));

    // Act
    // fakeGetAnAddressStatic will be executed!
    Address actual = BaseClassTest::getAnAddressStatic();

    // Assert
    EXPECT_EQ(expected, actual);
}

```

## Roadmap
There's a headache of C++ unit testing - No way to abstract the legacy code. As there's no reflection in C++, it is not that easy to change the behavior of non-virtual and static methods, which makes C++ unit testing extremely hard.

Injector++ intends to resolve the headache. Its goal is to make static method, non-virtual method and global functions testable without changing your production code. To accomplish this we still have to:

1. Add rich mock helper functions (e.g, return value expectation, input parameter mock, exception expectation...).
2. Add Linux support.
3. Adapt to mainstream unit test runners.

## Contribute
Welcome to help Injector++. You can submit bugs, suggestions and feature requests to the issue tracker, or send me pull request. Happy coding & testing!

### Build the source code

#### Windows

You need following prerequisites:
- cmake 2.6+
- Visual Studio 2008+ (Please create an issue if you need older MSVC support)

Run `.\build.cmd` to full build the source code as well as running tests.

Run `.\build.cmd /f` to clean the build directory and rebuild all the source code and running tests.

Run `.\build.cmd /skiptests` if you want build source code without building and running tests.

You can combine `/f` and `/skiptests`:

```sh
.\build.cmd /f /skiptests
```

After build finished, you'll find `injectorpp.sln` under `build` folder. You can use Visual Studio to edit and debug the code.

#### Linux
In progress.

#### Mac OS
In progress.

