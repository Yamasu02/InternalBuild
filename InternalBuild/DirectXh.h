#pragma once
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

static HWND window;

int windowWidth , windowHeight ;

extern LPDIRECT3DDEVICE9 pDevice = nullptr;

void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;



// callback to be called for each toplevel window
BOOL CALLBACK enumWind(HWND handle, LPARAM lp)
{
	DWORD procId;
	GetWindowThreadProcessId(handle, &procId);
	if (GetCurrentProcessId() != procId)
		return TRUE;

	window = handle;
	return FALSE;
}

// get process window
HWND GetProcessWindow()
{
	window = NULL;

	EnumWindows(enumWind, NULL);

	RECT size;
	GetWindowRect(window, &size);
	windowWidth = size.right - size.left;
	windowHeight = size.bottom - size.top;

	windowHeight -= 29;
	windowWidth -= 5;

	return window;
}



Vec2 size2()
{
	Vec2 dims ;
	RECT size;
	HWND hwnd = FindWindowA(NULL, "Counter Strike: Global Offensive");
	BOOL b1 = GetWindowRect(hwnd, &size);
	//if (b1)
	//{
		dims.x = size.right - size.left -29;
		dims.y = size.bottom - size.top - 5;
		return (dims);
	//}
}



// get vTable 
bool GetD3D9Device(void** pTable, size_t size)
{
	if (!pTable)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow(); //FindWindowW(NULL, L"Counter Strike: Global Offensive");

	HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDevCreated != S_OK) 
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK) 
		{
			pD3D->Release();
			return false;
		}
	}

	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return true;
}

