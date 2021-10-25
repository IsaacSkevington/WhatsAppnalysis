#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>
#include <fstream>
#include "DataStore.h"
#include "Test.h"
#include "NewDefinitions.h"
#include "graphing.h"
#include "Array Processing Functions.h"
#include "MessageStreamProcessor.h"
#include "DataStore.h"
#include "Map Processing Functions.h"
#include "Encryption.h"
#include "Decryption.h"
#include "Version.h"
#include <filesystem>


using namespace std;
std::string docStartPhrase = "serialization::archive";



//The data that is saved
class SaveData {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& messageAnalyser;
		ar& dataStore;
		ar& windowsCreatedBeforestore;
		if (version > 1) {
			ar& itemSelectedBeforestore;
		}
		//1
		ar& phraseEnteredstore;
		ar& idDisplayedText1store;
		ar& peopleSelected1store;
		ar& monthsSelected1store;
		ar& weeksSelected1store;
		ar& currentTopDataLine1store;
		ar& currentBottomDataLine1store;
		ar& currentTopCheckboxNamesLine1store;
		ar& currentBottomCheckboxNamesLine1store;
		ar& currentTopCheckboxWeeksLine1store;
		ar& currentBottomCheckboxWeeksLine1store;
		ar& currentTopCheckboxMonthsLine1store;
		ar& currentBottomCheckboxMonthsLine1store;
		ar& periodSelected1store;
		ar& firstTimePhrasestore;
		ar& phraseSearchDatastore;
		if (version > 1) {
			ar& phraseTimesGraphstore;
		}
		//2
		ar& idDisplayedText2store;
		ar& peopleSelected2store;
		ar& monthsSelected2store;
		ar& weeksSelected2store;
		ar& currentTopDataLine2store;
		ar& currentBottomDataLine2store;
		ar& currentTopCheckboxNamesLine2store;
		ar& currentBottomCheckboxNamesLine2store;
		ar& currentTopCheckboxMonthsLine2store;
		ar& currentBottomCheckboxMonthsLine2store;
		ar& currentTopCheckboxWeeksLine2store;
		ar& currentBottomCheckboxWeeksLine2store;
		ar& periodSelected2store;
		ar& messagesSentGraphstore;
		//3
		ar& startDatestore;
		ar& endDatestore;
		ar& idDisplayedText3store;
		ar& peopleSelected3store;
		ar& currentTopDataLine3store;
		ar& currentBottomDataLine3store;
		ar& currentTopCheckboxNamesLine3store;
		ar& currentBottomCheckboxNamesLine3store;
		ar& messagesOverTimeGraphstore;

		//4
		ar& idDisplayedText4store;
		ar& peopleSelected4store;
		ar& monthsSelected4store;
		ar& weeksSelected4store;
		ar& currentTopDataLine4store;
		ar& currentBottomDataLine4store;
		ar& currentTopCheckboxNamesLine4store;
		ar& currentBottomCheckboxNamesLine4store;
		ar& currentTopCheckboxMonthsLine4store;
		ar& currentBottomCheckboxMonthsLine4store;
		ar& currentTopCheckboxWeeksLine4store;
		ar& currentBottomCheckboxWeeksLine4store;
		ar& periodSelected4store;
		if (version > 1) {
			ar& replyTimesGraphstore;
		}

		//5
		ar& sendingTimesGraphstore;
		ar& peopleSelected5store;
		ar& currentTopCheckboxNamesLine5store;
		ar& currentBottomCheckboxNamesLine5store;
		//Password
		ar& password;
	}


