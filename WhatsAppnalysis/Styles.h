#pragma once
#include <windows.h>
#include <CommCtrl.h>
#include "Preferences.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////WINDOW STYLES/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Sizes
#define SUBWINDOWLENGTH 2000
#define SUBWINDOWHEIGHT 1000
#define NUMWINDOWS 5
#define WINDOWBORDER 20
#define WINDOWHEIGHT SUBWINDOWHEIGHT
#define WINDOWLENGTH SUBWINDOWLENGTH
#define WINDOWXPOS 0
#define WINDOWYPOS 50

int BUTTON_STYLE;
HBRUSH BACKGROUND_COLOUR_BRUSH;
COLORREF BACKGROUND_COLOUR;
COLORREF TEXT_COLOUR;

void setStyles(Preferences preferences) {
	if (preferences.darkMode) {
		BUTTON_STYLE = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP;
		BACKGROUND_COLOUR = RGB(85, 85, 85);
		BACKGROUND_COLOUR_BRUSH = CreateSolidBrush(BACKGROUND_COLOUR);
		TEXT_COLOUR = RGB(255, 255, 255);
	}
	else {
		BACKGROUND_COLOUR = RGB(240, 240, 240);
		BACKGROUND_COLOUR_BRUSH = CreateSolidBrush(BACKGROUND_COLOUR);
		BUTTON_STYLE = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
		TEXT_COLOUR = RGB(0, 0, 0);
	}
}




//TitleBar
#define TITLEBAR_COLOUR BACKGROUND_COLOUR
//Menu
#define MENU_COLOUR BACKGROUND_COLOUR
#define MENUTEXT_COLOUR TEXT_COLOUR

//Buttons

//Unhovered
namespace unhovered {
	#define BUTTONOUTLINE_COLOURINNER RGB (161,161,161)
	#define BUTTONOUTLINE_COLOUROUTER RGB(201,201,201)
	#define BUTTONTEXT_COLOUR RGB(220,220,220)
	#define BUTTON_COLOUR RGB(50,50,50)
	HBRUSH BUTTONOUTLINE_COLOUR_BRUSHOUTER = CreateSolidBrush(BUTTONOUTLINE_COLOURINNER);
	HBRUSH BUTTONOUTLINE_COLOUR_BRUSHINNER = CreateSolidBrush(BUTTONOUTLINE_COLOUROUTER);
	HBRUSH BUTTON_COLOUR_BRUSH = CreateSolidBrush(BUTTON_COLOUR);
}

//Hovered
namespace hovered {
	#define BUTTONOUTLINE_COLOUR1 RGB (161,161,161)
	#define BUTTONOUTLINE_COLOUR2 RGB(201,201,201)
	#define BUTTONTEXT_COLOUR RGB(220,220,220)
	#define BUTTON_COLOUR RGB(255,255,255)
	HBRUSH BUTTONOUTLINE_COLOUR_BRUSHOUTER = CreateSolidBrush(BUTTONOUTLINE_COLOUR1);
	HBRUSH BUTTONOUTLINE_COLOUR_BRUSHINNER = CreateSolidBrush(BUTTONOUTLINE_COLOUR2);
	HBRUSH BUTTON_COLOUR_BRUSH = CreateSolidBrush(BUTTON_COLOUR);
}



