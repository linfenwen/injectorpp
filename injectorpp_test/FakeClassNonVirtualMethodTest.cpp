#include "gmock/gmock.h"
#include "Injector.h"
#include "Fixtures/BaseClassTest.h"
#include "Fixtures/SubClassTest.h"
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
        return "fake string func";
    }

    std::string* FakeStringPointerFunc()
    {
        std::string* p = new std::string("Fake string pointer");

        return p;
    }

protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

std::string FakeStringFunc2()
{
    return "Fake string pointer";
}

int FakeIntFunc2()
{
    return 2;
}

std::string FakeStringFunc1()
{
    std::string ss = "dw";
    return ss;
}

int FakeIntFunc1()
{
    return FakeIntFunc2();
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeFunctionWithIntReturnDefaultValue)
{
    // Prepare
    /*Address* fakeAddress = INJECTORPP_FAKE<Address>();
    int expected = 0;

    // Act
    int actual = fakeAddress->GetZipCode();

    // Assert
    EXPECT_EQ(expected, actual);*/
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeFunctionWithIntNotReturnDefaultValue)
{
    // Prepare
    /*Address* fakeAddress = INJECTORPP_FAKE<Address>(false);
    int expected = -1;

    // Act
    int actual = fakeAddress->GetZipCode();

    // Assert
    EXPECT_EQ(expected, actual);*/
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeFunctionWithCCharReturnDefaultValue)
{
    // Prepare
    /*Address* fakeAddress = INJECTORPP_FAKE<Address>();
    char* expected = "aa";

    WHEN_CALLED(fakeAddress->GetCity()).Return(expected);

    // Act
    char* actual = fakeAddress->GetCity();

    // Assert
    EXPECT_EQ(std::string(expected), std::string(actual));*/
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeFunctionWithStringReturnDefaultValue)
{
    // Prepare
    /*Address* fakeAddress = INJECTORPP_FAKE<Address>();
    std::string expected("expectedString");

    WHEN_CALLED(fakeAddress->GetCapitalCityByCountry("")).Return(&expected);

    // Act
    std::string* actual = fakeAddress->GetCapitalCityByCountry("");

    // Assert
    EXPECT_EQ(expected, *actual);*/
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

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeStringFunctionWhenCalled)
{
    std::string s1 = FakeStringFunc1();

    // Prepare
    std::string expected = "Fake string pointer";
    InjectorPP::Injector injector;

    BaseClassTest b1 = BaseClassTest();
    b1.GetAString();

    injector.WhenCalled(INJECTORPP_MEMBER_FUNCTION(BaseClassTest::GetAString))
        .WillExecute(FakeStringFunc2);

    BaseClassTest b;

    // Act
    std::string actual = b.GetAString();

    // Assert
    EXPECT_EQ(expected, actual);
}

