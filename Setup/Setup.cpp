// RunAsAdminMonth : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;


bool TestBox(LPCWSTR dataIn = L"Test") {
    MessageBoxW(NULL,
        dataIn,
        L"Test",
        MB_ICONINFORMATION);
    return true;
}



int main()
{
    cout << "===============================================================" << endl;
    cout << "WHATSAPPNALYSIS SETUP V1.0.1 | COPYRIGHT ISAAC SKEVINGTON 2020" << endl;
    cout << "===============================================================" << endl<<endl;

    LPWSTR pBuf = new wchar_t[1000];
    GetModuleFileName(NULL, pBuf, 1000);
    LPCWSTR filePath = (LPCWSTR)pBuf;
    wstring newFilePath = filePath;
    for (int i = newFilePath.length() - 1; i > 0; i -= 1) {
        if (newFilePath[i] == '\\') {
            newFilePath = newFilePath.substr(0, i);
            i = -1;
        }
    }
    newFilePath += L"\\PermissionChange.bat";
    delete(pBuf);
    std::wstring commandLine = L"/c \"" +  newFilePath +L"\"";
    cout << "In order to finish the setup process to allow for automatic updates\n"
        "we must change the permissions to the folder you installed the application in.\n"
        "Please re-enter the administrator password on the UAC to finish the setup proceedure.\n"
        "NOTE: This will not affect other files on your computer" << endl;
    ShellExecuteW(NULL, L"runas", L"cmd.exe", commandLine.c_str(), NULL, SW_SHOW);
    Sleep(5000);
    cout << "Permissions changed successfully, enjoy the program!" << endl;
    cout << "Setup is cleaning up, then you will need to restart WhatsAppnalysis to implement the changes" << endl;
    Sleep(5000);
    return 0;
}

