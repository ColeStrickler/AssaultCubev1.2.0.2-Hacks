// AssaultCubeExternal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{
    // Get process ID
    DWORD procId = GetProcId(L"ac_client.exe");

    // Get Module Base Address
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    // Get handle to process
    HANDLE hProcess = 0;
    // set desired access, bool value to set inherited access rights, process id
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    // resolve base address of the pointer chain
    // add offset found via cheat engine
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10F4F4;
    std::cout << "DynamicPtrBaseAddr: 0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    //resolve pointer chain//
    //vector = dynamic container
    // this contains offsets to the ammo value
    std::vector<unsigned int> ammoOffsets = {0x374, 0x14, 0x0};
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);
    std::cout << "ammoAddr: 0x" << std::hex << ammoAddr << std::endl;
    // read value
    int ammoValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "CurrentAmmo: " << std::dec << ammoValue << std::endl;


    // write new value

    int newAmmo = 1337;
    // src and dst are switches in this API Call
    // writes value from newAmmo into the dynamic AmmoAddr
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    // read new value
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "NewAmmo: " << std::dec << ammoValue << std::endl;

    // require button press to close
    getchar();


    return 0;
}

