#include <windows.h>
#include <thread> 
#include <future>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <tchar.h>
#include <shobjidl.h>
#include <atlstr.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <typeinfo>
#include "Array Processing Functions.h"
#include "Conversion Functions.h"
#include "DataStore.h"
#include "FileProcessing.h"
#include "graphing.h"
#include "NewDefinitions.h"
#include "Key Retrieval Functions.h"
#include "Line.h"
#include "Map Processing Functions.h"
#include "MultiThread Functions.h"
#include "MessageStreamProcessor.h"
#include "resource.h"
#include "String Processing Functions.h"
#include "Styles.h"
#include "Test.h"
#include "Time Period Finding Functions.h"
#include "UIFunctions.h"
#include "Updater.h"
#include "Validation.h"
#include "resource.h"

//#include "resource.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/////////    ////////////     ///////////////////                      ///////////////////
////////     ///////////     ////////////////////////////   //////////////////////////////
///////     ///////////     ////////////////////////////   ///////////////////////////////
//////     ///////////     ////////////////////////////   ////////////////////////////////
/////     ///////////     ////////////////////////////   /////////////////////////////////
////     ///////////     ////////////////////////////   //////////////////////////////////
///     ///////////     ////////////////////////////   ///////////////////////////////////
//                     /////////////////////                      ////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

std::shared_future<StatReturnStorage> statsFuture;
std::shared_future<StatReturnStorage> phraseStatsFuture;
Msgstreamprocessor messageAnalyser;
DataStorage dataStore;
SaveData savedData;



void loadChatFile(SaveData savedData) {
	messageAnalyser = savedData.messageAnalyser;
	messageAnalyser.filepath = CHAT_RESTORE_FILE;
	rebuildChat(messageAnalyser);
	dataStore = savedData.dataStore;
	ifstream reconsChat(CHAT_RESTORE_FILE);
	std::string temp = "";
	reconsChat >> temp;
	reconsChat.close();
	if (temp != "") {
		windowsCreatedBefore = savedData.windowsCreatedBeforestore;
		windowsCreate(handleforwindow2);
		savedData.load();
		SendMessage(handleforwindow21, SDM_PHRASEDATALOAD, NULL, NULL);
		SendMessage(handleforwindow23, SDM_DAYDATALOAD, NULL, NULL);
		for (HWND window : handlesfordatawindows) {
			if (window != NULL) {
				SendMessage(window, WM_COMMAND, NULL, NULL);
			}
		}
	}
	else {
		MessageBoxA(NULL,
			"File is Empty, chose another",
			"Empty File!",
			MB_ICONWARNING | MB_OK);
	}
}


LRESULT CALLBACK windowprocessforwindow1(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow2(HWND handleforwindow2, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow21(HWND handleforwindow21, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow22(HWND handleforwindow22, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow23(HWND handleforwindow23, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow24(HWND handleforwindow24, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow25(HWND handleforwindow25, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PasswordEnterProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK PreferencesProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ProgressBarProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK VersionInfoProc(HWND, UINT, WPARAM, LPARAM);

bool betaTester() {
	ifstream betaFile(BETATESTFILE);
	std::string data;
	if (betaFile.is_open()) {
		getline(betaFile, data);
		if (data.size() == 3 + BETATESTPASSWORD.size()) {
			if (data.substr(0, 1) == "T") {
				if (data.substr(1, 2) == "||") {
					if (data.substr(3, 4) == BETATESTPASSWORD) {
						betaFile.close();
						return true;
					}
				}
			}
		}
		betaFile.close();
	}
	return false;
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	setVersionInfo();
	setStyles(preferences);
	if (not betaTester()) {
		if (not fileExists(FIRSTTIMEFILE)) {
			LPWSTR pBuf = new wchar_t[1000];
			GetModuleFileName(NULL, pBuf, 1000);
			LPCWSTR filePath = (LPCWSTR)pBuf;
			wstring filePathToConvert = filePath;
			wstring newFilePath = L"";
			delete[] pBuf;
			for (int i = (int)filePathToConvert.length() - 1; i > 0; i -= 1) {
				if (filePath[i] == '\\') {
					filePathToConvert = filePathToConvert.substr(0, i);
					i = -1;
				}
			}
			for (int i = (int)filePathToConvert.length() - 1; i > 0; i -= 1) {
				if (filePath[i] == '\\') {
					filePathToConvert = filePathToConvert.substr(0, i);
					i = -1;
				}
			}
			newFilePath = filePathToConvert + L"\\Setup.exe";
			LPCWSTR progFilePath = newFilePath.c_str();
			// start the program up
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			// set the size of the structures
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			CreateProcess(progFilePath,   // the path
				NULL,   
				NULL,   
				NULL,      
				FALSE,
				0,
				NULL,
				NULL,
				&si,
				&pi
			);
			// Close process and thread handles. 
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return 0;
		}
		if (installUpdates()) {
			return 0;
		}
	}
	hInst1 = hInst;
	IDSet();
	fileExtensionsSet();
	messageAnalyser.setFilepath("Not Set");
	bool endprogram = false;

	//create window 1

	
	WNDCLASSEX windowclassforwindow1;
	ZeroMemory(&windowclassforwindow1, sizeof(WNDCLASSEX));
	windowclassforwindow1.cbClsExtra = NULL;
	windowclassforwindow1.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow1.cbWndExtra = NULL;
	windowclassforwindow1.hbrBackground = BACKGROUND_COLOUR_BRUSH;
	windowclassforwindow1.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow1.hIcon = (HICON)LoadImage(NULL, L"F:\\all\\Code\\C++\\UserInterface\\UI\\ProgramIcon.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);;
	windowclassforwindow1.hIconSm = NULL;
	windowclassforwindow1.hInstance = hInst;
	windowclassforwindow1.lpfnWndProc = (WNDPROC)windowprocessforwindow1;
	windowclassforwindow1.lpszClassName = L"window class1";
	windowclassforwindow1.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	windowclassforwindow1.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&windowclassforwindow1))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window 1 class creation failed",
			L"Window Class Failed",
			MB_ICONERROR);
	}

	HWND handleforwindow1 = CreateWindowEx(NULL,
		windowclassforwindow1.lpszClassName,
		L"WhatsAppnalysis",
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, 
		0,
		0,
		GetSystemMetrics(SM_CXFULLSCREEN),
		GetSystemMetrics(SM_CYFULLSCREEN),
		NULL,
		NULL,
		hInst,
		NULL                /* No Window Creation data */
	);

	if (!handleforwindow1)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window 1 creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(handleforwindow1, SW_MAXIMIZE);
	
	// create window 2

	WNDCLASSEX windowclassforwindow2;
	ZeroMemory(&windowclassforwindow2, sizeof(WNDCLASSEX));
	windowclassforwindow2.cbClsExtra = NULL;
	windowclassforwindow2.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow2.cbWndExtra = NULL;
	windowclassforwindow2.hbrBackground = BACKGROUND_COLOUR_BRUSH;
	windowclassforwindow2.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow2.hIcon = NULL;
	windowclassforwindow2.hIconSm = NULL;
	windowclassforwindow2.hInstance = hInst;
	windowclassforwindow2.lpfnWndProc = (WNDPROC)windowprocessforwindow2;
	windowclassforwindow2.lpszClassName = L"window class2";
	windowclassforwindow2.lpszMenuName = NULL;
	windowclassforwindow2.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&windowclassforwindow2))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window 2 class creation failed",
			L"Window Class Failed",
			MB_ICONERROR);
	}
	
	handleforwindow2 = CreateWindowEx(NULL,
		windowclassforwindow2.lpszClassName,
		L"Window 2",
		WS_VISIBLE | WS_CHILD,
		0,
		25,
		WINDOWLENGTH,
		WINDOWHEIGHT,
		handlefortabmenu,
		NULL,
		hInst,
		NULL                
	);

	if (!handleforwindow2)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"Window 2 creation failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}
	ShowWindow(handleforwindow2, SW_SHOW);
	////////////////////////////////////////////////////SUB WINDOW CLASSES/////////////////////////////////////////////////////


	//1
	WNDCLASSEX windowclassforwindow21;
	ZeroMemory(&windowclassforwindow21, sizeof(WNDCLASSEX));
	windowclassforwindow21.cbClsExtra = NULL;
	windowclassforwindow21.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow21.cbWndExtra = NULL;
	windowclassforwindow21.hbrBackground = BACKGROUND_COLOUR_BRUSH;
	windowclassforwindow21.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow21.hIcon = NULL;
	windowclassforwindow21.hIconSm = NULL;
	windowclassforwindow21.hInstance = hInst;
	windowclassforwindow21.lpfnWndProc =
		(WNDPROC)windowprocessforwindow21;
	windowclassforwindow21.lpszClassName = L"window class21";
	windowclassforwindow21.lpszMenuName = NULL;
	windowclassforwindow21.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&windowclassforwindow21))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window 21 class creation failed",
			L"Window Class Failed",
			MB_ICONERROR);
	}
	windowClassForWindow21Public = windowclassforwindow21;


	//2
	WNDCLASSEX windowclassforwindow22;
	ZeroMemory(&windowclassforwindow22, sizeof(WNDCLASSEX));
	windowclassforwindow22.cbClsExtra = NULL;
	windowclassforwindow22.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow22.cbWndExtra = NULL;
	windowclassforwindow22.hbrBackground = BACKGROUND_COLOUR_BRUSH;
	windowclassforwindow22.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow22.hIcon = NULL;
	windowclassforwindow22.hIconSm = NULL;
	windowclassforwindow22.hInstance = hInst;
	windowclassforwindow22.lpfnWndProc = (WNDPROC)windowprocessforwindow22;
	windowclassforwindow22.lpszClassName = L"window class22";
	windowclassforwindow22.lpszMenuName = NULL;
	windowclassforwindow22.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&windowclassforwindow22))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window 22 class creation failed",
			L"Window Class Failed",
			MB_ICONERROR);
	}

	windowClassForWindow22Public = windowclassforwindow22;


	//3
	WNDCLASSEX windowclassforwindow23;
	ZeroMemory(&windowclassforwindow23, sizeof(WNDCLASSEX));
	windowclassforwindow23.cbClsExtra = NULL;
	windowclassforwindow23.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow23.cbWndExtra = NULL;
	windowclassforwindow23.hbrBackground = BACKGROUND_COLOUR_BRUSH;
	windowclassforwindow23.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow23.hIcon = NULL;
	windowclassforwindow23.hIconSm = NULL;
	windowclassforwindow23.hInstance = hInst;
	windowclassforwindow23.lpfnWndProc = (WNDPROC)windowprocessforwindow23;
	windowclassforwindow23.lpszClassName = L"window class23";
	windowclassforwindow23.lpszMenuName = NULL;
	windowclassforwindow23.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&windowclassforwindow23))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window 23 class creation failed",
			L"Window Class Failed",
			MB_ICONERROR);
	}
	windowClassForWindow23Public = windowclassforwindow23;


	//4
	WNDCLASSEX windowclassforwindow24;
	ZeroMemory(&windowclassforwindow24, sizeof(WNDCLASSEX));
	windowclassforwindow24.cbClsExtra = NULL;
	windowclassforwindow24.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow24.cbWndExtra = NULL;
	windowclassforwindow24.hbrBackground = BACKGROUND_COLOUR_BRUSH;
	windowclassforwindow24.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow24.hIcon = NULL;
	windowclassforwindow24.hIconSm = NULL;
	windowclassforwindow24.hInstance = hInst;
	windowclassforwindow24.lpfnWndProc = (WNDPROC)windowprocessforwindow24;
	windowclassforwindow24.lpszClassName = L"window class24";
	windowclassforwindow24.lpszMenuName = NULL;
	windowclassforwindow24.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&windowclassforwindow24))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window 24 class creation failed",
			L"Window Class Failed",
			MB_ICONERROR);
	}

	windowClassForWindow24Public = windowclassforwindow24;


	//5
	WNDCLASSEX windowclassforwindow25;
	ZeroMemory(&windowclassforwindow25, sizeof(WNDCLASSEX));
	windowclassforwindow25.cbClsExtra = NULL;
	windowclassforwindow25.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow25.cbWndExtra = NULL;
	windowclassforwindow25.hbrBackground = BACKGROUND_COLOUR_BRUSH;
	windowclassforwindow25.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow25.hIcon = NULL;
	windowclassforwindow25.hIconSm = NULL;
	windowclassforwindow25.hInstance = hInst;
	windowclassforwindow25.lpfnWndProc = (WNDPROC)windowprocessforwindow25;
	windowclassforwindow25.lpszClassName = L"window class25";
	windowclassforwindow25.lpszMenuName = NULL;
	windowclassforwindow25.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&windowclassforwindow25))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window 25 class creation failed",
			L"Window Class Failed",
			MB_ICONERROR);
	}

	windowClassForWindow25Public = windowclassforwindow25;


	HACCEL hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////MESSAGE LOOP

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	endprogram = false;
	while (not endprogram) {
		if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateAccelerator(handleforwindow1, hAccel, &msg);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (window1closed) {
			endprogram = true;
		}
	}
	return 0;
}






////////////////////////////////////////////////////////////////////WINDOW 1///////////////////////////////////////////////////////









