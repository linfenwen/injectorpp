#include "gmock/gmock.h"
#include "windows/injector.h"
#include "fixtures/baseclasstest.h"
#include "fixtures/subclasstest.h"
#include <string>

class FakeClassNonVirtualMethodTestFixture : public ::testing::Test
{
public:
    int FakeFunc()
    {
        return 6;
    }

    std::string FakeStringFunc()
    {
        return "Fake string func";
    }

    std::string* FakeStringPointerFunc()
    {
        std::string* p = new std::string("Fake string pointer");

        return p;
    }

    Address FakeGetAnAddress()
    {
        Address addr;
        addr.SetAddressLine("fakeAddressLine");
        addr.SetZipCode("fakeZipCode");

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

std::string FooReturnString()
{
    return "FooReturnString";
}

std::string FakeFooReturnString()
{
    return "FakeFooReturnString";
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeIntFunctionWhenCalled)
{
    // Prepare
    int expected = 6;
    InjectorPP::Injector injector;

    injector.WhenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::GetAnInteger))
        .WillExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::FakeFunc));

    BaseClassTest b = BaseClassTest();

    // Act
    int actual = b.GetAnInteger();

    // Assert
    EXPECT_EQ(expected, actual);
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeStringPointerFunctionWhenCalled)
{
    // Prepare
    std::string expected = "Fake string pointer";
    InjectorPP::Injector injector;
    
    injector.WhenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::GetAStringPointer))
        .WillExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::FakeStringPointerFunc));

    BaseClassTest b = BaseClassTest();

    // Act
    std::string* actual = b.GetAStringPointer();

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

    injector.WhenCalled(FooReturnString)
        .WillExecute(FakeFooReturnString);

    // Act
    std::string actual = FooReturnString();

    // Assert
    EXPECT_EQ(expected, actual);
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeStringFunctionWhenCalled)
{
    // Prepare
    std::string expected = "Fake string func";
    InjectorPP::Injector injector;

    injector.WhenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::GetAString))
        .WillExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::FakeStringFunc));

    BaseClassTest b;

    // Act
    std::string actual = b.GetAString();

    // Assert
    EXPECT_EQ(expected, actual);
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeFunctionReturnUserDefinedClassWhenCalled)
{
    // Prepare
    Address expected;
    expected.SetAddressLine("fakeAddressLine");
    expected.SetZipCode("fakeZipCode");

    InjectorPP::Injector injector;

    injector.WhenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::GetAnAddress))
        .WillExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::FakeGetAnAddress));

    BaseClassTest b;

    // Act
    Address actual = b.GetAnAddress();

    // Assert
    EXPECT_EQ(expected, actual);
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeStaticFunctionReturnUserDefinedClassWhenCalled)
{
    // Prepare
    Address expected;
    expected.SetAddressLine("fakeAddressLine");
    expected.SetZipCode("fakeZipCode");

    InjectorPP::Injector injector;

    injector.WhenCalled(INJECTORPP_STATIC_MEMBER_FUNCTION(BaseClassTest::GetAnAddressStatic))
        .WillExecute(INJECTORPP_MEMBER_FUNCTION(FakeClassNonVirtualMethodTestFixture::FakeGetAnAddress));

    // Act
    Address actual = BaseClassTest::GetAnAddressStatic();

    // Assert
    EXPECT_EQ(expected, actual);
}

