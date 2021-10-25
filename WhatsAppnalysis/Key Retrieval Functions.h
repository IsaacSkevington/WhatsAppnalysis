#pragma once
#include <vector>
#include <map>
#include <string>
using namespace std;


/////////KEY RETRIEVAL FUCNTIONS



//Retrieves an array of the keys of a string : int map
std::vector<std::string> mapkeys(std::map<string, int> themap) {
	std::vector<std::string> keys;
	for (map<std::string, int>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}

//Retrieves an array of the keys of a string : int map
std::vector<std::string> mapkeys(std::map<string, string> themap) {
	std::vector<std::string> keys;
	for (map<std::string, std::string>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}

//Retrieves an array of the keys of a string : double map
std::vector<std::string> mapkeysdouble(std::map<string, double> themap) {
	std::vector<std::string> keys;
	for (map<std::string, double>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}


//Retrieves an array of the keys of a string : COLORREF map
std::vector<std::string> mapkeysColour(std::map<string, COLORREF> themap) {
	std::vector<std::string> keys;
	for (map<std::string, COLORREF>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}


//Retrieves an array of the keys of a string : string map
std::vector<LPCWSTR> mapkeysLstring(std::map<LPCWSTR, LPCWSTR> themap) {
	std::vector<LPCWSTR> keys;
	for (map<LPCWSTR, LPCWSTR>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}

//Retrieves an array of the keys of a string : bool map
std::vector<std::string> mapkeysbool(std::map<string, bool> themap) {
	std::vector<std::string> keys;
	for (map<std::string, bool>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}




//Retrieves the outer keys of a 2D string : (string : int) map
std::vector<std::string> mapkeys2D(std::map<std::string, std::map<string, int> >themap) {
	std::vector<std::string> keys;
	for (map<std::string, std::map<std::string, int> >::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}

std::vector<std::string> mapkeys2D(std::map<std::string, std::map<string, double> >themap) {
	std::vector<std::string> keys;
	for (map<std::string, std::map<std::string, double> >::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}