public:
	Msgstreamprocessor messageAnalyser;
	DataStorage dataStore;
	bool windowsCreatedBeforestore;
	int itemSelectedBeforestore = 0;
	//1
	std::string phraseEnteredstore;
	std::string idDisplayedText1store;
	std::map <std::string, bool> peopleSelected1store;
	std::map <std::string, bool> monthsSelected1store;
	std::map <std::string, bool> weeksSelected1store;
	int currentTopDataLine1store;
	int currentBottomDataLine1store;
	int currentTopCheckboxNamesLine1store;
	int currentBottomCheckboxNamesLine1store;
	int currentTopCheckboxWeeksLine1store;
	int currentBottomCheckboxWeeksLine1store;
	int currentTopCheckboxMonthsLine1store;
	int currentBottomCheckboxMonthsLine1store;
	std::string periodSelected1store;
	bool firstTimePhrasestore;
	PhraseSearchData phraseSearchDatastore;
	LineGraph phraseTimesGraphstore;

	//2
	std::string idDisplayedText2store;
	std::map <std::string, bool> peopleSelected2store;
	std::map <std::string, bool> monthsSelected2store;
	std::map<std::string, bool> weeksSelected2store;
	int currentTopDataLine2store;
	int currentBottomDataLine2store;
	int currentTopCheckboxNamesLine2store;
	int currentBottomCheckboxNamesLine2store;
	int currentTopCheckboxMonthsLine2store;
	int currentBottomCheckboxMonthsLine2store;
	int currentTopCheckboxWeeksLine2store;
	int currentBottomCheckboxWeeksLine2store;
	std::string periodSelected2store;
	BarGraph messagesSentGraphstore;

	//3
	std::string startDatestore;
	std::string endDatestore;
	std::string idDisplayedText3store;
	std::map <std::string, bool> peopleSelected3store;
	int currentTopDataLine3store;
	int currentBottomDataLine3store;
	int currentTopCheckboxNamesLine3store;
	int currentBottomCheckboxNamesLine3store;
	LineGraph messagesOverTimeGraphstore;
		
	//4
	std::string idDisplayedText4store;
	std::map <std::string, bool> peopleSelected4store;
	std::map <std::string, bool> monthsSelected4store;
	std::map<std::string, bool> weeksSelected4store;
	int currentTopDataLine4store;
	int currentBottomDataLine4store;
	int currentTopCheckboxNamesLine4store;
	int currentBottomCheckboxNamesLine4store;
	int currentTopCheckboxMonthsLine4store;
	int currentBottomCheckboxMonthsLine4store;
	int currentTopCheckboxWeeksLine4store;
	int currentBottomCheckboxWeeksLine4store;
	std::string periodSelected4store;
	LineGraph replyTimesGraphstore;

	//5
	LineGraph sendingTimesGraphstore;
	std::map <std::string, bool> peopleSelected5store;
	int currentTopCheckboxNamesLine5store;
	int currentBottomCheckboxNamesLine5store;
	
	//Password
	std::wstring password;
	
