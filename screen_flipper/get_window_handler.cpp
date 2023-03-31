#include "get_window_handler.h"

HWND hFoundWindow = NULL;

using namespace std;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    DWORD dwID;
    GetWindowThreadProcessId(hwnd, &dwID);

    if (dwID == (DWORD)lParam)
    {
        // 找到了窗口句柄，将其保存在全局变量中
        hFoundWindow = hwnd;
        return FALSE;
    }
    return TRUE;
}

BOOL CALLBACK EnumWindowsProc2(HWND hwnd, LPARAM lParam)
{
    DWORD dwID;
    GetWindowThreadProcessId(hwnd, &dwID);

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwID);
    if (hProcess == NULL)
    {
        _tprintf(_T("Could not open process\n"));
        return 1;
    }

    TCHAR filename[MAX_PATH];
    GetModuleFileNameEx(hProcess, NULL, filename, MAX_PATH);

    //_tprintf(_T("Process name: %s\n"), filename);

    CloseHandle(hProcess);

    wstring currentFileName(filename);

    if (currentFileName.find((LPCWSTR)lParam) != std::string::npos)
    {
        // 找到了窗口句柄，将其保存在全局变量中
        hFoundWindow = hwnd;
        return FALSE;
    }

    return TRUE;
}

HWND FindWindowByProcessId(DWORD dwProcessId)
{
    hFoundWindow = NULL;
    EnumWindows(EnumWindowsProc, (LPARAM)dwProcessId);
    return hFoundWindow;
}

HWND FindWindowByProcessName(LPCWSTR processName)
{
    hFoundWindow = NULL;
    EnumWindows(EnumWindowsProc2, (LPARAM)processName);
    return hFoundWindow;
}