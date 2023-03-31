#pragma once
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <Psapi.h>
#include <string.h>

HWND FindWindowByProcessName(LPCWSTR processName);