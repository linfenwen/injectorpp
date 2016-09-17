# Injector++

Injector++ is a library to help you fake virtual/non-virtual methods, static methods and global functions without changing existing code.

## How to use
It's extremely easy to mock class methods via Injector++.

### Mock non-virtual methods

Below example mocks BaseClassTest::GetAnInteger() by using FakeFunc():

```cpp

class FakeClassNonVirtualMethodTestFixture : public ::testing::Test
{
public:
    int FakeFunc()
    {
        return 6;
    }
};

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeIntFunctionWhenCalled)
{
    // Prepare
    int expected = 6;
    InjectorPP::Injector injector;

    injector.WhenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::GetAnInteger))
        .WillExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::FakeFunc));

    BaseClassTest b = BaseClassTest();

    // Act
	// FakeFunc will be executed!
    int actual = b.GetAnInteger();

    // Assert
    EXPECT_EQ(expected, actual);
}

```

### Mock virtual methods
Injector++ supports virtual method mocking (Amazing, huh?). Below is a simple example:

```cpp

int FakeIntFuncForDerived()
{
    return 2;
}

TEST_F(FakeClassVirtualMethodTestFixture, MockDerivedClassVirtualMemberFunctionWhenCalled)
{
    // Prepare
    int expected = 2;
    BaseClassTest* derived = new SubClassTest();

    InjectorPP::Injector injector;
    injector.WhenCalledVirtualMethod(derived, "GetAnIntegerVirtual")
        .WillExecute(FakeIntFuncForDerived);

    // Act
	// FakeIntFuncForDerived() will be exectued!
    int actual = derived->GetAnIntegerVirtual();

    // Assert
    EXPECT_EQ(expected, actual);

    delete derived;
    derived = NULL;
}

```

### Mock static methods
Injector++ supports static method mocking. Below is a simple example:

```cpp

Address FakeGetAnAddress()
{
    Address addr;
    addr.SetAddressLine("fakeAddressLine");
    addr.SetZipCode("fakeZipCode");

    return addr;
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeStaticFunctionReturnUserDefinedClassWhenCalled)
{
    // Prepare
    Address expected;
    expected.SetAddressLine("fakeAddressLine");
    expected.SetZipCode("fakeZipCode");

    InjectorPP::Injector injector;

    injector.WhenCalled(INJECTORPP_STATIC_MEMBER_FUNCTION(BaseClassTest::GetAnAddressStatic))
        .WillExecute(FakeGetAnAddress);

    BaseClassTest b;

    // Act
	// FakeGetAnAddress will be executed!
    Address actual = b.GetAnAddressStatic();

    // Assert
    EXPECT_EQ(expected, actual);
}

```

## Prerequisites
1. Injector++ only supports Windows by now. Linux support will be added soon.
2. Injector++ needs to retrieve information from pdb, therefore please:

> In Visual Studio, open project property dialog, select Configuration Properties -> Linker -> Debuggin, change Generate Debug Info to "true"

> Add dbgHelp.lib as dependency in your test project setting.

> Project property dialog -> Linker -> General -> Enable Incremental Linking -> *No (/INCREMENTAL:NO)*.

## Known issue
For virtual methods, Injector++ only supports those functions return void or primitive types (int, char, pointers). 
Virtual methods return copy of std class or user-defined class will be added soon.

## Roadmap
There's a headache of C++ unit testing - No way to abstract the legacy code. As there's no reflection in C++, it is not that easy to change the behavior of non-virtual and static methods, which makes C++ unit testing extremely hard.

Injector++ intends to resolve the headache. It's goal is to make static method, non-virtual method and global functions testable without changing your production code. To accomplish this we still have to:

1. Add rich mock helper functions (e.g, return value expectation, input parameter mock, exception expectation...).
2. Add Linux support.
3. Adapt to mainstream unit test runners.

## Contribute
Welcome to help Injector++. You can submit bugs, suggestions and feature requests to the issue tracker, or send me pull request. Happy coding & testing!