public:
	//Fills the saved data object with the current program data
	void fill(Msgstreamprocessor messageAnalyserIn, DataStorage dataStoreIn){
		messageAnalyser = messageAnalyserIn;
		dataStore = dataStoreIn;
		windowsCreatedBeforestore = windowsCreatedBefore;
		itemSelectedBeforestore = itemSelectedBefore;
		//1
		phraseEnteredstore = phraseEntered;
		idDisplayedText1store = idDisplayedText1;
		peopleSelected1store = peopleSelected1;
		monthsSelected1store = monthsSelected1;
		weeksSelected1store = weeksSelected1;
		currentTopDataLine1store = currentTopDataLine1;
		currentBottomDataLine1store = currentBottomDataLine1;
		currentTopCheckboxNamesLine1store = currentTopCheckboxNamesLine1;
		currentBottomCheckboxNamesLine1store = currentBottomCheckboxNamesLine1;
		currentTopCheckboxWeeksLine1store = currentTopCheckboxWeeksLine1;
		currentBottomCheckboxWeeksLine1store = currentBottomCheckboxWeeksLine1;
		currentTopCheckboxMonthsLine1store = currentTopCheckboxMonthsLine1;
		currentBottomCheckboxMonthsLine1store = currentBottomCheckboxMonthsLine1;
		periodSelected1store = periodSelected1;
		firstTimePhrasestore = firstTimePhrase;
		phraseSearchDatastore = phraseSearchData;
		phraseTimesGraphstore = phraseTimesGraph;

		//2
		idDisplayedText2store = idDisplayedText2;
		peopleSelected2store = peopleSelected2;
		monthsSelected2store = monthsSelected2;
		weeksSelected2store = weeksSelected2;
	    currentTopDataLine2store = currentTopDataLine2;
		currentBottomDataLine2store = currentBottomDataLine2;
		currentTopCheckboxNamesLine2store = currentTopCheckboxNamesLine2;
		currentBottomCheckboxNamesLine2store = currentBottomCheckboxNamesLine2;
		currentTopCheckboxMonthsLine2store = currentTopCheckboxMonthsLine2;
		currentBottomCheckboxMonthsLine2store = currentBottomCheckboxMonthsLine2;
		currentTopCheckboxWeeksLine2store = currentTopCheckboxWeeksLine2;
		currentBottomCheckboxWeeksLine2store = currentBottomCheckboxWeeksLine2;
		periodSelected2store = periodSelected2;
		messagesSentGraphstore = messagesSentGraph;

		//3
		startDatestore = startDate;
		endDatestore = endDate;
		idDisplayedText3store = endDate;
		peopleSelected3store = peopleSelected3;
		currentTopDataLine3store = currentTopDataLine3;
		currentBottomDataLine3store = currentBottomDataLine3;
		currentTopCheckboxNamesLine3store = currentTopCheckboxNamesLine3;
		currentBottomCheckboxNamesLine3store = currentBottomCheckboxNamesLine3;
		messagesOverTimeGraphstore = messagesOverTimeGraph;

		//4
		idDisplayedText4store = idDisplayedText4;
		peopleSelected4store = peopleSelected4;
		monthsSelected4store = monthsSelected4;
		weeksSelected4store = weeksSelected4;
		currentTopDataLine4store = currentTopDataLine4;
		currentBottomDataLine4store = currentBottomDataLine4;
		currentTopCheckboxNamesLine4store = currentTopCheckboxNamesLine4;
		currentBottomCheckboxNamesLine4store = currentBottomCheckboxNamesLine4;
		currentTopCheckboxMonthsLine4store = currentTopCheckboxMonthsLine4;
		currentBottomCheckboxMonthsLine4store;
		currentTopCheckboxWeeksLine4store = currentTopCheckboxWeeksLine4store;
		currentBottomCheckboxWeeksLine4store = currentBottomCheckboxWeeksLine4;
		periodSelected4store = periodSelected4;
		replyTimesGraphstore = replyTimesGraph;

		//5
		sendingTimesGraphstore = sendingTimesGraph;
		peopleSelected5store = peopleSelected5;
		currentTopCheckboxNamesLine5store = currentTopCheckboxNamesLine5;
		currentBottomCheckboxNamesLine5store = currentBottomCheckboxNamesLine5;

		
		//Password
		password = passwordIn;
	}
	//Loads the contents of the save data object into the current program instance
	void load() {
		setSelectedTab(handlefortabmenu, handlesfordatawindows, itemSelectedBeforestore);
		phraseEntered = phraseEnteredstore;
		idDisplayedText1 = idDisplayedText1store;
		peopleSelected1 = peopleSelected1store;
		monthsSelected1 = monthsSelected1store;
		weeksSelected1 = weeksSelected1store;
		currentTopDataLine1 = currentTopDataLine1store;
		currentBottomDataLine1 = currentBottomDataLine1store;
		currentBottomDataLine1 = currentBottomDataLine1store;
		currentTopCheckboxNamesLine1 = currentTopCheckboxNamesLine1store;
		currentBottomCheckboxNamesLine1 = currentBottomCheckboxNamesLine1store;
		currentTopCheckboxWeeksLine1 = currentTopCheckboxWeeksLine1store;
		currentBottomCheckboxWeeksLine1 = currentBottomCheckboxWeeksLine1store;
		currentTopCheckboxMonthsLine1 = currentTopCheckboxMonthsLine1store;
		currentBottomCheckboxMonthsLine1 = currentBottomCheckboxMonthsLine1store;
		periodSelected1 = periodSelected1store;
		firstTimePhrase = firstTimePhrasestore;
		phraseSearchData = phraseSearchDatastore;
		phraseTimesGraph = phraseTimesGraphstore;
		phraseTimesGraph.window = handleforphraseTimesGraph;

		//2
		idDisplayedText2 = idDisplayedText2store;
		peopleSelected2 = peopleSelected2store;
		monthsSelected2 = monthsSelected2store;
		weeksSelected2 = weeksSelected2store;
		currentTopDataLine2 = currentTopDataLine2store;
		currentBottomDataLine2 = currentBottomDataLine2store;
		currentTopCheckboxNamesLine2 = currentTopCheckboxNamesLine2store;
		currentBottomCheckboxNamesLine2 = currentBottomCheckboxNamesLine2store;
		currentTopCheckboxMonthsLine2 = currentTopCheckboxMonthsLine2store;
		currentBottomCheckboxMonthsLine2 = currentBottomCheckboxMonthsLine2store;
		currentTopCheckboxWeeksLine2 = currentTopCheckboxWeeksLine2store;
		currentBottomCheckboxWeeksLine2 = currentBottomCheckboxWeeksLine2store;
		periodSelected2 = periodSelected2store;
		messagesSentGraph = messagesSentGraphstore;
		messagesSentGraph.window = handleformessagesSentGraph;

		//3
		startDate = startDatestore;
		endDate = endDatestore;
		idDisplayedText3 = endDatestore;
		peopleSelected3 = peopleSelected3store;
		currentTopDataLine3 = currentTopDataLine3store;
		currentBottomDataLine3 = currentBottomDataLine3store;
		currentTopCheckboxNamesLine3 = currentTopCheckboxNamesLine3store;
		currentBottomCheckboxNamesLine3 = currentBottomCheckboxNamesLine3store;
		messagesOverTimeGraph = messagesOverTimeGraphstore;
		messagesOverTimeGraph.window = handleformessagesOverTimeGraph;

		//4
		idDisplayedText4 = idDisplayedText4store;
		peopleSelected4 = peopleSelected4store;
		monthsSelected4 = monthsSelected4store;
		weeksSelected4 = weeksSelected4store;
		currentTopDataLine4 = currentTopDataLine4store;
		currentBottomDataLine4 = currentBottomDataLine4store;
		currentTopCheckboxNamesLine4 = currentTopCheckboxNamesLine4store;
		currentBottomCheckboxNamesLine4 = currentBottomCheckboxNamesLine4store;
		currentTopCheckboxMonthsLine4 = currentTopCheckboxMonthsLine4store;
		currentBottomCheckboxMonthsLine4store;
		currentTopCheckboxWeeksLine4 = currentTopCheckboxWeeksLine4store;
		currentBottomCheckboxWeeksLine4 = currentBottomCheckboxWeeksLine4store;
		periodSelected4 = periodSelected4store;
		replyTimesGraph = replyTimesGraphstore;
		replyTimesGraph.window = handleforreplyTimesGraph;

		//5
		sendingTimesGraph = sendingTimesGraphstore;
		sendingTimesGraph.window = handleforsendingTimesGraph;
		peopleSelected5 = peopleSelected5store;
		currentTopCheckboxNamesLine5 = currentTopCheckboxNamesLine5store;
		currentBottomCheckboxNamesLine5 = currentBottomCheckboxNamesLine5store;


		//Password
		passwordIn = password.c_str();
	}


	

};





