#pragma once
#include <string>
#include <map>
#include <vector>
#include <windows.h>
#include <chrono>
#include <shlobj.h>
#include "MessageStreamProcessor.h"
#include "DataStore.h"
#include "graphing.h"
#include "Preferences.h"
#include "Version.h"
using namespace std;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////BASIC CONTINUOUS VARIABLES////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Preferences preferences = getPreferences();
std::vector<std::string> BoxNames = { "Message Number Analysis", "Phrase Processing", "Day By Day", "Reply Times", "Sending Times" };
std::vector<HWND> handlesfordatawindows = {NULL,NULL,NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL};
std::chrono::milliseconds span(0);
bool waitingfordata = false;
bool passwordDiagFinished = false;
std::string passwordDiagText = "";
bool windowsCreatedBefore = false;
bool firstTimeMinimised = true;
bool firstTimeMaximised = false;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////WINDOWS AND IDS///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Format Group Number(16), ClassNumber (16 per group), Item Number (256 per class)
//Main Window GP0
	//UI helpers CL0
		//Browse Button
#define ID_BROWSE_BUTTON 0x0001
//Wait message
#define ID_WAIT_MESSAGE 0x0002
//Scroll Bar
#define ID_SCROLL_BAR_MAIN 0x0003
int prevScrollPosVert = 0;
int prevScrollPosHorz = 0;
//Handle for the inner container window
HWND handleforwindow1;
bool window1closed = false;
HWND handleforwindow2;
bool window2closed = false;
HWND handlefortabmenu;
int itemSelectedBefore = 0;
HWND handleforprogressdialog;
double progress = 0;
double fileLines = 0;
//handle for the outer container window
	//HWND handlefowindow3;
//Primary Boxes - CL1
	//Item 00-0F
long long int ID_WINDOW[16];
#define ID_WINDOWTEXT 0x0005
//Item 10-1F
long long int ID_CHECKBOX[16];
//Item 20-2F
long long int ID_CHECKBOXTEXT[16];

//Filepath chosen by user
std::string filepathChosen;

//Scroll Positions on the versions diag
int versionWindowSize;
int prevVersionScrollPosVert = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			




//Window 1 - Phrase Process - GP1
bool firstTimeWindow1 = true;
	//GP1
		//Data window CL0
#define ID_DATA_WINDOW1 0x1000
//CheckBoxes for each of the names CL1-4
long long int ID_DATA_CHECKBOXES1PERIODS[1024];
//IDs for each of the checkbox texts CL5-8
long long int ID_DATA_CHECKBOX1PERIODS_LABELS[1024];

//Checkboxes for each name CL9
long long int ID_DATA_CHECKBOXES1NAMES[256];
//Checkbox labels CLA
long long int ID_DATA_CHECKBOX1NAMES_LABELS[256];
//IDs for phrase entry CLB
#define ID_PHRASE_ENTRY_BOX 0x1B00
#define ID_PHRASE_ENTRY_LABEL 0x1B01
#define ID_PHRASE_ENTRY_SEARCH_BUTTON 0x1B02
#define  ID_WAIT_MESSAGE_PHRASE 0x1B03
//Up and Down Buttons for the Name Labels, Data and the Text - CLB
#define ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP 0x1B04
#define ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN 0x1B05
#define ID_DATA_CHECKBOXES1PERIODS_PAGE_UP 0x1B06
#define ID_DATA_CHECKBOXES1PERIODS_PAGE_DOWN 0x1B07
#define ID_DATA_CHECKBOXES1NAMES_SCROLL_UP 0x1B08
#define ID_DATA_CHECKBOXES1NAMES_SCROLL_DOWN 0x1B09
#define ID_DATA_CHECKBOXES1NAMES_PAGE_UP 0x1B0A
#define ID_DATA_CHECKBOXES1NAMES_PAGE_DOWN 0x1B0B
#define ID_DATA_WINDOW1_SCROLL_UP 0x1B0C
#define ID_DATA_WINDOW1_SCROLL_DOWN 0x1B0D
#define ID_DATA_WINDOW1_PAGE_UP 0x1B0E
#define ID_DATA_WINDOW1_PAGE_DOWN 0x1B0F
#define ID_CHECKBOX_CASEMATCH 0x1B10
#define ID_CHECKBOX_CASEMATCH_LABEL 0x1B11
//Button that switches the period displayed
#define ID_PERIODSWITCH1 0x1B12

