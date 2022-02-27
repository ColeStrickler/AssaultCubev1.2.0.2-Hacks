#pragma once
#include <vector>
#include <windows.h>
#include <tlhelp32.h>


DWORD GetProcId(const wchar_t* procname);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int>);