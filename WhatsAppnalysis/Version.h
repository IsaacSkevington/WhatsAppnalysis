#pragma once
#include "Map Processing Functions.h"
#include <fstream>
#include <ctime>
#define LASTUPDATEFILE "Update\\LastUpdate.txt"
#define LASTUPDATECHECKFILE "Update\\LastUpdateCheck.txt"


//Version Text Doc
#define VERSIONS "Update\\Release Notes.txt"

//Line Numbers in Doc
#define LINES 4
#define VERSIONLINENUM 2
#define UPDATERVERSIONLINENUM 4
#define PROGRAMVERSION 0
#define UPDATERVERSION 1
#define VERSIONRETRIEVEERROR -1
#define ERRORHIT "-1"
#define NOERRORHIT "0"


//Version Structure

#define MAINVERSION 0
#define SUBVERSION 1
#define SUBSUBVERSION 2


//Current Program Version
#define CURRENTVERSION "0.8.14"

//Current Updater Version
#define CURRENTUPDATERVERSION "0.7.5"
#define CURRENTSETUPVERSION "1.0.1"

//File Version
#define CURRENTFILEVERSION 2

#define CHARSPERLINE 90


class VersionInfo {
public:
	VersionInfo(std::string);
	std::string versionNumber;
	std::vector<std::string> newFeatures;
	std::vector <std::string> bugsFixed;
	bool beta = false;
	bool release = false;

	void addBugFixed(std::string bug) {
		std::string newBug = "";
		int lastSpace = 0;
		int lastSpaceSubstringed = 0;
		if (bug.size() > CHARSPERLINE) {
			for (int i = 1; i < bug.size(); i++) {
				if (bug[i] == ' ') {
					lastSpace = i;
				}
				if (i % CHARSPERLINE == 0) {
					newBug += bug.substr(lastSpaceSubstringed, (lastSpace - lastSpaceSubstringed)) + "\n		 ";
					lastSpaceSubstringed = lastSpace;
				}
			}
			newBug += bug.substr(lastSpaceSubstringed, (bug.size()) - lastSpaceSubstringed);
		}
		else {
			newBug = bug;
		}
		bugsFixed.push_back(newBug);
	}
	void addNewFeature(std::string feature) {
		std::string newFeature = "";
		int lastSpace = 0;
		int lastSpaceSubstringed = 0;
		if (feature.size() > CHARSPERLINE) {
			for (int i = 1; i < feature.size(); i++) {
				if (feature[i] == ' ') {
					lastSpace = i;
				}
				if (i % CHARSPERLINE == 0) {
					newFeature += feature.substr(lastSpaceSubstringed, (lastSpace - lastSpaceSubstringed)) + "\n		 ";
					lastSpaceSubstringed = lastSpace;
				}
			}
			newFeature += feature.substr(lastSpaceSubstringed, (feature.size()) - lastSpaceSubstringed);
		}
		else {
			newFeature = feature;
		}
		newFeatures.push_back(newFeature);
	}

	std::string reformInfo() {
		std::string info = "";
		if (not(newFeatures.size() == 0 && bugsFixed.size() == 0)) {
			if (beta) {
				info += "Version " + versionNumber + " [BETA]" +"\n";
			}
			else if (release) {
				info += "Version " + versionNumber + " - Current Release" + "\n";
			}
			else {
				info += "Version " + versionNumber + "\n";
			}
			if (newFeatures.size() != 0) {
				info += "	New Features: \n";
				for (std::string newFeature : newFeatures) {
					info += "		-" + newFeature + "\n";
				}
				info += "\n";
			}
			if (bugsFixed.size() != 0) {
				info += "	Bugs Fixed: \n";
				for (std::string bugFixed : bugsFixed) {
					info += "		-" + bugFixed + "\n";
				}
				info += "\n";
			}
		}
		return info;
	}
};

VersionInfo::VersionInfo(std::string versionNumberIn) {
	versionNumber = versionNumberIn;
}

std::vector<VersionInfo> programVersions = {};
std::vector<VersionInfo> updaterVersions = {};










