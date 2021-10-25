#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>
#include <fstream>
#include <windows.h>
#include <string>
#include "Version.h"

using namespace std;
#define PREFERENCES_FILE  "Data\\Preferences.ini"

std::vector<LPCTSTR> checkForUpdateOptions = { TEXT("Every Week"), TEXT("Every Day"), TEXT("Every Open") };
#define CU_EVERYWEEK 0
#define CU_EVERYDAY 1
#define CU_EVERYOPEN 2

std::vector<LPCTSTR> fileSaveModeOptions = { TEXT("No Password"), TEXT("System Password") };
#define FSM_NOPWD 0
#define FSM_SYSTEMPWD 1
#define PASSWORD L"Zx91TXAaXXw1rzkoGDfWy48uJsy1"

//Checks if a file exists
bool preferencesFileExists() {
	ifstream ifile(PREFERENCES_FILE);
	if (ifile) {
		return true;
	}
	else {
		return false;
	}
}


class Preferences {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& checkForUpdates;
		ar& darkMode;
		ar& fileSaveMode;
	}
public:
	int checkForUpdates = CU_EVERYDAY;
	bool darkMode = true;
	int fileSaveMode = FSM_SYSTEMPWD;
	LPCWSTR userPwd = L"";

	int getUpdatePeriod() {
		switch (checkForUpdates) {
		case CU_EVERYWEEK: return 604800;
		case CU_EVERYDAY: return 86400;
		case CU_EVERYOPEN: return 0;
		}
	}

	LPCWSTR getPassword() {
		switch (fileSaveMode) {
		case FSM_NOPWD: return NULL;
		case FSM_SYSTEMPWD: return PASSWORD;
		}
	}

};

Preferences getPreferences() {
	Preferences preferences;
	if (preferencesFileExists()) {
		std::ifstream inputFile(PREFERENCES_FILE);
		boost::archive::text_iarchive fileOpened(inputFile);
		fileOpened >> preferences;
		inputFile.close();
	}
	return preferences;
}

bool setPreferences(Preferences preferences) {
	std::ofstream outputFile(PREFERENCES_FILE);
	boost::archive::text_oarchive fileOpened(outputFile);
	fileOpened << preferences;
	outputFile.close();
	return true;
}












BOOST_CLASS_VERSION(Preferences, CURRENTFILEVERSION);



