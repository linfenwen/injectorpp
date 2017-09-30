#include <string>
#include <exception>

#include "address.h"

using std::string;

std::string Address::GetZipCode()
{
    return this->zipCode;
}

void Address::SetZipCode(const std::string& zipCode)
{
    this->zipCode = zipCode;
}

std::string Address::GetAddressLine()
{
    return this->addressLine;
}

void Address::SetAddressLine(const std::string& addressLine)
{
    this->addressLine = addressLine;
}