void setVersionInfo() {
	
	////////////////////////////////////////////////////////////////FILE///////////////////////////////////////////////////////////////
	//Format versionInfoPG[VersionNum]

	//////ADD HIGHER VERSIONS HERE///////
	

	///////////0.8.14
	VersionInfo versionInfoPG0814("0.8.14");

	//Features
	versionInfoPG0814.addNewFeature("Multiple phrases can now be entered for search");
	versionInfoPG0814.addNewFeature("Removed horizontal scrolling with arrows");
	versionInfoPG0814.addNewFeature("Added a graph view to the phrase searching tab");
	versionInfoPG0814.addNewFeature("Added searching for the same phrase again in a new file");
	versionInfoPG0814.addNewFeature("Made minor changes to the visual look of the graphs");
	versionInfoPG0814.addNewFeature("Increase the length of the version info dialog");
	//

	//Bugs Fixed
	versionInfoPG0814.addBugFixed("Fixed an issue where the down button on the phrase tab's month view checkbox "
								  "scroll didn't work");
	versionInfoPG0814.addBugFixed("Fixed an issue of data not loading unless the window recieved a command");
	versionInfoPG0814.addBugFixed("Fixed an issue of the line of best fit check box text being partially obscurred");
	versionInfoPG0814.addBugFixed("Fixed an issue where the graph Y-Axis values didn't line up with the"
								  "coordinate values");
	versionInfoPG0814.addBugFixed("Fixed an issue of the incorrect tab being selected on program restore");


	//

	programVersions.push_back(versionInfoPG0814);
	//////////0.8.14


	//////////0.8.13
	VersionInfo versionInfoPG0813("0.8.13");

	//Features
	//

	//Bugs Fixed
	versionInfoPG0813.addBugFixed("Fixed horizontal scroll issue");
	versionInfoPG0813.addBugFixed("Permissions to files handled after setup to stop failure to update"
								  "when installed on Windows drive");
	//

	programVersions.push_back(versionInfoPG0813);
	/////////0.8.13




	///////0.8.11
	VersionInfo versionInfoPG0811("0.8.11");

	//Features
	versionInfoPG0811.addNewFeature("Added an check for updates now button in the preferences dialog");
	versionInfoPG0811.addNewFeature("Added display of the update check and physical update times to the"
									"versions dialog");
	//

	//Bugs
	versionInfoPG0811.addBugFixed("Fixed issue with update details going off screen");
	versionInfoPG0811.addBugFixed("Fixed 'Ghost' OK button in the versions dialog");
	versionInfoPG0811.addBugFixed("Removed unused 'Restore data mode' setting in preferences dialog");
	//

	programVersions.push_back(versionInfoPG0811);
	////////0.8.11


	////////0.8.10
	VersionInfo versionInfoPG0810("0.8.10");

	//Features
	versionInfoPG0810.addNewFeature("Added version info viewing");
	//

	//Bugs
	versionInfoPG0810.addBugFixed("Fixed problem with light mode");
	versionInfoPG0810.addBugFixed("Fixed overscroll");
	//

	programVersions.push_back(versionInfoPG0810);
	////////0.8.10
	



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////UPDATER/////////////////////////////////////////////////////////////
	//Format versionInfoUD[VersionNum]

	///////////ADD HIGHER VERSIONS HERE///////


	//////0.7.5
	VersionInfo versionInfoUD075("0.7.5");

	//Features Added
	versionInfoUD075.addNewFeature("Updater gives a larger view of what is going on while update is in progress");
	//

	//Bugs Fixed
	versionInfoUD075.addBugFixed("Fixed a bug where the updater didn't delete the previous program");
	versionInfoUD075.addBugFixed("Added a dialog to allow the user to delete the file if access is initially denied");
	//

	updaterVersions.push_back(versionInfoUD075);
	/////0.7.5




	//////0.7.3
	VersionInfo versionInfoUD073("0.7.3");

	//Bugs Fixed
	versionInfoUD073.addBugFixed("Updater now waits 5 seconds for the program to close before download");
	//
	updaterVersions.push_back(versionInfoUD073);






	/////0.7.2
	VersionInfo versionInfoUD072("0.7.2");

	//Features
	versionInfoUD072.addNewFeature("Updater Added");
	//

	updaterVersions.push_back(versionInfoUD072);

	/////0.7.2




	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}







std::map<int, std::string> getReleaseVersions() {
	std::map<int, std::string> versions = {};
	int lineNum = 1;
	std::string line;
	ifstream versionFile(VERSIONS);
	if (versionFile.is_open())
	{
		while (getline(versionFile, line))
		{
			switch (lineNum) {
			case VERSIONLINENUM: {
				versions[PROGRAMVERSION] = line;
				break;
			}
			case UPDATERVERSIONLINENUM: {
				versions[UPDATERVERSION] = line;
				break;
			}
			}
			lineNum++;
		}
		versionFile.close();
	}
	else {
		versions[VERSIONRETRIEVEERROR] = ERRORHIT;
		return versions;
	}
	versions[VERSIONRETRIEVEERROR] = NOERRORHIT;
	return versions;
}

