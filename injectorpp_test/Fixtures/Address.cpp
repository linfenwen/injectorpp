#include <string>
#include <exception>

#include "Address.h"

using std::string;

Address* Address::singleton = NULL;

void Address::SetCountry(string country)
{
    throw;
}

string Address::GetCountry()
{
    throw;
}

void Address::GetCountryByReference(string& country)
{
    throw;
}


int Address::GetZipCode()
{
    return -1;
}

char* Address::GetCity()
{
    throw;
}

Address* Address::GetSingleton()
{
    if (singleton == NULL)
        singleton = new Address();
    return singleton;
}

void Address::ClearSingleton()
{
    CountReference();
    if (singleton != NULL)
    {
        delete singleton;
        singleton = NULL;
    }
}

string Address::GetCapitalCityByCountry(string country)
{
    throw;
}

void Address::SetStreetName(char* streetName)
{
    throw;
}

void Address::GetStreetName(string* street)
{
    *street = "";
}

void Address::SetCountryOnSingleton(string country)
{
    Address::GetSingleton()->SetCountryInternal(country);
}

void Address::CallSetCountryInternal(string country)
{
    SetCountryInternal(country);
}

void Address::SetCountryInternal(std::string country)
{
    // do something here
}
void Address::CallOverloadedPrivate()
{
    OverloadedPrivate(1);
}

void Address::OverloadedPrivate(int i)
{
    // do something here
}
void Address::OverloadedPrivate(char *name)
{
    // do something here
}

void Address::CountReference()
{
    throw;
}

char* Address::GetZipCode(char *country, char *streetName)
{
    throw;
}


