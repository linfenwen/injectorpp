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

int FakeIntFuncForDerived()
{
    return 2;
}

int FakeIntFuncForBase()
{
    return 6;
}

std::string FakeStringFuncForBase()
{
    return "Faked base string";
}

TEST_F(FakeClassVirtualMethodTestFixture, MockBaseClassVirtualMemberFunctionWhenCalled)
{
    // Prepare
    int expected = 6;
    BaseClassTest* base = new BaseClassTest();

    InjectorPP::Injector injector;
    injector.WhenCalledVirtualMethod(base, "GetAnIntegerVirtual")
        .WillExecute(FakeIntFuncForBase);

    // Act
    int actual = base->GetAnIntegerVirtual();

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
    injector.WhenCalledVirtualMethod(derived, "GetAnIntegerVirtual")
        .WillExecute(FakeIntFuncForDerived);

    // Act
    int actual = derived->GetAnIntegerVirtual();

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

    std::string a1 = base->GetStringVirtual();

    InjectorPP::Injector injector;
    injector.WhenCalledVirtualMethod(base, "GetStringVirtual")
        .WillExecute(FakeStringFuncForBase);

    // Act
    std::string actual = base->GetStringVirtual();

    // Assert
    EXPECT_EQ(expected, actual);

    delete base;
    base = NULL;
}