//The phrase entered
std::string phraseEntered = "";

//String storing the text displayed in ID_DATA_WINDOW_1
std::string idDisplayedText1 = "";
//Map storing who is selected in ID_DATA_WINDOW_1
std::map <std::string, bool> peopleSelected1 = {};
//Map storing which month is selected in ID_DATA_WINDOW_1
std::map <std::string, bool> monthsSelected1 = {};

//Map storing which week is selected in ID_DATA_WINDOW_1
std::map <std::string, bool> weeksSelected1 = {};
//Current data lines being displayed
int currentTopDataLine1 = 0;
int currentBottomDataLine1 = 10;
//Current Names CheckBox lines being displayed
int currentTopCheckboxNamesLine1 = 0;
int currentBottomCheckboxNamesLine1 = 11;
int currentTopCheckboxWeeksLine1 = 0;
int currentBottomCheckboxWeeksLine1 = 11;
int currentTopCheckboxMonthsLine1 = 0;
int currentBottomCheckboxMonthsLine1 = 11;

//Handle to the data window
HWND handleforwindow21;
HWND handleforphraseprogressdialog;
HWND handleforphraseTimesGraph;
LineGraph phraseTimesGraph;
#define GRAPH1XPOS 0
#define GRAPH1YPOS 425
#define GRAPH1XSIZE 1500
#define GRAPH1YSIZE 400
#define ID_LOBFCHECKBOX1TEXT 0x1B13
#define ID_LOBFCHECKBOX1 0x1B14
WNDCLASSEX windowClassForWindow21Public;
//Which Period is Selected - Takes value M - Months or W - weeks
std::string periodSelected1 = "M";
HINSTANCE hInst1;
bool firstTimePhrase = true;
//Information about search criteria format [0] = Phrase, [1] = Case Match (takes values 'on' of 'off')
class PhraseSearchData {
private:
	friend class boost::serialization::access;

	
	
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& phrase;
		ar& caseMatch;
	}
public:
	std::string phrase = "Not Set";
	bool caseMatch = false;

};
PhraseSearchData phraseSearchData;
bool waitingforphrasedata = false;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






//DataBox2 - GP2
		//Data window CL0
#define ID_DATA_WINDOW2 0x2000
//Checkboxes for each month CL1-4
long long int ID_DATA_CHECKBOXES2PERIODS[1024];
//Labels for month checkboxes CL5-8
long long int ID_DATA_CHECKBOX2PERIODS_LABELS[1024];
//Checkboxes for each name CL9
long long int ID_DATA_CHECKBOXES2NAMES[256];
//Labels for name checkboxes CLA
long long int ID_DATA_CHECKBOX2NAMES_LABELS[256];
//Up and Down Buttons for the Name Labels, Data and the Text - CLB
#define ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP 0x2B00
#define ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN 0x2B01
#define ID_DATA_CHECKBOXES2PERIODS_PAGE_UP 0x2B02
#define ID_DATA_CHECKBOXES2PERIODS_PAGE_DOWN 0x2B03
#define ID_DATA_CHECKBOXES2NAMES_SCROLL_UP 0x2B04
#define ID_DATA_CHECKBOXES2NAMES_SCROLL_DOWN 0x2B05
#define ID_DATA_CHECKBOXES2NAMES_PAGE_UP 0x2B06
#define ID_DATA_CHECKBOXES2NAMES_PAGE_DOWN 0x2B07
#define ID_DATA_WINDOW2_SCROLL_UP 0x2B08
#define ID_DATA_WINDOW2_SCROLL_DOWN 0x2B09
#define ID_DATA_WINDOW2_PAGE_UP 0x2B0A
#define ID_DATA_WINDOW2_PAGE_DOWN 0x2B0B
//Button that switches the period displayed
#define ID_PERIODSWITCH2 0x2B0C
//String storing the text displayed in ID_DATA_WINDOW_2
std::string idDisplayedText2 = "";
//Map storing who is selected in ID_DATA_WINDOW_2
std::map <std::string, bool> peopleSelected2 = {};
//Map storing which month is selected in ID_DATA_WINDOW_2
std::map <std::string, bool> monthsSelected2 = {};
//Map storing which week is selected in ID_DATA_WINDOW_2
std::map<std::string, bool> weeksSelected2 = {};
//Current data lines being displayed
int currentTopDataLine2 = 0;
int currentBottomDataLine2 = 10;
//Current Names CheckBox lines being displayed
int currentTopCheckboxNamesLine2 = 0;
int currentBottomCheckboxNamesLine2 = 11;
int currentTopCheckboxMonthsLine2 = 0;
int currentBottomCheckboxMonthsLine2 = 11;
int currentTopCheckboxWeeksLine2 = 0;
int currentBottomCheckboxWeeksLine2 = 11;

