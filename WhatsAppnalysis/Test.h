#pragma once
#include "Conversion Functions.h"
#include <Windows.h>
using namespace std;
///////////////////////////////////////////////TEST FUNCTIONS/////////////////////////////////////////
int TestNumber = 0;

//Creates a TestBox with data to be displayed 'dataIn' otherwise "Test" and the test number 
bool TestBox(std::string dataIn = "Test") {
	TestNumber++;
	MessageBoxA(NULL,
		("Test" + itos(TestNumber) + "\n" + dataIn).c_str(),
		"Test",
		MB_ICONINFORMATION);
	return true;
}

bool TestBoxL(LPCWSTR dataIn = L"Test") {
	TestNumber++;
	MessageBoxW(NULL,
		dataIn,
		L"Test",
		MB_ICONINFORMATION);
	return true;
}

