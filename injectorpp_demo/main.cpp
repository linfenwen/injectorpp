#include <iostream>
#include "injector.h"
#include "address.h"

using injectorpp::Injector;
using injectorpp::InjectorCore;

int main()
{
    INJECTORPP_SETUP();

    Address* fakeAddress = INJECTORPP_FAKE<Address>();

    std::cout << fakeAddress->GetZipCode() << std::endl;

    INJECTORPP_CLEANUP();

    return 0;
}