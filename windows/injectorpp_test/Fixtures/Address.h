#ifndef INJECTORPPTEST_FIXTURES_ADDRESS_H
#define INJECTORPPTEST_FIXTURES_ADDRESS_H

#include <string>

using std::string;

class Address
{
public:
    Address()
    {
    };

    Address(const Address& rhs)
    {
        this->zipCode = rhs.zipCode;
        this->addressLine = rhs.addressLine;
    }

    Address& operator=(Address rhs)
    {
        this->zipCode = rhs.zipCode;
        this->addressLine = rhs.addressLine;

        return *this;
    }

    bool operator==(const Address& rhs) const
    {
        return this->zipCode == rhs.zipCode && this->addressLine == rhs.addressLine;
    }

    std::string GetZipCode();
    void SetZipCode(const std::string& zipCode);

    std::string GetAddressLine();
    void SetAddressLine(const std::string& addressLine);

private:
    std::string zipCode;
    std::string addressLine;
};

#endif