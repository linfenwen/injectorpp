#include "gmock/gmock.h"
#include "Injector.h"
#include "Fixtures/Address.h"

class FakeClassNonVirtualMethodTestFixture : public ::testing::Test
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

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeFunctionWithIntReturnDefaultValue)
{
    // Prepare
    Address* fakeAddress = INJECTORPP_FAKE<Address>();
    int expected = 0;

    // Act
    int actual = fakeAddress->GetZipCode();

    // Assert
    EXPECT_EQ(expected, actual);
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeFunctionWithCCharReturnDefaultValue)
{
    // Prepare
    Address* fakeAddress = INJECTORPP_FAKE<Address>();
    char* expected = "aa";

    WHEN_CALLED(fakeAddress->GetCity()).Return("aa");

    // Act
    char* actual = fakeAddress->GetCity();

    // Assert
    EXPECT_EQ(std::string(expected), std::string(actual));
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeFunctionWithStringReturnDefaultValue)
{
    // Prepare
    Address* fakeAddress = INJECTORPP_FAKE<Address>();
    std::string expected = "";

    WHEN_CALLED(fakeAddress->GetCapitalCityByCountry("")).Return("bb");

    // Act
    std::string actual = fakeAddress->GetCapitalCityByCountry("bb");

    // Assert
    EXPECT_EQ(expected, actual);
}

TEST_F(FakeClassNonVirtualMethodTestFixture, FakeFunctionWhenCalled)
{
    // Prepare
    Address* fakeAddress = INJECTORPP_FAKE<Address>();
    int expected = 100;

    WHEN_CALLED(fakeAddress->GetMyCode()).Return(100);

    // Act
    int actual = fakeAddress->GetMyCode();

    // Assert
    EXPECT_EQ(expected, actual);
}
