#pragma once
#include <stdlib.h>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "Line.h"
#include "Validation.h"
#include "Key Retrieval Functions.h"
#include "Test.h"
using namespace std;
//////////////////////////////////ARRAY PROCESSING FUNCTIONS////////////////////////////

//Finds Smallest Value in an array
int findSmallest(std::vector<double> array) {
	int currentSmallest = 0;
	for (int i = 1; i < array.size(); i++) {
		if (array[i] < array[currentSmallest]) {
			currentSmallest = i;
		}
	}
	return currentSmallest;
}

//Finds Largest Value in an array
int findLargest(std::vector<double> array) {
	int currentLargest = 0;
	for (int i = 1; i < array.size(); i++) {
		if (array[i] < array[currentLargest]) {
			currentLargest = i;
		}
	}
	return currentLargest;
}

//Reverses the order of terms in an int array
std::vector <int> arrayreverse(std::vector <int> reverse) {
	std::vector <int> reversed;
	for (int i = 0; i < reverse.size(); i++) {
		reversed.push_back(reverse[(reverse.size() - 1) - i]);

	}
	return reversed;
}

//Reverses the order of terms in an double array
std::vector <double> arrayreverse(std::vector <double> reverse) {
	std::vector <double> reversed;
	for (int i = 0; i < reverse.size(); i++) {
		reversed.push_back(reverse[(reverse.size() - 1) - i]);

	}
	return reversed;
}


/////////SORTING FUNCTIONS



//Sorts an integer array in order smallest to largest
std::vector <int> sortarr(std::vector <int> tosort) {
	std::vector <int> sorted;
	int sortedsize;
	sorted.push_back(0);
	bool added = false;
	for (int sortnum = 0; sortnum < tosort.size(); sortnum++) {
		sortedsize = sorted.size();
		for (int sortednum = 0; sortednum < sortedsize; sortednum++) {
			if (tosort[sortnum] > sorted[sortednum]) {
				sorted.insert(sorted.begin() + sortednum, tosort[sortnum]);
				added = true;
				sortednum = sortedsize + 1;
			}

		}

		if (!added) {
			sorted.push_back(tosort[sortnum]);
		}

	}
	return sorted;
}

//Sorts an double array in order smallest to largest
std::vector <double> sortarr(std::vector <double> tosort) {
	std::vector<double> sorted = {};
	int arraySize = (int)tosort.size();
	int smallest;
	for (int term = 0; term < arraySize; term++) {
		smallest = findSmallest(tosort);
		sorted.push_back(tosort[smallest]);
		tosort.erase(tosort.begin() + smallest);
		}

	return sorted;
}

//Sorts 2D array smallest to largest of the terms in list 0
std::vector <std::vector<double> > sort2DArray(std::vector <std::vector<double> > tosort) {
	std::vector<std::vector <double> > sorted = { {}, {} };
	int arraySize = (int)tosort[0].size();
	int smallest;
	for (int term = 0; term < arraySize; term++) {
		smallest = findSmallest(tosort[0]);
		sorted[0].push_back(tosort[0][smallest]);
		sorted[1].push_back(tosort[1][smallest]);
		tosort[0].erase(tosort[0].begin() + smallest);
		tosort[1].erase(tosort[1].begin() + smallest);
	}
	
	return sorted;
}


//Sorts an array of days from most to least recent with 'All Time' placed at the top
std::vector<std::string> dayArraySort(std::vector<std::string> theArray) {
	std::vector<std::string> sortedArray = {};
	std::vector<std::string> intermediateArray;
	std::map<std::string, int> mapSorter = {};
	std::string newDate = "";
	for (int i = 0; i < theArray.size(); i++) {
		if (theArray[i] == "All Time") {
			newDate = theArray[i];
		}
		else {
			newDate = theArray[i].substr(6, 4) + theArray[i].substr(3, 2) + theArray[i].substr(0, 2);
		}
		mapSorter[newDate] = 1;
	}
	intermediateArray = mapkeys(mapSorter);
	for (int i = 0; i < intermediateArray.size(); i++) {
		if (intermediateArray[i] == "All Time") {
		}
		else {
			intermediateArray[i] = intermediateArray[i].substr(6, 2) + "/" + intermediateArray[i].substr(4, 2) + "/" + intermediateArray[i].substr(0, 4);
		}
	}
	for (int i = (int)intermediateArray.size() - 1; i >= 0; i -= 1) {
		sortedArray.push_back(intermediateArray[i]);
	}
	return(sortedArray);
}


//Sorts an array of days from most to least recent with 'All Time' placed at the top
std::vector<std::string> dateArraySort(std::vector<std::string> theArray) {
	std::vector<std::string> sortedArray = {};
	std::vector<std::string> intermediateArray;
	std::map<std::string, int> mapSorter = {};
	std::string newDate = "";
	for (int i = 0; i < theArray.size(); i++) {
		if (theArray[i] == "All Time") {
			newDate = theArray[i];
		}
		else {
			newDate = theArray[i].substr(3, 4) + theArray[i].substr(0, 2);
		}
		mapSorter[newDate] = 1;
	}
	intermediateArray = mapkeys(mapSorter);
	for (int i = 0; i < intermediateArray.size(); i++) {
		if (intermediateArray[i] == "All Time") {
		}
		else {
			intermediateArray[i] = intermediateArray[i].substr(4, 2) + "/" + intermediateArray[i].substr(0, 4);
		}
	}
	for (int i = (int)intermediateArray.size() - 1; i >= 0; i -= 1) {
		sortedArray.push_back(intermediateArray[i]);
	}
	return(sortedArray);
}



//Sorts an array of people from A-Z with 'Total Messages' at the top
std::vector<std::string> peopleArraySort(std::vector<std::string> theArray) {
	std::vector<std::string> sortedArray = {};
	std::vector<std::string> intermediateArray;
	std::map<std::string, int> mapSorter = {};
	for (int i = 0; i < theArray.size(); i++) {
		if (theArray[i] != "Total Messages") {
			mapSorter[theArray[i]] = 1;
		}
	}
	intermediateArray = mapkeys(mapSorter);
	sortedArray.push_back("Total Messages");
	for (int i = 0; i < intermediateArray.size(); i++) {
		sortedArray.push_back(intermediateArray[i]);
	}
	return(sortedArray);
}



////////////


///////////////////////////////////////////////////////////////////////////////////////////////////




