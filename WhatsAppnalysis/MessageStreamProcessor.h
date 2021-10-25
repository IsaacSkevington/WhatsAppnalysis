#pragma once
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <tchar.h>
#include <shobjidl.h>
#include <atlstr.h>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <stdio.h>
#include <typeinfo>
#include <windows.h>
#include "Line.h"
#include "Array Processing Functions.h"
//#include "FileProcessing.h"
#include "Map Processing Functions.h"
#include "Key Retrieval Functions.h"
#include "Time Period Finding Functions.h"



#define PROGRESS_STEP 100
using namespace std;
int countLines(std::string filepath) {
	std::ifstream myfile(filepath);
	myfile.unsetf(std::ios_base::skipws);
	unsigned lineCount = (unsigned)std::count(
		std::istream_iterator<char>(myfile),
		std::istream_iterator<char>(),
		'\n');
	return lineCount;
}

class Msgstreamprocessor {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		//All file lines 
		ar& allLines;
		ar& fileType;
		//Analysis path
		ar& filepath;
		//Time periods
		ar& allweeks;
		ar& months;
		ar& currentweek;
		//People
		ar& people;
		//All Messages Analysis
		ar& totalmessagesmonthly;
		ar& totalmessagesweekly;
		//Phrase Analysis
		ar& phrasebyweek;
		ar& phrasebymonth;
		ar& totalmessagesphrase;
		if (version > 1) {
			ar& phrases;
			ar& phrasebyday;
		}
		ar& caseMatch;
		//Day Analysis
		ar& totalmessagesdaily;
		//Reply time analysis
		//Format Month:(Person: Time)
		ar& replytimesmonthly;
		ar& replytimesweekly;
		ar& replytimesmonthlyaverage;
		ar& replytimesweeklyaverage;
		ar& numberRepliesMonth;
		ar& numberRepliesWeek;
		ar& currentperson;
		ar& lasttime;
		ar& lastdate;

		//Common word Analysis
		ar& words;
		ar& wordtotals;

		//Times of sending
		ar& phrasetimes;
		ar& alltimes;
		ar& timetotals;
		ar& alldays;

		ar& firstTime;

	}
