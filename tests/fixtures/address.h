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
        this->m_zipCode = rhs.m_zipCode;
        this->m_addressLine = rhs.m_addressLine;
    }

    Address& operator=(Address rhs)
    {
        this->m_zipCode = rhs.m_zipCode;
        this->m_addressLine = rhs.m_addressLine;

        return *this;
    }

    bool operator==(const Address& rhs) const
    {
        return this->m_zipCode == rhs.m_zipCode && this->m_addressLine == rhs.m_addressLine;
    }

    std::string getZipCode();
    void setZipCode(const std::string& zipCode);

    std::string getAddressLine();
    void setAddressLine(const std::string& addressLine);

private:
    std::string m_zipCode;
    std::string m_addressLine;
};

#endif