LRESULT CALLBACK windowprocessforwindow1(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam) {
	/*
	if (waitingfordata) {
		if (statsFuture.wait_for(span) == std::future_status::ready) {
			waitingfordata = false;
			SendMessage(handleforwindow, SDM_DATAREADY, NULL, NULL);
		}
	}
	*/
	switch (msg)
	{
	
	
	case WM_CTLCOLORSTATIC: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CTLCOLORBTN: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}
	case WM_CTLCOLORSCROLLBAR: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}
	

	case WM_NOTIFY: {
		OnNotify(handlefortabmenu, lParam, handlesfordatawindows, &itemSelectedBefore);
		break;
	}
	case WM_CREATE: {
		RECT rect;
		GetWindowRect(handleforwindow, &rect);
		SetScrollRange(handleforwindow, SB_VERT, 0, WINDOWHEIGHT - (rect.bottom - rect.top), true);
		SetScrollRange(handleforwindow, SB_HORZ, 0, WINDOWLENGTH - (rect.right-rect.left), true);
		HICON hIconSm;
		HICON hIconLg;
		handlefortabmenu = DoCreateTabControl(handleforwindow, hInst1, BoxNames.size());
		hIconSm = (HICON)LoadImage(NULL, L"F:\\all\\Code\\C++\\UserInterface\\UI\\ProgramIcon.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		hIconLg = (HICON)LoadImage(NULL, L"F:\\all\\Code\\C++\\UserInterface\\UI\\ProgramIcon.ico", IMAGE_ICON, 1024, 1024, LR_LOADFROMFILE);
		if (hIconSm) {
			SendMessage(handleforwindow, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
			SendMessage(handleforwindow, WM_SETICON, ICON_BIG, (LPARAM)hIconLg);
		}
		else {
			MessageBox(handleforwindow, L"Could not load small icon!", L"Error", MB_OK | MB_ICONERROR);
		}
		break;
	}


	case SDM_SECONDSTART: {
		LPCWSTR password = PASSWORD;
		if (fileExists(PROGRAM_CLOSE_FILE)) {
			SaveData savedData;
			if (openFile(PROGRAM_CLOSE_FILE, password, &savedData)) {
				loadChatFile(savedData);
			}
			else {
				remove(PROGRAM_CLOSE_FILE);
				MessageBoxA(NULL,
					"Couldn't open previous session, reverting to default",
					"Restore Error!",
					MB_ICONWARNING | MB_OK);
			}
		}
		else {
			MessageBoxA(NULL,
				"No previous session found, starting new session",
				"Restore Error!",
				MB_ICONWARNING | MB_OK);
		}
		return 0;
		
	}

	case WM_SIZE: {
		break;
	}

		
	case WM_VSCROLL: {
		int BarPosition = NULL;
		switch (LOWORD(wParam)) {
		case SB_THUMBTRACK: {
			BarPosition = HIWORD(wParam);
			if (BarPosition < 0 || BarPosition == NULL) {
				BarPosition = 0;
			}
			break;

		}
		case SB_LINEUP: {
			if (prevScrollPosVert - 20 > 0) {
				BarPosition = prevScrollPosVert - 20;
			}
			else {
				BarPosition = 0;
			}
			break;
		}

		case SB_LINEDOWN: {
			if (prevScrollPosVert + 20 < WINDOWHEIGHT) {
				BarPosition = prevScrollPosVert + 20;
			}
			else {
				BarPosition = WINDOWHEIGHT;
			}
			break;
		}
		case SB_PAGEUP: {
			if (prevScrollPosVert - 100 > 0) {
				BarPosition = prevScrollPosVert - 100;
			}
			else {
				BarPosition = 0;
			}
			break;
		}

		case SB_PAGEDOWN: {
			if (prevScrollPosVert + 100 < WINDOWHEIGHT) {
				BarPosition = prevScrollPosVert + 100;
			}
			else {
				BarPosition = WINDOWHEIGHT;
			}
			break;
		}
		default: {
			return DefWindowProc(handleforwindow, msg, wParam, lParam);
		}
		}
		SetScrollPos(
			handleforwindow,
			SB_VERT,
			BarPosition,
			true
		);
		int scrollAmount = BarPosition - prevScrollPosVert;
		scrollAmount *= -1;
		ScrollWindowEx(handleforwindow, 0, scrollAmount, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
		prevScrollPosVert = BarPosition;
		break;
	}


	case WM_HSCROLL: {
		int BarPosition = 0;
		switch (LOWORD(wParam)) {
		case SB_THUMBTRACK: {
			BarPosition = HIWORD(wParam);
			if (BarPosition < 0 || BarPosition == NULL) {
				BarPosition = 0;
			}
			break;
		}
		case SB_LINEUP: {
			if (prevScrollPosHorz - 20 > 0) {
				BarPosition = prevScrollPosHorz - 20;
			}
			else {
				BarPosition = 0;
			}
			break;
		}

		case SB_LINEDOWN: {
			if (prevScrollPosHorz + 20 < WINDOWLENGTH) {
				BarPosition = prevScrollPosHorz + 20;
			}
			else {
				BarPosition = WINDOWLENGTH;
			}
			break;
		}
		case SB_PAGEUP: {
			if (prevScrollPosHorz - 100 > 0) {
				BarPosition = prevScrollPosHorz - 100;
			}
			else {
				BarPosition = 0;
			}
			break;
		}

		case SB_PAGEDOWN: {
			if (prevScrollPosHorz + 100 < WINDOWLENGTH) {
				BarPosition = prevScrollPosHorz + 100;
			}
			else {
				BarPosition = WINDOWLENGTH;
			}
			break;
		}
		default: {
			return DefWindowProc(handleforwindow, msg, wParam, lParam);
		}
		}
		SetScrollPos(
			handleforwindow,
			SB_HORZ,
			BarPosition,
			true
		);
		int scrollAmount = BarPosition - prevScrollPosHorz;
		scrollAmount *= -1;
		ScrollWindowEx(handleforwindow, scrollAmount, 0, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
		prevScrollPosHorz = BarPosition;
		break;
	}



	case WM_PAINT: {
		if (firstTimeWindow1) {
			firstTimeWindow1 = false;
			SendMessage(handleforwindow, SDM_SECONDSTART, NULL, NULL);
		}
		InvalidateRect(handleforwindow, NULL, true);
		break;
	}

		
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case HK_OPEN: {
			SendMessage(handleforwindow, WM_COMMAND, MAKEWPARAM(ID_FILE_OPEN, NULL), NULL);
			break;
		}
		case HK_SAVE: {
			SendMessage(handleforwindow, WM_COMMAND, MAKEWPARAM(ID_FILE_SAVE, NULL), NULL);
			break;
		}
		case HK_SAVEAS: {
			SendMessage(handleforwindow, WM_COMMAND, MAKEWPARAM(ID_FILE_SAVEAS, NULL), NULL);
			break;
		}
		case HK_IMPORTWHATSAPP: {
			SendMessage(handleforwindow, WM_COMMAND, MAKEWPARAM(ID_IMPORT_WHATSAPP, NULL), NULL);
			break;
		}
		case HK_IMPORTVIBER: {
			SendMessage(handleforwindow, WM_COMMAND, MAKEWPARAM(ID_IMPORT_VIBER, NULL), NULL);
			break;
		}
		case HK_PAGEUP: {
			SendMessage(handleforwindow, WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, NULL), NULL);
			break;
		}
		case HK_PAGEDOWN: {
			SendMessage(handleforwindow, WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, NULL), NULL);
			break;
		}
		case HK_ARROWUP: {
			SendMessage(handleforwindow, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, NULL), NULL);
			break;
		}
		case HK_ARROWDOWN: {
			SendMessage(handleforwindow, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, NULL), NULL);
			break;
		}
		case HK_ARROWLEFT: {
			SendMessage(handleforwindow, WM_HSCROLL, MAKEWPARAM(SB_LINELEFT, NULL), NULL);
			break;
		}
		case HK_ARROWRIGHT: {
			SendMessage(handleforwindow, WM_HSCROLL, MAKEWPARAM(SB_LINERIGHT, NULL), NULL);
			break;
		}
		case ID_IMPORT_WHATSAPP: {
			windowsCreatedBefore = false;
			std::string filepath = openFileDialog(EXPORTEDCHAT, L"*.txt");
			if (filepath != "") {
				messageAnalyser.setFilepath(filepath);
				messageAnalyser.fileType = WHATSAPP_FILE;
				handleforprogressdialog = CreateDialog(hInst1, MAKEINTRESOURCE(IDD_DIALOG3), NULL, (DLGPROC)ProgressBarProc);
				std::future<StatReturnStorage> statsFutureTemp = std::async(statsGet, messageAnalyser, dataStore, handleforprogressdialog, handleforwindow);
				statsFuture = statsFutureTemp.share();
				waitingfordata = true;
			}
			break;
		}
		
		case ID_FILE_OPEN: {
			std::string filePath = openFileDialog(ANALYSEDCHAT, L"*.ca");
			currentFilePath = filePath;
			SaveData saveData;
			LPCWSTR password = NULL;
			bool success = false;
			if (filePath != "") {
				if (noPwdCheck(filePath, &saveData)) {
					success = true;
				}
				while (!success) {
					passwordDiagText = "The file is password protected, enter the password:";
					if (DialogBox(hInst1, MAKEINTRESOURCE(IDD_DIALOG1), handleforwindow, (DLGPROC)PasswordEnterProc) == IDOK) {
						password = passwordIn;
					}
					else {
						success = true;
					}
					if (openFile(filePath, password, &saveData)) {
						success = true;
					}
					else {
						if (password != NULL) {
							int passwordIncorrect = MessageBoxA(NULL,
								"Password Incorrect",
								"Password Error!",
								MB_ICONWARNING | MB_RETRYCANCEL);
							if (passwordIncorrect == IDRETRY) {
							}

							else {
								success = true;
							}

						}
						else {
							MessageBoxA(NULL,
								"File password protected or corrupt",
								"Opening Error!",
								MB_ICONWARNING | MB_OK);
							success = true;
						}
					}
				}
				loadChatFile(saveData);
				
			}
			break;
		}
		case ID_FILE_SAVE: {
			if (currentFilePath == "" || not fileExists(currentFilePath)) {

			}
			else {
				saveFile(currentFilePath, messageAnalyser, dataStore, passwordIn);
				break;
			}
		}
		case ID_FILE_SAVEAS: {

			std::string filePath = saveFileDialog(ANALYSEDCHAT, L"*.ca");
			currentFilePath = filePath;
			if (filePath != "") {
				LPCWSTR password;
				passwordDiagText = "If you wish, you may enter a password for the file below:";
				if (DialogBox(hInst1, MAKEINTRESOURCE(IDD_DIALOG1), handleforwindow, (DLGPROC)PasswordEnterProc) == IDOK) {
					password = passwordIn;
				}
				else {
					password = PASSWORD;
				}
				passwordIn = PASSWORD;
				saveFile(filePath, messageAnalyser, dataStore, password);


			}
			break;
		}
		case ID_FILE_PREFERENCES: {
			if (DialogBox(hInst1, MAKEINTRESOURCE(IDD_DIALOG2), handleforwindow, (DLGPROC)PreferencesProc) == SDM_UPDATENOW) {
				SendMessage(handleforwindow, SDM_EXIT, NULL, NULL);
			}
			break;
		}
		case ID_FILE_VERSIONINFO: {
			DialogBox(hInst1, MAKEINTRESOURCE(IDD_DIALOG4), handleforwindow, (DLGPROC)VersionInfoProc);
			break;
		}
		}
		break;
	}

	case SDM_EXIT: {
		LPCWSTR password = PASSWORD;
		saveFile(PROGRAM_CLOSE_FILE, messageAnalyser, dataStore, password);
		remove(CHAT_RESTORE_FILE);
		setPreferences(preferences);
		window1closed = true;
		return 0;
	}

	case SDM_DATAREADY: {
		Sleep(10);
		SendMessage(handleforprogressdialog, SDM_SET100, NULL, NULL);
		resetParameters();
		StatReturnStorage returnedStats = statsFuture.get();
		messageAnalyser = returnedStats.messageAnalyser;
		dataStore = returnedStats.dataStore;
		windowsCreate(handleforwindow2);
		windowsCreatedBefore = true;
		SendMessage(handleforprogressdialog, SDM_ANALYSISFINSIHED, NULL, NULL);
		if (messageAnalyser.phrases[0] != "Not Set") {
			SendMessage(handleforwindow21, SDM_NEWPHRASESEARCH, NULL, NULL);
			phraseSearchData.phrase = phraseEntered;
		}
		return 0;
	}
	
	
	case WM_DESTROY: {
		SendMessage(handleforwindow, SDM_EXIT, NULL, NULL);
		MessageBox(NULL,
			L"Thank you for using the WhatsApp analysis tool, see you soon!",
			L"Program Exiting",
			MB_ICONINFORMATION);
		return 0;
	}
	}
	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}







/////////////////////////////////////////////////////////////WINDOW 2//////////////////////////////////////////////////////////////








LRESULT CALLBACK windowprocessforwindow2(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{

	TCHAR greeting[] = _T("Hello, Windows desktop!");
	BOOL ShowText = false;


	switch (msg)
	{

	case WM_CTLCOLORSTATIC: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CTLCOLORBTN: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}


	case WM_CREATE:
	{
		applyStyles(handleforwindow);;

		CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			150, 150, 100, 100,
			handleforwindow,
			(HMENU)ID_WINDOWTEXT, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		SetWindowTextA(
			GetDlgItem(handleforwindow, ID_WINDOWTEXT),
			"Filepath not set"
			//(messageAnalyser.totalmsgsprocess()).c_str()
		);
		CreateWindow(L"static",
			L"",
			WS_CHILD | WS_VISIBLE,
			200, 200, 200, 50,
			handleforwindow, (HMENU)ID_WAIT_MESSAGE, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_WAIT_MESSAGE));

		if (dataStore.firstTime) {
			ShowWindow(GetDlgItem(handleforwindow, ID_WINDOWTEXT), SW_SHOW);
		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_WINDOWTEXT), SW_HIDE);
		}
		applyStyles(GetDlgItem(handleforwindow, ID_WINDOWTEXT));
		
	}

	case WM_PAINT:
	{
		break;
	}
	case WM_DESTROY: {

		return 0;
		break;
	}

	case WM_COMMAND:
	{
		if (dataStore.firstTime) {
			ShowWindow(GetDlgItem(handleforwindow, ID_WINDOWTEXT), SW_SHOW);
		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_WINDOWTEXT), SW_HIDE);
		}
		break;
	}
	}
	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}






///////////////////////////////////////////////////////////////SUBWINDOW 1/////////////////////////////////////////////////////////














