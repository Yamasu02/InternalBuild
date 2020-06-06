#include <Windows.h> 
#include <iostream>
#include "MainFuncs.h"
#include "minhook/minhook.h"
#include "minhook/buffer.h"
#include "minhook/trampoline.h"
#include "classes/cmd.h"
#include "csgotrace.h"
#include "imgui shit/imgui.h"
#include "imgui shit/imgui_impl_dx9.h"
#include "imgui shit/imgui_impl_win32.h"
#include "DirectXh.h"
#include "drawing.h"
#include "Hook.h"
#include <mutex>


using namespace std;

bool b1 = true;
bool spinlock = false;
const char* windowName = "Counter-Strike: Global Offensive";

//keno

mutex m;


void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice)  //hooking func
{
		if (!pDevice)
			pDevice = o_pDevice;
		SnapLines();
		Draw2DLoop(color2, color2, 3);
		oEndScene(pDevice);
}


bool __stdcall hk_createmove(float sampletime, UserCmd* cmd)
{
	
	m.lock();
	if (GetVisible() && IsScoping() == true)
	{
			cmd->buttons |= ~cmd->IN_ATTACK2;
	}
	glowEsp(0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f);
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
	m.unlock();
	return false;
	
}




int main(HMODULE hModule)
{

	//AllocConsole();
	//FILE* f;
	//freopen_s(&f, "CONOUT$", "w", stdout);
	MH_Initialize();
	MH_CreateHook((void*)get_virtual(clientmode, 24), &hk_createmove, (void**)&o_createmove);
	MH_EnableHook(MH_ALL_HOOKS);
	

	//keno


	//hook
	if (GetD3D9Device(d3d9Device , sizeof(d3d9Device)))
	{
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7); 
		oEndScene = (tEndScene)TrampHook32((BYTE*)d3d9Device[42], (BYTE*)hkEndScene, 7);
	}

	while (!GetAsyncKeyState(VK_END))
	{		

		if (GetAsyncKeyState(VK_INSERT))
		{
			Jitter(0, 120, 3);
		}

		
		if (GetAsyncKeyState(VK_RBUTTON))
		{
			spinlock = false;
			int xt = GetClosestEnemy();
			Vector3 uh = GetBonePosEnemy(xt, 8);
			AimAt(&uh);
			Sleep(1);
			spinlock = true;
		}
		
		if (GetAsyncKeyState(VK_LMENU))
		{
			int xt = GetClosestEnemy();
			Vector3 uh = GetBonePosEnemy(xt, 8);
			aa(&uh);
			Sleep(1);
		}
		
		Sleep(1);
	}
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
	
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7); 
	//if (f)
	//{
		//fclose(f);
	//}
	//FreeConsole();
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