//Handle to the data window
HWND handleforwindow22;
WNDCLASSEX windowClassForWindow22Public;
//Which Period is Selected - Takes value M - Months or W - weeks
std::string periodSelected2 = "M";

//Bar Chart

#define GRAPH2XPOS 0
#define GRAPH2YPOS 425
#define GRAPH2XSIZE 1500
#define GRAPH2YSIZE 400
HWND handleformessagesSentGraph;
BarGraph messagesSentGraph;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//DataBox 3 - GP3
		//Data window and helpers CL0
#define ID_DATA_WINDOW3 0x3000
#define ID_SEARCH_BUTTON_DATES 0x3001
#define ID_STARTDATE_BOX 0x3002
#define ID_ENDDATE_BOX 0x3003
#define ID_INVALIDDATE_TEXT 0x3004
#define ID_STARTDATE_BOX_LABEL 0x3005
#define ID_ENDDATE_BOX_LABEL 0x3006
#define ID_DATE_BOX_LABEL 0x3007
std::string notEnoughDataText = "Enter a date range";
//Checkboxes for each name CL1
long long int ID_DATA_CHECKBOXES3NAMES[256];
//Labels for name checkboxes CL2
long long int ID_DATA_CHECKBOX3NAMES_LABELS[256];
//Up and Down Buttons for the Name Labels, Data and the Text - CL3
#define ID_DATA_CHECKBOXES3NAMES_SCROLL_UP 0x3300
#define ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN 0x3301
#define ID_DATA_CHECKBOXES3NAMES_PAGE_UP 0x3302
#define ID_DATA_CHECKBOXES3NAMES_PAGE_DOWN 0x3303
#define ID_DATA_WINDOW3_SCROLL_UP 0x3304
#define ID_DATA_WINDOW3_SCROLL_DOWN 0x3305
#define ID_DATA_WINDOW3_PAGE_UP 0x3306
#define ID_DATA_WINDOW3_PAGE_DOWN 0x3307
//String storing the entered data in the search boxes
std::string startDate = "DD/MM/YYYY";
std::string endDate = "DD/MM/YYYY";
//String storing the text displayed in ID_DATA_WINDOW_3
std::string idDisplayedText3 = "";
//Map storing who is selected in ID_DATA_WINDOW_2
std::map <std::string, bool> peopleSelected3 = {};
//Current data lines being displayed
int currentTopDataLine3 = 0;
int currentBottomDataLine3 = 10;
//Current Names CheckBox lines being displayed
int currentTopCheckboxNamesLine3 = 0;
int currentBottomCheckboxNamesLine3 = 11;
//Handle to the data window
HWND handleforwindow23;
WNDCLASSEX windowClassForWindow23Public;
#define GRAPH3XPOS 0
#define GRAPH3YPOS 425
#define GRAPH3XSIZE 1500
#define GRAPH3YSIZE 350
HWND handleformessagesOverTimeGraph;
LineGraph messagesOverTimeGraph;
#define ID_LOBFCHECKBOX3 0x3308
#define ID_LOBFCHECKBOX3TEXT 0x3309


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//DataBox4 - GP4
		//Data window CL0