LRESULT CALLBACK windowprocessforwindow21(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	BOOL ShowText = false;
	/*
	if (waitingforphrasedata) {
		if (phraseStatsFuture.wait_for(span) == std::future_status::ready) {
			waitingforphrasedata = false;
			SendMessage(handleforwindow, SDM_PHRASEDATAREADY, NULL, NULL);
		}
	}
	*/
	switch (msg)
	{
	
	case WM_CTLCOLORSTATIC: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CTLCOLORBTN: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_DRAWITEM:
	{
		
		drawButton(lParam);
		return TRUE;
	}

	case WM_CREATE:
	{
		//Graph
		handleforphraseTimesGraph = CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			GRAPH1XPOS, GRAPH1YPOS, GRAPH1XSIZE, GRAPH1YSIZE,
			handleforwindow,
			NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		ShowWindow(handleforphraseTimesGraph, SW_SHOW);
		CreateWindow(L"BUTTON", NULL,
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
			50, 120, 20, 20, handleforwindow, (HMENU)ID_LOBFCHECKBOX1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CheckDlgButton(handleforwindow, ID_LOBFCHECKBOX1, BST_CHECKED);
		CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			100, 120, 100, 20, handleforwindow,
			(HMENU)ID_LOBFCHECKBOX1TEXT, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_LOBFCHECKBOX1TEXT));
		SetWindowTextA(GetDlgItem(handleforwindow, ID_LOBFCHECKBOX1TEXT), "Line of Best Fit");
		ShowWindow(GetDlgItem(handleforwindow, ID_LOBFCHECKBOX1), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_LOBFCHECKBOX1TEXT), SW_HIDE);





		CreateWindow(L"BUTTON",
			L"Week View",
			BUTTON_STYLE,
			700, 0, 90, 20,
			handleforwindow, (HMENU)ID_PERIODSWITCH1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		ShowWindow(GetDlgItem(handleforwindow, ID_PERIODSWITCH1), SW_HIDE);
		applyStyles(GetDlgItem(handleforwindow, ID_PERIODSWITCH1));


		CreateWindow(L"Static", L"Enter words or phrases. Separate with ';;' :",
			WS_VISIBLE | WS_CHILD | WS_TABSTOP,
			10, 10, 300, 40,
			handleforwindow, (HMENU)ID_PHRASE_ENTRY_LABEL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_PHRASE_ENTRY_LABEL));
		CreateWindow(TEXT("EDIT"), NULL,
			WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL,
			10, 40, 200, 23,
			handleforwindow, (HMENU)ID_PHRASE_ENTRY_BOX, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_PHRASE_ENTRY_BOX));
		CreateWindow(L"BUTTON",
			L"Search",
			BUTTON_STYLE,
			250, 40, 90, 20,
			handleforwindow, (HMENU)ID_PHRASE_ENTRY_SEARCH_BUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_PHRASE_ENTRY_SEARCH_BUTTON));
		CreateWindow(L"BUTTON",
			NULL,
			WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
			50, 80, 20, 20,
			handleforwindow, (HMENU)ID_CHECKBOX_CASEMATCH, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateWindow(L"Static", L"Case Match",
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			100, 80, 100, 20,
			handleforwindow, (HMENU)ID_CHECKBOX_CASEMATCH_LABEL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_CHECKBOX_CASEMATCH_LABEL));
		ShowWindow(GetDlgItem(handleforwindow, ID_CHECKBOX_CASEMATCH_LABEL), SW_SHOW);
		CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			850, 20, 300, 180,
			handleforwindow,
			(HMENU)ID_DATA_WINDOW1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW1));
		SetWindowTextA(
			GetDlgItem(handleforwindow, ID_DATA_WINDOW1),
			"No Phrase Entered"
		);
		CreateWindow(L"BUTTON",
			L" /\\        UP        /\\ ",
			BUTTON_STYLE,
			850, 0, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW1_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_SCROLL_UP));
		CreateWindow(L"BUTTON",
			L" \\/     DOWN     \\/ ",
			BUTTON_STYLE,
			850, 200, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW1_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_SCROLL_DOWN));
		CreateWindow(L"BUTTON",
			L" /\\    PAGEUP    /\\ ",
			BUTTON_STYLE,
			970, 0, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW1_PAGE_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_PAGE_UP));
		CreateWindow(L"BUTTON",
			L" \\/  PAGEDOWN  \\/ ",
			BUTTON_STYLE,
			970, 200, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW1_PAGE_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_PAGE_DOWN));
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_SCROLL_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_SCROLL_DOWN), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_PAGE_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1_PAGE_DOWN), SW_HIDE);
		
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			CreateWindow(L"BUTTON", NULL,
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				375, (20 + (20 * i)), 20, 20,
				handleforwindow, (HMENU)ID_DATA_CHECKBOXES1NAMES[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i], BST_CHECKED);
			CreateWindow(L"Static", NULL,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
				400, (20 + (20 * i)), 200, 20,
				handleforwindow,
				(HMENU)ID_DATA_CHECKBOX1NAMES_LABELS[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			SetWindowTextA(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[i]),
				(messageAnalyser.people[i]).c_str()
			);
			applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[i]));
			peopleSelected1[messageAnalyser.people[i]] = true;
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i]), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[i]), SW_HIDE);
		}
		CreateWindow(L"BUTTON",
			L" /\\    UP    /\\ ",
			BUTTON_STYLE,
			450, 0, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES1NAMES_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES_SCROLL_UP));
		CreateWindow(L"BUTTON",
			L" \\/ DOWN \\/ ",
			BUTTON_STYLE,
			450, 200, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES1NAMES_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES_SCROLL_DOWN));
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES_SCROLL_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES_SCROLL_DOWN), SW_HIDE);
		int numWeeks = (int)messageAnalyser.allweeks.size();
		for (int i = 0; i < numWeeks; i++) {
			CreateWindow(L"BUTTON", NULL,
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				620, (20 + ((numWeeks - 1) * 20) - (20 * i)), 20, 20,
				handleforwindow, (HMENU)ID_DATA_CHECKBOXES1PERIODS[i], hInst1, NULL);

			CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_CHECKED);
			CreateWindow(L"Static", NULL,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
				670, (20 + ((numWeeks - 1) * 20) - (20 * i)), 100, 20,
				handleforwindow,
				(HMENU)ID_DATA_CHECKBOX1PERIODS_LABELS[i], hInst1, NULL);
			applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]));


			if (i < messageAnalyser.months.size()) {
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
					(messageAnalyser.months[i]).c_str()
				);
				monthsSelected1[messageAnalyser.months[i]] = true;
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]), SW_HIDE);
			}
			weeksSelected1[messageAnalyser.allweeks[i]] = true;
		}





		CreateWindow(L"BUTTON",
			L" /\\    UP    /\\ ",
			BUTTON_STYLE,
			625, 0, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP));
		CreateWindow(L"BUTTON",
			L" \\/ DOWN \\/ ",
			BUTTON_STYLE,
			625, 200, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN));
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN), SW_HIDE);
	}

	case WM_DESTROY: {
		break;
	}


	case SDM_PHRASEDATAREADY: {
		Sleep(10);
		SendMessage(handleforphraseprogressdialog, SDM_SET100, NULL, NULL);
		StatReturnStorage returnedStats;
		returnedStats = phraseStatsFuture.get();
		messageAnalyser = returnedStats.messageAnalyser;
		dataStore = returnedStats.dataStore;
		displayPhraseData(handleforwindow, messageAnalyser);
		dataStore.firstTimeDynamics = false;
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1), SW_SHOW);
		SendMessage(handleforphraseprogressdialog, SDM_ANALYSISFINSIHED, NULL, NULL);
		SetWindowTextA(GetDlgItem(handleforwindow, ID_PHRASE_ENTRY_BOX), phraseEntered.c_str());
		return 0;
	}
	case SDM_PHRASEDATALOAD: {
		SetWindowTextA(GetDlgItem(handleforwindow, ID_PHRASE_ENTRY_BOX), phraseEntered.c_str());
		if (messageAnalyser.caseMatch) {
			CheckDlgButton(handleforwindow, ID_CHECKBOX_CASEMATCH, BST_CHECKED);
		}
		else{
			CheckDlgButton(handleforwindow, ID_CHECKBOX_CASEMATCH, BST_UNCHECKED);
		}
		if (phraseEntered != "") {
			ShowWindow(GetDlgItem(handleforwindow, ID_PERIODSWITCH1), SW_SHOW);
			ShowWindow(GetDlgItem(handleforwindow, ID_LOBFCHECKBOX1), SW_SHOW);
			ShowWindow(GetDlgItem(handleforwindow, ID_LOBFCHECKBOX1TEXT), SW_SHOW);
			for (int i = 0; i < messageAnalyser.people.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i]),
					HWND_BOTTOM,
					375,
					20 + (20 * (i - currentTopCheckboxNamesLine1)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[i]),
					HWND_BOTTOM,
					400,
					20 + (20 * (i - currentTopCheckboxNamesLine1)),
					200,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxNamesLine1) < 0 && currentTopCheckboxNamesLine1 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxNamesLine1) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[i]), SW_HIDE);
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
			
			for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
					""
				);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
			}
			if (periodSelected1 == "W") {
				for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]),
						HWND_BOTTOM,
						620,
						20 + (20 * (i - currentTopCheckboxWeeksLine1)),
						20,
						20,
						SWP_SHOWWINDOW
					);
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
						HWND_BOTTOM,
						670,
						20 + (20 * (i - currentTopCheckboxWeeksLine1)),
						100,
						20,
						SWP_SHOWWINDOW
					);
					if ((i - currentTopCheckboxWeeksLine1) < 0 && currentTopCheckboxWeeksLine1 != 0) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]), SW_HIDE);
					}
					if ((i - currentTopCheckboxWeeksLine1) > 8) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]), SW_HIDE);
					}
					SetWindowTextA(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
						messageAnalyser.allweeks[i].c_str()
					);
				}

				if (currentTopCheckboxWeeksLine1 == 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP), SW_SHOW);
				}
				if (currentTopCheckboxWeeksLine1 + 9 >= (messageAnalyser.allweeks.size())) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN), SW_SHOW);
				}
			}
			else {
				for (int i = 0; i < messageAnalyser.months.size(); i++) {
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]),
						HWND_BOTTOM,
						620,
						20 + (20 * (i - currentTopCheckboxMonthsLine1)),
						20,
						20,
						SWP_SHOWWINDOW
					);
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
						HWND_BOTTOM,
						670,
						20 + (20 * (i - currentTopCheckboxMonthsLine1)),
						100,
						20,
						SWP_SHOWWINDOW
					);
					if ((i - currentTopCheckboxMonthsLine1) < 0 && currentTopCheckboxMonthsLine1 != 0) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]), SW_HIDE);
					}
					if ((i - currentTopCheckboxMonthsLine1) > 8) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]), SW_HIDE);
					}
					SetWindowTextA(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
						messageAnalyser.months[i].c_str()
					);
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
			






			int numPeopleDisplayed = 0;
			int numPeriodsDisplayed = 0;
			idDisplayedText1 = "";
			std::vector<std::string> lines = {};
			std::vector <std::string> peopleSelectedArray = mapkeysbool(peopleSelected1);
			std::vector <std::string> monthsSelectedArray = mapkeysbool(monthsSelected1);
			std::vector <std::string> weeksSelectedArray = mapkeysbool(weeksSelected1);
			peopleSelectedArray = peopleArraySort(peopleSelectedArray);
			monthsSelectedArray = dateArraySort(monthsSelectedArray);
			weeksSelectedArray = dayArraySort(weeksSelectedArray);
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
		}
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			if (peopleSelected1[messageAnalyser.people[i]]) {
				CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i], BST_CHECKED);
			}
			else {
				CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i], BST_UNCHECKED);
			}
		}
		if (periodSelected1 == "M") {
			for (int i = 0; i < messageAnalyser.months.size(); i++) {
				if (monthsSelected1[messageAnalyser.months[i]]) {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_CHECKED);
				}
				else {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_UNCHECKED);
				}
			}
		}
		else {
			for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
				if (weeksSelected1[messageAnalyser.allweeks[i]]) {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_CHECKED);
				}
				else {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_UNCHECKED);
				}
			}
		}
		return 0;
	}

	case SDM_NEWPHRASESEARCH: {
		handleforphraseprogressdialog = CreateDialog(hInst1, MAKEINTRESOURCE(IDD_DIALOG3), NULL, (DLGPROC)ProgressBarProc);
		SetWindowTextA(GetDlgItem(handleforphraseprogressdialog, IDC_STATIC1), "Searching for your phrase(s)...");
		std::future<StatReturnStorage> phraseStatsFutureTemp = std::async(phraseStatsGet, messageAnalyser, dataStore, handleforphraseprogressdialog, handleforwindow);
		waitingforphrasedata = true;
		phraseStatsFuture = phraseStatsFutureTemp.share();
	}
	case WM_COMMAND:
	{
		bool checkBox1NamesScroll = false;
		bool checkBox1PeriodsScroll = false;
		switch (LOWORD(wParam)) {

		case ID_PHRASE_ENTRY_SEARCH_BUTTON: {
			LPWSTR phraseEnteredRaw = new TCHAR[1000];
			GetWindowText(GetDlgItem(handleforwindow, ID_PHRASE_ENTRY_BOX), phraseEnteredRaw, 1000);
			phraseEntered = CW2A(phraseEnteredRaw);
			if (phraseEntered != phraseSearchData.phrase || (((IsDlgButtonChecked(handleforwindow, ID_CHECKBOX_CASEMATCH)) != (phraseSearchData.caseMatch)) && (phraseSearchData.phrase) != ("Not Set"))) {
				messageAnalyser.phrases = getPhrases(phraseEntered, ";;");
				phraseSearchData.caseMatch = IsDlgButtonChecked(handleforwindow, ID_CHECKBOX_CASEMATCH);
				phraseSearchData.phrase = phraseEntered;
				handleforphraseprogressdialog = CreateDialog(hInst1, MAKEINTRESOURCE(IDD_DIALOG3), NULL, (DLGPROC)ProgressBarProc);
				SetWindowTextA(GetDlgItem(handleforphraseprogressdialog, IDC_STATIC1), "Searching for your phrase(s)...");
				std::future<StatReturnStorage> phraseStatsFutureTemp= std::async(phraseStatsGet, messageAnalyser, dataStore, handleforphraseprogressdialog, handleforwindow);
				waitingforphrasedata = true;
				phraseStatsFuture = phraseStatsFutureTemp.share();
			}
			break;
		}
		case ID_DATA_WINDOW1_SCROLL_UP: {
			currentTopDataLine1 -= 1;
			currentBottomDataLine1 -= 1;
			break;
		}
		case ID_DATA_WINDOW1_SCROLL_DOWN: {
			currentBottomDataLine1 += 1;
			currentTopDataLine1 += 1;
			break;
		}
		case ID_DATA_WINDOW1_PAGE_UP: {
			currentTopDataLine1 -= 10;
			currentBottomDataLine1 -= 10;
			if (currentTopDataLine1 < 0) {
				currentTopDataLine1 = 0;
				currentBottomDataLine1 = 10;
			}
			break;
		}
		case ID_DATA_WINDOW1_PAGE_DOWN: {
			currentBottomDataLine1 += 10;
			currentTopDataLine1 += 10;
			break;
		}
		case ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP: {
			currentTopCheckboxWeeksLine1 -= 1;
			currentBottomCheckboxWeeksLine1 -= 1;
			currentTopCheckboxMonthsLine1 -= 1;
			currentBottomCheckboxMonthsLine1 -= 1;
			checkBox1PeriodsScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN: {
			currentTopCheckboxWeeksLine1 += 1;
			currentBottomCheckboxWeeksLine1 += 1;
			currentTopCheckboxMonthsLine1 += 1;
			currentBottomCheckboxMonthsLine1 += 1;
			checkBox1PeriodsScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES1NAMES_SCROLL_UP: {
			currentTopCheckboxNamesLine1 -= 1;
			currentBottomCheckboxNamesLine1 -= 1;
			checkBox1NamesScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES1NAMES_SCROLL_DOWN: {
			currentTopCheckboxNamesLine1 += 1;
			currentBottomCheckboxNamesLine1 += 1;
			checkBox1NamesScroll = true;
			break;
		}
		case ID_CHECKBOX_CASEMATCH: {
			BOOL checked;
			checked = IsDlgButtonChecked(handleforwindow, ID_CHECKBOX_CASEMATCH);
			if (checked) {
				CheckDlgButton(handleforwindow, ID_CHECKBOX_CASEMATCH, BST_UNCHECKED);
				messageAnalyser.caseMatch = false;
			}
			else {
				CheckDlgButton(handleforwindow, ID_CHECKBOX_CASEMATCH, BST_CHECKED);
				messageAnalyser.caseMatch = true;
			}
			break;
		}

		case ID_PERIODSWITCH1: {
			if (periodSelected1 == "M") {
				periodSelected1 = "W";
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_PERIODSWITCH1),
					"Month View"
				);


				for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
					if (weeksSelected1[messageAnalyser.allweeks[i]]) {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_CHECKED);
					}
					else {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_UNCHECKED);
					}
				}


			}
			else {
				periodSelected1 = "M";
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_PERIODSWITCH1),
					"Week View"
				);
				for (int i = 0; i < messageAnalyser.months.size(); i++) {
					if (monthsSelected1[messageAnalyser.months[i]]) {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_CHECKED);
					}
					else {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_UNCHECKED);
					}
				}
			}
			checkBox1PeriodsScroll = true;

			currentTopDataLine1 = 0;
			currentBottomDataLine1 = 10;
			currentTopCheckboxMonthsLine1 = 0;
			currentBottomCheckboxMonthsLine1 = 11;
			currentTopCheckboxWeeksLine1 = 0;
			currentBottomCheckboxWeeksLine1 = 11;
			break;
		}

		case ID_LOBFCHECKBOX1: {
			if (IsDlgButtonChecked(handleforwindow, ID_LOBFCHECKBOX1)) {
				CheckDlgButton(handleforwindow, ID_LOBFCHECKBOX1, BST_UNCHECKED);
				phraseTimesGraph.deselectLOBF();
			}
			else {
				CheckDlgButton(handleforwindow, ID_LOBFCHECKBOX1, BST_CHECKED);
				phraseTimesGraph.selectLOBF();
			}
			break;
		}

		}
		if (checkBox1NamesScroll) {
			for (int i = 0; i < messageAnalyser.people.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i]),
					HWND_BOTTOM,
					375,
					20 + (20 * (i - currentTopCheckboxNamesLine1)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[i]),
					HWND_BOTTOM,
					400,
					20 + (20 * (i - currentTopCheckboxNamesLine1)),
					200,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxNamesLine1) < 0 && currentTopCheckboxNamesLine1 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxNamesLine1) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1NAMES_LABELS[i]), SW_HIDE);
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
		}
		if (checkBox1PeriodsScroll) {

			for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
					""
				);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
			}
			if (periodSelected1 == "W") {
				for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]),
						HWND_BOTTOM,
						620,
						20 + (20 * (i - currentTopCheckboxWeeksLine1)),
						20,
						20,
						SWP_SHOWWINDOW
					);
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
						HWND_BOTTOM,
						670,
						20 + (20 * (i - currentTopCheckboxWeeksLine1)),
						100,
						20,
						SWP_SHOWWINDOW
					);
					if ((i - currentTopCheckboxWeeksLine1) < 0 && currentTopCheckboxWeeksLine1 != 0) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]), SW_HIDE);
					}
					if ((i - currentTopCheckboxWeeksLine1) > 8) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]), SW_HIDE);
					}
					SetWindowTextA(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
						messageAnalyser.allweeks[i].c_str()
					);
				}

				if (currentTopCheckboxWeeksLine1 == 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_UP), SW_SHOW);
				}
				if (currentTopCheckboxWeeksLine1 + 9 >= (messageAnalyser.allweeks.size())) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS_SCROLL_DOWN), SW_SHOW);
				}
			}
			else {
				for (int i = 0; i < messageAnalyser.months.size(); i++) {
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]),
						HWND_BOTTOM,
						620,
						20 + (20 * (i - currentTopCheckboxMonthsLine1)),
						20,
						20,
						SWP_SHOWWINDOW
					);
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
						HWND_BOTTOM,
						670,
						20 + (20 * (i - currentTopCheckboxMonthsLine1)),
						100,
						20,
						SWP_SHOWWINDOW
					);
					if ((i - currentTopCheckboxMonthsLine1) < 0 && currentTopCheckboxMonthsLine1 != 0) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]), SW_HIDE);
					}
					if ((i - currentTopCheckboxMonthsLine1) > 8) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]), SW_HIDE);
					}
					SetWindowTextA(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX1PERIODS_LABELS[i]),
						messageAnalyser.months[i].c_str()
					);
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
		}

		if (!dataStore.firstTimeDynamics) {
			BOOL checked;
			//CheckBox 1
			std::map<std::string, bool> peopleSelectedBefore = peopleSelected1;
			peopleSelected1 = {};

			for (int i = 0; i < messageAnalyser.people.size(); i++) {
				if (LOWORD(wParam) == ID_DATA_CHECKBOXES1NAMES[i]) {
					currentTopDataLine1 = 0;
					currentBottomDataLine1 = 10;
					checked = IsDlgButtonChecked(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i]);
					if (checked) {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i], BST_UNCHECKED);
						peopleSelected1[messageAnalyser.people[i]] = false;
					}
					else {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1NAMES[i], BST_CHECKED);
						peopleSelected1[messageAnalyser.people[i]] = true;
					}
				}
				else {
					peopleSelected1[messageAnalyser.people[i]] = peopleSelectedBefore[messageAnalyser.people[i]];

				}
			}
			std::map<std::string, bool>  weeksSelectedBefore = weeksSelected1;
			std::map<std::string, bool>  monthsSelectedBefore = monthsSelected1;
			for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
				if (LOWORD(wParam) == ID_DATA_CHECKBOXES1PERIODS[i]) {
					checked = IsDlgButtonChecked(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i]);
					currentTopDataLine1 = 0;
					currentBottomDataLine1 = 10;
					if (checked) {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_UNCHECKED);
						if (periodSelected1 == "M") {
							if (i < messageAnalyser.months.size()) {
								monthsSelected1[messageAnalyser.months[i]] = false;
							}
						}
						else {
							weeksSelected1[messageAnalyser.allweeks[i]] = false;
						}
					}
					else {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES1PERIODS[i], BST_CHECKED);
						if (periodSelected1 == "M") {
							if (i < messageAnalyser.months.size()) {
								monthsSelected1[messageAnalyser.months[i]] = true;
							}
						}
						else {
							weeksSelected1[messageAnalyser.allweeks[i]] = true;
						}

					}
				}
				else {
					if (periodSelected1 == "M") {
						if (i < messageAnalyser.months.size()) {
							monthsSelected1[messageAnalyser.months[i]] = monthsSelectedBefore[messageAnalyser.months[i]];
						}
					}
					else {
						weeksSelected1[messageAnalyser.allweeks[i]] = weeksSelectedBefore[messageAnalyser.allweeks[i]];
					}

				}
			}
			int numPeopleDisplayed = 0;
			int numPeriodsDisplayed = 0;
			idDisplayedText1 = "";
			std::vector<std::string> lines = {};
			std::vector <std::string> peopleSelectedArray = mapkeysbool(peopleSelected1);
			std::vector <std::string> monthsSelectedArray = mapkeysbool(monthsSelected1);
			std::vector <std::string> weeksSelectedArray = mapkeysbool(weeksSelected1);
			peopleSelectedArray = peopleArraySort(peopleSelectedArray);
			monthsSelectedArray = dateArraySort(monthsSelectedArray);
			weeksSelectedArray = dayArraySort(weeksSelectedArray);
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

		}
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW1), SW_SHOW);
		if (not dataStore.firstTimeDynamics) {
			std::vector<std::string> peopleSelectedArray = mapkeysbool(peopleSelected1);
			for (std::string person : peopleSelectedArray) {
				if (peopleSelected1[person]) {
					phraseTimesGraph.selectLine(person);
				}
				else {
					phraseTimesGraph.deselectLine(person);
				}
			}
			if (peopleSelected1["Total Messages"]) {
				phraseTimesGraph.selectLine("Total");
			}
			else {
				phraseTimesGraph.deselectLine("Total");
			}
			if (not dataStore.firstTimeDynamics) {
				phraseTimesGraph.refresh();
			}
		}
		break;
	}
	case WM_PAINT: {
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = SUBWINDOWLENGTH;
		rect.bottom = GRAPH1YPOS;
		if (not dataStore.firstTimeDynamics) {
			phraseTimesGraph.refresh();
		}
		ValidateRgn(handleforwindow, NULL);
		InvalidateRect(handleforwindow, &rect, true);
		break;

	}
	}
	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}













///////////////////////////////////////////////////////////////SUBWINDOW 2///////////////////////////////////////////////////////










