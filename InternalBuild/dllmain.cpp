#include <Windows.h> 
#include <iostream>
#include "UniversalFuncsh.h"
#include "minhook/minhook.h"
#include "minhook/buffer.h"
#include "minhook/trampoline.h"
#include "classes/cmd.h"
#include "csgotrace.h"
#include "DirectXh.h"
#include "drawing.h"
#include "Hook.h"


using namespace std;







void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) 
{
		if (!pDevice)
			pDevice = o_pDevice;
		//SnapLines();
		//Draw2DLoop(color2, color2, 3);
		oEndScene(pDevice);
}


bool __stdcall hk_createmove(float sampletime, UserCmd* cmd)
{
	if (GetVisible() && IsScoping() == true)
	{
		cmd->buttons |= ~cmd->IN_ATTACK2;
	}
	
	if (GetVisible())
	{
		GetVisible();
	}

	if (GetPlayerAddress(GetIndex())) 
	{
		if (cmd->buttons & cmd->IN_JUMP && !(*(int*)(GetPlayerAddress(GetIndex()) + fFlagsOffset) & (1 << 0)))
		{
			cmd->buttons &= ~cmd->IN_JUMP;
		}	
	}
	return false;
}




int main(HMODULE hModule)
{

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	MH_Initialize();
	MH_CreateHook((void*)get_virtual(clientmode, 24), &hk_createmove, (void**)&o_createmove);
	MH_EnableHook(MH_ALL_HOOKS);
	

	

	/*
	if (GetD3D9Device(d3d9Device , sizeof(d3d9Device)))
	{
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7); 
		oEndScene = (tEndScene)TrampHook32((BYTE*)d3d9Device[42], (BYTE*)hkEndScene, 7);
	}
	*/


	while (!GetAsyncKeyState(VK_END))
	{		
		GlowEsp(0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f);
		if (GetAsyncKeyState(VK_RBUTTON))
		{
			AimAt(&GetBonePosEnemy(GetClosestEnemy(), 8));
		}

		Sleep(1);
	}

	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
	
	//Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7); 
	if (f)
	{
		fclose(f);
	}
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) 
	{
		DisableThreadLibraryCalls(hModule);
		CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL));
	}
	return TRUE;
}
