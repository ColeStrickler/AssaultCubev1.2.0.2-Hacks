// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "mem.h"
#include "proc.h"
#include "esp.h"






DWORD WINAPI HackThread(HMODULE hModule) {
	// create console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "|-----------------------------------------------|\n";
	std::cout << "|------ASSAULTCUBEv1.2.0.2 INTERNAL TRAINER-----|\n";
	std::cout << "|-----------------------------------------------|\n";
	std::cout << "|-------------AUTHOR: COLE STRICKLER------------|\n";
	std::cout << "|-----------------------------------------------|\n";
	std::cout << "|-----------------------------------------------|\n";
	std::cout << "|------[NUMPAD1] TOGGLE INVINCIBILITY-----------|\n";
	std::cout << "|------[NUMPAD2] TOGGLE l337 ARMOR--------------|\n";
	std::cout << "|------[NUMPAD3] TOGGLE INFINITE AMMO-----------|\n";
	std::cout << "|------[NUMPAD4] TOGGLE NO RECOIL---------------|\n";
	std::cout << "|------[NUMPAD5] TOGGLE INFINITE GRENADES-------|\n";
	std::cout << "|------[NUMPAD6] TOGGLE SPEED HACKS-------------|\n";
	std::cout << "|------[NUMPAD7] TOGGLE ESP---------------------|\n";
	std::cout << "|-----------------------------------------------|\n";

	// get module base
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	bool bHealth = false, bAmmo = false, bRecoil = false, bArmor = false, bGrenades = false, bSpeed = false, bESP = false;


	// l337 hax loop
	while (true) {
		// key input
		if (GetAsyncKeyState(VK_END) & 1) {
			break;
		}
		if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
			bHealth = !bHealth;
		}
		if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
			bAmmo = !bAmmo;
		}
		if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
			bRecoil = !bRecoil;
			if (bRecoil) {
				// nop recoil function
				mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
			}
			// return recoil function to normal
			else {
				mem::Patch((BYTE*)(moduleBase + 0x63786),(BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);


			}
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
			bArmor = !bArmor;
		}
		if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
			bGrenades = !bGrenades;
		}
		if (GetAsyncKeyState(VK_NUMPAD6) & 1) {
			bSpeed = !bSpeed;

			if (bSpeed) {
				mem::Patch((BYTE*)(moduleBase + 0x5bF00), (BYTE*)"\xb8\x04\x00\x00\x00", 5);
				mem::Patch((BYTE*)(moduleBase + 0x5bF60), (BYTE*)"\xb8\xfc\xff\xff\xff", 5);
				mem::Patch((BYTE*)(moduleBase + 0x5bea0), (BYTE*)"\xb8\x04\x00\x00\x00", 5);
				mem::Patch((BYTE*)(moduleBase + 0x5be40), (BYTE*)"\xb8\xfc\xff\xff\xff", 5);

			}
			else {
				mem::Patch((BYTE*)(moduleBase + 0x5bF00), (BYTE*)"\xb8\x02\x00\x00\x00", 5);
				mem::Patch((BYTE*)(moduleBase + 0x5bF60), (BYTE*)"\xb8\xfe\xff\xff\xff", 5);
				mem::Patch((BYTE*)(moduleBase + 0x5bea0), (BYTE*)"\xb8\x02\x00\x00\x00", 5);
				mem::Patch((BYTE*)(moduleBase + 0x5be40), (BYTE*)"\xb8\xfe\xff\xff\xff", 5);
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD7) & 1) {
			bESP = !bESP;
		}
		
		
	
		// continuous write
		uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10f4f4);
		// only work when game is in session
		if (localPlayerPtr) {
			if (bHealth) {
				*(int*)(*localPlayerPtr + 0xf8) = 1337;
			}
			if (bAmmo) {
				uintptr_t ammoAddr = mem::FindDMAAddy(moduleBase + 0x10f4f4, { 0x374, 0x14, 0x0 });
				int* ammo = (int*)ammoAddr;
				*ammo = 1337;

				// or do this
				// *(int*)mem::FindDMAAddy(moduleBase + 0x10f4f4, { 0x374, 0x14, 0x0 }) = 1337;

				
			}
			if (bArmor) {
				*(int*)(*localPlayerPtr + 0xfc) = 1337;
			}
			if (bGrenades) {
				*(int*)(*localPlayerPtr + 0x158) = 1337;
			}
			if (bESP) {
				esp(bESP);
			}
		}
		Sleep(5);
	}

	
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
	

	// cleanup
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}