LRESULT CALLBACK windowprocessforwindow22(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	BOOL ShowText = false;


	switch (msg)
	
	
	{

	case WM_DRAWITEM:
	{
		drawButton(lParam);
		return TRUE;
	}
	
	case WM_CTLCOLORSTATIC: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CTLCOLORBTN: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CREATE:
	{
		
		//Graph
		handleformessagesSentGraph = CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			GRAPH2XPOS, GRAPH2YPOS, GRAPH2XSIZE, GRAPH2XSIZE,
			handleforwindow,
			NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		if (not windowsCreatedBefore) {
			applyStyles(handleformessagesSentGraph);
			int maxHeight = 0;
			for (std::string month : messageAnalyser.months) {
				if (month != "All Time") {
					if (messageAnalyser.totalmessagesmonthly[month]["Total Messages"] > maxHeight) {
						maxHeight = messageAnalyser.totalmessagesmonthly[month]["Total Messages"];
					}
				}
			}
			std::vector<double> heights{};
			heights.push_back(maxHeight);
			std::vector<double> yAxisValues = yAxisValuesFind(heights);
			messagesSentGraph = CreateBarGraph(handleformessagesSentGraph, DATETYPE,
				GRAPH2XSIZE - 100, GRAPH2YSIZE - 100,
				yAxisValues,
				"Month", "Num Messages", "Total Messages",
				GRAPH2XPOS, GRAPH2YPOS,
				50, 25);
			messagesSentGraph.addSubBarNoVals("Total");
			std::map<std::string, double> barData = {};
			std::map<std::string, double> barDataTotal = barData;
			for (std::string month : messageAnalyser.months) {
				if (month != "All Time") {
					barData = {};
					barData["Total"] = messageAnalyser.totalmessagesmonthly[month]["Total Messages"];
					for (std::string person : messageAnalyser.people) {
						if (person != "Total Messages") {
							barData[person] = messageAnalyser.totalmessagesmonthly[month][person];
						}
					}
					messagesSentGraph.addBar(barData, month, true);
				}
			}
		}
		else {
			messagesSentGraph.window = handleformessagesSentGraph;
		}

		








		//Data
		
		CreateWindow(L"BUTTON",
			L"Week View",
			BUTTON_STYLE,
			330, 0, 90, 20,
			handleforwindow, (HMENU)ID_PERIODSWITCH2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_PERIODSWITCH2));

		//CheckBox 1
		CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			500, 20, 300, 180,
			handleforwindow,
			(HMENU)ID_DATA_WINDOW2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW2));
		SetWindowTextA(
			GetDlgItem(handleforwindow, ID_DATA_WINDOW2),
			"No Options Selected"
		);

		CreateWindow(L"BUTTON",
			L" /\\        UP        /\\ ",
			BUTTON_STYLE,
			500, 0, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW2_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_UP));


		CreateWindow(L"BUTTON",
			L" \\/     DOWN     \\/ ",
			BUTTON_STYLE,
			500, 200, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW2_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_DOWN));

		CreateWindow(L"BUTTON",
			L" /\\    PAGEUP    /\\ ",
			BUTTON_STYLE,
			620, 0, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW2_PAGE_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_UP));

		CreateWindow(L"BUTTON",
			L" \\/  PAGEDOWN  \\/ ",
			BUTTON_STYLE,
			620, 200, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW2_PAGE_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_DOWN));



		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_DOWN), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_DOWN), SW_HIDE);
		
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			CreateWindow(L"BUTTON", NULL,
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				25, (20 + (20 * i)), 20, 20,
				handleforwindow, (HMENU)ID_DATA_CHECKBOXES2NAMES[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i], BST_CHECKED);
			CreateWindow(L"Static", NULL,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
				50, (20 + (20 * i)), 200, 20,
				handleforwindow,
				(HMENU)ID_DATA_CHECKBOX2NAMES_LABELS[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2NAMES_LABELS[i]));

			SetWindowTextA(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2NAMES_LABELS[i]),
				(messageAnalyser.people[i]).c_str()
			);
			peopleSelected2[messageAnalyser.people[i]] = true;
		}
		CreateWindow(L"BUTTON",
			L" /\\    UP    /\\ ",
			BUTTON_STYLE,
			100, 0, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES2NAMES_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_UP));

		CreateWindow(L"BUTTON",
			L" \\/ DOWN \\/ ",
			BUTTON_STYLE,
			100, 200, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES2NAMES_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_DOWN)); 
		
		if (messageAnalyser.people.size() > 9) {
		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_UP), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_DOWN), SW_HIDE);
		}
		
		int numWeeks = (int)messageAnalyser.allweeks.size();
		for (int i = 0; i < numWeeks; i++) {
			CreateWindow(TEXT("button"), NULL,
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				270, (20 + ((numWeeks - 1) * 20) - (20 * i)), 20, 20,
				handleforwindow, (HMENU)ID_DATA_CHECKBOXES2PERIODS[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_CHECKED);
			CreateWindow(L"Static", NULL,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
				320, (20 + ((numWeeks - 1) * 20) - (20 * i)), 100, 20,
				handleforwindow,
				(HMENU)ID_DATA_CHECKBOX2PERIODS_LABELS[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]));
			if (i < messageAnalyser.months.size()) {
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]),
					(messageAnalyser.months[i]).c_str()
				);
				monthsSelected2[messageAnalyser.months[i]] = true;
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]), SW_HIDE);
			}
			weeksSelected2[messageAnalyser.allweeks[i]] = true;
		}

		CreateWindow(L"BUTTON",
			L" /\\    UP    /\\ ",
			BUTTON_STYLE,
			245, 0, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP)); 
		
		CreateWindow(L"BUTTON",
			L" \\/ DOWN \\/ ",
			BUTTON_STYLE,
			245, 200, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN));

		if (messageAnalyser.months.size() > 9) {

		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN), SW_HIDE);
		}
		int numPeopleDisplayed = 0;
		int numMonthsDisplayed = 0;
		idDisplayedText2 = "";
		std::vector<std::string> lines = {};
		std::vector <std::string> peopleSelectedArray = mapkeysbool(peopleSelected2);
		std::vector <std::string> monthsSelectedArray = mapkeysbool(monthsSelected2);
		peopleSelectedArray = peopleArraySort(peopleSelectedArray);
		monthsSelectedArray = dateArraySort(monthsSelectedArray);
		for (int i = 0; i < monthsSelectedArray.size(); i++) {
			if (monthsSelected2[monthsSelectedArray[i]]) {
				lines.push_back(monthsSelectedArray[i] + ": \n");
				numMonthsDisplayed++;
				for (int j = 0; j < peopleSelectedArray.size(); j++) {
					if (peopleSelected2[peopleSelectedArray[j]]) {
						lines.push_back("        " + peopleSelectedArray[j] + ": " + itos(messageAnalyser.totalmessagesmonthly[monthsSelectedArray[i]][peopleSelectedArray[j]]) + "\n");
						numPeopleDisplayed++;
					}
				}
			}
		}

		int linesNew = 0;
		if (lines.size() > 11) {
			if (currentTopDataLine2 != 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_UP), SW_SHOW);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_UP), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_UP), SW_HIDE);
			}
			for (int i = (currentBottomDataLine2 - 1); i >= currentTopDataLine2; i -= 1) {
				idDisplayedText2 = lines[i] + idDisplayedText2;
			}
			if (currentBottomDataLine2 < lines.size()) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_DOWN), SW_SHOW);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_DOWN), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_DOWN), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_DOWN), SW_HIDE);
			}
		}
		else {
			if (numMonthsDisplayed == 0 || numPeopleDisplayed == 0) {
				idDisplayedText2 = "Select more options";
			}
			else {
				for (int i = 0; i < lines.size(); i++) {
					idDisplayedText2 = idDisplayedText2 + lines[i];
				}
			}
		}
		SetWindowTextA(
			GetDlgItem(handleforwindow, ID_DATA_WINDOW2),
			(idDisplayedText2).c_str()
		);

		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			SetWindowPos(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i]),
				HWND_BOTTOM,
				25,
				20 + (20 * (i - currentTopCheckboxNamesLine2)),
				20,
				20,
				SWP_SHOWWINDOW
			);
			SetWindowPos(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2NAMES_LABELS[i]),
				HWND_BOTTOM,
				50,
				20 + (20 * (i - currentTopCheckboxNamesLine2)),
				200,
				20,
				SWP_SHOWWINDOW
			);
			if ((i - currentTopCheckboxNamesLine2) < 0 && currentTopCheckboxNamesLine2 != 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2NAMES_LABELS[i]), SW_HIDE);
			}
			if ((i - currentTopCheckboxNamesLine2) > 8) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2NAMES_LABELS[i]), SW_HIDE);
			}
		}

		if (currentTopCheckboxNamesLine2 == 0) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_UP), SW_HIDE);
		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_UP), SW_SHOW);
		}
		
		if (currentTopCheckboxNamesLine2 + 9 >= (messageAnalyser.people.size())) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_DOWN), SW_HIDE);

		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_DOWN), SW_SHOW);
		}



		if (periodSelected2 == "M") {
			for (int i = 0; i < messageAnalyser.months.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]),
					HWND_BOTTOM,
					270,
					20 + (20 * (i - currentTopCheckboxMonthsLine2)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]),
					HWND_BOTTOM,
					320,
					20 + (20 * (i - currentTopCheckboxMonthsLine2)),
					100,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxMonthsLine2) < 0 && currentTopCheckboxMonthsLine2 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxMonthsLine2) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]), SW_HIDE);
				}
			}

			if (currentTopCheckboxMonthsLine2 == 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP), SW_SHOW);
			}
			if (currentTopCheckboxMonthsLine2 + 9 >= (messageAnalyser.months.size())) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN), SW_SHOW);
			}
		}
		else {
			for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]),
					HWND_BOTTOM,
					270,
					20 + (20 * (i - currentTopCheckboxWeeksLine2)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]),
					HWND_BOTTOM,
					320,
					20 + (20 * (i - currentTopCheckboxWeeksLine2)),
					100,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxWeeksLine2) < 0 && currentTopCheckboxWeeksLine2 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxWeeksLine2) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]), SW_HIDE);
				}
			}

			if (currentTopCheckboxWeeksLine2 == 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP), SW_SHOW);
			}
			if (currentTopCheckboxWeeksLine2 + 9 >= (messageAnalyser.allweeks.size())) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN), SW_SHOW);
			}
		}


		break;
	}

	case WM_PAINT: {
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = SUBWINDOWLENGTH;
		rect.bottom = GRAPH2YPOS;
		messagesSentGraph.refresh();
		ValidateRgn(handleforwindow, NULL);
		InvalidateRect(handleforwindow, &rect, true);
		break;
	} 

	case WM_DESTROY: {
		return 0;
	}

	case WM_COMMAND:
	{
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			if (peopleSelected2[messageAnalyser.people[i]]) {
				CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i], BST_CHECKED);
			}
			else {
				CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i], BST_UNCHECKED);
			}
		}
		if (periodSelected2 == "M") {
			for (int i = 0; i < messageAnalyser.months.size(); i++) {
				if (monthsSelected2[messageAnalyser.months[i]]) {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_CHECKED);
				}
				else {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_UNCHECKED);
				}
			}
		}
		else {
			for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
				if (weeksSelected2[messageAnalyser.allweeks[i]]) {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_CHECKED);
				}
				else {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_UNCHECKED);
				}
			}
		}
		bool checkBox2NamesScroll = false;
		bool checkBox2MonthsScroll = false;
		switch (LOWORD(wParam)) {
		case ID_PERIODSWITCH2: {
			if (periodSelected2 == "M") {
				periodSelected2 = "W";
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_PERIODSWITCH2),
					"Month View"
				);

				for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
					if (weeksSelected2[messageAnalyser.allweeks[i]]) {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_CHECKED);
					}
					else {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_UNCHECKED);
					}
				}


			}
			else {
				periodSelected2 = "M";
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_PERIODSWITCH2),
					"Week View"
				);
				for (int i = 0; i < messageAnalyser.months.size(); i++) {
					if (monthsSelected2[messageAnalyser.months[i]]) {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_CHECKED);
					}
					else {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_UNCHECKED);
					}
				}
			}
			checkBox2MonthsScroll = true;
			currentTopDataLine2 = 0;
			currentBottomDataLine2 = 10;
			currentTopCheckboxMonthsLine2 = 0;
			currentBottomCheckboxMonthsLine2 = 11;
			currentTopCheckboxWeeksLine2 = 0;
			currentBottomCheckboxWeeksLine2 = 11;
			break;
		}
		case ID_DATA_WINDOW2_SCROLL_UP: {
			currentTopDataLine2 -= 1;
			currentBottomDataLine2 -= 1;
			break;
		}
		case ID_DATA_WINDOW2_SCROLL_DOWN: {
			currentBottomDataLine2 += 1;
			currentTopDataLine2 += 1;
			break;
		}
		case ID_DATA_WINDOW2_PAGE_UP: {
			currentTopDataLine2 -= 10;
			currentBottomDataLine2 -= 10;
			if (currentTopDataLine2 < 0) {
				currentTopDataLine2 = 0;
				currentBottomDataLine2 = 10;
			}
			break;
		}
		case ID_DATA_WINDOW2_PAGE_DOWN: {
			currentBottomDataLine2 += 10;
			currentTopDataLine2 += 10;
			break;
		}
		case ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP: {
			currentTopCheckboxMonthsLine2 -= 1;
			currentTopCheckboxWeeksLine2 -= 1;
			currentBottomCheckboxMonthsLine2 -= 1;
			currentBottomCheckboxWeeksLine2 -= 1;
			checkBox2MonthsScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN: {
			currentTopCheckboxMonthsLine2 += 1;
			currentTopCheckboxWeeksLine2 += 1;
			currentBottomCheckboxMonthsLine2 += 1;
			currentBottomCheckboxWeeksLine2 += 1;
			checkBox2MonthsScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES2NAMES_SCROLL_UP: {
			currentTopCheckboxNamesLine2 -= 1;
			currentBottomCheckboxNamesLine2 -= 1;
			checkBox2NamesScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES2NAMES_SCROLL_DOWN: {
			currentTopCheckboxNamesLine2 += 1;
			currentBottomCheckboxNamesLine2 += 1;
			checkBox2NamesScroll = true;
			break;
		}
		}
		if (checkBox2NamesScroll) {
			for (int i = 0; i < messageAnalyser.people.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i]),
					HWND_BOTTOM,
					25,
					20 + (20 * (i - currentTopCheckboxNamesLine2)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2NAMES_LABELS[i]),
					HWND_BOTTOM,
					50,
					20 + (20 * (i - currentTopCheckboxNamesLine2)),
					200,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxNamesLine2) < 0 && currentTopCheckboxNamesLine2 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2NAMES_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxNamesLine2) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2NAMES_LABELS[i]), SW_HIDE);
				}
			}

			if (currentTopCheckboxNamesLine2 == 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_UP), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_UP), SW_SHOW);
			}
			if (currentTopCheckboxNamesLine2 + 9 >= (messageAnalyser.people.size())) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_DOWN), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2NAMES_SCROLL_DOWN), SW_SHOW);
			}
		}

		if (checkBox2MonthsScroll) {

			for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]),
					""
				);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]), SW_HIDE);
			}
			if (periodSelected2 == "M") {
				for (int i = 0; i < messageAnalyser.months.size(); i++) {
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]),
						HWND_BOTTOM,
						270,
						20 + (20 * (i - currentTopCheckboxMonthsLine2)),
						20,
						20,
						SWP_SHOWWINDOW
					);
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]),
						HWND_BOTTOM,
						320,
						20 + (20 * (i - currentTopCheckboxMonthsLine2)),
						100,
						20,
						SWP_SHOWWINDOW
					);
					if ((i - currentTopCheckboxMonthsLine2) < 0 && currentTopCheckboxMonthsLine2 != 0) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]), SW_HIDE);
					}
					if ((i - currentTopCheckboxMonthsLine2) > 8) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]), SW_HIDE);
					}
					SetWindowTextA(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]),
						messageAnalyser.months[i].c_str()
					);

				}

				if (currentTopCheckboxMonthsLine2 == 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP), SW_SHOW);
				}
				if (currentTopCheckboxMonthsLine2 + 9 >= messageAnalyser.months.size()) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN), SW_SHOW);
				}
			}
			else {
				for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]),
						HWND_BOTTOM,
						270,
						20 + (20 * (i - currentTopCheckboxWeeksLine2)),
						20,
						20,
						SWP_SHOWWINDOW
					);
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]),
						HWND_BOTTOM,
						320,
						20 + (20 * (i - currentTopCheckboxWeeksLine2)),
						100,
						20,
						SWP_SHOWWINDOW
					);
					if ((i - currentTopCheckboxWeeksLine2) < 0 && currentTopCheckboxWeeksLine2 != 0) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]), SW_HIDE);
					}
					if ((i - currentTopCheckboxWeeksLine2) > 8) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]), SW_HIDE);
					}
					SetWindowTextA(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX2PERIODS_LABELS[i]),
						messageAnalyser.allweeks[i].c_str()
					);

				}

				if (currentTopCheckboxWeeksLine2 == 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_UP), SW_SHOW);
				}
				if (currentTopCheckboxWeeksLine2 + 9 >= messageAnalyser.allweeks.size()) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES2PERIODS_SCROLL_DOWN), SW_SHOW);
				}
			}
		}
		BOOL checked;
		//CheckBox 1
		std::map<std::string, bool> peopleSelectedBefore = peopleSelected2;
		peopleSelected2 = {};
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			if (LOWORD(wParam) == ID_DATA_CHECKBOXES2NAMES[i]) {
				currentTopDataLine2 = 0;
				currentBottomDataLine2 = 10;
				checked = IsDlgButtonChecked(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i]);
				if (checked) {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i], BST_UNCHECKED);
					peopleSelected2[messageAnalyser.people[i]] = false;
				}
				else {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2NAMES[i], BST_CHECKED);
					peopleSelected2[messageAnalyser.people[i]] = true;
				}
			}
			else {
				peopleSelected2[messageAnalyser.people[i]] = peopleSelectedBefore[messageAnalyser.people[i]];

			}
		}
		std::map<std::string, bool>  monthsSelectedBefore = monthsSelected2;
		std::map<std::string, bool>  weeksSelectedBefore = weeksSelected2;
		for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
			if (LOWORD(wParam) == ID_DATA_CHECKBOXES2PERIODS[i]) {
				checked = IsDlgButtonChecked(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i]);
				currentTopDataLine2 = 0;
				currentBottomDataLine2 = 10;
				if (checked) {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_UNCHECKED);
					if (periodSelected2 == "M") {
						if (i < messageAnalyser.months.size()) {
							monthsSelected2[messageAnalyser.months[i]] = false;
						}
					}
					else {
						weeksSelected2[messageAnalyser.allweeks[i]] = false;
					}
				}
				else {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES2PERIODS[i], BST_CHECKED);
					if (periodSelected2 == "M") {
						if (i < messageAnalyser.months.size()) {
							monthsSelected2[messageAnalyser.months[i]] = true;
						}
					}
					else {
						weeksSelected2[messageAnalyser.allweeks[i]] = true;
					}

				}
			}
			else {
				if (periodSelected2 == "M") {
					if (i < messageAnalyser.months.size()) {
						monthsSelected2[messageAnalyser.months[i]] = monthsSelectedBefore[messageAnalyser.months[i]];
					}
				}
				else {
					weeksSelected2[messageAnalyser.allweeks[i]] = weeksSelectedBefore[messageAnalyser.allweeks[i]];
				}

			}
		}
		int numPeopleDisplayed = 0;
		int numPeriodsDisplayed = 0;
		idDisplayedText2 = "";
		std::vector<std::string> lines = {};
		std::vector <std::string> peopleSelectedArray = mapkeysbool(peopleSelected2);
		std::vector <std::string> monthsSelectedArray = mapkeysbool(monthsSelected2);
		std::vector <std::string> weeksSelectedArray = mapkeysbool(weeksSelected2);
		peopleSelectedArray = peopleArraySort(peopleSelectedArray);
		monthsSelectedArray = dateArraySort(monthsSelectedArray);
		weeksSelectedArray = dayArraySort(weeksSelectedArray);
		if (periodSelected2 == "M") {
			for (int i = 0; i < monthsSelectedArray.size(); i++) {
				if (monthsSelected2[monthsSelectedArray[i]]) {
					lines.push_back(monthsSelectedArray[i] + ": \n");
					numPeriodsDisplayed++;
					for (int j = 0; j < peopleSelectedArray.size(); j++) {
						if (peopleSelected2[peopleSelectedArray[j]]) {
							lines.push_back("        " + peopleSelectedArray[j] + ": " + itos(messageAnalyser.totalmessagesmonthly[monthsSelectedArray[i]][peopleSelectedArray[j]]) + "\n");
							numPeopleDisplayed++;
						}
					}
				}
			}

		}
		else {
			for (int i = 0; i < weeksSelectedArray.size(); i++) {
				if (weeksSelected2[weeksSelectedArray[i]]) {
					lines.push_back(weeksSelectedArray[i] + ": \n");
					numPeriodsDisplayed++;
					for (int j = 0; j < peopleSelectedArray.size(); j++) {
						if (peopleSelected2[peopleSelectedArray[j]]) {
							lines.push_back("        " + peopleSelectedArray[j] + ": " + itos(messageAnalyser.totalmessagesweekly[weeksSelectedArray[i]][peopleSelectedArray[j]]) + "\n");
							numPeopleDisplayed++;
						}
					}
				}
			}
		}

		if (currentBottomDataLine2 > lines.size()) {
			currentTopDataLine2 = lines.size() - 10;
			currentBottomDataLine2 = lines.size();
		}

		int linesNew = 0;
		if (lines.size() > 11) {
			if (currentTopDataLine2 != 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_UP), SW_SHOW);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_UP), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_UP), SW_HIDE);
			}
			for (int i = currentTopDataLine2; i < currentBottomDataLine2; i++) {
				idDisplayedText2 += lines[i];
			}
			if (currentBottomDataLine2 < lines.size()) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_DOWN), SW_SHOW);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_DOWN), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_SCROLL_DOWN), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW2_PAGE_DOWN), SW_HIDE);
			}
		}
		else {
			if (numPeriodsDisplayed == 0 || numPeopleDisplayed == 0) {
				idDisplayedText2 = "Select more options";
			}
			else {
				for (int i = 0; i < lines.size(); i++) {
					idDisplayedText2 = idDisplayedText2 + lines[i];
				}
			}
		}
		SetWindowTextA(
			GetDlgItem(handleforwindow, ID_DATA_WINDOW2),
			(idDisplayedText2).c_str()
		);
		std::vector<std::string> peopleSelectedArrayGraph = mapkeysbool(peopleSelected2);
		std::vector<std::string> monthsSelectedArrayGraph = mapkeysbool(monthsSelected2);
		for (std::string person : peopleSelectedArrayGraph) {
			if (person != "Total Messages") {
				if (peopleSelected2[person]) {
					messagesSentGraph.selectSubBarAll(person);
				}
				else {
					messagesSentGraph.deselectSubBarAll(person);
				}
			}
		}
		if (peopleSelected2["Total Messages"]) {
			messagesSentGraph.selectSubBarAll("Total");
		}
		else {
			messagesSentGraph.deselectSubBarAll("Total");
		}
		
		for (std::string month : monthsSelectedArrayGraph) {
			if (monthsSelected2[month]) {
				messagesSentGraph.selectBar(month);
			}
			else {
				messagesSentGraph.deselectBar(month);
			}
		}
		messagesSentGraph.refresh();

		break;
	}



	}
	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}




