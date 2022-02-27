#pragma once

#include "stdafx.h"
#include <stdio.h>
#include "proc.h"


// process ID in windows api is a dword
// wchar_t because unicode use
DWORD GetProcId(const wchar_t* procname) {
	DWORD procId = 0;
	// snap of processes
	HANDLE hSnap = (CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
	// check if not fail value
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
		// grabs first process in snapshot and stores in procEntry
		if (Process32First(hSnap, &procEntry)) {
			do {
				// compares the file name of the process to the one given
				// wide char string compare, case insensitive
				if (!_wcsicmp(procEntry.szExeFile, procname)) {
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

// uintptr_t is an architecture independent variable
// compile with architecture of the game process

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
	uintptr_t modBaseAddr = 0;
	// get correct architecture
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName)) {
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}

	}
	CloseHandle(hSnap);
	return modBaseAddr;
}


// handle to process, base pointer, and vector of offsets
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++) {
		// (processID, baseaddr, bufferaddress(stores here), # of bytes to read, # of bytes copied)
		// reads value of addr and stores it in the same place
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}

	return addr;
}

