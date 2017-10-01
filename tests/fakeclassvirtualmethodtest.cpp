#include "gmock/gmock.h"
#include "windows/injector.h"
#include <string>
#include "fixtures/baseclasstest.h"
#include "fixtures/subclasstest.h"

class FakeClassVirtualMethodTestFixture : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

int fakeIntFuncForDerived()
{
    return 2;
}

int fakeIntFuncForBase()
{
    return 6;
}

std::string fakeStringFuncForBase()
{
    return "Faked base string";
}

TEST_F(FakeClassVirtualMethodTestFixture, MockBaseClassVirtualMemberFunctionWhenCalled)
{
    // Prepare
    int expected = 6;
    BaseClassTest* base = new BaseClassTest();

    InjectorPP::Injector injector;
    injector.whenCalledVirtualMethod(base, "getAnIntegerVirtual")
        .willExecute(fakeIntFuncForBase);

    // Act
    int actual = base->getAnIntegerVirtual();

    // Assert
    EXPECT_EQ(expected, actual);

    delete base;
    base = NULL;
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
    int actual = derived->getAnIntegerVirtual();

    // Assert
    EXPECT_EQ(expected, actual);

    delete derived;
    derived = NULL;
}

TEST_F(FakeClassVirtualMethodTestFixture, MockBaseClassVirtualMemberFunctionReturnStringWhenCalled)
{
    // Prepare
    std::string expected = "Faked base string";
    BaseClassTest* base = new BaseClassTest();    

    InjectorPP::Injector injector;
    injector.whenCalledVirtualMethod(base, "getStringVirtual")
        .willExecute(fakeStringFuncForBase);

    // Act
    std::string actual = base->getStringVirtual();

    // Assert
    EXPECT_EQ(expected, actual);

    delete base;
    base = NULL;
}