//////////////////////////////////////////////////////SUBWINDOW 3//////////////////////////////////////////////////////////////////













LRESULT CALLBACK windowprocessforwindow23(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {


	case WM_DRAWITEM:
	{
		
		drawButton(lParam);
		return TRUE;
	}

	case WM_CTLCOLORSTATIC: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CTLCOLORBTN: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CREATE: {

		//Graph
		handleformessagesOverTimeGraph = CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			GRAPH3XPOS, GRAPH3YPOS, GRAPH3XSIZE, GRAPH3YSIZE,
			handleforwindow,
			NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		if (not windowsCreatedBefore) {
			//Graph Window			
			std::vector <std::vector <double> > coordinates = { { },  { } };
			std::map<std::string, int> averagedays = {};
			std::string startDay = messageAnalyser.alldays[0];
			std::map<std::string, int> messagesDailyPerson = {};
			for (std::string day : messageAnalyser.alldays) {
				messagesDailyPerson[day] = messageAnalyser.totalmessagesdaily[day]["Total Messages"];
			}
			std::vector<std::string> alldays = getDaysBetween(messageAnalyser.alldays[0], messageAnalyser.alldays[messageAnalyser.alldays.size() - 1]);
			for (std::string day : alldays) {
				coordinates[0].push_back(getNumDays(day, startDay));
				coordinates[1].push_back((averagePastDays(5, day, messagesDailyPerson)));
			}
			std::vector<std::string> allmonths = getMonthsBetween(alldays[0], alldays[alldays.size() - 1]);
			std::vector<double> xAxisVals = getMonthsDays(allmonths);
			std::vector<double>	yAxisVals = yAxisValuesFind(coordinates[1]);
			applyStyles(handleformessagesOverTimeGraph);
			messagesOverTimeGraph = CreateLineGraph(handleformessagesOverTimeGraph,
				GRAPH3XSIZE - 100, GRAPH3YSIZE - 100,
				xAxisVals, yAxisVals,
				MONTHTYPE, INTTYPE,
				"Month", "Num Messages",
				("Daily messages across " + itos(messageAnalyser.alldays.size())) + " days",
				GRAPH3XPOS, GRAPH3YPOS,
				allmonths[0]);
			messagesOverTimeGraph.addLine(coordinates, "Total", true);
			for (std::string person : messageAnalyser.people) {
				if (person != "Total Messages") {
					coordinates = { { },  { } };
					messagesDailyPerson = {};
					for (std::string day : messageAnalyser.alldays) {
						messagesDailyPerson[day] = messageAnalyser.totalmessagesdaily[day][person];
					}
					for (std::string day : messageAnalyser.alldays) {
						coordinates[0].push_back(getNumDays(day, startDay));
						coordinates[1].push_back((averagePastDays(5, day, messagesDailyPerson)));
					}
					messagesOverTimeGraph.addLine(coordinates, person, true);
				}
				for (std::string person : messageAnalyser.people) {
					peopleSelected3[person] = true;
				}
			}
		}
		else {
			messagesOverTimeGraph.window = handleformessagesOverTimeGraph;
		}
		CreateWindow(L"BUTTON", NULL,
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
			53, 120, 20, 20,
			handleforwindow, (HMENU)ID_LOBFCHECKBOX3, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CheckDlgButton(handleforwindow, ID_LOBFCHECKBOX3, BST_CHECKED);
		CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			53 + 25, 120, 75, 50,
			handleforwindow,
			(HMENU)ID_LOBFCHECKBOX3TEXT, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_LOBFCHECKBOX3TEXT));
		SetWindowTextA(GetDlgItem(handleforwindow, ID_LOBFCHECKBOX3TEXT), "Line of Best Fit");
		


		
		//Data
		CreateWindow(L"Static", L"",
			WS_VISIBLE | WS_CHILD | WS_TABSTOP,
			150, 80, 100, 100,
			handleforwindow, (HMENU)ID_INVALIDDATE_TEXT, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_INVALIDDATE_TEXT));


		CreateWindow(L"Static", L"Search",
			WS_VISIBLE | WS_CHILD | WS_TABSTOP,
			10, 10, 40, 20,
			handleforwindow, (HMENU)ID_DATE_BOX_LABEL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATE_BOX_LABEL));
		CreateWindow(L"Static", L"from:",
			WS_VISIBLE | WS_CHILD | WS_TABSTOP,
			53, 10, 40, 20,
			handleforwindow, (HMENU)ID_STARTDATE_BOX_LABEL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_STARTDATE_BOX_LABEL));
		CreateWindow(TEXT("EDIT"), L"DD/MM/YYYY",
			WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL,
			90, 10, 120, 23,
			handleforwindow, (HMENU)ID_STARTDATE_BOX, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_STARTDATE_BOX));
		CreateWindow(L"Static", L"to:",
			WS_VISIBLE | WS_CHILD | WS_TABSTOP,
			53, 35, 40, 20,
			handleforwindow, (HMENU)ID_ENDDATE_BOX_LABEL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_ENDDATE_BOX_LABEL));
		CreateWindow(TEXT("EDIT"), L"DD/MM/YYYY",
			WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL,
			90, 35, 120, 23,
			handleforwindow, (HMENU)ID_ENDDATE_BOX, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_ENDDATE_BOX));

		CreateWindow(L"BUTTON",
			L"Search",
			BUTTON_STYLE,
			50, 80, 90, 20,
			handleforwindow, (HMENU)ID_SEARCH_BUTTON_DATES, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_SEARCH_BUTTON_DATES));
		CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			600, 20, 300, 180,
			handleforwindow,
			(HMENU)ID_DATA_WINDOW3, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW3));
		SetWindowTextA(
			GetDlgItem(handleforwindow, ID_DATA_WINDOW3),
			"Enter a date range"
		);

		CreateWindow(L"BUTTON",
			L" /\\        UP        /\\ ",
			BUTTON_STYLE,
			600, 0, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW3_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP));
		CreateWindow(L"BUTTON",
			L" \\/     DOWN     \\/ ",
			BUTTON_STYLE,
			600, 200, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW3_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN));
		CreateWindow(L"BUTTON",
			L" /\\    PAGEUP    /\\ ",
			BUTTON_STYLE,
			720, 0, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW3_PAGE_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP));
		CreateWindow(L"BUTTON",
			L" \\/  PAGEDOWN  \\/ ",
			BUTTON_STYLE,
			720, 200, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW3_PAGE_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN));

		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_HIDE);
		
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			CreateWindow(L"BUTTON", NULL,
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				350, (10 + (20 * i)), 20, 20,
				handleforwindow, (HMENU)ID_DATA_CHECKBOXES3NAMES[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i], BST_CHECKED);
			CreateWindow(L"Static", NULL,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
				375, (10 + (20 * i)), 200, 20,
				handleforwindow,
				(HMENU)ID_DATA_CHECKBOX3NAMES_LABELS[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]));

			SetWindowTextA(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]),
				(messageAnalyser.people[i]).c_str()
			);
			peopleSelected3[messageAnalyser.people[i]] = true;
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]), SW_SHOW);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]), SW_SHOW);
		}
		CreateWindow(L"BUTTON",
			L" /\\    UP    /\\ ",
			BUTTON_STYLE,
			375, 0, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES3NAMES_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_UP));

		CreateWindow(L"BUTTON",
			L" \\/ DOWN \\/ ",
			BUTTON_STYLE,
			375, 200, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN));
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN), SW_HIDE);
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			SetWindowPos(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]),
				HWND_BOTTOM,
				350,
				20 + (20 * (i - currentTopCheckboxNamesLine3)),
				20,
				20,
				SWP_SHOWWINDOW
			);
			SetWindowPos(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]),
				HWND_BOTTOM,
				375,
				20 + (20 * (i - currentTopCheckboxNamesLine3)),
				200,
				20,
				SWP_SHOWWINDOW
			);
			if ((i - currentTopCheckboxNamesLine3) < 0 && currentTopCheckboxNamesLine3 != 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]), SW_HIDE);
			}
			if ((i - currentTopCheckboxNamesLine3) > 8) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]), SW_HIDE);
			}
		}

		if (currentTopCheckboxNamesLine3 == 0) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_UP), SW_HIDE);
		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_UP), SW_SHOW);
		}

		if (currentTopCheckboxNamesLine3 + 9 >= (messageAnalyser.people.size())) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN), SW_HIDE);

		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN), SW_SHOW);
		}
		break;
	}

	case SDM_DAYDATALOAD: {
		SetWindowTextA(GetDlgItem(handleforwindow, ID_STARTDATE_BOX), startDate.c_str());
		SetWindowTextA(GetDlgItem(handleforwindow, ID_ENDDATE_BOX), endDate.c_str());
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			if (peopleSelected3[messageAnalyser.people[i]]) {
				CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i], BST_CHECKED);
			}
			else {
				CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i], BST_UNCHECKED);
			}
		}
		int numPeopleDisplayed = 0;
		int numDaysDisplayed = 0;
		idDisplayedText3 = "";
		std::vector<std::string> lines = {};
		std::vector <std::string> peopleSelectedArray = mapkeysbool(peopleSelected3);
		std::vector <std::string> daysSelectedArray = dayArraySort(getDaysBetween(startDate, endDate));
		peopleSelectedArray = peopleArraySort(peopleSelectedArray);
		lines.push_back("All Time: \n");
		for (int i = 0; i < peopleSelectedArray.size(); i++) {
			lines.push_back("        " + peopleSelectedArray[i] + ": " + itos(personTotalFind(messageAnalyser, peopleSelectedArray[i], daysSelectedArray)) + "\n");
		}
		for (int i = 0; i < daysSelectedArray.size(); i++) {
			if (messageAnalyser.totalmessagesdaily[daysSelectedArray[i]]["Total Messages"] != 0) {
				lines.push_back(daysSelectedArray[i] + ": \n");
				numDaysDisplayed++;
				for (int j = 0; j < peopleSelectedArray.size(); j++) {
					lines.push_back("        " + peopleSelectedArray[j] + ": " + itos(messageAnalyser.totalmessagesdaily[daysSelectedArray[i]][peopleSelectedArray[j]]) + "\n");
					numPeopleDisplayed++;
				}
			}
		}
		if (currentBottomDataLine3 > lines.size()) {
			currentTopDataLine3 = lines.size() - 10;
			currentBottomDataLine3 = lines.size();
		}

		int linesNew = 0;

		if (lines.size() > 11) {
			if (currentTopDataLine3 != 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_SHOW);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_HIDE);
			}
			for (int i = (currentBottomDataLine3 - 1); i >= currentTopDataLine3; i -= 1) {

				idDisplayedText3 = lines[i] + idDisplayedText3;
			}
			if (currentBottomDataLine3 < lines.size()) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_SHOW);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_HIDE);
			}
		}
		else {
			for (int i = 0; i < lines.size(); i++) {
				idDisplayedText3 = idDisplayedText3 + lines[i];
			}
		}
		if (numDaysDisplayed == 0) {
			idDisplayedText3 = notEnoughDataText;
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_HIDE);

		}
		else {
			if (numPeopleDisplayed == 0) {
				idDisplayedText3 = "Select more options";
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_HIDE);

			}
		}

		SetWindowTextA(
			GetDlgItem(handleforwindow, ID_DATA_WINDOW3),
			(idDisplayedText3).c_str()
		);

		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			SetWindowPos(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]),
				HWND_BOTTOM,
				350,
				20 + (20 * (i - currentTopCheckboxNamesLine3)),
				20,
				20,
				SWP_SHOWWINDOW
			);
			SetWindowPos(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]),
				HWND_BOTTOM,
				375,
				20 + (20 * (i - currentTopCheckboxNamesLine3)),
				200,
				20,
				SWP_SHOWWINDOW
			);
			if ((i - currentTopCheckboxNamesLine3) < 0 && currentTopCheckboxNamesLine3 != 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]), SW_HIDE);
			}
			if ((i - currentTopCheckboxNamesLine3) > 8) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]), SW_HIDE);
			}
		}

		if (currentTopCheckboxNamesLine3 == 0) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_UP), SW_HIDE);
		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_UP), SW_SHOW);
		}

		if (currentTopCheckboxNamesLine3 + 9 >= (messageAnalyser.people.size())) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN), SW_HIDE);

		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN), SW_SHOW);
		}
		return 0;
	}

	case WM_COMMAND: {
		bool dataUpdateRequired = false;
		bool checkBox3NamesScroll = false;
		switch LOWORD(wParam) {
		case ID_SEARCH_BUTTON_DATES: {
			currentTopDataLine3 = 0;
			currentBottomDataLine3 = 10;
			currentTopCheckboxNamesLine3 = 0;
			currentBottomCheckboxNamesLine3 = 11;

			std::string textEnteredStart = "";
			std::string textEnteredEnd = "";
			LPWSTR textEnteredRawStart = new TCHAR[1000];
			GetWindowText(GetDlgItem(handleforwindow, ID_STARTDATE_BOX), textEnteredRawStart, 1000);
			textEnteredStart = CW2A(textEnteredRawStart);
			LPWSTR textEnteredRawEnd = new TCHAR[1000];
			GetWindowText(GetDlgItem(handleforwindow, ID_ENDDATE_BOX), textEnteredRawEnd, 1000);
			textEnteredEnd = CW2A(textEnteredRawEnd);
			dateValidationData startDateValidData = dateFormatCorrect(textEnteredStart);
			dateValidationData endDateValidData = dateFormatCorrect(textEnteredEnd);
			if (textEnteredStart == startDate && textEnteredEnd == endDate) {
				if (startDateValidData.dayCorrect && endDateValidData.dayCorrect) {
					SetWindowText(GetDlgItem(handleforwindow, ID_INVALIDDATE_TEXT), L"");
				}
				break;
			}
			if (startDateValidData.dayCorrect && endDateValidData.dayCorrect) {
			}
			else {
				SetWindowText(GetDlgItem(handleforwindow, ID_INVALIDDATE_TEXT), L"Incorrect Date. Please Check and try again");
				notEnoughDataText = "Enter a correct date";
				break;
			}

			if (biggerDate(textEnteredStart, textEnteredEnd) == textEnteredEnd) {

			}
			else {
				SetWindowText(GetDlgItem(handleforwindow, ID_INVALIDDATE_TEXT), L"Wrong Order. Please Check and try again");
				notEnoughDataText = "Enter the dates in the correct order";
				break;
			}
			notEnoughDataText = "No messages found, enter a wider date range";
			startDate = startDateValidData.newDate;
			endDate = endDateValidData.newDate;
			SetWindowText(GetDlgItem(handleforwindow, ID_INVALIDDATE_TEXT), L"");

			for (int i = 0; i < messageAnalyser.people.size(); i++) {
				CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i], BST_CHECKED);
				peopleSelected3[messageAnalyser.people[i]] = true;
			}
			int numPeopleDisplayed = 0;
			int numDaysDisplayed = 0;
			idDisplayedText3 = "";
			std::vector<std::string> lines = {};
			std::vector <std::string> peopleSelectedArray = mapkeysbool(peopleSelected3);
			std::vector <std::string> daysSelectedArray = dayArraySort(getDaysBetween(startDate, endDate));
			peopleSelectedArray = peopleArraySort(peopleSelectedArray);
			lines.push_back("All Time: \n");
			for (int i = 0; i < peopleSelectedArray.size(); i++) {
				lines.push_back("        " + peopleSelectedArray[i] + ": " + itos(personTotalFind(messageAnalyser, peopleSelectedArray[i], daysSelectedArray)) + "\n");
			}
			for (int i = 0; i < daysSelectedArray.size(); i++) {
				if (messageAnalyser.totalmessagesdaily[daysSelectedArray[i]]["Total Messages"] != 0) {
					lines.push_back(daysSelectedArray[i] + ": \n");
					numDaysDisplayed++;
					for (int j = 0; j < peopleSelectedArray.size(); j++) {
						lines.push_back("        " + peopleSelectedArray[j] + ": " + itos(messageAnalyser.totalmessagesdaily[daysSelectedArray[i]][peopleSelectedArray[j]]) + "\n");
						numPeopleDisplayed++;
					}
				}
			}
			if (currentBottomDataLine3 > lines.size()) {
				currentTopDataLine3 = lines.size() - 10;
				currentBottomDataLine3 = lines.size();
			}

			int linesNew = 0;

			if (lines.size() > 11) {
				if (currentTopDataLine3 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_SHOW);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_SHOW);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_HIDE);
				}
				for (int i = (currentBottomDataLine3 - 1); i >= currentTopDataLine3; i -= 1) {

					idDisplayedText3 = lines[i] + idDisplayedText3;
				}
				if (currentBottomDataLine3 < lines.size()) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_SHOW);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_SHOW);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_HIDE);
				}
			}
			else {
				for (int i = 0; i < lines.size(); i++) {
					idDisplayedText3 = idDisplayedText3 + lines[i];
				}
			}
			if (numDaysDisplayed == 0) {
				idDisplayedText3 = notEnoughDataText;
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_HIDE);

			}
			else {
				if (numPeopleDisplayed == 0) {
					idDisplayedText3 = "Select more options";
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_HIDE);

				}
			}

			SetWindowTextA(
				GetDlgItem(handleforwindow, ID_DATA_WINDOW3),
				(idDisplayedText3).c_str()
			);

			for (int i = 0; i < messageAnalyser.people.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]),
					HWND_BOTTOM,
					350,
					20 + (20 * (i - currentTopCheckboxNamesLine3)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]),
					HWND_BOTTOM,
					375,
					20 + (20 * (i - currentTopCheckboxNamesLine3)),
					200,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxNamesLine3) < 0 && currentTopCheckboxNamesLine3 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxNamesLine3) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]), SW_HIDE);
				}
			}

			if (currentTopCheckboxNamesLine3 == 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_UP), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_UP), SW_SHOW);
			}

			if (currentTopCheckboxNamesLine3 + 9 >= (messageAnalyser.people.size())) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN), SW_HIDE);

			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN), SW_SHOW);
			}



			break;
		}
		case ID_DATA_WINDOW3_SCROLL_UP: {
			currentTopDataLine3 -= 1;
			currentBottomDataLine3 -= 1;
			dataUpdateRequired = true;
			break;
		}
		case ID_DATA_WINDOW3_SCROLL_DOWN: {
			currentBottomDataLine3 += 1;
			currentTopDataLine3 += 1;
			dataUpdateRequired = true;
			break;
		}
		case ID_DATA_WINDOW3_PAGE_UP: {
			currentTopDataLine3 -= 10;
			currentBottomDataLine3 -= 10;
			if (currentTopDataLine3 < 0) {
				currentTopDataLine3 = 0;
				currentBottomDataLine3 = 10;
			}
			dataUpdateRequired = true;
			break;
		}
		case ID_DATA_WINDOW3_PAGE_DOWN: {
			currentBottomDataLine3 += 10;
			currentTopDataLine3 += 10;
			dataUpdateRequired = true;
			break;
		}
		case ID_DATA_CHECKBOXES3NAMES_SCROLL_UP: {
			currentTopCheckboxNamesLine3 -= 1;
			currentBottomCheckboxNamesLine3 -= 1;
			checkBox3NamesScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN: {
			currentTopCheckboxNamesLine3 += 1;
			currentBottomCheckboxNamesLine3 += 1;
			checkBox3NamesScroll = true;
			break;
		}
		case ID_LOBFCHECKBOX3: {
			if (IsDlgButtonChecked(handleforwindow, ID_LOBFCHECKBOX3)) {
				CheckDlgButton(handleforwindow, ID_LOBFCHECKBOX3, BST_UNCHECKED);
				messagesOverTimeGraph.deselectLOBF();
			}
			else {
				CheckDlgButton(handleforwindow, ID_LOBFCHECKBOX3, BST_CHECKED);
				messagesOverTimeGraph.selectLOBF();
			}
			break;
		}
		}
		if (checkBox3NamesScroll) {
			for (int i = 0; i < messageAnalyser.people.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]),
					HWND_BOTTOM,
					350,
					20 + (20 * (i - currentTopCheckboxNamesLine3)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]),
					HWND_BOTTOM,
					375,
					20 + (20 * (i - currentTopCheckboxNamesLine3)),
					200,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxNamesLine3) < 0 && currentTopCheckboxNamesLine3 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxNamesLine3) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX3NAMES_LABELS[i]), SW_HIDE);
				}
			}

			if (currentTopCheckboxNamesLine3 == 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_UP), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_UP), SW_SHOW);
			}
			if (currentTopCheckboxNamesLine3 + 9 >= (messageAnalyser.people.size())) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES3NAMES_SCROLL_DOWN), SW_SHOW);
			}
		}

		BOOL checked;
		//CheckBox 1
		std::map<std::string, bool> peopleSelectedBefore = peopleSelected3;
		peopleSelected3 = {};
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			if (LOWORD(wParam) == ID_DATA_CHECKBOXES3NAMES[i]) {
				dataUpdateRequired = true;
				currentTopDataLine3 = 0;
				currentBottomDataLine3 = 10;
				checked = IsDlgButtonChecked(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i]);
				if (checked) {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i], BST_UNCHECKED);
					peopleSelected3[messageAnalyser.people[i]] = false;
				}
				else {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES3NAMES[i], BST_CHECKED);
					peopleSelected3[messageAnalyser.people[i]] = true;
				}
			}
			else {
				peopleSelected3[messageAnalyser.people[i]] = peopleSelectedBefore[messageAnalyser.people[i]];

			}
		}
		if (dataUpdateRequired) {
			int numPeopleDisplayed = 0;
			int numDaysDisplayed = 0;
			idDisplayedText3 = "";
			std::vector<std::string> lines = {};
			std::vector <std::string> peopleSelectedArray = mapkeysbool(peopleSelected3);
			std::vector <std::string> daysSelectedArray = dayArraySort(getDaysBetween(startDate, endDate));
			peopleSelectedArray = peopleArraySort(peopleSelectedArray);
			lines.push_back("All Time: \n");
			for (int i = 0; i < peopleSelectedArray.size(); i++) {
				if (peopleSelected3[peopleSelectedArray[i]]) {
					lines.push_back("        " + peopleSelectedArray[i] + ": " + itos(personTotalFind(messageAnalyser, peopleSelectedArray[i], daysSelectedArray)) + "\n");
				}
			}
			for (int i = 0; i < daysSelectedArray.size(); i++) {
				if (messageAnalyser.totalmessagesdaily[daysSelectedArray[i]]["Total Messages"] != 0) {
					lines.push_back(daysSelectedArray[i] + ": \n");
					numDaysDisplayed++;
					for (int j = 0; j < peopleSelectedArray.size(); j++) {
						if (peopleSelected3[peopleSelectedArray[j]]) {
							lines.push_back("        " + peopleSelectedArray[j] + ": " + itos(messageAnalyser.totalmessagesdaily[daysSelectedArray[i]][peopleSelectedArray[j]]) + "\n");
							numPeopleDisplayed++;
						}
					}
				}
			}
			//TestBox();
			if (currentBottomDataLine3 > lines.size()) {
				currentTopDataLine3 = lines.size() - 10;
				currentBottomDataLine3 = lines.size();
			}
			int linesNew = 0;
			if (lines.size() > 11) {
				if (currentTopDataLine3 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_SHOW);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_SHOW);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_HIDE);
				}
				for (int i = currentTopDataLine3; i < currentBottomDataLine3; i++) {
					idDisplayedText3 += lines[i];
				}
				if (currentBottomDataLine3 < lines.size()) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_SHOW);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_SHOW);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_HIDE);
				}
			}
			else {
				if ((numDaysDisplayed == 0 || numPeopleDisplayed == 0)) {

				}
				else {
					for (int i = 0; i < lines.size(); i++) {
						idDisplayedText3 = idDisplayedText3 + lines[i];
					}
				}
			}
			if (numDaysDisplayed == 0) {
				idDisplayedText3 = notEnoughDataText;
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_HIDE);

			}
			else {
				if (numPeopleDisplayed == 0) {
					idDisplayedText3 = "Select more options";
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_UP), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_UP), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_SCROLL_DOWN), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW3_PAGE_DOWN), SW_HIDE);

				}
			}
			SetWindowTextA(
				GetDlgItem(handleforwindow, ID_DATA_WINDOW3),
				(idDisplayedText3).c_str()
			);
		}
		std::vector<std::string> peopleSelectedArray = mapkeysbool(peopleSelected3);
		for (std::string person : peopleSelectedArray) {
			if (peopleSelected3[person]) {
				messagesOverTimeGraph.selectLine(person);
			}
			else {
				messagesOverTimeGraph.deselectLine(person);
			}
		}
		if (peopleSelected3["Total Messages"]) {
			messagesOverTimeGraph.selectLine("Total");
		}
		else {
			messagesOverTimeGraph.deselectLine("Total");
		}
		messagesOverTimeGraph.refresh();
		break;
	}
	case WM_PAINT: {
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = SUBWINDOWLENGTH;
		rect.bottom = GRAPH3YPOS;
		messagesOverTimeGraph.refresh();
		ValidateRgn(handleforwindow, NULL);
		InvalidateRect(handleforwindow, &rect, true);
		break;

	}
	}
	



	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}