public:
	//Saved lines
	std::vector<std::string> allLines = {};
	std::vector<Line> allMsgLines = {};
	int fileType;
	//Analysis path
	std::string filepath = "";
	//Time periods
	std::vector<std::string> allweeks = {};
	std::vector <std::string> months = {};
	std::string currentweek = "undefined";
	//People
	std::vector <std::string> people = {};
	//All Messages Analysis
	std::map <std::string, std::map<std::string, int> > totalmessagesmonthly = {};
	std::map <std::string, std::map<std::string, int> > totalmessagesweekly = {};
	//Phrase Analysis
	std::map <std::string, std::map<std::string, int> > phrasebyweek = {};
	std::map <std::string, std::map<std::string, int> > phrasebymonth = {};
	std::map<std::string, std::map<std::string, int> > phrasebyday = {};
	std::map<std::string, int> totalmessagesphrase = {};
	std::vector<std::string> phrases = { "Not Set" };
	bool caseMatch = false;
	//Day Analysis
	std::map <std::string, std::map<std::string, int> > totalmessagesdaily;
	//Reply time analysis
	//Format Month:(Person: Time)
	std::map <std::string, std::map<std::string, int> > replytimesmonthly;
	std::map <std::string, std::map<std::string, int> > replytimesweekly;
	std::map <std::string, std::map<std::string, double> > replytimesmonthlyaverage;
	std::map <std::string, std::map<std::string, double> > replytimesweeklyaverage;
	std::map <std::string, std::map<std::string, int> > numberRepliesMonth;
	std::map <std::string, std::map<std::string, int> > numberRepliesWeek;
	std::string currentperson = "undefined";
	std::string lasttime = "undefined";
	std::string lastdate = "undefined";


	//Common word Analysis
	std::vector <std::string> words;
	std::map <std::string, int> wordtotals;

	//Times of sending
	std::map<std::string, int> phrasetimes = {};
	std::vector<std::string>  alltimes = allTimesGet();
	std::map<std::string, std::map <std::string, int> > timetotals = {};
	std::vector<std::string> alldays = {};

	bool firstTime = true;

	void setFilepath(std::string path) {
		filepath = path;
	}

	std::string getFilepath() {
		return filepath;
	}

	//Resets Phrase Data
	void phraseReset() {
		phrasebyweek = {};
		phrasebyday = {};
		phrasebymonth = {};
		phrasebyweek["All Time"]["Total Messages"] = 0;
		phrasebymonth["All Time"]["Total Messages"] = 0;
	}

	//Resets all data
	void reset() {
		phraseReset();
		phrases = { "Not Set" };
		allLines = {};
		people = {};
		people.push_back("Total Messages");
		totalmessagesmonthly = {};
		months = {};
		totalmessagesdaily = {};
		words = {};
		wordtotals = {};
		currentweek = "undefined";
		allweeks = {};
		timetotals = {};
		replytimesmonthly = {};
		replytimesweekly = {};
		replytimesmonthlyaverage = {};
		replytimesweeklyaverage = {};
		numberRepliesMonth = {};
		numberRepliesWeek = {};
		currentperson = "undefined";
		lasttime = "undefined";
		lastdate = "undefined";
		alldays = {};

	}


	//Processes the times messages are sent after analysis
	void sendingtimes() {
		std::vector<int> alltimeintervals;
		std::map <int, int> timetotalintervals;
		for (std::string person : people) {
			for (std::string time : alltimes) {
				if (timetotalintervals.find(stoint(time.substr(0, 2))) == timetotalintervals.end()) {
					alltimeintervals.push_back(stoint(time.substr(0, 2)));
					timetotalintervals[stoint(time.substr(0, 2))] = 0;
				}
				timetotalintervals[stoint(time.substr(0, 2))] += timetotals[person][time];
			}
		}
		alltimeintervals = arrayreverse(sortarr(alltimeintervals));
	}

	//Calculates average reply times after analysis
	void replytimesprocess() {
		double numtimes = 0, totaltime = 0;
		for (std::string month : months) {
			for (std::string person : people) {
				if (not ((replytimesmonthly[month][person] == 0) && (numberRepliesMonth[month][person] == 0))) {
					replytimesmonthlyaverage[month][person] = (replytimesmonthly[month][person] / numberRepliesMonth[month][person]) / 60.0;
				}
				else {
					replytimesmonthlyaverage[month][person] = 0.00;
				}
			}
		}
		for (std::string week : allweeks) {
			for (std::string person : people) {
				if (not ((replytimesweekly[week][person] == 0) && (numberRepliesWeek[week][person] == 0))) {
					replytimesweeklyaverage[week][person] = (replytimesweekly[week][person] / numberRepliesWeek[week][person]) / 60.0;
				}
				else {
					replytimesweeklyaverage[week][person] = 0.00;
				}
			}
		}
	}
	

	//Processes most common words in a chat
	std::string commonwordsprocess(int numberofwords) {
		std::string commonwordsprocessoutput = "";
		words = mapsort(wordtotals);
		for (int commonwordprint = 0; commonwordprint < numberofwords; commonwordprint++) {
			commonwordsprocessoutput += (unsigned __int64)commonwordprint + 1 + " " + words[commonwordprint] + " occurred " + itos(wordtotals[words[commonwordprint]]) + " times" + "\n";
		}

	}



	//Reads all lines and dispatches info for analysis
	void lineread(HWND handleforwindow = NULL) {
		int fileLines = countLines(filepath);
		std::string msgline;
		int lineNum = 0;
		ifstream whatsappChat;
		int n = (int)filepath.length();
		whatsappChat.open(filepath);
		if (whatsappChat.is_open()) {
			while (!whatsappChat.eof()) {
				getline(whatsappChat, msgline);
				if (msgline.size() > 28) {
					if (msgline.substr(20, 8) != "Messages") {
						if (msgline[2] == '/' && msgline[5] == '/') {
							Line currline(msgline, fileType);
							allMsgLines.push_back(currline);
							if (currline.personfind().size() < 30) {
								if (phrases[0] != "Not Set") {
									phraseoccurenceanalyse(currline);
								}
								else {
									totalMessagesAnalysis(currline);
									dayanalysis(currline);
									//wordsanalysis(currline);
									timeanalysis(currline);
									replytimeanalysis(currline);
									allLines.push_back(msgline);
								}
							}
							
						}
					}
				}
				lineNum++;
				if (handleforwindow != NULL) {
					if (lineNum % PROGRESS_STEP == 0) {
						PostMessageA(handleforwindow, 0x9007, NULL, NULL);
					}
				}
				else {
					cout << "\rPercentage complete: " << int((double(lineNum) / double(fileLines)) * double(100)) << " %  ";
				}
			}
		}

		whatsappChat.close();

	}

	//Analyses reply times
	void replytimeanalysis(Line currline) {
		int spacenumber = 0;
		std::string currentmonth = currline.monthfind();

		int timeforreply = 0;
		if (lastdate == "undefined") {
			numberRepliesMonth[currentmonth][currline.personfind()] = 0;
			numberRepliesWeek[currentmonth][currline.personfind()] = 0;
			replytimesmonthly[currentmonth][currline.personfind()] = 0;
			replytimesweekly[currentmonth][currline.personfind()] = 0;
		}
		else {
			if (currline.personfind() != currentperson) {
				if (lastdate == currline.datefind()) {
					timeforreply = secondsfind(currline.timefind()) - secondsfind(lasttime);
					if (timeforreply == 0) {
						timeforreply = 15;
					}
				}
				else {
					timeforreply = secondsfind(currline.timefind()) - secondsfind("07:00");
					if (timeforreply <= 0) {
						timeforreply = (secondsfind("24:00") - secondsfind(lasttime)) + (secondsfind(currline.timefind()));
					}
				}


				if (numberRepliesMonth["All Time"].find(currline.personfind()) == numberRepliesMonth["All Time"].end()) {
					numberRepliesMonth["All Time"][currline.personfind()] = 0;
				}
				if (replytimesmonthly["All Time"].find(currline.personfind()) == replytimesmonthly["All Time"].end()) {
					replytimesmonthly["All Time"][currline.personfind()] = 0;
				}

				if (numberRepliesMonth.find(currentmonth) == numberRepliesMonth.end()) {
					numberRepliesMonth[currentmonth] = {};
					numberRepliesMonth[currentmonth]["Total Messages"] = 0;

				}

				if (replytimesmonthly.find(currentmonth) == replytimesmonthly.end()) {
					replytimesmonthly[currentmonth] = {};
					replytimesmonthly[currentmonth]["Total Messages"] = 0;
				}


				if (currline.personfind() != "error") {
					if (numberRepliesMonth[currentmonth].find(currline.personfind()) == numberRepliesMonth[currentmonth].end()) {
						numberRepliesMonth[currentmonth][currline.personfind()] = 1;
						numberRepliesMonth[currentmonth]["Total Messages"] ++;
						numberRepliesMonth["All Time"][currline.personfind()]++;
						numberRepliesMonth["All Time"]["Total Messages"] ++;
					}
					else {
						numberRepliesMonth[currentmonth][currline.personfind()] ++;
						numberRepliesMonth[currentmonth]["Total Messages"]++;
						numberRepliesMonth["All Time"][currline.personfind()] ++;
						numberRepliesMonth["All Time"]["Total Messages"] ++;
					}

					if (replytimesmonthly[currentmonth].find(currline.personfind()) == replytimesmonthly[currentmonth].end()) {
						replytimesmonthly[currentmonth][currline.personfind()] = timeforreply;
						replytimesmonthly[currentmonth]["Total Messages"] += timeforreply;
						replytimesmonthly["All Time"][currline.personfind()] += timeforreply;
						replytimesmonthly["All Time"]["Total Messages"] += timeforreply;

					}

					else {
						replytimesmonthly[currentmonth][currline.personfind()] += timeforreply;
						replytimesmonthly[currentmonth]["Total Messages"] += timeforreply;
						replytimesmonthly["All Time"][currline.personfind()] += timeforreply;
						replytimesmonthly["All Time"]["Total Messages"] += timeforreply;
					}



					if (currline.datefind()[2] == '/') {
						if (numberRepliesWeek["All Time"].find(currline.personfind()) == numberRepliesWeek["All Time"].end()) {
							numberRepliesWeek["All Time"][currline.personfind()] = 0;
						}
						if (replytimesweekly["All Time"].find(currline.personfind()) == replytimesweekly["All Time"].end()) {
							replytimesweekly["All Time"][currline.personfind()] = 0;
						}

						if (numberRepliesWeek.find(currentweek) == numberRepliesWeek.end()) {
							numberRepliesWeek[currentweek]["Total Messages"] = 0;
						}
						if ((numberRepliesWeek[currentweek]).find(currline.personfind()) == (numberRepliesWeek[currentweek]).end()) {
							numberRepliesWeek[currentweek][currline.personfind()] = 0;
						}
						if (numberRepliesWeek["All Time"].find(currline.personfind()) == numberRepliesWeek["All Time"].end()) {
							numberRepliesWeek["All Time"][currline.personfind()] = 0;
						}

						if (replytimesweekly.find(currentweek) == replytimesweekly.end()) {
							replytimesweekly[currentweek]["Total Messages"] = 0;
						}
						if ((replytimesweekly[currentweek]).find(currline.personfind()) == (replytimesweekly[currentweek]).end()) {
							replytimesweekly[currentweek][currline.personfind()] = 0;
						}
						if (replytimesweekly["All Time"].find(currline.personfind()) == replytimesweekly["All Time"].end()) {
							replytimesweekly["All Time"][currline.personfind()] = 0;
						}

						numberRepliesWeek[currentweek][currline.personfind()] ++;
						numberRepliesWeek[currentweek]["Total Messages"] ++;
						numberRepliesWeek["All Time"]["Total Messages"] ++;
						numberRepliesWeek["All Time"][currline.personfind()] ++;

						replytimesweekly[currentweek][currline.personfind()] += timeforreply;
						replytimesweekly[currentweek]["Total Messages"] += timeforreply;
						replytimesweekly["All Time"]["Total Messages"] += timeforreply;
						replytimesweekly["All Time"][currline.personfind()] += timeforreply;

					}
				}
			}
		}
		lastdate = currline.datefind();
		lasttime = currline.timefind();
		currentperson = currline.personfind();
	}

	//Analyses the times messages are sent
	void timeanalysis(Line currline) {
		if (currline.timefind()[2] == ':') {
			if (find(alldays.begin(), alldays.end(), currline.datefind()) == alldays.end()) {
				alldays.push_back(currline.datefind());
			}
			if (timetotals.find("Total Messages") == timetotals.end()) {
				timetotals["Total Messages"] = allTimesZero();
			}
			if (timetotals.find(currline.personfind()) == timetotals.end()) {
				timetotals[currline.personfind()] = allTimesZero();
			}
			timetotals[currline.personfind()][currline.timefind()] ++;
			timetotals["Total Messages"][currline.timefind()] ++;
		}
	}


	//Analyses the number of times a phrase is sent
	void phraseoccurenceanalyse(Line currline) {
		std::string day = currline.datefind();

		if (phrasebyday.find(day) == phrasebyday.end()) {
			phrasebyday[day]["Total Messages"] = 0;
		}
		if ((phrasebyday[day]).find(currline.personfind()) == (phrasebyday[day]).end()) {
			phrasebyday[day][currline.personfind()] = 0;
		}
		for (int i = 0; i < currline.messagefind().size(); i++) {
			if (caseMatch) {
				for (std::string phrase : phrases) {
					if (currline.messagefind().substr(i, phrase.size()) == (phrase)) {
						phrasebyday[day][currline.personfind()] ++;
						phrasebyday[day]["Total Messages"] ++;
					}
				}
			}
			else {
				for (std::string phrase : phrases) {
					if (tolowerstring(currline.messagefind().substr(i, phrase.size())) == tolowerstring(phrase)) {
						phrasebyday[day][currline.personfind()] ++;
						phrasebyday[day]["Total Messages"] ++;
					
					}
				}
			}

		}
		
		
		std::string month = currline.datefind().substr(3, 7);

		if (phrasebymonth.find(month) == phrasebymonth.end()) {
			phrasebymonth[month]["Total Messages"] = 0;
		}
		if ((phrasebymonth[month]).find(currline.personfind()) == (phrasebymonth[month]).end()) {
			phrasebymonth[month][currline.personfind()] = 0;
		}
		if (phrasebymonth["All Time"].find(currline.personfind()) == phrasebymonth["All Time"].end()) {
			phrasebymonth["All Time"][currline.personfind()] = 0;
		}
		for (int i = 0; i < currline.messagefind().size(); i++) {
			if (caseMatch) {
				for (std::string phrase : phrases) {
					if (currline.messagefind().substr(i, phrase.size()) == (phrase)) {
						phrasebymonth[month][currline.personfind()] ++;
						phrasebymonth[month]["Total Messages"] ++;
						phrasebymonth["All Time"]["Total Messages"] ++;
						phrasebymonth["All Time"][currline.personfind()] ++;
					}
				}
			}
			else {
				for (std::string phrase : phrases) {
					if (tolowerstring(currline.messagefind().substr(i, phrase.size())) == tolowerstring(phrase)) {
						phrasebymonth[month][currline.personfind()] ++;
						phrasebymonth[month]["Total Messages"] ++;
						phrasebymonth["All Time"]["Total Messages"] ++;
						phrasebymonth["All Time"][currline.personfind()] ++;

					}
				}
			}

		}
		if (currline.datefind()[2] == '/') {
			std::string weekforphrase = WeekFind(currline, currentweek);
			currentweek = weekforphrase;
			if (phrasebyweek.find(weekforphrase) == phrasebyweek.end()) {
				phrasebyweek[weekforphrase]["Total Messages"] = 0;
			}
			if ((phrasebyweek[weekforphrase]).find(currline.personfind()) == (phrasebyweek[weekforphrase]).end()) {
				phrasebyweek[weekforphrase][currline.personfind()] = 0;
			}
			if (phrasebyweek["All Time"].find(currline.personfind()) == phrasebyweek["All Time"].end()) {
				phrasebyweek["All Time"][currline.personfind()] = 0;
			}
			for (int i = 0; i < currline.messagefind().size(); i++) {
				if (caseMatch) {
					for (std::string phrase : phrases) {
						if (currline.messagefind().substr(i, phrase.size()) == (phrase)) {
							phrasebyweek[weekforphrase][currline.personfind()] ++;
							phrasebyweek[weekforphrase]["Total Messages"] ++;
							phrasebyweek["All Time"]["Total Messages"] ++;
							phrasebyweek["All Time"][currline.personfind()] ++;
						}
					}
				}
				else {
					for (std::string phrase : phrases) {
						if (tolowerstring(currline.messagefind().substr(i, phrase.size())) == tolowerstring(phrase)) {
							phrasebyweek[weekforphrase][currline.personfind()] ++;
							phrasebyweek[weekforphrase]["Total Messages"] ++;
							phrasebyweek["All Time"]["Total Messages"] ++;
							phrasebyweek["All Time"][currline.personfind()] ++;

						}
					}
				}

			}

		}
	}

	
	//Calculates the total messages sent
	void totalMessagesAnalysis(Line currline) {
		std::string month = currline.datefind().substr(3, 7);
		int spacenumber = 0;
		if (totalmessagesmonthly.find(month) == totalmessagesmonthly.end()) {
			totalmessagesmonthly[month] = {};
			months.push_back(month);
			totalmessagesmonthly[month]["Total Messages"] = 0;

		}

		if (std::find(people.begin(), people.end(), currline.personfind()) == people.end()) {
			if (currline.personfind() != "error") {
				people.push_back(currline.personfind());
			}
		}

		if (currline.personfind() != "error") {

			if (totalmessagesmonthly[month].find(currline.personfind()) == totalmessagesmonthly[month].end()) {
				for (int i = 0; i < currline.personfind().size(); i++) {
					if (currline.personfind()[i] == 32) {
						spacenumber++;
					}
				}
				if (spacenumber < 4) {
					totalmessagesmonthly[month][currline.personfind()] = 1;
					totalmessagesmonthly[month]["Total Messages"] ++;
					totalmessagesmonthly["All Time"][currline.personfind()]++;
					totalmessagesmonthly["All Time"]["Total Messages"] ++;
				}
			}

			else {
				totalmessagesmonthly[month][currline.personfind()] ++;
				totalmessagesmonthly[month]["Total Messages"]++;
				totalmessagesmonthly["All Time"][currline.personfind()] ++;
				totalmessagesmonthly["All Time"]["Total Messages"] ++;
			}



			if (currline.datefind()[2] == '/') {
				std::string week = WeekFind(currline, currentweek);
				currentweek = week;
				if (totalmessagesweekly.find(week) == totalmessagesweekly.end()) {
					allweeks.push_back(week);
					totalmessagesweekly[week]["Total Messages"] = 0;
				}
				if ((totalmessagesweekly[week]).find(currline.personfind()) == (totalmessagesweekly[week]).end()) {
					totalmessagesweekly[week][currline.personfind()] = 0;
				}
				if (totalmessagesweekly["All Time"].find(currline.personfind()) == totalmessagesweekly["All Time"].end()) {
					totalmessagesweekly["All Time"][currline.personfind()] = 0;
				}
				totalmessagesweekly[week][currline.personfind()] ++;
				totalmessagesweekly[week]["Total Messages"] ++;
				totalmessagesweekly["All Time"]["Total Messages"] ++;
				totalmessagesweekly["All Time"][currline.personfind()] ++;

			}



		}
	}

	//Calculates messages sent on a day by day basis
	void dayanalysis(Line currline) {
		std::string day = currline.datefind();
		int spacenumber = 0;
		if (totalmessagesdaily.find(day) == totalmessagesdaily.end()) {
			totalmessagesdaily[day] = {};
			totalmessagesdaily[day]["Total Messages"] = 0;
		}
		if (currline.personfind() != "error") {
			if (totalmessagesdaily[day].find(currline.personfind()) == totalmessagesdaily[day].end()) {
				totalmessagesdaily[day][currline.personfind()] = 1;
			}
			else {
				totalmessagesdaily[day][currline.personfind()] ++;
				totalmessagesdaily[day]["Total Messages"] ++;
			}
		}
	}

	//Most common word analysis
	void wordsanalysis(Line currline) {

		int lastspace = 0;
		std::vector <std::string> wordsinmessage;
		std::string message = currline.messagefind();
		for (int character = 0; character < message.length(); character++) {
			if (message[character] == ' ') {
				wordsinmessage.push_back(message.substr(lastspace, (unsigned __int64)character - (unsigned __int64)lastspace));
				lastspace = character + 1;
			}
		}
		wordsinmessage.push_back(message.substr(lastspace, (message.length() - lastspace)));
		for (int wordcheck = 0; wordcheck < wordsinmessage.size(); wordcheck++) {
			if (wordtotals.find(wordsinmessage[wordcheck]) == wordtotals.end()) {
				words.push_back(wordsinmessage[wordcheck]);
				wordtotals[wordsinmessage[wordcheck]] = 1;
			}
			else {
				wordtotals[wordsinmessage[wordcheck]] ++;
			}
		}
	}
};



Msgstreamprocessor initialiseMSGStreamProcessor(std::string dir) {
	cout << "Initialising message stream processor..." << endl;
	Msgstreamprocessor m;
	cout << "Setting directory" << endl;
	m.filepath = dir;
	cout << "Analysing chat..." << endl;
	m.lineread();
	cout << "Complete!" << endl;
	return m;
}