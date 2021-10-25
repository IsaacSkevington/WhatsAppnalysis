#pragma once
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include "Conversion Functions.h"
#include "Test.h"
using namespace std;


///////////////////////TIME PERIOD FINDING FUNCTIONS///////////////////////////
//Finds the maximum day in a month
int maxDayFind(int month, int year) {
	int maxDay = 0;
	if (month == 1 ||
		month == 3 ||
		month == 5 ||
		month == 7 ||
		month == 8 ||
		month == 10 ||
		month == 12)
	{
		maxDay = 31;
	}

	else {
		if (month == 2) {
			if (year % 4 == 0) {
				if (year % 100 == 0) {
					if (year % 400) {
						maxDay = 29;
					}
					else {
						maxDay = 29;
					}
				}
				else {
					maxDay = 29;
				}
			}
			else {
				maxDay = 28;
			}
		}
		else {
			maxDay = 30;
		}
	}
	return maxDay;
}



//Finds which day of the week a day is
int dayofweek(int d, int m, int y)
{
	static int t[] = { 0, 3, 2, 5, 0, 3,
					   5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	return (y + y / 4 - y / 100 +
		y / 400 + t[m - 1] + d) % 7;
}



//Finds the number of seconds after 00:00 in a timestamp
int secondsfind(std::string time) {
	return (stoint(time.substr(0, 2)) * 3600) + (stoint(time.substr(3, 2)) * 60);
}


//Finds the first day of a week
std::string WeekFind(Line currline, std::string currentweek) {
	std::string week;
	int weekday = 0;
	int dayrequired = 0;
	int monthrequired = 0;
	int yearrequired = 0;
	int day = stoint(currline.datefind().substr(0, 2));
	int month = stoint(currline.datefind().substr(3, 2));
	int year = stoint(currline.datefind().substr(6, 4));
	weekday = dayofweek(day, month, year);
	if (weekday == 0) {
		weekday = 6;
	}
	else {
		weekday += -1;
	}
	if (weekday != 0) {
		dayrequired = day - weekday;
		if (dayrequired <= 0) {
			monthrequired = month - 1;
			if (monthrequired <= 0) {
				yearrequired = year - 1;
				monthrequired = 12;
			}
			else {
				yearrequired = year;
			}
			if (monthrequired == 1 ||
				monthrequired == 3 ||
				monthrequired == 5 ||
				monthrequired == 7 ||
				monthrequired == 8 ||
				monthrequired == 10 ||
				monthrequired == 12)
			{
				dayrequired += 31;
			}

			else {
				if (monthrequired == 2) {
					if (year % 4 == 0) {
						if (year % 100 == 0) {
							if (year % 400) {
								dayrequired += 29;
							}
							else {
								dayrequired += 28;
							}
						}
						else {
							dayrequired += 29;
						}
					}
					else {
						dayrequired += 28;
					}
				}
				else {
					dayrequired += 30;
				}
			}
		}
		else {
			monthrequired = month;
			yearrequired = year;
		}
		day = dayrequired;
		month = monthrequired;
		year = yearrequired;
	}
	currentweek = addzeros(itos(day)) + "/" + addzeros(itos(month)) + "/" + itos(year);
	return currentweek;
}



//Finds an HH:MM timeStamp from a number of seconds
std::string timeStamp(int seconds) {
	int hours = 0;
	int minutes = 0;
	hours = (int)floor(seconds / 3600);
	minutes = (int)floor((seconds % 3600) / 60);
	return(addzeros(itos(hours)) + ":" + addzeros(itos(minutes)));
}


//Get all the times between 00:00 and 23:59
std::vector<std::string> allTimesGet() {
	std::vector<std::string> alltimes = {};
	for (int i = 0; i < 1440; i++) {
		alltimes.push_back(timeStamp(i * 60));
	}
	return alltimes;

}

//Zeros all the the times in a map
std::map<std::string, int> allTimesZero() {
	std::map<std::string, int> timesZeroed = {};
	for (int i = 0; i < 1440; i++) {
		timesZeroed[timeStamp(i * 60)] = 0;
	}
	return timesZeroed;

}

//Returns all the time stamps between 2 second values
std::vector<std::string> getAllStampsBetween(int startSecs, int endSecs) {
	std::vector<std::string> stamps = {};
	if (startSecs < endSecs) {
		for (int i = startSecs; i <= endSecs; i += 60) {
			stamps.push_back(timeStamp(i));

		}
	}
	else {
		if (endSecs != 0) {
			for (int i = startSecs; i < 86340; i += 60) {
				stamps.push_back(timeStamp(i));
			}
			for (int i = 0; i < endSecs; i += 60) {
				stamps.push_back(timeStamp(i));
			}
		}
		else {
			for (int i = startSecs; i < 86340; i += 60) {
				stamps.push_back(timeStamp(i));
			}
			stamps.push_back(timeStamp(0));

		}
	}
	return stamps;
}


//Finds the average number of something in the last half hour with records every minute
int averagePast(int numMins, std::string TStamp, std::map <std::string, int> totals) {
	int average = 0;
	int total = 0;
	int stampCount = 0;
	int startSecs = 0;
	startSecs = secondsfind(TStamp) - (numMins * 60);
	if (startSecs < 0) {
		startSecs = 0 - (startSecs - 86400);
	}
	std::vector<std::string> allTimeStamps = getAllStampsBetween(startSecs, secondsfind(TStamp));
	for (std::string stamp : allTimeStamps) {
		total += totals[stamp];
		stampCount++;
	}
	return total / stampCount;
}


//Gets an array of the dates of all the days between 'start' and 'end'
std::vector<std::string> getDaysBetween(std::string start, std::string end) {
	std::vector<std::string> dayArray = {};
	int dayCurrent = stoint(start.substr(0, 2)),
		monthCurrent = stoint(start.substr(3, 2)),
		yearCurrent = stoint(start.substr(6, 4));
	std::string currentDate = start;
	dayArray.push_back(start);
	while (currentDate != end) {
		if (dayCurrent == maxDayFind(monthCurrent, yearCurrent)) {
			dayCurrent = 1;

			if (monthCurrent == 12) {
				yearCurrent++;
				monthCurrent = 1;
			}
			else {
				monthCurrent++;
			}
		}
		else {
			dayCurrent++;
		}
		currentDate = addzeros(itos(dayCurrent)) + "/" + addzeros(itos(monthCurrent)) + "/" + itos(yearCurrent);
		dayArray.push_back(currentDate);
	}
	return dayArray;
}


//Gets an array of the dates of the days up to 'daysBefore' days before given 'date'
std::vector<std::string> getDaysBefore(int daysBefore, std::string date) {
	std::vector<std::string> dates = {};
	int day = stoi(date.substr(0, 2));
	int month = stoi(date.substr(3, 2));
	int year = stoi(date.substr(6, 4));
	int newDay = day - (daysBefore - 1);
	int newMonth = month;
	int newYear = year;
	if (newDay > 0) {
		newMonth = month;
		newYear = year;
	}
	else {
		while (newDay <= 0) {
			if (newMonth == 1) {
				newMonth = 12;
				newYear = newYear - 1;
			}
			else {
				newMonth = month - 1;
				newYear = newYear;
			}
			newDay += maxDayFind(newMonth, newYear);
		}
	}
	std::string startDate = addzeros(to_string(newDay)) + "/" + addzeros(to_string(newMonth)) + "/" + to_string(newYear);
	dates = getDaysBetween(startDate, date);
	return dates;
}

//Finds the average number of something in the last 'numDays' days with records every day
int averagePastDays(int numDays, std::string day, std::map <std::string, int> totals) {
	int average = 0;
	int total = 0;
	int dayCount = 0;
	std::vector<std::string> days = getDaysBefore(numDays, day);
	for (std::string day : days) {
		if (totals.find(day) == totals.end()) {
			dayCount++;
		}
		else {
			total += totals[day];
			dayCount++;
		}
	}
	return total / dayCount;
}


//Gets the number of days between 'startDate' and 'date'
int getNumDays(std::string date, std::string startDate) {
	return (int)getDaysBetween(startDate, date).size() - 1;
}


//Gets an array of the dates of all the days in an array of months 'months'
std::vector<double> getMonthsDays(std::vector<std::string> months) {
	std::vector<double> days;
	int monthint, yearint;
	monthint = stoi(months[0].substr(0, 2));
	yearint = stoi(months[0].substr(3, 4));
	int total = maxDayFind(monthint, yearint) * -1;
	for (std::string month : months) {
		monthint = stoi(month.substr(0, 2));
		yearint = stoi(month.substr(3, 4));
		total += maxDayFind(monthint, yearint);
		days.push_back(total);

	}
	return days;
}

//Gets an array of the month stamps of all the months 'days' days after the month 'startMonthFull' 
std::vector<std::string> getDaysMonths(std::string startMonthFull, std::vector<double> days) {
	std::vector<std::string> months;
	int startMonth = stoi(startMonthFull.substr(0, 2));
	int startYear = stoi(startMonthFull.substr(3, 4));
	months.push_back(startMonthFull);
	for (int i = 0; i < days.size() - 1; i++) {
		startMonth++;
		if (startMonth == 13) {
			startMonth = 1;
			startYear++;
		}
		months.push_back(addzeros(to_string(startMonth)) + "/" + to_string(startYear));
	}
	return months;
}


//Gets the month stamps of months between 'startDay' and 'endDay'
std::vector<std::string> getMonthsBetween(std::string startDay, std::string endDay) {
	std::vector<std::string> months;
	int startMonth = stoi(startDay.substr(3, 2));
	int startYear = stoi(startDay.substr(6, 4));
	int endMonth = stoi(endDay.substr(3, 2));
	int endYear = stoi(endDay.substr(6, 4));
	months.push_back(addzeros(to_string(startMonth)) + "/" + to_string(startYear));
	while (not((startMonth == endMonth) && (startYear == endYear))) {
		startMonth++;
		if (startMonth == 13) {
			startMonth = 1;
			startYear++;
		}
		months.push_back(addzeros(to_string(startMonth)) + "/" + to_string(startYear));
	}
	return months;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
