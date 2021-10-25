#pragma once
#include <string>
#include "String Processing Functions.h"
#include "Conversion Functions.h"
#include "Time Period Finding Functions.h"
using namespace std;


//Data about whether a date is valid
class dateValidationData {
public:
	bool dayCorrect;
	std::string newDate;
};



//Checks Validity of a date
dateValidationData dateFormatCorrect(std::string date) {
	dateValidationData returnValues;
	int day = 0;
	int month = 0;
	int year = 0;
	std::string format = "";
	int prevNonDigitChar = 0;
	std::string newDate = "";
	for (int i = 0; i < date.size(); i++) {
		if (isalpha(date[i])) {
			returnValues.dayCorrect = false;
			returnValues.newDate = date;
			return returnValues;
		}
		if (not isdigit(date[i])) {
			if (prevNonDigitChar == 0) {

				newDate += addzeros(date.substr(0, i)) + "/";
				prevNonDigitChar = i;
			}

			else {
				newDate += addzeros(date.substr(prevNonDigitChar + 1, i - (prevNonDigitChar + 1))) + "/";
				prevNonDigitChar = i;
			}
		}
	}
	if (prevNonDigitChar == 0) {


	}
	else {
		newDate += date.substr(prevNonDigitChar + 1, date.size() - (prevNonDigitChar + 1));
		date = newDate;
	}
	switch (date.size()) {
	case 6: {
		for (int i = 0; i < date.size(); i++) {

			if (isdigit(date[i])) {

			}
			else {
				returnValues.dayCorrect = false;
				returnValues.newDate = date;
				return returnValues;
			}
		}
		day = stoint(date.substr(0, 2));
		month = stoint(date.substr(2, 2));
		year = stoint("20" + date.substr(4, 2));
		break;
	}

	case 8: {
		for (int i = 0; i < date.size(); i++) {
			if (i == 2 || i == 5) {
				if ((not isdigit(date[2])) && (not isdigit(date[5]))) {
					format = "DDxMMxYY";
				}
				else {
					if (isdigit(date[2]) && isdigit(date[5])) {
						format = "DDMMYYYY";
					}
					else {
						returnValues.dayCorrect = false;
						returnValues.newDate = date;
						return returnValues;
					}
				}
			}
			else {
				if (isdigit(date[i])) {

				}
				else {
					returnValues.dayCorrect = false;
					returnValues.newDate = date;
					return returnValues;
				}
			}
		}
		if (format == "DDxMMxYY") {
			day = stoint(date.substr(0, 2));
			month = stoint(date.substr(3, 2));
			year = stoint("20" + date.substr(6, 2));
		}
		if (format == "DDMMYYYY") {
			day = stoint(date.substr(0, 2));
			month = stoint(date.substr(2, 2));
			year = stoint(date.substr(4, 4));
		}


		break;
	}
	case 10: {
		for (int i = 0; i < date.size(); i++) {
			if (i == 2 || i == 5) {

			}
			else {
				if (isdigit(date[i])) {

				}
				else {
					returnValues.dayCorrect = false;
					returnValues.newDate = date;
					return returnValues;
				}
			}
		}
		day = stoint(date.substr(0, 2));
		month = stoint(date.substr(3, 2));
		year = stoint(date.substr(6, 4));

		break;
	}
	}

	if (month > 12) {
		returnValues.dayCorrect = false;
		returnValues.newDate = date;
		return returnValues;
	}

	if (day > maxDayFind(month, year)) {
		returnValues.dayCorrect = false;
		returnValues.newDate = date;
		return returnValues;
	}

	returnValues.dayCorrect = true;
	returnValues.newDate = addzeros(itos(day)) + "/" + addzeros(itos(month)) + "/" + itos(year);
	return returnValues;
}
