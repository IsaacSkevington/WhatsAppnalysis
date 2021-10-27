#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <Windows.h>
#pragma comment(lib, "urlmon.lib")

#define LASTUPDATEFILE "Update/LastUpdate.txt\0\0"
#define PROGRAMFILE "WhatsAppnalysis.exe\0\0"

using namespace std;
int main(){
    for (int i = 5; i >= 0; i -= 1) {
        cout << "================================================================" << endl;
        cout << "WHATSAPPNALYSIS UPDATER V0.7.5 | COPYRIGHT ISAAC SKEVINGTON 2020" << endl;
        cout << "================================================================" << endl <<endl;
        cout << "Waiting for program to close " + to_string(i);
        for (int i = 0; i < 5; i++) {
            cout << ".";
            Sleep(200);
        }
        system("CLS");
    }
    SHFILEOPSTRUCTA fileOpStruct;
    fileOpStruct.hwnd = NULL;
    fileOpStruct.wFunc = FO_DELETE;
    fileOpStruct.pFrom = PROGRAMFILE;
    fileOpStruct.pTo = NULL;
    fileOpStruct.fFlags = FOF_SILENT;

    if (DeleteFileA(PROGRAMFILE) == 0) {
        cout << "Automatic Program Removal Unsuccessful" << endl;
        switch (GetLastError()) {
        case ERROR_ACCESS_DENIED: {
            cout << "Access Denied" << endl;
            cout << "Please click yes on the next pop up, this will delete the program manually ready for updating" << endl;
            Sleep(5000);
            if (SHFileOperationA(&fileOpStruct) != 0) {
                cout << "Update Failed, file could not be deleted." << endl;
                cout << "To solve this problem, go to the program directory, delete " << PROGRAMFILE << "and run the AutoUpdater again." << endl;
            }
            else {
                cout << "Deleted successfully, proceeding with update" << endl;
            }
            break;
            //return 0;
        }
        case ERROR_FILE_NOT_FOUND: {
            cout << "File doesn't exist, proceeding with update" << endl;
            break;
        }
        }
    }
    
    cout << "File Removed" << endl;
    cout << "Downloading File" << endl;
    std::string dwnld_URL = "https://whatsappnalysis.s3.eu-west-2.amazonaws.com/WhatsAppnalysis.exe";
    std::string savepath = PROGRAMFILE;
    HRESULT fileDownload = URLDownloadToFileA(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL);
    if (fileDownload != S_OK) {
        cout << "File download failed, restarting the program" << endl;
    }
    else {
        cout << "File Downloaded, starting WhatsAppnalysis" << endl;
    }
    Sleep(3000);
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // start the program up
    CreateProcess(TEXT(PROGRAMFILE),  
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
    remove(LASTUPDATEFILE);
    ofstream myfile(LASTUPDATEFILE);
    if (myfile.is_open())
    {
        myfile << std::time(nullptr);
        myfile.close();
    }
    Sleep(5000);
    return 0;
}