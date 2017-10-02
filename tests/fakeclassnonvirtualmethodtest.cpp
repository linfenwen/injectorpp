#include "gmock/gmock.h"
#include "windows/injector.h"
#include "fixtures/baseclasstest.h"
#include "fixtures/subclasstest.h"
#include <string>

class FakeClassNonVirtualMethodTestFixture : public ::testing::Test
{
  public:
    int fakeFunc()
    {
        return 6;
    }

    std::string fakeStringFunc()
    {
        return "Fake string func";
    }

    std::string *fakeStringPointerFunc()
    {
        std::string *p = new std::string("Fake string pointer");

        return p;
    }

    Address fakeGetAnAddress()
    {
        Address addr;
        addr.setAddressLine("fakeAddressLine");
        addr.setZipCode("fakeZipCode");

        return addr;
    }

    static Address fakeGetAnAddressStatic()
    {
        Address addr;
        addr.setAddressLine("fakeAddressLine");
        addr.setZipCode("fakeZipCode");

        return addr;
    }

protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

std::string fooReturnString()
{
    return "FooReturnString";
}

std::string fooReturnStringWithParameter(std::string input)
{
    return "FooReturnStringWithParameter";
}

std::string fakeFooReturnString()
{
    return "FakeFooReturnString";
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeIntFunctionWhenCalled)
{
    // Prepare
    int expected = 6;
    InjectorPP::Injector injector;

    injector.whenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::getAnInteger))
        .willExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::fakeFunc));

    BaseClassTest b = BaseClassTest();

    // Act
    int actual = b.getAnInteger();

    // Assert
    EXPECT_EQ(expected, actual);
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeStringPointerFunctionWhenCalled)
{
    // Prepare
    std::string expected = "Fake string pointer";
    InjectorPP::Injector injector;

    injector.whenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::getAStringPointer))
        .willExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::fakeStringPointerFunc));

    BaseClassTest b = BaseClassTest();

    // Act
    std::string *actual = b.getAStringPointer();

    // Assert
    EXPECT_EQ(expected, *actual);

    delete actual;
    actual = NULL;
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeGlobalStringFunctionWhenCalled)
{
    // Prepare
    std::string expected = "FakeFooReturnString";
    InjectorPP::Injector injector;

    injector.whenCalled(fooReturnString)
        .willExecute(fakeFooReturnString);

    // Act
    std::string actual = fooReturnString();

    // Assert
    EXPECT_EQ(expected, actual);
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeGlobalStringFunctionWithParameterWhenCalled)
{
    // Prepare
    std::string expected = "FakeFooReturnString";
    InjectorPP::Injector injector;

    injector.whenCalled(fooReturnStringWithParameter)
        .willExecute(fakeFooReturnString);

    // Act
    std::string actual = fooReturnStringWithParameter("Test input");

    // Assert
    EXPECT_EQ(expected, actual);
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeStringFunctionWhenCalled)
{
    // Prepare
    std::string expected = "Fake string func";
    InjectorPP::Injector injector;

    injector.whenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::getAString))
        .willExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::fakeStringFunc));

    BaseClassTest b;

    // Act
    std::string actual = b.getAString();

    // Assert
    EXPECT_EQ(expected, actual);
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeFunctionReturnUserDefinedClassWhenCalled)
{
    // Prepare
    Address expected;
    expected.setAddressLine("fakeAddressLine");
    expected.setZipCode("fakeZipCode");

    InjectorPP::Injector injector;

    injector.whenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::getAnAddress))
        .willExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::fakeGetAnAddress));

    BaseClassTest b;

    // Act
    Address actual = b.getAnAddress();

    // Assert
    EXPECT_EQ(expected, actual);
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
    Address actual = BaseClassTest::getAnAddressStatic();

    // Assert
    EXPECT_EQ(expected, actual);
}