//Versioning
BOOST_CLASS_VERSION(BarGraph, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(DataStorage, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(Graph, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(GraphBar, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(GraphLine, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(GraphMultiBar, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(Line, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(LineGraph, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(LineOfBestFit, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(Msgstreamprocessor, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(PhraseSearchData, CURRENTFILEVERSION);
BOOST_CLASS_VERSION(SaveData, CURRENTFILEVERSION);













//Data returned upon file open
class OpenFileReturnData {
public:
	SaveData saveData;
	bool success;
};


//Checks if a file exists
bool fileExists(std::string path) {
	ifstream ifile(path);
	if (ifile) {
		return true;
	}
	else {
		return false;
	}
}


//Opens a file 
bool openFile(std::string filename, LPCWSTR password, SaveData* saveData) {
	remove(TEMPORARY_FILE);
	decrypt(s2ws(filename), s2ws(TEMPORARY_FILE), password);
	std::ifstream inputFileTemp(TEMPORARY_FILE);
	std::string temp = "";
	std::string startFile = "";
	inputFileTemp >> temp;
	inputFileTemp >> temp;
	inputFileTemp.close();
	if (temp == docStartPhrase) {
		std::ifstream inputFile(TEMPORARY_FILE);
		boost::archive::text_iarchive fileOpened(inputFile);
		fileOpened >> *saveData;
		inputFile.close();
		remove(TEMPORARY_FILE);
		return true;
	}
	else {
		return false;
	}
}


//Checks if a file has a password set
bool noPwdCheck(std::string filename, SaveData* decrypted) {
	if (openFile(filename, NULL, decrypted)) {
		return true;
	}
	if (openFile(filename, PASSWORD, decrypted)) {
		return true;
	}
	else {
		return false;
	}
}


//Saves and encrypts a file
void saveFile(std::string filename, Msgstreamprocessor messageAnalyser, DataStorage dataStore, LPCWSTR password) {
	remove(filename.c_str());
	remove(TEMPORARY_FILE);
	std::ofstream outputFile(TEMPORARY_FILE);
	SaveData saveData;
	saveData.fill(messageAnalyser, dataStore);
	boost::archive::text_oarchive fileOpened(outputFile);
	fileOpened << saveData;
	outputFile.close();
	encrypt(s2ws(TEMPORARY_FILE), s2ws(filename), password);
	remove(TEMPORARY_FILE);
	
}


//Rebuilds the chat file from the saved data
void rebuildChat(Msgstreamprocessor messageAnalyser) {
	if (fileExists(CHAT_RESTORE_FILE)) {
		remove(CHAT_RESTORE_FILE);
	}
	ofstream newChat;
	newChat.open(CHAT_RESTORE_FILE);
	for (int i = 0; i < messageAnalyser.allLines.size(); i++) {
		newChat << messageAnalyser.allLines[i] +"\n";
	}
	newChat.close();
}


