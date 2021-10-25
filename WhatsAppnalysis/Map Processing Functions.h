#pragma once
#include <vector>
#include <map>
#include <string>
#include "Array Processing Functions.h"
using namespace std;


/////////////////////////////////////////MAP PROCESSING FUNCTIONS/////////////////////////////////////

////////Number processing 


//Finds the number of a thing selected in a string, bool map
int numSelected(std::map<std::string, bool> themap) {
	int selected = 0;
	for (std::string key : mapkeysbool(themap)) {
		if (themap[key]) {
			selected++;
		}
	}
	return selected;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////VALUE RETRIEVAL FUNCTIONS

//Retrieves all the values of a string : double map
std::vector<double> mapvalues(std::map<string, double> themap) {
	std::vector<std::string> keys;
	std::vector <double> values;
	for (map<std::string, double>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	for (int i = 0; i < keys.size(); i++) {
		values.push_back(themap[keys[i]]);
	}
	return values;
}





//Retrieves all the values of a string : int map
std::vector<int> mapvalues(std::map<string, int> themap) {
	std::vector<std::string> keys;
	std::vector <int> values;
	for (map<std::string, int>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	for (int i = 0; i < keys.size(); i++) {
		values.push_back(themap[keys[i]]);
	}

	return values;
}

//Retrieves all the values of a string : COLORREF map
std::vector<COLORREF> mapvaluesColour(std::map<string, COLORREF> themap) {
	std::vector<std::string> keys;
	std::vector <COLORREF> values;
	for (map<std::string, COLORREF>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	for (int i = 0; i < keys.size(); i++) {
		values.push_back(themap[keys[i]]);
	}
	return values;
}


//Retrieves all the values of a string : string map
std::vector<LPCWSTR> mapvaluesLstring(std::map<LPCWSTR, LPCWSTR> themap) {
	std::vector<LPCWSTR> keys;
	std::vector <LPCWSTR> values;
	for (map<LPCWSTR, LPCWSTR>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	for (int i = 0; i < keys.size(); i++) {
		values.push_back(themap[keys[i]]);
	}
	return values;
}

//Retrieves all the values of a string : bool map
std::vector<bool> mapvaluesbool(std::map<string, bool> themap) {
	std::vector<std::string> keys;
	std::vector <bool> values;
	for (map<std::string, bool>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	for (int i = 0; i < keys.size(); i++) {
		values.push_back(themap[keys[i]]);
	}
	return values;
}

///////////////////

////////KEY RETRIEVAL FUNCTIONS






//Returns the keys of a string : int map in order smallest to largest of the int value
std::vector<std::string> mapsort(std::map<std::string, int> tosort) {
	std::vector<std::string> sortedkeys;
	std::vector<std::string> tosortkeys = mapkeys(tosort);
	std::vector<int> valuessorted = sortarr(mapvalues(tosort));
	for (int sorind = 0; sorind < valuessorted.size(); sorind++) {
		for (int unsorind = 0; unsorind < tosortkeys.size(); unsorind++) {
			if (tosort[tosortkeys[unsorind]] == valuessorted[sorind]) {
				sortedkeys.push_back(tosortkeys[unsorind]);
				tosort[tosortkeys[unsorind]] = -1;
				unsorind = tosortkeys.size() + 1;
			}
		}
	}
	return sortedkeys;
}

//////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///Map to array conversions


std::string* keyArray(std::map<std::string, bool> theMap) {
	std::vector<std::string> keys = mapkeysbool(theMap);
	std::string* theArray = new std::string[keys.size()];
	for (int i = 0; i < keys.size(); i++) {
		theArray[i] = keys[i];
	}
	return theArray;
}

bool* valueArray(std::map<std::string, bool> theMap) {
	std::vector<bool> values = mapvaluesbool(theMap);
	bool* theArray = new bool[values.size()];
	for (int i = 0; i < values.size(); i++) {
		theArray[i] = values[i];
	}
	return theArray;
}


std::map<std::string, bool> mapReform(std::string* keys, bool* values) {
	std::map<std::string, bool> theMap = {};
	for (int i = 0; i < sizeof(keys); i++) {
		theMap[keys[i]] = values[i];
	}
	return theMap;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////2D Map Functions

//Gets *string* of the highest value in the string:map(*string*:double) map
std::string getHighestInnerIndex2DMap(std::map <std::string, std::map<std::string, double> > themap) {
	std::vector<std::string> outerKeys = mapkeys2D(themap);
	std::map<std::string, std::vector<std::string> > innerKeys = {};
	for (std::string key : outerKeys) {
		innerKeys[key] = mapkeysdouble(themap[key]);
	}
	double highest = themap[outerKeys[0]][innerKeys[outerKeys[0]][0]];
	std::string highestInnerIndex = innerKeys[outerKeys[0]][0];
	for (std::string key : outerKeys) {
		for (std::string innerkey : innerKeys[key]) {
			if (themap[key][innerkey] > highest) {
				highest = themap[key][innerkey];
				highestInnerIndex = innerkey;
			}
		}
	}
	return highestInnerIndex;
}