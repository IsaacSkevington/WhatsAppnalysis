#pragma once
#include <string>
#include "Test.h"
#include "Key Retrieval Functions.h"

using namespace std;

////////////////////////////////STRING PROCESSING FUNCTIONS///////////////////////////////////////
//Adds zeros to a 1 figure date
std::string addzeros(std::string number) {
	if (number.size() == 1) {
		number = "0" + number;
	}
	return number;
}

//Converts a string to all lower case characters
std::string tolowerstring(std::string thestring) {
	for (int i = 0; i < thestring.size(); i++) {
		thestring[i] = tolower(thestring[i]);
	}
	return thestring;
}


//Rounds a string integer
std::string roundval(double value) {
	std::string roundedValue = to_string(value);
	for (int i = 0; i < roundedValue.size(); i++) {
		if (roundedValue[i] == '.') {
			roundedValue = roundedValue.substr(0, i + 3);
		}
	}
	return roundedValue;

}

//////////////////////////////////////////////////////////////////////////////////////////

//Gets an array of phrases in a string separated by 'separatingCharacter'
std::vector<std::string> getPhrases(std::string textIn, std::string separatingCharacter) {
	std::vector<std::string> phrases = {};
	int lastPhraseStart = 0;
	for (int i = 0; i < textIn.length() - separatingCharacter.length(); i++) {
		if (textIn.substr(i, separatingCharacter.length()) == separatingCharacter) {
			phrases.push_back(textIn.substr(lastPhraseStart, i - lastPhraseStart));
			i += (int)separatingCharacter.length() - 1;
			lastPhraseStart = i + 1;
		}
	}
	phrases.push_back(textIn.substr(lastPhraseStart, textIn.length()));

	std::map<std::string, bool> removePhrases = {};
	for (int i = 0; i < phrases.size(); i++) {
		removePhrases[phrases[i]] = false;
		for (int j = 0; j < phrases.size(); j++) {
			if (phrases[i] != phrases[j]) {
				if (phrases[i].find(phrases[j]) != std::string::npos) {
					removePhrases[phrases[i]] = true;
				}
			}
			
		}
	}
	std::vector<std::string> newPhrases = {};
	for (std::string phrase : mapkeysbool(removePhrases)) {
		if (not removePhrases[phrase]) {
			newPhrases.push_back(phrase);
		}
	}
	return newPhrases;
}