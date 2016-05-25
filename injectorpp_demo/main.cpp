#include <iostream>
#include "injector.h"
#include "address.h"

using InjectorPP::Injector;
using InjectorPP::InjectorCore;

int main()
{
    INJECTORPP_SETUP();

    Address* fakeAddress = INJECTORPP_FAKE<Address>();

    // Will return 0 instead of the real implementation!
    std::cout << fakeAddress->GetZipCode() << std::endl;

    // Will return "" instead of the real implementation!
    std::cout << fakeAddress->GetCity() << std::endl;

    // Will return "" instead of the real implementation!
    std::cout << fakeAddress->GetCapitalCityByCountry("") << std::endl;

    INJECTORPP_CLEANUP();

    return 0;
}