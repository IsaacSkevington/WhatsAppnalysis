#pragma once
#include <Windows.h>
#include "MessageStreamProcessor.h"
#include "DataStore.h"
#include "NewDefinitions.h"
using namespace std;


////////////////////////////////////////////////////////STATIC STATS///////////////////////////////////////////////////////////////////


//A stat return object containing data to be returned after stat calculation
class StatReturnStorage {
public:
	Msgstreamprocessor messageAnalyser;
	DataStorage dataStore;
};




//Get the stats
StatReturnStorage statsGet(Msgstreamprocessor messageAnalyser, DataStorage dataStore, HWND handlefordiagwindow, HWND handleforinfowindow) {

	if (messageAnalyser.filepath != "") {
		dataStore.firstTimeStartUp(messageAnalyser, handlefordiagwindow);
	}
	
	dataStore.firstTimeDynamics = true;
	StatReturnStorage statsReturn;
	statsReturn.messageAnalyser = messageAnalyser;
	statsReturn.dataStore = dataStore;
	PostMessageA(handleforinfowindow, SDM_DATAREADY, NULL, NULL);
	return statsReturn;

}


//Creates the windows to show the stats
void windowsCreate(HWND handleforwindow) {
	//Scroll Window to top
	ScrollWindowEx(handleforwindow, 0, prevScrollPosVert, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
	ScrollWindowEx(handleforwindow, prevScrollPosHorz, 0, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
	///////////////////////////////////////////////////////
	//All windows Checkboxes
	for (int i = 0; i < BoxNames.size(); i++) {
		SetWindowTextA(
			GetDlgItem(handleforwindow2, ID_WINDOW[(int)i]),
			""
		);
	}

	//////////////////////////////////////////////////////

	//Window 21
	DestroyWindow(
		handleforwindow21
	);
	handleforwindow21 = CreateWindowEx(NULL,
		windowClassForWindow21Public.lpszClassName,
		L"Window 21",

		WS_VISIBLE | WS_CHILD,
		WINDOWXPOS,
		WINDOWYPOS,
		SUBWINDOWLENGTH,
		SUBWINDOWHEIGHT,
		handleforwindow,
		NULL,
		windowClassForWindow21Public.hInstance,
		NULL
	);


	if (!handleforwindow21)
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window 21 creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}
	handlesfordatawindows[1] = handleforwindow21;
	ShowWindow(handleforwindow21, SW_HIDE);

	/////////////////////////////////////////////////

	//Window 22
	DestroyWindow(
		handleforwindow22
	);
	handleforwindow22 = CreateWindowEx(NULL,
		windowClassForWindow22Public.lpszClassName,
		L"Window 22",
		WS_VISIBLE | WS_CHILD,
		WINDOWXPOS,
		WINDOWYPOS,
		SUBWINDOWLENGTH,
		SUBWINDOWHEIGHT,
		handleforwindow,
		NULL,
		windowClassForWindow22Public.hInstance,
		NULL                /* No Window Creation data */
	);

	if (!handleforwindow22)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window 22 creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}
	handlesfordatawindows[0] = handleforwindow22;
	ShowWindow(handleforwindow22, SW_HIDE);
	
	/////////////////////////////////////////////////

	//Window 23
	DestroyWindow(
		handleforwindow23
	);
	handleforwindow23 = CreateWindowEx(NULL,
		windowClassForWindow23Public.lpszClassName,
		L"Window 23",
		WS_VISIBLE | WS_CHILD,
		WINDOWXPOS,
		WINDOWYPOS,
		SUBWINDOWLENGTH,
		SUBWINDOWHEIGHT,
		handleforwindow,
		NULL,
		windowClassForWindow23Public.hInstance,
		NULL                /* No Window Creation data */
	);


	if (!handleforwindow23)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window 23 creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}
	handlesfordatawindows[2] = handleforwindow23;
	ShowWindow(handleforwindow23, SW_HIDE);
	
	//////////////////////////////////////////////////

	//Window 24
	DestroyWindow(
		handleforwindow24
	);
	handleforwindow24 = CreateWindowEx(NULL,
		windowClassForWindow24Public.lpszClassName,
		L"Window 24",
		WS_VISIBLE | WS_CHILD,
		WINDOWXPOS,
		WINDOWYPOS,
		SUBWINDOWLENGTH,
		SUBWINDOWHEIGHT,
		handleforwindow,
		NULL,
		windowClassForWindow24Public.hInstance,
		NULL                /* No Window Creation data */
	);


	if (!handleforwindow24)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window 24 creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}
	handlesfordatawindows[3] = handleforwindow24;
	ShowWindow(handleforwindow24, SW_HIDE);

	////////////////////////////////////////////////

	//Window 25
	DestroyWindow(
		handleforwindow25
	);
	handleforwindow25 = CreateWindowEx(NULL,
		windowClassForWindow25Public.lpszClassName,
		L"Window 25",
		WS_VISIBLE | WS_CHILD,
		WINDOWXPOS,
		WINDOWYPOS,
		SUBWINDOWLENGTH,
		SUBWINDOWHEIGHT,
		handleforwindow,
		NULL,
		windowClassForWindow25Public.hInstance,
		NULL                /* No Window Creation data */
	);


	if (!handleforwindow25)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window 25 creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}
	handlesfordatawindows[4] = handleforwindow25;
	ShowWindow(handleforwindow25, SW_HIDE);



	//Scroll the window back
	ScrollWindowEx(handleforwindow, 0, 0 - prevScrollPosVert, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
	ScrollWindowEx(handleforwindow, 0 - prevScrollPosHorz, 0, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
	for (int i = 0; i < BoxNames.size(); i++) {
		CheckDlgButton(handleforwindow2, ID_CHECKBOX[(int)i], BST_UNCHECKED);
	}
	ShowWindow(handlesfordatawindows[0], SW_SHOW);
	ShowWindow(GetDlgItem(handleforwindow, ID_WINDOWTEXT), SW_HIDE);
	setSelectedTab(handlefortabmenu, handlesfordatawindows, itemSelectedBefore);
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////PHRASE STATS///////////////////////////////////////////////////////////////////////


//Gets the phrase stats
StatReturnStorage phraseStatsGet(Msgstreamprocessor messageAnalyser, DataStorage dataStore, HWND handlefordiagwindow, HWND handleforinfowindow) {
	dataStore.dynamicsGet(messageAnalyser, handlefordiagwindow);
	dataStore.firstTimeDynamics = false;
	StatReturnStorage returnData;
	returnData.dataStore = dataStore;
	returnData.messageAnalyser = messageAnalyser;
	PostMessageA(handleforinfowindow, SDM_PHRASEDATAREADY, NULL, NULL);
	return returnData;
}

//Displays phrase data
void displayPhraseData(HWND handleforwindow, Msgstreamprocessor messageAnalyser) {
	std::vector <std::vector <double> > coordinates = { { },  { } };
	std::map<std::string, int> averagedays = {};
	std::string startDay = messageAnalyser.alldays[0];
	std::map<std::string, int> messagesDailyPerson = {};
	for (std::string day : messageAnalyser.alldays) {
		messagesDailyPerson[day] = messageAnalyser.phrasebyday[day]["Total Messages"];
	}
	std::vector<std::string> alldays = getDaysBetween(messageAnalyser.alldays[0], messageAnalyser.alldays[messageAnalyser.alldays.size() - 1]);
	for (std::string day : alldays) {
		coordinates[0].push_back(getNumDays(day, startDay));
		coordinates[1].push_back((averagePastDays(1, day, messagesDailyPerson)));
	}
	std::vector<std::string> allmonths = getMonthsBetween(alldays[0], alldays[alldays.size() - 1]);
	std::vector<double> xAxisVals = getMonthsDays(allmonths);
	std::vector<double>	yAxisVals = yAxisValuesFind(coordinates[1]);
	applyStyles(handleforphraseTimesGraph);
	phraseTimesGraph = CreateLineGraph(handleforphraseTimesGraph, GRAPH1XSIZE - 100, GRAPH1YSIZE - 100, xAxisVals, yAxisVals, MONTHTYPE, INTTYPE, "Month", "Num Messages", ("Daily messages across " + itos(messageAnalyser.alldays.size())) + " days", GRAPH1XPOS, GRAPH1YPOS, allmonths[0]);
	phraseTimesGraph.addLine(coordinates, "Total", true);
	for (std::string person : messageAnalyser.people) {
		if (person != "Total Messages") {
			coordinates = { { },  { } };
			messagesDailyPerson = {};
			for (std::string day : messageAnalyser.alldays) {
				messagesDailyPerson[day] = messageAnalyser.phrasebyday[day][person];
			}
			for (std::string day : messageAnalyser.alldays) {
				coordinates[0].push_back(getNumDays(day, startDay));
				coordinates[1].push_back((averagePastDays(5, day, messagesDailyPerson)));
			}
			phraseTimesGraph.addLine(coordinates, person, true);
		}
	}
	ShowWindow(GetDlgItem(handleforwindow, ID_LOBFCHECKBOX1), SW_SHOW);
	ShowWindow(GetDlgItem(handleforwindow, ID_LOBFCHECKBOX1TEXT), SW_SHOW);




	if (messageAnalyser.allweeks.size() > 0) {
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_PERIODSWITCH1), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES_SCROLL_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES_SCROLL_DOWN), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_SCROLL_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_PAGE_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_SCROLL_DOWN), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_PAGE_DOWN), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1), SW_HIDE);

	}
	for (int i = 0; i < messageAnalyser.people.size(); i++) {
		peopleSelected1[messageAnalyser.people[i]] = true;
		CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1NAMES[(int)i], BST_CHECKED);
	}
	for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
		weeksSelected1[messageAnalyser.allweeks[i]] = true;
		if (i < messageAnalyser.months.size()) {
			monthsSelected1[messageAnalyser.months[i]] = true;
		}
		CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[(int)i], BST_CHECKED);
	}

	ShowWindow(GetDlgItem(handleforwindow, ID_PERIODSWITCH1), SW_SHOW);
	ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP), SW_HIDE);
	ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN), SW_HIDE);
	idDisplayedText1 = "";
	std::vector<std::string> lines = {};
	std::vector <std::string> peopleSelectedArray = mapkeysbool(peopleSelected1);
	std::vector <std::string> monthsSelectedArray = mapkeysbool(monthsSelected1);
	std::vector <std::string> weeksSelectedArray = mapkeysbool(weeksSelected1);
	peopleSelectedArray = peopleArraySort(peopleSelectedArray);
	monthsSelectedArray = dateArraySort(monthsSelectedArray);
	weeksSelectedArray = dayArraySort(weeksSelectedArray);
	int numPeriodsDisplayed = 0, numPeopleDisplayed = 0;
	if (periodSelected1 == "M") {
		for (int i = 0; i < monthsSelectedArray.size(); i++) {
			if (monthsSelected1[monthsSelectedArray[i]]) {
				lines.push_back(monthsSelectedArray[i] + ": \n");
				numPeriodsDisplayed++;
				for (int j = 0; j < peopleSelectedArray.size(); j++) {
					if (peopleSelected1[peopleSelectedArray[j]]) {
						lines.push_back("        " + peopleSelectedArray[j] + ": " + itos(messageAnalyser.phrasebymonth[monthsSelectedArray[i]][peopleSelectedArray[j]]) + "\n");
						numPeopleDisplayed++;
					}
				}
			}
		}

	}
	else {
		for (int i = 0; i < weeksSelectedArray.size(); i++) {
			if (weeksSelected1[weeksSelectedArray[i]]) {
				lines.push_back(weeksSelectedArray[i] + ": \n");
				numPeriodsDisplayed++;
				for (int j = 0; j < peopleSelectedArray.size(); j++) {
					if (peopleSelected1[peopleSelectedArray[j]]) {
						lines.push_back("        " + peopleSelectedArray[j] + ": " + itos(messageAnalyser.phrasebyweek[weeksSelectedArray[i]][peopleSelectedArray[j]]) + "\n");
						numPeopleDisplayed++;
					}
				}
			}
		}
	}
	if (currentBottomDataLine1 > lines.size()) {
		currentTopDataLine1 = lines.size() - 10;
		currentBottomDataLine1 = lines.size();
	}

	int linesNew = 0;
	if (lines.size() > 11) {
		if (currentTopDataLine1 != 0) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_SCROLL_UP), SW_SHOW);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_PAGE_UP), SW_SHOW);
		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_SCROLL_UP), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_PAGE_UP), SW_HIDE);
		}
		for (int i = (currentBottomDataLine1 - 1); i >= currentTopDataLine1; i -= 1) {
			idDisplayedText1 = lines[i] + idDisplayedText1;
		}
		if (currentBottomDataLine1 < lines.size()) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_SCROLL_DOWN), SW_SHOW);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_PAGE_DOWN), SW_SHOW);
		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_SCROLL_DOWN), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_PAGE_DOWN), SW_HIDE);
		}
	}
	else {
		if (numPeriodsDisplayed == 0 || numPeopleDisplayed == 0) {
			idDisplayedText1 = "Select more options";
		}
		else {
			for (int i = 0; i < lines.size(); i++) {
				idDisplayedText1 = idDisplayedText1 + lines[i];
			}
		}
	}
	SetWindowTextA(
		GetDlgItem(handleforwindow, ID_DATA_WINDOW1),
		(idDisplayedText1).c_str()
	);

	ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1), SW_SHOW);

	for (int i = 0; i < messageAnalyser.people.size(); i++) {
		SetWindowPos(
			GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES[(int)i]),
			HWND_BOTTOM,
			375,
			20 + (20 * (i - currentTopCheckboxNamesLine1)),
			20,
			20,
			SWP_SHOWWINDOW
		);
		SetWindowPos(
			GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[(int)i]),
			HWND_BOTTOM,
			400,
			20 + (20 * (i - currentTopCheckboxNamesLine1)),
			100,
			20,
			SWP_SHOWWINDOW
		);
		if ((i - currentTopCheckboxNamesLine1) < 0 && currentTopCheckboxNamesLine1 != 0) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES[(int)i]), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[(int)i]), SW_HIDE);
		}
		if ((i - currentTopCheckboxNamesLine1) > 8) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES[(int)i]), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[(int)i]), SW_HIDE);
		}
	}

	if (currentTopCheckboxNamesLine1 == 0) {
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES_SCROLL_UP), SW_HIDE);
	}
	else {
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES_SCROLL_UP), SW_SHOW);
	}
	if (currentTopCheckboxNamesLine1 + 9 >= (messageAnalyser.people.size())) {
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES_SCROLL_DOWN), SW_HIDE);
	}
	else {
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES_SCROLL_DOWN), SW_SHOW);
	}

	for (int i = 0; i < messageAnalyser.months.size(); i++) {
		SetWindowPos(
			GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[(int)i]),
			HWND_BOTTOM,
			620,
			20 + (20 * (i - currentTopCheckboxMonthsLine1)),
			20,
			20,
			SWP_SHOWWINDOW
		);
		SetWindowPos(
			GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[(int)i]),
			HWND_BOTTOM,
			670,
			20 + (20 * (i - currentTopCheckboxMonthsLine1)),
			100,
			20,
			SWP_SHOWWINDOW
		);
		if ((i - currentTopCheckboxMonthsLine1) < 0 && currentTopCheckboxMonthsLine1 != 0) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[(int)i]), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[(int)i]), SW_HIDE);
		}
		if ((i - currentTopCheckboxMonthsLine1) > 8) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[(int)i]), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[(int)i]), SW_HIDE);
		}
	}

	if (currentTopCheckboxMonthsLine1 == 0) {
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP), SW_HIDE);
	}
	else {
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP), SW_SHOW);
	}
	if (currentTopCheckboxMonthsLine1 + 9 >= (messageAnalyser.months.size())) {
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN), SW_HIDE);
	}
	else {
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN), SW_SHOW);
	}
}


bool getArchive(std::string filename){
	std::ifstream inputFile(filename);
	boost::archive::text_iarchive fileOpened(inputFile);
	return true;
}