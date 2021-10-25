#pragma once
#include "MessageStreamProcessor.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;




//Data Store
class DataStorage {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& firstTime;
		ar& firstTimeDynamics;
	}
public:
	bool firstTime = true;
	bool firstTimeDynamics = true;
	
	//Gets the statics which stay the same
	void staticsGet(Msgstreamprocessor& messageAnalyser, HWND handleforwindow) {
		messageAnalyser.reset();
		messageAnalyser.lineread(handleforwindow);
		messageAnalyser.months = mapkeys2D(messageAnalyser.totalmessagesmonthly);
		messageAnalyser.months = dateArraySort(messageAnalyser.months);
		messageAnalyser.allweeks = mapkeys2D(messageAnalyser.totalmessagesweekly);
		messageAnalyser.allweeks = dayArraySort(messageAnalyser.allweeks);
		messageAnalyser.people = peopleArraySort(messageAnalyser.people);
		messageAnalyser.replytimesprocess();
	}

	//Get Changing statistics
	void dynamicsGet(Msgstreamprocessor &messageAnalyser, HWND handleforwindow) {
		messageAnalyser.phraseReset();
		messageAnalyser.lineread(handleforwindow);
		firstTimeDynamics = false;
	}

	//Called the first time the store is used
	void firstTimeStartUp(Msgstreamprocessor &messageAnalyser, HWND handleforwindow) {
		std::vector<std::string> phrasesBefore = messageAnalyser.phrases;
		staticsGet(messageAnalyser, handleforwindow);
		firstTime = false;
		if (not firstTimeDynamics) {
			if (MessageBoxA(NULL, "Do you wish to search for your phrase(s) again now", "Re-search phrase", MB_YESNO) == IDYES) {
				messageAnalyser.phrases = phrasesBefore;
			}
		}

	}

};









