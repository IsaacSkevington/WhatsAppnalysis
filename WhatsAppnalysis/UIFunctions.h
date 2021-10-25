#pragma once
///////////////////////////////INCLUDES////////////////////////////////////////////////
#include <windows.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <tchar.h>
#include <shobjidl.h>
#include <atlstr.h>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <typeinfo>
#include "Line.h"
#include "Validation.h"
#include "Array Processing Functions.h"
using namespace std;



////////////////////////////////////////////DAY PROCESSING FUNCTIONS//////////////////////////////

//Finds the most recent of 2 dates
std::string biggerDate(std::string dateA, std::string dateB) {
	std::vector<std::string> datesArray = { dateA, dateB };
	datesArray = dateArraySort(datesArray);
	if (datesArray[0] == dateA) {
		return dateA;
	}
	else {
		return dateB;
	}
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////TOTAL FINDING FUNCTIONS////////////////////////////////////////

//Finds the total messages for a person in a selection of days
int personTotalFind(Msgstreamprocessor messageAnalyser, std::string person, std::vector<std::string> days) {
	int personTotal = 0;
	for (int i = 0; i < days.size(); i++) {
		personTotal += messageAnalyser.totalmessagesdaily[days[i]][person];
	}
	return personTotal;
}







//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////FILE PROCESSING FUNCTIONS//////////////////////////////////////////////


//Gets a chosen filepath 
std::string openFileDialog(std::map<LPCWSTR, LPCWSTR> fileTypes, LPCWSTR defaultType) {
	std::string filepath;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			COMDLG_FILTERSPEC* fileExtensionSelect = new COMDLG_FILTERSPEC[fileTypes.size()];
			std::vector<LPCWSTR> fileTypesExtensions = mapkeysLstring(fileTypes);
			std::vector<LPCWSTR> fileTypesDescriptions = mapvaluesLstring(fileTypes);
			for (int i = 0; i < fileTypesExtensions.size(); i++) {
				fileExtensionSelect[i].pszName = fileTypesExtensions[i];
				fileExtensionSelect[i].pszSpec = fileTypesDescriptions[i];
			}
			// Show the Open dialog box.
			pFileOpen->SetDefaultExtension((defaultType));
			pFileOpen->SetFileTypes(fileTypes.size(), fileExtensionSelect);
			pFileOpen->SetFileTypeIndex(1);

			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						filepath = CW2A(pszFilePath);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return filepath;
}



//Gets a chosen filepath 
std::string saveFileDialog(std::map<LPCWSTR, LPCWSTR> fileTypes, LPCWSTR defaultType) {
	std::string filepath;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileSaveDialog* pFileSave;
		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

		if (SUCCEEDED(hr))
		{
			COMDLG_FILTERSPEC* fileExtensionSelect = new COMDLG_FILTERSPEC[fileTypes.size()];
			std::vector<LPCWSTR> fileTypesExtensions = mapkeysLstring(fileTypes);
			std::vector<LPCWSTR> fileTypesDescriptions = mapvaluesLstring(fileTypes);
			for (int i = 0; i < fileTypesExtensions.size(); i++) {
				fileExtensionSelect[i].pszName = fileTypesExtensions[i];
				fileExtensionSelect[i].pszSpec = fileTypesDescriptions[i];
			}
			// Show the Open dialog box.
			pFileSave->SetDefaultExtension((defaultType));
			pFileSave->SetFileTypes(fileTypes.size(), fileExtensionSelect);
			pFileSave->SetFileTypeIndex(1);

			hr = pFileSave->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileSave->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						filepath = CW2A(pszFilePath);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}
	return filepath;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////SCROLL BAR FUNCTIONS////////////////////////////////////////////////////////////

//Creates a scroll bar
HWND SBCreate(HWND hwndParent, int width, HINSTANCE hInstance, long long int ScrollID)

{
	RECT rect;

	// Get the dimensions of the parent window's client area;
	if (!GetClientRect(hwndParent, &rect))
		return NULL;

	// Create the scroll bar.
	return (CreateWindowEx(
		0,                      // no extended styles 
		L"SCROLLBAR",           // scroll bar control class 
		(PTSTR)NULL,           // no window text 
		WS_CHILD | WS_VISIBLE | WS_EX_TRANSPARENT | WS_EX_LAYERED   // window styles  
		| SBS_VERT,         // horizontal scroll bar style 
		rect.left + (rect.right - width),              // horizontal position 
		rect.top, // vertical position 
		width,             // width of the scroll bar 
		rect.bottom,               // height of the scroll bar
		hwndParent,             // handle to main window 
		(HMENU)ScrollID,           // no menu 
		hInstance,                // instance owning this window 
		(PVOID)NULL            // pointer not needed 
	));
}

//Moves a scroll bar thumb
void moveScroll(HWND hWndParent, long long int scrollID) {

	RECT rect;

	// Get the dimensions of the parent window's client area;
	if (!GetClientRect(hWndParent, &rect))

		SetWindowPos(GetDlgItem(hWndParent, (int)scrollID), HWND_BOTTOM,
			rect.left + (rect.right - 20),       // horizontal position 
			rect.top,							// vertical position 
			20,							   // width of the scroll bar 
			rect.bottom,					 // height of the scroll bar
			SWP_SHOWWINDOW);

}


//////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
