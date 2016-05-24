#include <iostream>
#include "injector.h"
#include "address.h"

using injectorpp::Injector;
using injectorpp::InjectorCore;

int main()
{
    Injector::initialize();

    Address* mockAddress = Injector::getInstance()->fake<Address>();

    std::cout << mockAddress->GetZipCode() << std::endl;

    Injector::uninitialize();

    return 0;
}