#define ID_DATA_WINDOW4 0x2000
//Checkboxes for each month CL1-4
long long int ID_DATA_CHECKBOXES4PERIODS[1024];
//Labels for month checkboxes CL5-8
long long int ID_DATA_CHECKBOX4PERIODS_LABELS[1024];
//Checkboxes for each name CL9
long long int ID_DATA_CHECKBOXES4NAMES[256];
//Labels for name checkboxes CLA
long long int ID_DATA_CHECKBOX4NAMES_LABELS[256];
//Up and Down Buttons for the Name Labels, Data and the Text - CLB
#define ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP 0x4B00
#define ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN 0x4B01
#define ID_DATA_CHECKBOXES4PERIODS_PAGE_UP 0x4B02
#define ID_DATA_CHECKBOXES4PERIODS_PAGE_DOWN 0x4B03
#define ID_DATA_CHECKBOXES4NAMES_SCROLL_UP 0x4B04
#define ID_DATA_CHECKBOXES4NAMES_SCROLL_DOWN 0x4B05
#define ID_DATA_CHECKBOXES4NAMES_PAGE_UP 0x4B06
#define ID_DATA_CHECKBOXES4NAMES_PAGE_DOWN 0x4B07
#define ID_DATA_WINDOW4_SCROLL_UP 0x4B08
#define ID_DATA_WINDOW4_SCROLL_DOWN 0x4B09
#define ID_DATA_WINDOW4_PAGE_UP 0x4B0A
#define ID_DATA_WINDOW4_PAGE_DOWN 0x4B0B
//Button that switches the period displayed
#define ID_PERIODSWITCH4 0x4B0C
//String storing the text displayed in ID_DATA_WINDOW_4
std::string idDisplayedText4 = "";
//Map storing who is selected in ID_DATA_WINDOW_4
std::map <std::string, bool> peopleSelected4 = {};
//Map storing which month is selected in ID_DATA_WINDOW_4
std::map <std::string, bool> monthsSelected4 = {};
//Map storing which week is selected in ID_DATA_WINDOW_4
std::map<std::string, bool> weeksSelected4 = {};
//Current data lines being displayed
int currentTopDataLine4 = 0;
int currentBottomDataLine4 = 10;
//Current Names CheckBox lines being displayed
int currentTopCheckboxNamesLine4 = 0;
int currentBottomCheckboxNamesLine4 = 11;
int currentTopCheckboxMonthsLine4 = 0;
int currentBottomCheckboxMonthsLine4 = 11;
int currentTopCheckboxWeeksLine4 = 0;
int currentBottomCheckboxWeeksLine4 = 11;

//Handle to the data window
HWND handleforwindow24;
WNDCLASSEX windowClassForWindow24Public;
//Which Period is Selected - Takes value M - Months or W - weeks
std::string periodSelected4 = "M";

//Graph

LineGraph replyTimesGraph;
HWND handleforreplyTimesGraph;
#define GRAPH4XPOS 0
#define GRAPH4YPOS 425
#define GRAPH4XSIZE 1500
#define GRAPH4YSIZE 350





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//DataBox 5 GP5
		//Handle to data window
HWND handleforwindow25;
WNDCLASSEX windowClassForWindow25Public;

//TrackBar - GP0
#define ID_TRACKBAR5 0x5000
#define ID_TEST5 0x5001



//Checkboxes for each name CL9
long long int ID_DATA_CHECKBOXES5NAMES[256];
//Labels for name checkboxes CLA
long long int ID_DATA_CHECKBOX5NAMES_LABELS[256];
//Up and Down Buttons for the Name Labels, Data and the Text - CLB
#define ID_DATA_CHECKBOXES5NAMES_SCROLL_UP 0x5B04
#define ID_DATA_CHECKBOXES5NAMES_SCROLL_DOWN 0x5B05
#define ID_DATA_CHECKBOXES5NAMES_PAGE_UP 0x5B06
#define ID_DATA_CHECKBOXES5NAMES_PAGE_DOWN 0x5B07
//Button that switches the period displayed
//Map storing who is selected in ID_DATA_WINDOW_5
std::map <std::string, bool> peopleSelected5 = {};
//Current Names CheckBox lines being displayed
int currentTopCheckboxNamesLine5 = 0;
int currentBottomCheckboxNamesLine5 = 11;