std::map<int, int> parseVersion(std::string version) {
	std::map<int, int> versionDetails = {};
	int versionsFound = 0;
	int lastdot = -1;
	for (int i = 0; i < version.size(); i++) {
		if (version[i] == '.') {
			versionDetails[versionsFound] = stoi(version.substr(lastdot + 1, (i - 1) - lastdot));
			lastdot = i;
			versionsFound++;
		}
	}
	versionDetails[versionsFound] = stoi(version.substr(lastdot + 1, version.size() - 1));
	return versionDetails;
}


bool isGreaterVersion(std::string baseVersion, std::string compareVersion) {
	std::map<int, int> baseVersionDetails = parseVersion(baseVersion);
	std::map<int, int> compareVersionDetails = parseVersion(compareVersion);
	if (compareVersionDetails[MAINVERSION] > baseVersionDetails[MAINVERSION]) {
		return true;
	}
	else {
		if (compareVersionDetails[SUBVERSION] > baseVersionDetails[SUBVERSION]) {
			return true;
		}
		else {
			if (compareVersionDetails[SUBSUBVERSION] > baseVersionDetails[SUBSUBVERSION]) {
				return true;
			}
			else {
				return false;
			}
		}
	}
}

bool isGreaterThanRelease(std::string version, int aspectID) {
	std::map<int, std::string> versions = getReleaseVersions();
	if (versions[VERSIONRETRIEVEERROR] != ERRORHIT) {
		std::string inspectVersion = versions[aspectID];
		if (isGreaterVersion(inspectVersion, version)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool isRelease(std::string versionNum, int aspectID) {
	std::map<int, std::string> versions = getReleaseVersions();
	if (versions[aspectID] == versionNum) {
		return true;
	}
	else {
		return false;
	}

}


std::string getLastUpdateTime() {
	std::string timeChecked;
	ifstream lastUpdateFile(LASTUPDATEFILE);
	if (lastUpdateFile.is_open())
	{
		getline(lastUpdateFile, timeChecked);
		lastUpdateFile.close();
	}
	else {
		return "Never";
	}
	if (stoi(timeChecked) == 0) {
		return "Never";
	}
	time_t time = stoi(timeChecked);
	tm buf;
	localtime_s(&buf, &time);
	char str[256];
	asctime_s(str, sizeof str, &buf);
	return str;

}

std::string getLastUpdateCheckTime() {
	std::string timeChecked;
	ifstream lastUpdateFile(LASTUPDATECHECKFILE);
	if (lastUpdateFile.is_open())
	{
		getline(lastUpdateFile, timeChecked);
		lastUpdateFile.close();
	}
	else {
		return "Never";
	}
	if (stoi(timeChecked) == 0) {
		return "Never";
	}
	time_t time = stoi(timeChecked);
	tm buf;
	localtime_s(&buf, &time);
	char str[256];
	asctime_s(str, sizeof str, &buf);
	return str;
}

std::string getVersionInfo() {
	std::string info = "";
	info += "PROGRAM INFO: \n\n";
	for (VersionInfo version : programVersions) {
		if (version.versionNumber == CURRENTVERSION) {
			info += "[Current]\n";
		}
		if (isGreaterThanRelease(version.versionNumber, PROGRAMVERSION)) {
			version.beta = true;
		}
		else if (isRelease(version.versionNumber, PROGRAMVERSION)) {
			version.release = true;
		}
		info += version.reformInfo();
	}
	for (int i = 0; i < CHARSPERLINE + 40; i++) {
		info += "-";
	}
	info += "\n\n";
	info += "UPDATER INFO: \n\n";
	for (VersionInfo version : updaterVersions) {
		if (version.versionNumber == CURRENTUPDATERVERSION) {
			info += "[Current]\n";
		}
		if (isGreaterThanRelease(version.versionNumber, UPDATERVERSION)) {
			version.beta = true;
		}
		else if (isRelease(version.versionNumber, UPDATERVERSION)) {
			version.release = true;
		}
		info += version.reformInfo();
	}
	for (int i = 0; i < CHARSPERLINE + 40; i++) {
		info += "-";
	}
	info += "\n\n";
	info += "Last checked for updates on " + getLastUpdateCheckTime() + "\n";
	info += "Last updated on " + getLastUpdateTime() + "\n\n";
	std::string currVersion = CURRENTVERSION;
	info += "WhatsAppnalysis Version " + currVersion + " | Copyright 2020 Isaac Skevington";
	return info;

}


int getLines(std::string string) {
	int lines = 0;
	if (string.size() > 1) {
		lines++;
		for (char character : string) {
			if (character == '\n') {
				lines++;
			}
		}
	}
	return lines;
}


