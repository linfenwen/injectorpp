#include <string>
#include <exception>

#include "address.h"

using std::string;

std::string Address::getZipCode()
{
    return this->m_zipCode;
}

void Address::setZipCode(const std::string& zipCode)
{
    this->m_zipCode = zipCode;
}

std::string Address::getAddressLine()
{
    return this->m_addressLine;
}

void Address::setAddressLine(const std::string& addressLine)
{
    this->m_addressLine = addressLine;
}