//LineGraph - GP1
LineGraph sendingTimesGraph;
HWND handleforsendingTimesGraph;
int paintTimes = 0;
bool firstTime = true;
#define GRAPH5XPOS 0
#define GRAPH5YPOS 200
#define GRAPH5XSIZE 1100
#define GRAPH5YSIZE 400

std::vector <std::string> xAxisHours = { "00:00", "02:00", "04:00", "06:00", "08:00", "10:00", "12:00",
									"14:00", "16:00", "18:00", "20:00", "22:00", "00:00" };

bool dataGot = false;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////WINDOW MESSAGES///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////ALL BEGIN SDM


//Thread finished messages

#define SDM_DATAREADY 0x9000
#define SDM_PHRASEDATAREADY 0x9001
#define SDM_SECONDSTART 0x9002
#define SDM_LOADCBD 0x9003
#define SDM_PHRASEDATALOAD 0x9004
#define SDM_DAYDATALOAD 0x9005
#define SDM_REPLYGRAPHLOAD 0x9006
#define SDM_ADVANCEPROGRESS 0x9007
#define SDM_ANALYSISFINSIHED 0x9008
#define SDM_SET100 0x9009
#define SDM_EXIT 0x900A
#define SDM_UPDATENOW 0x900B
#define SDM_SWITCHTOPHRASE 0x900C
#define SDM_NEWPHRASESEARCH 0x900D

//////////////////////////////////////////////////////FILE TYPES////////////////////////////////////////////////////////////////////

#define WHATSAPP_FILE 0








////////////////////////////////////////////////////////FILE EXTENSIONS//////////////////////////////////////////////////////////////
std::map<LPCWSTR, LPCWSTR> EXPORTEDCHAT;
std::map<LPCWSTR, LPCWSTR> ANALYSEDCHAT;
void fileExtensionsSet() {
	EXPORTEDCHAT[L"Exported Chat file"] = L"*.txt";
	ANALYSEDCHAT[L"Analysed Chat"] = L"*.ca";
}

////////////////////////////////////////////////////////FILES USED////////////////////////////////////////////////////////////////////
/*std::string getAppDataFolder() {
	wchar_t* localAppData = 0;
	SHGetKnownFolderPath(FOLDERID_AppDataProgramData, 0, NULL, &localAppData);

	wstringstream ss;
	ss << localAppData;

	CoTaskMemFree(static_cast<void*>(localAppData));
	std::wstring wide = ss.str();
	std::string newString(wide.begin(), wide.end());
	return newString;
}
#define APP_DATA_FOLDER getAppDataFolder()*/
#define CHAT_RESTORE_FILE "Data\\Rchat.rmf"
#define PROGRAM_CLOSE_FILE "Data\\ProgramRestoreData.mrd"
#define TEMPORARY_FILE "Temp\\temp.tmp"
#define UPDATER "AutoUpdater.exe\0\0"
#define FIRSTTIMEFILE "Data\\FirstTime.txt"
#define BETATESTFILE "Data\\BetaTester.ini"
std::string BETATESTPASSWORD = "0208";
//In Seconds (1 day)
int updateCheckPeriod = preferences.getUpdatePeriod();
std::string currentFilePath = "";

//////////////////////////////////////////////////////ENCRYPTION DEFINITIONS////////////////////////////////////////////////////////
#define KEYLENGTH  0x00800000
#define ENCRYPT_ALGORITHM CALG_RC4 
#define ENCRYPT_BLOCK_SIZE 8 
LPCWSTR passwordIn = preferences.getPassword();


