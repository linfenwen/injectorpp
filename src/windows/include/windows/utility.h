#ifndef INJECTORPP_UTILITY_H
#define INJECTORPP_UTILITY_H

#include <string>
#include <vector>

namespace InjectorPP
{
    class Utility
    {
    public:
        static std::string getLastErrorStdStr();
        static std::string w2m(const wchar_t* str);
        static std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
        static std::vector<std::string> split(const std::string &s, char delim);
    private:
        Utility();
        ~Utility();
        Utility(const Utility&);
    };
}

#endif