LRESULT CALLBACK windowprocessforwindow24(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR greeting[] = _T("Hello, Windows desktop!");
	BOOL ShowText = false;


	switch (msg)
	{

	case WM_DRAWITEM:
	{
		
		drawButton(lParam);
		return TRUE;
	}

	case WM_CTLCOLORSTATIC: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CTLCOLORBTN: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CREATE:
	{
		//Graph
		handleforreplyTimesGraph = CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			GRAPH4XPOS, GRAPH4YPOS, GRAPH4XSIZE, GRAPH3YSIZE,
			handleforwindow,
			NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		if (not windowsCreatedBefore) {
			//Graph Window			
			std::string personHighest = getHighestInnerIndex2DMap(messageAnalyser.replytimesweeklyaverage);
			std::vector <std::vector <double> > coordinates = { { },  { } };
			for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
				coordinates[0].push_back(i*7);
				coordinates[1].push_back(messageAnalyser.replytimesweeklyaverage[messageAnalyser.allweeks[i]][personHighest]);
			}
			std::vector<std::string> newMonths = {};
			for (std::string month : messageAnalyser.months) {
				if (month != "All Time") {
					newMonths.push_back(month);
				}
			}
			std::vector<std::string> alldays = getDaysBetween(messageAnalyser.alldays[0], messageAnalyser.alldays[messageAnalyser.alldays.size() - 1]);
			std::vector<std::string> allmonths = getMonthsBetween(alldays[0], alldays[alldays.size() - 1]);
			std::vector<double> xAxisVals = getMonthsDays(allmonths);
			std::vector<double>	yAxisVals = yAxisValuesFind(coordinates[1]);
			applyStyles(handleforreplyTimesGraph);
			replyTimesGraph = CreateLineGraph(handleforreplyTimesGraph, GRAPH4XSIZE - 100, GRAPH4YSIZE - 100,
				xAxisVals, yAxisVals,
				MONTHTYPE, INTTYPE,
				"Month", "Num Messages",
				("Daily messages across " + itos(messageAnalyser.alldays.size())) + " days",
				GRAPH4XPOS, GRAPH4YPOS,
				allmonths[0]);
			replyTimesGraph.addLine(coordinates, personHighest, true);
			for (std::string person : messageAnalyser.people) {
				if (person != personHighest) {
					coordinates = { { },  { } };
					for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
						coordinates[0].push_back(i*7);
						coordinates[1].push_back(messageAnalyser.replytimesweeklyaverage[messageAnalyser.allweeks[i]][person]);
					}
					if (person == "Total Messages") {
						replyTimesGraph.addLine(coordinates, "Total", true);
					}
					else {
						replyTimesGraph.addLine(coordinates, person, true);
					}
				}
				for (std::string person : messageAnalyser.people) {
					peopleSelected3[person] = true;
				}
			}
		}
		else {
			replyTimesGraph.window = handleforreplyTimesGraph;
		}











		CreateWindow(L"BUTTON",
			L"Week View",
			BUTTON_STYLE,
			330, 0, 90, 20,
			handleforwindow, (HMENU)ID_PERIODSWITCH4, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_PERIODSWITCH4));

		//CheckBox 1
		CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			500, 20, 300, 180,
			handleforwindow,
			(HMENU)ID_DATA_WINDOW4, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW4));
		SetWindowTextA(
			GetDlgItem(handleforwindow, ID_DATA_WINDOW4),

			"No Options Selected"
		);

		CreateWindow(L"BUTTON",
			L" /\\        UP        /\\ ",
			BUTTON_STYLE,
			500, 0, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW4_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_UP));

		CreateWindow(L"BUTTON",
			L" \\/     DOWN     \\/ ",
			BUTTON_STYLE,
			500, 200, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW4_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_DOWN));
		CreateWindow(L"BUTTON",
			L" /\\    PAGEUP    /\\ ",
			BUTTON_STYLE,
			620, 0, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW4_PAGE_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_UP));
		CreateWindow(L"BUTTON",
			L" \\/  PAGEDOWN  \\/ ",
			BUTTON_STYLE,
			620, 200, 120, 20,
			handleforwindow, (HMENU)ID_DATA_WINDOW4_PAGE_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_DOWN));


		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_DOWN), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_UP), SW_HIDE);
		ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_DOWN), SW_HIDE);
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			CreateWindow(L"BUTTON", NULL,
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				25, (20 + (20 * i)), 20, 20,
				handleforwindow, (HMENU)ID_DATA_CHECKBOXES4NAMES[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES4NAMES[i], BST_CHECKED);
			CreateWindow(L"Static", NULL,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
				50, (20 + (20 * i)), 200, 20,
				handleforwindow,
				(HMENU)ID_DATA_CHECKBOX4NAMES_LABELS[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4NAMES_LABELS[i]));
			SetWindowTextA(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4NAMES_LABELS[i]),
				(messageAnalyser.people[i]).c_str()
			);
			peopleSelected4[messageAnalyser.people[i]] = true;
		}
		CreateWindow(L"BUTTON",
			L" /\\    UP    /\\ ",
			BUTTON_STYLE,
			100, 0, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES4NAMES_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_UP));

		CreateWindow(L"BUTTON",
			L" \\/ DOWN \\/ ",
			BUTTON_STYLE,
			100, 200, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES4NAMES_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_DOWN));
		if (messageAnalyser.people.size() > 9) {

		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_UP), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_DOWN), SW_HIDE);
		}
		int numWeeks = (int)messageAnalyser.allweeks.size();
		for (int i = 0; i < numWeeks; i++) {
			CreateWindow(TEXT("button"), NULL,
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				270, (20 + ((numWeeks - 1) * 20) - (20 * i)), 20, 20,
				handleforwindow, (HMENU)ID_DATA_CHECKBOXES4PERIODS[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i], BST_CHECKED);
			CreateWindow(L"Static", NULL,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
				320, (20 + ((numWeeks - 1) * 20) - (20 * i)), 100, 20,
				handleforwindow,
				(HMENU)ID_DATA_CHECKBOX4PERIODS_LABELS[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]));
			if (i < messageAnalyser.months.size()) {
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]),
					(messageAnalyser.months[i]).c_str()
				);
				monthsSelected4[messageAnalyser.months[i]] = true;
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]), SW_HIDE);
			}
			weeksSelected4[messageAnalyser.allweeks[i]] = true;
		}

		CreateWindow(L"BUTTON",
			L" /\\    UP    /\\ ",
			BUTTON_STYLE,
			245, 0, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP));
		CreateWindow(L"BUTTON",
			L" \\/ DOWN \\/ ",
			BUTTON_STYLE,
			245, 200, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN));

		if (messageAnalyser.months.size() > 9) {

		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN), SW_HIDE);
		}








		int numPeopleDisplayed = 0;
		int numMonthsDisplayed = 0;
		idDisplayedText4 = "";
		std::vector<std::string> lines = {};
		std::vector <std::string> peopleSelectedArray = mapkeysbool(peopleSelected4);
		std::vector <std::string> monthsSelectedArray = mapkeysbool(monthsSelected4);
		peopleSelectedArray = peopleArraySort(peopleSelectedArray);
		monthsSelectedArray = dateArraySort(monthsSelectedArray);
		for (int i = 0; i < monthsSelectedArray.size(); i++) {
			if (monthsSelected4[monthsSelectedArray[i]]) {
				lines.push_back(monthsSelectedArray[i] + ": \n");
				numMonthsDisplayed++;
				for (int j = 0; j < peopleSelectedArray.size(); j++) {
					if (peopleSelected4[peopleSelectedArray[j]]) {
						if (peopleSelectedArray[j] == "Total Messages") {
							lines.push_back("        Average Time: " + ((roundval(messageAnalyser.replytimesmonthlyaverage[monthsSelectedArray[i]][peopleSelectedArray[j]]))) + "\n");
						}
						else {
							lines.push_back("        " + peopleSelectedArray[j] + ": " + ((roundval(messageAnalyser.replytimesmonthlyaverage[monthsSelectedArray[i]][peopleSelectedArray[j]]))) + "\n");
						}
						numPeopleDisplayed++;
					}
				}
			}
		}

		int linesNew = 0;
		if (lines.size() > 11) {
			if (currentTopDataLine4 != 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_UP), SW_SHOW);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_UP), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_UP), SW_HIDE);
			}
			for (int i = (currentBottomDataLine4 - 1); i >= currentTopDataLine4; i -= 1) {
				idDisplayedText4 = lines[i] + idDisplayedText4;
			}
			if (currentBottomDataLine4 < lines.size()) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_DOWN), SW_SHOW);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_DOWN), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_DOWN), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_DOWN), SW_HIDE);
			}
		}
		else {
			if (numMonthsDisplayed == 0 || numPeopleDisplayed == 0) {
				idDisplayedText4 = "Select more options";
			}
			else {
				for (int i = 0; i < lines.size(); i++) {
					idDisplayedText4 = idDisplayedText4 + lines[i];
				}
			}
		}
		SetWindowTextA(
			GetDlgItem(handleforwindow, ID_DATA_WINDOW4),
			(idDisplayedText4).c_str()
		);

		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			SetWindowPos(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES[i]),
				HWND_BOTTOM,
				25,
				20 + (20 * (i - currentTopCheckboxNamesLine4)),
				20,
				20,
				SWP_SHOWWINDOW
			);
			SetWindowPos(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4NAMES_LABELS[i]),
				HWND_BOTTOM,
				50,
				20 + (20 * (i - currentTopCheckboxNamesLine4)),
				200,
				20,
				SWP_SHOWWINDOW
			);
			if ((i - currentTopCheckboxNamesLine4) < 0 && currentTopCheckboxNamesLine4 != 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4NAMES_LABELS[i]), SW_HIDE);
			}
			if ((i - currentTopCheckboxNamesLine4) > 8) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4NAMES_LABELS[i]), SW_HIDE);
			}
		}

		if (currentTopCheckboxNamesLine4 == 0) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_UP), SW_HIDE);
		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_UP), SW_SHOW);
		}
		if (currentTopCheckboxNamesLine4 + 9 >= (messageAnalyser.people.size())) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_DOWN), SW_HIDE);

		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_DOWN), SW_SHOW);
		}



		if (periodSelected4 == "M") {
			for (int i = 0; i < messageAnalyser.months.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]),
					HWND_BOTTOM,
					270,
					20 + (20 * (i - currentTopCheckboxMonthsLine4)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]),
					HWND_BOTTOM,
					320,
					20 + (20 * (i - currentTopCheckboxMonthsLine4)),
					100,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxMonthsLine4) < 0 && currentTopCheckboxMonthsLine4 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxMonthsLine4) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]), SW_HIDE);
				}
			}

			if (currentTopCheckboxMonthsLine4 == 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP), SW_SHOW);
			}
			if (currentTopCheckboxMonthsLine4 + 9 >= (messageAnalyser.months.size())) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN), SW_SHOW);
			}
		}
		else {
			for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]),
					HWND_BOTTOM,
					270,
					20 + (20 * (i - currentTopCheckboxWeeksLine4)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]),
					HWND_BOTTOM,
					320,
					20 + (20 * (i - currentTopCheckboxWeeksLine4)),
					100,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxWeeksLine4) < 0 && currentTopCheckboxWeeksLine4 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxWeeksLine4) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]), SW_HIDE);
				}
			}

			if (currentTopCheckboxWeeksLine4 == 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP), SW_SHOW);
			}
			if (currentTopCheckboxWeeksLine4 + 9 >= (messageAnalyser.allweeks.size())) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN), SW_SHOW);
			}
		}


		break;
	}


	case WM_COMMAND:
	{
		bool checkBox4NamesScroll = false;
		bool checkBox4MonthsScroll = false;
		switch (LOWORD(wParam)) {

		case ID_PERIODSWITCH4: {
			if (periodSelected4 == "M") {
				periodSelected4 = "W";
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_PERIODSWITCH4),
					"Month View"
				);

				for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
					if (weeksSelected4[messageAnalyser.allweeks[i]]) {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i], BST_CHECKED);
					}
					else {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i], BST_UNCHECKED);
					}
				}


			}
			else {
				periodSelected4 = "M";
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_PERIODSWITCH4),
					"Week View"
				);
				for (int i = 0; i < messageAnalyser.months.size(); i++) {
					if (monthsSelected4[messageAnalyser.months[i]]) {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i], BST_CHECKED);
					}
					else {
						CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i], BST_UNCHECKED);
					}
				}
			}
			checkBox4MonthsScroll = true;
			currentTopDataLine4 = 0;
			currentBottomDataLine4 = 10;
			currentTopCheckboxMonthsLine4 = 0;
			currentBottomCheckboxMonthsLine4 = 11;
			currentTopCheckboxWeeksLine4 = 0;
			currentBottomCheckboxWeeksLine4 = 11;
			break;
		}
		case ID_DATA_WINDOW4_SCROLL_UP: {
			currentTopDataLine4 -= 1;
			currentBottomDataLine4 -= 1;
			break;
		}
		case ID_DATA_WINDOW4_SCROLL_DOWN: {
			currentBottomDataLine4 += 1;
			currentTopDataLine4 += 1;
			break;
		}
		case ID_DATA_WINDOW4_PAGE_UP: {
			currentTopDataLine4 -= 10;
			currentBottomDataLine4 -= 10;
			if (currentTopDataLine4 < 0) {
				currentTopDataLine4 = 0;
				currentBottomDataLine4 = 10;
			}
			break;
		}
		case ID_DATA_WINDOW4_PAGE_DOWN: {
			currentBottomDataLine4 += 10;
			currentTopDataLine4 += 10;
			break;
		}
		case ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP: {
			currentTopCheckboxMonthsLine4 -= 1;
			currentTopCheckboxWeeksLine4 -= 1;
			currentBottomCheckboxMonthsLine4 -= 1;
			currentBottomCheckboxWeeksLine4 -= 1;
			checkBox4MonthsScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN: {
			currentTopCheckboxMonthsLine4 += 1;
			currentTopCheckboxWeeksLine4 += 1;
			currentBottomCheckboxMonthsLine4 += 1;
			currentBottomCheckboxWeeksLine4 += 1;
			checkBox4MonthsScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES4NAMES_SCROLL_UP: {
			currentTopCheckboxNamesLine4 -= 1;
			currentBottomCheckboxNamesLine4 -= 1;
			checkBox4NamesScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES4NAMES_SCROLL_DOWN: {
			currentTopCheckboxNamesLine4 += 1;
			currentBottomCheckboxNamesLine4 += 1;
			checkBox4NamesScroll = true;
			break;
		}
		}
		if (checkBox4NamesScroll) {
			for (int i = 0; i < messageAnalyser.people.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES[i]),
					HWND_BOTTOM,
					25,
					20 + (20 * (i - currentTopCheckboxNamesLine4)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4NAMES_LABELS[i]),
					HWND_BOTTOM,
					50,
					20 + (20 * (i - currentTopCheckboxNamesLine4)),
					200,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxNamesLine4) < 0 && currentTopCheckboxNamesLine4 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4NAMES_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxNamesLine4) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4NAMES_LABELS[i]), SW_HIDE);
				}
			}

			if (currentTopCheckboxNamesLine4 == 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_UP), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_UP), SW_SHOW);
			}
			if (currentTopCheckboxNamesLine4 + 9 >= (messageAnalyser.people.size())) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_DOWN), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4NAMES_SCROLL_DOWN), SW_SHOW);
			}
		}

		if (checkBox4MonthsScroll) {

			for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
				SetWindowTextA(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]),
					""
				);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]), SW_HIDE);
			}
			if (periodSelected4 == "M") {
				for (int i = 0; i < messageAnalyser.months.size(); i++) {
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]),
						HWND_BOTTOM,
						270,
						20 + (20 * (i - currentTopCheckboxMonthsLine4)),
						20,
						20,
						SWP_SHOWWINDOW
					);
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]),
						HWND_BOTTOM,
						320,
						20 + (20 * (i - currentTopCheckboxMonthsLine4)),
						100,
						20,
						SWP_SHOWWINDOW
					);
					if ((i - currentTopCheckboxMonthsLine4) < 0 && currentTopCheckboxMonthsLine4 != 0) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]), SW_HIDE);
					}
					if ((i - currentTopCheckboxMonthsLine4) > 8) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]), SW_HIDE);
					}
					SetWindowTextA(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]),
						messageAnalyser.months[i].c_str()
					);

				}

				if (currentTopCheckboxMonthsLine4 == 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP), SW_SHOW);
				}
				if (currentTopCheckboxMonthsLine4 + 9 >= messageAnalyser.months.size()) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN), SW_SHOW);
				}
			}
			else {
				for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]),
						HWND_BOTTOM,
						270,
						20 + (20 * (i - currentTopCheckboxWeeksLine4)),
						20,
						20,
						SWP_SHOWWINDOW
					);
					SetWindowPos(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]),
						HWND_BOTTOM,
						320,
						20 + (20 * (i - currentTopCheckboxWeeksLine4)),
						100,
						20,
						SWP_SHOWWINDOW
					);
					if ((i - currentTopCheckboxWeeksLine4) < 0 && currentTopCheckboxWeeksLine4 != 0) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]), SW_HIDE);
					}
					if ((i - currentTopCheckboxWeeksLine4) > 8) {
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]), SW_HIDE);
						ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]), SW_HIDE);
					}
					SetWindowTextA(
						GetDlgItem(handleforwindow, ID_DATA_CHECKBOX4PERIODS_LABELS[i]),
						messageAnalyser.allweeks[i].c_str()
					);

				}

				if (currentTopCheckboxWeeksLine4 == 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_UP), SW_SHOW);
				}
				if (currentTopCheckboxWeeksLine4 + 9 >= messageAnalyser.allweeks.size()) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN), SW_HIDE);
				}
				else {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES4PERIODS_SCROLL_DOWN), SW_SHOW);
				}
			}
		}
		BOOL checked;
		//CheckBox 1
		std::map<std::string, bool> peopleSelectedBefore = peopleSelected4;
		peopleSelected4 = {};
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			if (LOWORD(wParam) == ID_DATA_CHECKBOXES4NAMES[i]) {
				currentTopDataLine4 = 0;
				currentBottomDataLine4 = 10;
				checked = IsDlgButtonChecked(handleforwindow, ID_DATA_CHECKBOXES4NAMES[i]);
				if (checked) {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES4NAMES[i], BST_UNCHECKED);
					peopleSelected4[messageAnalyser.people[i]] = false;
					//LastItSet(1, false);
				}
				else {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES4NAMES[i], BST_CHECKED);
					peopleSelected4[messageAnalyser.people[i]] = true;
				}
			}
			else {
				peopleSelected4[messageAnalyser.people[i]] = peopleSelectedBefore[messageAnalyser.people[i]];

			}
		}
		std::map<std::string, bool>  monthsSelectedBefore = monthsSelected4;
		std::map<std::string, bool>  weeksSelectedBefore = weeksSelected4;
		for (int i = 0; i < messageAnalyser.allweeks.size(); i++) {
			if (LOWORD(wParam) == ID_DATA_CHECKBOXES4PERIODS[i]) {
				checked = IsDlgButtonChecked(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i]);
				currentTopDataLine4 = 0;
				currentBottomDataLine4 = 10;
				if (checked) {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i], BST_UNCHECKED);
					if (periodSelected4 == "M") {
						if (i < messageAnalyser.months.size()) {
							monthsSelected4[messageAnalyser.months[i]] = false;
						}
					}
					else {
						weeksSelected4[messageAnalyser.allweeks[i]] = false;
					}
				}
				else {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES4PERIODS[i], BST_CHECKED);
					if (periodSelected4 == "M") {
						if (i < messageAnalyser.months.size()) {
							monthsSelected4[messageAnalyser.months[i]] = true;
						}
					}
					else {
						weeksSelected4[messageAnalyser.allweeks[i]] = true;
					}

				}
			}
			else {
				if (periodSelected4 == "M") {
					if (i < messageAnalyser.months.size()) {
						monthsSelected4[messageAnalyser.months[i]] = monthsSelectedBefore[messageAnalyser.months[i]];
					}
				}
				else {
					weeksSelected4[messageAnalyser.allweeks[i]] = weeksSelectedBefore[messageAnalyser.allweeks[i]];
				}

			}
		}
		int numPeopleDisplayed = 0;
		int numPeriodsDisplayed = 0;
		idDisplayedText4 = "";
		std::vector<std::string> lines = {};
		std::vector <std::string> peopleSelectedArray = mapkeysbool(peopleSelected4);
		std::vector <std::string> monthsSelectedArray = mapkeysbool(monthsSelected4);
		std::vector <std::string> weeksSelectedArray = mapkeysbool(weeksSelected4);
		peopleSelectedArray = peopleArraySort(peopleSelectedArray);
		monthsSelectedArray = dateArraySort(monthsSelectedArray);
		weeksSelectedArray = dayArraySort(weeksSelectedArray);
		if (periodSelected4 == "M") {
			for (int i = 0; i < monthsSelectedArray.size(); i++) {
				if (monthsSelected4[monthsSelectedArray[i]]) {
					lines.push_back(monthsSelectedArray[i] + ": \n");
					numPeriodsDisplayed++;
					for (int j = 0; j < peopleSelectedArray.size(); j++) {
						if (peopleSelected4[peopleSelectedArray[j]]) {
							if (peopleSelectedArray[j] == "Total Messages") {
								lines.push_back("        Average Time: " + ((roundval(messageAnalyser.replytimesmonthlyaverage[monthsSelectedArray[i]][peopleSelectedArray[j]]))) + "\n");
							}
							else {
								lines.push_back("        " + peopleSelectedArray[j] + ": " + ((roundval(messageAnalyser.replytimesmonthlyaverage[monthsSelectedArray[i]][peopleSelectedArray[j]]))) + "\n");
							}
							numPeopleDisplayed++;
						}
					}
				}
			}

		}
		else {
			for (int i = 0; i < weeksSelectedArray.size(); i++) {
				if (weeksSelected4[weeksSelectedArray[i]]) {
					lines.push_back(weeksSelectedArray[i] + ": \n");
					numPeriodsDisplayed++;
					for (int j = 0; j < peopleSelectedArray.size(); j++) {
						if (peopleSelectedArray[j] == "Total Messages") {
							lines.push_back("        Average Time: " + ((roundval(messageAnalyser.replytimesweeklyaverage[weeksSelectedArray[i]][peopleSelectedArray[j]]))) + "\n");
						}
						else {
							lines.push_back("        " + peopleSelectedArray[j] + ": " + ((roundval(messageAnalyser.replytimesweeklyaverage[weeksSelectedArray[i]][peopleSelectedArray[j]]))) + "\n");
						}
					}
				}
			}
		}

		if (currentBottomDataLine4 > lines.size()) {
			currentTopDataLine4 = lines.size() - 10;
			currentBottomDataLine4 = lines.size();
		}

		int linesNew = 0;
		if (lines.size() > 11) {
			if (currentTopDataLine4 != 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_UP), SW_SHOW);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_UP), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_UP), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_UP), SW_HIDE);
			}
			for (int i = currentTopDataLine4; i < currentBottomDataLine4; i++) {
				idDisplayedText4 += lines[i];
			}
			if (currentBottomDataLine4 < lines.size()) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_DOWN), SW_SHOW);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_DOWN), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_SCROLL_DOWN), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_WINDOW4_PAGE_DOWN), SW_HIDE);
			}
		}
		else {
			if (numPeriodsDisplayed == 0 || numPeopleDisplayed == 0) {
				idDisplayedText4 = "Select more options";
			}
			else {
				for (int i = 0; i < lines.size(); i++) {
					idDisplayedText4 = idDisplayedText4 + lines[i];
				}
			}
		}
		SetWindowTextA(
			GetDlgItem(handleforwindow, ID_DATA_WINDOW4),
			(idDisplayedText4).c_str()
		);
		peopleSelectedArray = mapkeysbool(peopleSelected4);
		for (std::string person : peopleSelectedArray) {
			if (peopleSelected4[person]) {
				replyTimesGraph.selectLine(person);
			}
			else {
				replyTimesGraph.deselectLine(person);
			}
		}
		if (peopleSelected4["Total Messages"]) {
			replyTimesGraph.selectLine("Total");
		}
		else {
			replyTimesGraph.deselectLine("Total");
		}
		replyTimesGraph.refresh();
		break;
	}
	case WM_PAINT: {
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = SUBWINDOWLENGTH;
		rect.bottom = GRAPH4YPOS;
		replyTimesGraph.refresh();
		ValidateRgn(handleforwindow, NULL);
		InvalidateRect(handleforwindow, &rect, true);
		break;

	}



	}
	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}





