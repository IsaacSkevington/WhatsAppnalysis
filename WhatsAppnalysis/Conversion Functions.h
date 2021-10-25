#pragma once
#include <sstream>
#include <string>
#include <Windows.h>
#include "String Processing Functions.h"
using namespace std;

////////////////////////////////CONVERSION FUNCTIONS///////////////////////////



//Converts integer to string
std::string itos(int x) {
    return std::to_string(x);
}

//Converts a string to an integer
int stoint(std::string string) {
	stringstream integerstring(string);
	int x = 0;
	integerstring >> x;
	return x;
}

//Converts string to LPCWSTR
LPCWSTR s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    LPCWSTR out = r.c_str();
    return out;
}

