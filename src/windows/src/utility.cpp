#include <Windows.h>
#include <sstream>

#include "windows/utility.h"

namespace InjectorPP
{
    Utility::Utility()
    {
    }

    Utility::~Utility()
    {
    }

    std::string Utility::GetLastErrorStdStr()
    {
        DWORD error = GetLastError();
        if (error)
        {
            LPVOID lpMsgBuf;
            DWORD bufLen = FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                error,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf,
                0, NULL);
            if (bufLen)
            {
                LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
                std::string result(lpMsgStr, lpMsgStr + bufLen);

                LocalFree(lpMsgBuf);

                return result;
            }
        }

        return std::string();
    }

    std::string Utility::W2M(const wchar_t* str)
    {
        int muLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
        if (muLen == 0)
        {
            return "";
        }

        char* muValue = new char[muLen];
        int nRet = WideCharToMultiByte(CP_ACP, 0, str, -1, muValue, muLen, NULL, NULL);
        if (nRet == 0)
        {
            delete[] muValue;
            return "";
        }

        std::string temp = muValue;
        delete[] muValue;

        return temp;
    }

    std::vector<std::string>& Utility::Split(const std::string &s, char delim, std::vector<std::string> &elems)
    {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            elems.push_back(item);
        }

        return elems;
    }


    std::vector<std::string> Utility::Split(const std::string &s, char delim)
    {
        std::vector<std::string> elems;
        Utility::Split(s, delim, elems);

        return elems;
    }
}