LRESULT CALLBACK windowprocessforwindow25(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DRAWITEM:
	{
		
		drawButton(lParam);
		return TRUE;
	}


	case WM_CTLCOLORSTATIC: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CTLCOLORBTN: {
		HDC hdc = (HDC)wParam;
        SetTextColor(hdc, TEXT_COLOUR);
		SetBkColor(hdc, BACKGROUND_COLOUR);
		return (INT_PTR)BACKGROUND_COLOUR_BRUSH;
	}

	case WM_CREATE: {
		//CheckBoxes
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			CreateWindow(L"BUTTON", NULL,
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				25, (20 + (20 * i)), 20, 20,
				handleforwindow, (HMENU)ID_DATA_CHECKBOXES5NAMES[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i], BST_CHECKED);
			CreateWindow(L"Static", NULL,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
				50, (20 + (20 * i)), 200, 20,
				handleforwindow,
				(HMENU)ID_DATA_CHECKBOX5NAMES_LABELS[i], ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX5NAMES_LABELS[i]));;

			SetWindowTextA(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOX5NAMES_LABELS[i]),
				(messageAnalyser.people[i]).c_str()
			);
			peopleSelected5[messageAnalyser.people[i]] = true;
		}
		CreateWindow(L"BUTTON",
			L" /\\    UP    /\\ ",
			BUTTON_STYLE,
			100, 0, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES5NAMES_SCROLL_UP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_UP));

		CreateWindow(L"BUTTON",
			L" \\/ DOWN \\/ ",
			BUTTON_STYLE,
			100, 200, 75, 20,
			handleforwindow, (HMENU)ID_DATA_CHECKBOXES5NAMES_SCROLL_DOWN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		applyStyles(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_DOWN));
		if (messageAnalyser.people.size() > 9) {

		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_UP), SW_HIDE);
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_DOWN), SW_HIDE);
		}

		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			SetWindowPos(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i]),
				HWND_BOTTOM,
				25,
				20 + (20 * (i - currentTopCheckboxNamesLine5)),
				20,
				20,
				SWP_SHOWWINDOW
			);
			SetWindowPos(
				GetDlgItem(handleforwindow, ID_DATA_CHECKBOX5NAMES_LABELS[i]),
				HWND_BOTTOM,
				50,
				20 + (20 * (i - currentTopCheckboxNamesLine5)),
				200,
				20,
				SWP_SHOWWINDOW
			);
			if ((i - currentTopCheckboxNamesLine5) < 0 && currentTopCheckboxNamesLine5 != 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX5NAMES_LABELS[i]), SW_HIDE);
			}
			if ((i - currentTopCheckboxNamesLine5) > 8) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i]), SW_HIDE);
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX5NAMES_LABELS[i]), SW_HIDE);
			}
		}

		if (currentTopCheckboxNamesLine5 == 0) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_UP), SW_HIDE);
		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_UP), SW_SHOW);
		}
		if (currentTopCheckboxNamesLine5 + 9 >= (messageAnalyser.people.size())) {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_DOWN), SW_HIDE);

		}
		else {
			ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_DOWN), SW_SHOW);
		}
		

		//Graph
		handleforsendingTimesGraph = CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			GRAPH5XPOS, GRAPH5YPOS, GRAPH5XSIZE, GRAPH5YSIZE,
			handleforwindow,
			NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		if (not windowsCreatedBefore) {
			std::vector <std::vector <double> > coordinates = { { },  { } };
			std::map<std::string, int> averageTimes = {};
			for (std::string time : messageAnalyser.alltimes) {

				coordinates[0].push_back(secondsfind(time));
				coordinates[1].push_back((averagePast(50, time, messageAnalyser.timetotals["Total Messages"])));
			}
			std::vector<double> xAxisVals = {}, yAxisVals = yAxisValuesFind(coordinates[1]);

			for (std::string hour : xAxisHours) {
				xAxisVals.push_back(secondsfind(hour));
			}

			applyStyles(handleforsendingTimesGraph);
			sendingTimesGraph = CreateLineGraph(handleforsendingTimesGraph, GRAPH5XSIZE - 100, GRAPH5YSIZE - 100, xAxisVals, yAxisVals, TIMETYPE, INTTYPE, "Hour", "Num Messages", ("Daily messages across " + itos(messageAnalyser.alldays.size())) + " days", GRAPH5XPOS, GRAPH5YPOS);
			sendingTimesGraph.deselectLOBF();
			sendingTimesGraph.addLine(coordinates, "Total", true);
			for (std::string person : messageAnalyser.people) {
				if (person != "Total Messages") {
					std::vector <std::vector <double> > coordinates = { { },  { } };
					for (std::string time : messageAnalyser.alltimes) {
						coordinates[0].push_back(secondsfind(time));
						coordinates[1].push_back((averagePast(50, time, messageAnalyser.timetotals[person])));
					}
					sendingTimesGraph.addLine(coordinates, person, true);
				}
			}
			GraphLine temp = sendingTimesGraph.lines["Total"];
		}
		else {
			sendingTimesGraph.window = handleforsendingTimesGraph;
		}

		break;

	}


	case SDM_REPLYGRAPHLOAD: {
		//break;
	}



	case WM_COMMAND: {
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			if (peopleSelected5[messageAnalyser.people[i]]) {
				CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i], BST_CHECKED);
			}
			else {
				CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i], BST_UNCHECKED);
			}
		}
		bool checkBox5NamesScroll = false;
		switch (LOWORD(wParam)) {
		case ID_DATA_CHECKBOXES5NAMES_SCROLL_UP: {
			currentTopCheckboxNamesLine5 -= 1;
			currentBottomCheckboxNamesLine5 -= 1;
			checkBox5NamesScroll = true;
			break;
		}
		case ID_DATA_CHECKBOXES5NAMES_SCROLL_DOWN: {
			currentTopCheckboxNamesLine5 += 1;
			currentBottomCheckboxNamesLine5 += 1;
			checkBox5NamesScroll = true;
			break;
		}
		}
		if (checkBox5NamesScroll) {
			for (int i = 0; i < messageAnalyser.people.size(); i++) {
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i]),
					HWND_BOTTOM,
					25,
					20 + (20 * (i - currentTopCheckboxNamesLine5)),
					20,
					20,
					SWP_SHOWWINDOW
				);
				SetWindowPos(
					GetDlgItem(handleforwindow, ID_DATA_CHECKBOX5NAMES_LABELS[i]),
					HWND_BOTTOM,
					50,
					20 + (20 * (i - currentTopCheckboxNamesLine5)),
					200,
					20,
					SWP_SHOWWINDOW
				);
				if ((i - currentTopCheckboxNamesLine5) < 0 && currentTopCheckboxNamesLine5 != 0) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX5NAMES_LABELS[i]), SW_HIDE);
				}
				if ((i - currentTopCheckboxNamesLine5) > 8) {
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i]), SW_HIDE);
					ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOX5NAMES_LABELS[i]), SW_HIDE);
				}
			}

			if (currentTopCheckboxNamesLine5 == 0) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_UP), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_UP), SW_SHOW);
			}
			if (currentTopCheckboxNamesLine5 + 9 >= (messageAnalyser.people.size())) {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_DOWN), SW_HIDE);
			}
			else {
				ShowWindow(GetDlgItem(handleforwindow, ID_DATA_CHECKBOXES5NAMES_SCROLL_DOWN), SW_SHOW);
			}
		}

		BOOL checked;
		//CheckBox 1
		std::map<std::string, bool> peopleSelectedBefore = peopleSelected5;
		peopleSelected5 = {};
		for (int i = 0; i < messageAnalyser.people.size(); i++) {
			if (LOWORD(wParam) == ID_DATA_CHECKBOXES5NAMES[i]) {
				checked = IsDlgButtonChecked(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i]);
				if (checked) {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i], BST_UNCHECKED);
					peopleSelected5[messageAnalyser.people[i]] = false;
					//LastItSet(1, false);
				}
				else {
					CheckDlgButton(handleforwindow, ID_DATA_CHECKBOXES5NAMES[i], BST_CHECKED);
					peopleSelected5[messageAnalyser.people[i]] = true;
				}
			}
			else {
				peopleSelected5[messageAnalyser.people[i]] = peopleSelectedBefore[messageAnalyser.people[i]];

			}
		}
		std::vector<std::string> peopleSelectedArray = mapkeysbool(peopleSelected5);
		for (std::string person : peopleSelectedArray) {
			if (peopleSelected5[person]) {
				sendingTimesGraph.selectLine(person);
			}
			else {
				sendingTimesGraph.deselectLine(person);
			}
		}
		if (peopleSelected5["Total Messages"]) {
			sendingTimesGraph.selectLine("Total");
		}
		else {
			sendingTimesGraph.deselectLine("Total");
		}
		sendingTimesGraph.refresh();
		break;
	}

	case WM_PAINT: {
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = SUBWINDOWLENGTH;
		rect.bottom = GRAPH5YPOS;
		sendingTimesGraph.refresh();
		ValidateRgn(handleforwindow, NULL);
		InvalidateRect(handleforwindow, &rect, true);
		break;
	}
	}
	return DefWindowProc(handleforwindow, msg, wParam, lParam);


}













