#ifndef INJECTORPPTEST_FIXTURES_ADDRESS_H
#define INJECTORPPTEST_FIXTURES_ADDRESS_H

#include <string>

using std::string;

class Address
{
private:
    static Address* singleton;

public:
    static Address* GetSingleton();
    static void ClearSingleton();
    static string GetCapitalCityByCountry(string country);

    Address() {};
    void SetCountry(string country);
    string GetCountry();
    void GetCountryByReference(string& country);
    int GetZipCode();
    char* GetCity();
    void GetStreetName(string* street);
    void SetStreetName(char* streetName);
    void SetCountryOnSingleton(string country);
    void CallSetCountryInternal(string country);
    void CallOverloadedPrivate();

    static char *GetZipCode(char *country, char *streetName);
private:
    void SetCountryInternal(std::string country);
    static void CountReference();
    void OverloadedPrivate(int i);
    void OverloadedPrivate(char *name);
};

#endif