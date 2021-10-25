#pragma once

#include <Windows.h>
#include <string>
#include <fstream>
#include "NewDefinitions.h"
#include <ctime>
#include "Test.h"

#pragma comment(lib, "urlmon.lib")

#define FILEDOWNLOADFAIL 0
#define NOUPDATES 1
#define NOCHECK 1
#define UPDATES 2

using namespace std;
int checkForUpdates(std::string currentProgramVersion, std::string currentUpdaterVersion){
    std::string line;
    std::string timeChecked;
    ifstream lastUpdateFile(LASTUPDATECHECKFILE);
    if (lastUpdateFile.is_open())
    {
        getline(lastUpdateFile, line);
        timeChecked = line;
        lastUpdateFile.close();
    }
    else {
        timeChecked = "0";
    }

    std::time_t timeNow = std::time(nullptr);
    if (timeNow < stoi(timeChecked) + updateCheckPeriod) {
       return NOCHECK;
    }
    remove(VERSIONS);
    std::string dwnld_URL = "https://whatsappnalysis.s3.eu-west-2.amazonaws.com/Release+Notes.txt";
    std::string savepath = VERSIONS;
    HRESULT fileDownload = URLDownloadToFileA(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL);
    if (fileDownload != S_OK) {
        return FILEDOWNLOADFAIL;
    }
    remove(LASTUPDATECHECKFILE);
    ofstream myfile(LASTUPDATECHECKFILE);
    if (myfile.is_open())
    {
        myfile << std::time(nullptr);
        myfile.close();
    }
    std::map<int, std::string> versions = getReleaseVersions();
    if (versions[VERSIONRETRIEVEERROR] == NOERRORHIT) {
        std::string versionNum = versions[PROGRAMVERSION];
        std::string updaterVersionNum = versions[UPDATERVERSION];
        if (isGreaterVersion(currentUpdaterVersion, updaterVersionNum)) {
            if (DeleteFileA(UPDATER) == 0) {
                switch (GetLastError()) {
                case ERROR_ACCESS_DENIED: {
                    MessageBoxA(handleforwindow1, "Please click yes on the next pop up", "Automatic Removal Unsuccessful", MB_OK);
                    SHFILEOPSTRUCTA fileOpStruct;
                    fileOpStruct.hwnd = NULL;
                    fileOpStruct.wFunc = FO_DELETE;
                    fileOpStruct.pFrom = UPDATER;
                    fileOpStruct.pTo = NULL;
                    fileOpStruct.fFlags = FOF_SILENT;
                    if (SHFileOperationA(&fileOpStruct) != 0) {
                        MessageBoxA(handleforwindow1, "Please contact support", "Updater Removal Failed", MB_OK);
                        return FILEDOWNLOADFAIL;
                    }
                    break;
                }
                case ERROR_FILE_NOT_FOUND: {
                    break;
                }
                }
                
            }
            dwnld_URL = "https://whatsappnalysis.s3.eu-west-2.amazonaws.com/AutoUpdater.exe";
            savepath = UPDATER;
            fileDownload = URLDownloadToFileA(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL);
            if (fileDownload != S_OK) {
                return FILEDOWNLOADFAIL;
            }
        }
        if (isGreaterVersion(currentProgramVersion, versionNum)) {
            if (not fileExists(UPDATER)) {
                dwnld_URL = "https://whatsappnalysis.s3.eu-west-2.amazonaws.com/AutoUpdater.exe";
                savepath = UPDATER;
                fileDownload = URLDownloadToFileA(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL);
                if (fileDownload != S_OK) {
                    return FILEDOWNLOADFAIL;
                }
            }
            return UPDATES;
        }
        return NOUPDATES;
    }
    else {
        return FILEDOWNLOADFAIL;
    }
}


bool installUpdates(bool useDefaultUpdatePeriod = true) {
    int prevUpdatePeriod;
    if (not useDefaultUpdatePeriod) {
        prevUpdatePeriod = updateCheckPeriod;
        updateCheckPeriod = 0;
    }
    int updateRequired = checkForUpdates(CURRENTVERSION, CURRENTUPDATERVERSION);
    if (not useDefaultUpdatePeriod) {
        updateCheckPeriod = prevUpdatePeriod;
    }
    switch (updateRequired) {
    case FILEDOWNLOADFAIL: {
        MessageBoxA(NULL,
            "Downloading failed, updates may be available",
            "Download failed",
            MB_ICONWARNING | MB_OK);
        return false;
    }
    case UPDATES: {
        if (MessageBoxA(NULL,
            "A program update is available, do you wish to update now?",
            "Update Available",
            MB_ICONINFORMATION | MB_YESNO) == IDYES) {
            // additional information
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            // set the size of the structures
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            // start the program up
            CreateProcess(TEXT(UPDATER),   // the path
                NULL,        // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                0,              // No creation flags
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory 
                &si,            // Pointer to STARTUPINFO structure
                &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
            );
            // Close process and thread handles. 
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return true;
        }
        else {
            return false;
        }
    }
    case NOUPDATES: {
        return false;
    }
    }
    return false;
}