BOOL CALLBACK PasswordEnterProc(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG: {
		SetWindowTextA(GetDlgItem(handleforwindow, IDC_STATIC1), passwordDiagText.c_str());
		CheckDlgButton(handleforwindow, IDC_CHECK1, BST_CHECKED);
	}


	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDOK: {
			if (IsDlgButtonChecked(handleforwindow, IDC_CHECK1)) {
				LPWSTR textEnteredRaw = new TCHAR[1000];
				GetWindowText(GetDlgItem(handleforwindow, IDC_EDIT1), textEnteredRaw, 1000);
				if (textEnteredRaw == L"") {
					passwordIn = PASSWORD;
				}
				else {
					passwordIn = textEnteredRaw;
				}
			}
			else {
				passwordIn = PASSWORD;
			}
		}
		case IDCANCEL: {
			EndDialog(handleforwindow, wParam);

			passwordDiagFinished = false;
			return TRUE;
		}
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(handleforwindow, wParam);
		return TRUE;
	}

	}

	return FALSE;
}









BOOL CALLBACK PreferencesProc(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG: {

		ShowWindow(handleforwindow, SW_SHOW);
		for (LPCTSTR option : checkForUpdateOptions) {
			SendMessage(GetDlgItem(handleforwindow, IDC_COMBO7), (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)option);
		}
		SendMessage(GetDlgItem(handleforwindow, IDC_COMBO7), CB_SETCURSEL, (WPARAM)preferences.checkForUpdates, (LPARAM)0);
		for (LPCTSTR option : fileSaveModeOptions) {
			SendMessage(GetDlgItem(handleforwindow, IDC_COMBO5), (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)option);
		}
		SendMessage(GetDlgItem(handleforwindow, IDC_COMBO5), CB_SETCURSEL, (WPARAM)preferences.fileSaveMode, (LPARAM)0);
		if (preferences.darkMode) {
			CheckDlgButton(handleforwindow, IDC_CHECK2, BST_CHECKED);
		}
		else {
			CheckDlgButton(handleforwindow, IDC_CHECK2, BST_UNCHECKED);
		}
		break;
	}


	case WM_COMMAND: {

		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			int ItemIndex = (int)SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0);
			switch (LOWORD(wParam))
			{
			case IDC_COMBO7: {
				preferences.checkForUpdates = ItemIndex;
				break;
			}
			case IDC_COMBO5: {
				preferences.fileSaveMode = ItemIndex;
				break;
			}
			}
		}


		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: {
			if (installUpdates(false)) {
				EndDialog(handleforwindow, SDM_UPDATENOW);
			}
			else {
				MessageBox(handleforwindow,
					L"No updates are currently available",
					L"No Updates",
					MB_ICONINFORMATION);
			}
			break;
		}
		case IDC_CHECK2: {
			if (IsDlgButtonChecked(handleforwindow, IDC_CHECK2)) {
				preferences.darkMode = true;
			}
			else {
				preferences.darkMode = false;
			}
			MessageBoxA(handleforwindow,
				"You will need to restart the program for changes to take effect",
				"Restart Required",
				MB_OK);
			break;
		}
		case IDOK: {
			setPreferences(preferences);
		}
		case IDCANCEL: {
			EndDialog(handleforwindow, wParam);
			return TRUE;
		}
		}
		break;
	}
	case WM_CLOSE: {
		EndDialog(handleforwindow, wParam);
		return TRUE;
	}
	}

	return FALSE;
}



BOOL CALLBACK ProgressBarProc(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG: {	
		SetWindowPos(handleforwindow, HWND_TOP, 200, 200, 0, 0, SWP_NOSIZE);
		ShowWindow(handleforwindow, SW_SHOW);
		fileLines = countLines(messageAnalyser.filepath);
		progress = 0;
		SendMessage(GetDlgItem(handleforwindow, IDC_PROGRESS1), PBM_SETRANGE32, (WPARAM)0, (LPARAM)fileLines);	
		break;
	}

	case SDM_ADVANCEPROGRESS: {
		SendMessage(GetDlgItem(handleforwindow, IDC_PROGRESS1), PBM_DELTAPOS, (WPARAM)PROGRESS_STEP, (LPARAM)0);
		progress += PROGRESS_STEP;
		double progressPercent = (progress / fileLines * 100.0);
		int progressPercentInt = (int)progressPercent + 1;
		SetWindowTextA(handleforwindow, ("Progress " + to_string(progressPercentInt) + "%").c_str());
		break;
	}
	
	case SDM_SWITCHTOPHRASE: {
		SendMessage(GetDlgItem(handleforwindow, IDC_PROGRESS1), PBM_DELTAPOS, (WPARAM)(fileLines * -1), (LPARAM)0);
		progress = 0;
		SetWindowTextA(GetDlgItem(handleforwindow, IDC_STATIC1), "Searching for your phrase(s)...");
		SetWindowTextA(handleforwindow, "Progress 0%");
		break;
	}


	case SDM_SET100: {
		SendMessage(GetDlgItem(handleforwindow, IDC_PROGRESS1), PBM_DELTAPOS, (WPARAM)fileLines, (LPARAM)0);
		SetWindowTextA(handleforwindow, "Progress 100%");
		break;
	}
	case SDM_ANALYSISFINSIHED:{
	
	}
	
	case WM_CLOSE: {
		EndDialog(handleforwindow, wParam);
		return TRUE;
	}

	}

	return FALSE;
}




BOOL CALLBACK VersionInfoProc(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG: {
		applyStyles(handleforwindow);
		SetWindowPos(handleforwindow, HWND_TOP, 200, 200, 0, 0, SWP_NOSIZE);
		RECT rect;
		GetWindowRect(handleforwindow, &rect);
		std::string versionInfo = getVersionInfo();
		versionWindowSize = getLines(versionInfo) * 20;
		SetScrollRange(handleforwindow, SB_VERT, 0, versionWindowSize - (rect.bottom - rect.top), true);
		HWND versionInfoStatic = CreateWindow(L"Static", NULL,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			30, 30, (rect.right - rect.left) - 60, versionWindowSize,
			handleforwindow,
			(HMENU)0, hInst1, NULL);
		applyStyles(versionInfoStatic);
		SetWindowTextA(
			versionInfoStatic,
			versionInfo.c_str()
		);
		prevVersionScrollPosVert = 0;
		break;
	}
	case WM_VSCROLL: {
		int BarPosition = NULL;
		switch (LOWORD(wParam)) {
		case SB_THUMBTRACK: {
			BarPosition = HIWORD(wParam);
			if (BarPosition < 0 || BarPosition == NULL) {
				BarPosition = 0;
			}
			break;

		}
		case SB_LINEUP: {
			if (prevVersionScrollPosVert - 20 > 0) {
				BarPosition = prevVersionScrollPosVert - 20;
			}
			else {
				BarPosition = 0;
			}
			break;
		}

		case SB_LINEDOWN: {
			if (prevVersionScrollPosVert + 20 < versionWindowSize) {
				BarPosition = prevVersionScrollPosVert + 20;
			}
			else {
				BarPosition = versionWindowSize;
			}
			break;
		}
		case SB_PAGEUP: {
			if (prevVersionScrollPosVert - 100 > 0) {
				BarPosition = prevVersionScrollPosVert - 100;
			}
			else {
				BarPosition = 0;
			}
			break;
		}

		case SB_PAGEDOWN: {
			if (prevVersionScrollPosVert + 100 < versionWindowSize) {
				BarPosition = prevVersionScrollPosVert + 100;
			}
			else {
				BarPosition = versionWindowSize;
			}
			break;
		}
		default: {
			return FALSE;
		}
		}
		SetScrollPos(
			handleforwindow,
			SB_VERT,
			BarPosition,
			true
		);
		int scrollAmount = BarPosition - prevVersionScrollPosVert;
		scrollAmount *= -1;
		ScrollWindowEx(handleforwindow, 0, scrollAmount, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
		prevVersionScrollPosVert = BarPosition;
		break;
	}




	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDOK: {

		}
		}
	}

	

	case WM_CLOSE: {
		EndDialog(handleforwindow, wParam);
		return TRUE;
	}

	}

	return FALSE;
}