//Font
HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
	CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Calibri"));


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////STYLE SETTING FUNCTIONS/////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Applies the font to a window
void applyFont(HWND window) {
	SendMessage(window, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void applyBackground(HWND window) {
}
void applyStyles(HWND window) {
	applyFont(window);
	applyBackground(window);
}

bool rectangleShrink(int pixels, RECT &rect) {
	rect.top = rect.top + pixels;
	rect.left = rect.left + pixels;
	rect.bottom = rect.bottom - pixels;
	rect.right = rect.right - pixels;
	return true;
}

bool drawButton(LPARAM lParam) {
	LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	if (PtInRect(&pDIS->rcItem, cursorPos)) {
		using namespace hovered;
		SetTextColor(pDIS->hDC, BUTTONTEXT_COLOUR);
		SetBkMode(pDIS->hDC, TRANSPARENT);
		WCHAR staticText[99];
		int len = (int)SendMessage(pDIS->hwndItem, WM_GETTEXT,
			ARRAYSIZE(staticText), (LPARAM)staticText);
		RECT innerOutline = pDIS->rcItem;
		rectangleShrink(2, innerOutline);
		FillRect(pDIS->hDC, &innerOutline, BUTTON_COLOUR_BRUSH);
		SetTextAlign(pDIS->hDC, TA_CENTER | TA_BOTTOM);
		TextOut(pDIS->hDC, (innerOutline.left + innerOutline.right) / 2, innerOutline.bottom, staticText, len);
		FrameRect(pDIS->hDC, &pDIS->rcItem, BUTTONOUTLINE_COLOUR_BRUSHOUTER);
		FrameRect(pDIS->hDC, &innerOutline, BUTTONOUTLINE_COLOUR_BRUSHINNER);
		SetTextAlign(pDIS->hDC, TA_LEFT);
	}
	else {
		using namespace unhovered;
		SetTextColor(pDIS->hDC, BUTTONTEXT_COLOUR);
		SetBkMode(pDIS->hDC, TRANSPARENT);
		WCHAR staticText[99];
		int len = (int)SendMessage(pDIS->hwndItem, WM_GETTEXT,
			ARRAYSIZE(staticText), (LPARAM)staticText);
		RECT innerOutline = pDIS->rcItem;
		rectangleShrink(2, innerOutline);
		FillRect(pDIS->hDC, &innerOutline, BUTTON_COLOUR_BRUSH);
		SetTextAlign(pDIS->hDC, TA_CENTER | TA_BOTTOM);
		TextOut(pDIS->hDC, (innerOutline.left + innerOutline.right) / 2, innerOutline.bottom, staticText, len);
		FrameRect(pDIS->hDC, &pDIS->rcItem, BUTTONOUTLINE_COLOUR_BRUSHOUTER);
		FrameRect(pDIS->hDC, &innerOutline, BUTTONOUTLINE_COLOUR_BRUSHINNER);
		SetTextAlign(pDIS->hDC, TA_LEFT);
	}
	return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////TAB MENU//////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BOXNAMESSTART 1
void setSelectedTab(HWND hwndTab, std::vector<HWND> handlesfordatawindows, int tab) {
	TabCtrl_SetCurSel(hwndTab, tab);
	for (int i = 0; i < handlesfordatawindows.size(); i++) {
		if (i == tab) {
			ShowWindow(handlesfordatawindows[i], SW_SHOW);
		}
		else {
			ShowWindow(handlesfordatawindows[i], SW_HIDE);
		}
	}
	InvalidateRect(hwndTab, NULL, false);
}



HWND DoCreateTabControl(HWND hwndParent, HINSTANCE hInst, int numItems)
{
	RECT rcClient;
	HWND hwndTab;
	TCITEM tie;
	int i;
	TCHAR achTemp[256];  // Temporary buffer for strings.

	// Initialize common controls.
	//icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//icex.dwICC = ICC_TAB_CLASSES;
	//InitCommonControlsEx(&icex);

	// Get the dimensions of the parent window's client area, and 
	// create a tab control child window of that size. Note that g_hInst
	// is the global instance handle.
	GetClientRect(hwndParent, &rcClient);
	hwndTab = CreateWindow(WC_TABCONTROL, L"",
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
		0, 0, WINDOWLENGTH, WINDOWHEIGHT - 100,
		hwndParent, NULL, hInst, NULL);
	applyStyles(hwndTab);
	if (hwndTab == NULL)
	{
		return NULL;
	}

	// Add tabs for each day of the week. 
	tie.mask = TCIF_TEXT | TCIF_IMAGE;
	tie.iImage = -1;
	tie.pszText = achTemp;

	for (i = 0; i < numItems; i++)
	{
		// Load the day string from the string resources. Note that
		// g_hInst is the global instance handle.
		LoadString(hInst, BOXNAMESSTART+i,
			achTemp, sizeof(achTemp) / sizeof(achTemp[0]));
		if (TabCtrl_InsertItem(hwndTab, i, &tie) == -1)
		{
			DestroyWindow(hwndTab);
			return NULL;
		}
	}
	return hwndTab;
}





BOOL OnNotify(HWND hwndTab, LPARAM lParam, std::vector<HWND> handlesfordatawindows, int* itemSelectedBefore)
{
	TCHAR achTemp[256]; // temporary buffer for strings

	switch (((LPNMHDR)lParam)->code)
	{
	case TCN_SELCHANGING:
	{
		// Return FALSE to allow the selection to change.
		return FALSE;
	}

	case TCN_SELCHANGE:
	{
		int tab = TabCtrl_GetCurSel(hwndTab);
		*itemSelectedBefore = tab;
		setSelectedTab(hwndTab, handlesfordatawindows, tab);
	}
	}
	return TRUE;
}