////////////////////////////////////////////////////HOT KEY DEFINITIONS/////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////FUNCTION DEFINITIONS///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Value Reseting Macro
void resetParameters() {

	peopleSelected1 = {};
	monthsSelected1 = {};
	weeksSelected1 = {};
	currentTopDataLine1 = 0;
	currentBottomDataLine1 = 10;
	currentTopCheckboxNamesLine1 = 0;
	currentBottomCheckboxNamesLine1 = 11;
	currentTopCheckboxWeeksLine1 = 0;
	currentBottomCheckboxWeeksLine1 = 11;
	currentTopCheckboxMonthsLine1 = 0;
	currentBottomCheckboxMonthsLine1 = 11;
	periodSelected1 = "M";
	firstTimePhrase = true;
	phraseSearchData.phrase = "Not Set";
	phraseSearchData.caseMatch = false;


	peopleSelected2 = {};
	monthsSelected2 = {};
	weeksSelected2 = {};
	currentTopDataLine2 = 0;
	currentBottomDataLine2 = 10;
	currentTopCheckboxNamesLine2 = 0;
	currentBottomCheckboxNamesLine2 = 11;
	currentTopCheckboxMonthsLine2 = 0;
	currentBottomCheckboxMonthsLine2 = 11;
	currentTopCheckboxWeeksLine2 = 0;
	currentBottomCheckboxWeeksLine2 = 11;
	periodSelected2 = "M";



	peopleSelected3 = {};
	currentTopDataLine3 = 0;
	currentBottomDataLine3 = 10;
	currentTopCheckboxNamesLine3 = 0;
	currentBottomCheckboxNamesLine3 = 11;
	startDate = "DD/MM/YYYY";
	endDate = "DD/MM/YYYY";
	notEnoughDataText = "Enter a date range";




	peopleSelected4 = {};
	monthsSelected4 = {};
	weeksSelected4 = {};
	currentTopDataLine4 = 0;
	currentBottomDataLine4 = 10;
	currentTopCheckboxNamesLine4 = 0;
	currentBottomCheckboxNamesLine4 = 11;
	currentTopCheckboxMonthsLine4 = 0;
	currentBottomCheckboxMonthsLine4 = 11;
	currentTopCheckboxWeeksLine4 = 0;
	currentBottomCheckboxWeeksLine4 = 11;
	periodSelected4 = "M";

	peopleSelected5 = {};
	currentTopCheckboxNamesLine5 = 0;
	currentBottomCheckboxNamesLine5 = 11;



}



//ID Setting Macro
void IDSet() {
	long long int i;
	for (i = 0; i < 16; i++) {
		ID_WINDOW[i] = 0x0100 + i;
		ID_CHECKBOX[i] = 0x0110 + i;
		ID_CHECKBOXTEXT[i] = 0x0120 + i;
	}
	for (i = 0; i < 1024; i++) {
		ID_DATA_CHECKBOXES1PERIODS[i] = 0x1100 + i;
		ID_DATA_CHECKBOX1PERIODS_LABELS[i] = 0x1500 + i;
		ID_DATA_CHECKBOXES2PERIODS[i] = 0x2100 + i;
		ID_DATA_CHECKBOX2PERIODS_LABELS[i] = 0x2500 + i;
		ID_DATA_CHECKBOXES4PERIODS[i] = 0x4100 + i;
		ID_DATA_CHECKBOX4PERIODS_LABELS[i] = 0x4500 + i;
	}
	for (i = 0; i < 256; i++) {
		ID_DATA_CHECKBOXES1NAMES[i] = 0x1900 + i;
		ID_DATA_CHECKBOX1NAMES_LABELS[i] = 0x1A00 + i;
		ID_DATA_CHECKBOXES2NAMES[i] = 0x2900 + i;
		ID_DATA_CHECKBOX2NAMES_LABELS[i] = 0x2A00 + i;
		ID_DATA_CHECKBOXES3NAMES[i] = 0x3100 + i;
		ID_DATA_CHECKBOX3NAMES_LABELS[i] = 0x3200 + i;
		ID_DATA_CHECKBOXES4NAMES[i] = 0x4900 + i;
		ID_DATA_CHECKBOX4NAMES_LABELS[i] = 0x4A00 + i;
		ID_DATA_CHECKBOXES5NAMES[i] = 0x5900 + i;
		ID_DATA_CHECKBOX5NAMES_LABELS[i] = 0x5A00 + i;


	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




