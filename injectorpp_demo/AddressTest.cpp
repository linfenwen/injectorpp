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

    std::string fakeCountry("fake country");
    WHEN_CALLED(fakeAddress->GetCapitalCityByCountry("")).Return(&fakeCountry);

    // Will return "fake country" instead of the real implementation!
    std::cout << *fakeAddress->GetCapitalCityByCountry("") << std::endl;
}