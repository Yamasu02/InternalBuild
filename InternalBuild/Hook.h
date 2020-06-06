#pragma once
#include <vector>
#include <Windows.h>


void Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5) return false;

	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	uintptr_t relativeAddress = dst - src - 5;

	*src = 0xE9;

	*(uintptr_t*)(src + 1) = relativeAddress;

	VirtualProtect(src, len, curProtection, &curProtection);
	return true;
}

BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5) return 0;

	//Create Gateway
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//write the stolen bytes to the gateway
	memcpy_s(gateway, len, src, len);

	//Get the gateway to destination address
	uintptr_t gatewayRelativeAddr = src - gateway - 5;

	// add the jmp opcode to the end of the gateway
	*(gateway + len) = 0xE9;

	//Write the address of the gateway to the jmp
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelativeAddr;

	//Perform the detour
	Detour32(src, dst, len);

	return gateway;
}

struct Hook
{
	bool bStatus{ false };

	BYTE* src{ nullptr };
	BYTE* dst{ nullptr };
	BYTE* PtrToGatewayFnPtr{ nullptr }; //a ptr to the ptr, make sure you do it right
	int len{ 0 };

	BYTE originalBytes[10]{ 0 };

	Hook(BYTE* src, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len)
	{
		this->src = src;
		this->dst = dst;
		this->len = len;
		this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
	}
	Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len)
	{
		HMODULE hMod = GetModuleHandleA(modName);

		this->src = (BYTE*)GetProcAddress(hMod, exportName);
		this->dst = dst;
		this->len = len;
		this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
	}
	void Enable()
	{
		memcpy(originalBytes, src, len);
		*(uintptr_t*)PtrToGatewayFnPtr = (uintptr_t)TrampHook32(src, dst, len);
		bStatus = true;
	}

	void Disable()
	{
		Patch((BYTE*)src, originalBytes, len);
		bStatus = false;
	}

	void Toggle()
	{
		if (!bStatus) Enable();
		else Disable();
	}

};
