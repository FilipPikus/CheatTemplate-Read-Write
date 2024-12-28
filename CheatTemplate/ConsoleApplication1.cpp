#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>


DWORD GetProcessIdByName(const std::wstring& processName) {
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Fehler beim Erstellen des Snapshots.\n";
        return 0;
    }

    PROCESSENTRY32W processEntry = { 0 };
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshot, &processEntry)) {
        do {
            if (processName == std::wstring(processEntry.szExeFile)) {
                processId = processEntry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return processId;
}

int main() {
    const std::wstring targetProcessName = L"test.exe"; // Dein Prozess
    DWORD processId = GetProcessIdByName(targetProcessName);

    if (processId == 0) {
        std::wcerr << L"Prozess \"" << targetProcessName << L"\" nicht gefunden.\n";
        return 1;
    }

    std::wcout << L"Prozess gefunden! Prozess-ID: " << processId << L"\n";

    
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) {
        std::wcerr << L"Fehler beim Öffnen des Prozesses.\n";
        return 1;
    }


    uintptr_t targetAddress = 0x1BEF88FC3C0; //Deine Adresse angeben und herrausfinden(CheatEngine oder so)
    int newValue = 20; //dein neuer wert


    if (!WriteProcessMemory(hProcess, (LPVOID)targetAddress, &newValue, sizeof(newValue), nullptr)) {
        std::wcerr << L"Fehler beim Schreiben in den Speicher des Prozesses.\n";
        CloseHandle(hProcess);
        return 1;
    }

    std::wcout << L"Zahl erfolgreich auf " << newValue << L" gesetzt!\n";

    
    CloseHandle(hProcess);
    return 0;
}
