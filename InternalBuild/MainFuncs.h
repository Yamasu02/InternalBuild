#pragma once
#include <Windows.h>
#include <iostream>
#include "Structs.h"
#include "Offsets.h"
#include "csgotrace.h"
#include "classes/cmd.h"
#include "DirectXh.h"
#include "drawing.h"
#include <map>





#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define ABS(x) ((x < 0) ? (-x) : (x))
#define TORAD(x) ((x) * 0.01745329252)

using namespace std;


bool xd;
DWORD modulePanorama = (DWORD)GetModuleHandle(L"client_panorama.dll");
DWORD moduleEngine = (DWORD)GetModuleHandle(L"engine.dll");

typedef void(__fastcall* tTraceRay)(void* thisptr, void* not_edx, Ray_t& ray, unsigned int fMask, CTraceFilter* pTraceFilter, CGameTrace* pTrace);
typedef void*(__cdecl* tCreateInterface)(const char* name, int* pReturnCode);
typedef  bool(__thiscall* FuncPtr)(Clientmode*, float, UserCmd*);
FuncPtr o_createmove = nullptr;
ID3DXLine* LineL;
ID3DXFont* FontF;
D3DCOLOR color2 = D3DCOLOR_ARGB(255, 0, 139, 87);
D3DCOLOR color3 = D3DCOLOR_ARGB(255, 255, 0, 0);


void* GetInterface(tCreateInterface fn, const char* name)
{
	return (fn)(name, 0);
}

tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress((HMODULE)moduleEngine, "CreateInterface");
tCreateInterface CreateInterface2 = (tCreateInterface)GetProcAddress((HMODULE)modulePanorama, "CreateInterface");
IEngineTrace* EngineTrace = (IEngineTrace*)GetInterface(CreateInterface, "EngineTraceClient004");
Client* client = (Client*)GetInterface(CreateInterface2, "VClient018");
Clientmode* clientmode = **reinterpret_cast<Clientmode***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);


tTraceRay TraceRay;
CGameTrace trace;
Ray_t ray;
CTraceFilter tracefilter;




unsigned int get_virtual(void* class_, unsigned int index)
{
	return (unsigned int)(*(int**)class_)[index];
}

DWORD GetPlayerAddress(int index)
{
	return *(DWORD*)(modulePanorama + EntityListOffset + 0X10 * index);
}

bool GetDormant(int i)
{
	return  *(bool*)(GetPlayerAddress(i) + DormantOffset);
}

int GetIndex()
{
	return *(int*)((*(DWORD*)(moduleEngine + ClientStateOffset)) + LocalIndexOffset);// +1;
}

int GetPlayerHealth(int index)
{
	return *(int*)(GetPlayerAddress(index) + HealthOffset);
}

int GetPlayerTeam(int index)
{
	return *(int*)(GetPlayerAddress(index) + TeamOffset);
}

bool IsScoping()
{
	return *(bool*)(GetPlayerAddress(GetIndex()) + isScoped);
}

bool GetImmunity(int i)
{
	return *(bool*)(GetPlayerAddress(i) + Immunity);
}

int GetPlayerArmor(int i)
{
	return *(int*)(GetPlayerAddress(i) + ArmorOffset);
}

bool HasHelmet(int i)
{
	return *(bool*)(GetPlayerAddress(i) + HelmetOffset);
}

int ClientGameState()
{
	return *(int*)(*(DWORD*)(moduleEngine + ClientStateOffset) + dwClientState_State);
}

Vector3 GetPlayerOrigin(int index)
{
	return *(Vector3*)(GetPlayerAddress(index) + VecOriginOffset);
}

Vector3 GetPlayerView(int index)
{
	return *(Vector3*)(GetPlayerAddress(index) + ViewOffset);
}

bool CheckEntityAimbot(int i)
{
	if (!GetPlayerAddress(i) || GetImmunity(i) || GetPlayerHealth(i) > 100 || GetPlayerHealth(i) < 0 || i == GetIndex() || GetPlayerTeam(i) == GetPlayerTeam(GetIndex()))
	{
		return false;
	}
}

Vector3 GetBonePosEnemy(int index, int boneid)
{
	if (GetPlayerAddress(index) && ((DWORD*)(GetPlayerAddress(index) + BoneMatrixOffset) != nullptr))
	{
		DWORD addrR = *(DWORD*)(GetPlayerAddress(index) + BoneMatrixOffset);
		float* addr1 = (float*)(addrR + 0x30 * boneid + 0x0C);
		float* addr2 = (float*)(addrR + 0x30 * boneid + 0x1C);
		float* addr3 = (float*)(addrR + 0x30 * boneid + 0x2C);
		Vector3 BoneCoords2 = { *(float*)(addr1),*(float*)(addr2),*(float*)(addr3) };
		return BoneCoords2;
	}
	else
	{
		return { 0,0,0 };
	}
}

ViewMatrix GetViewMatrix()
{
	return *(ViewMatrix*)(modulePanorama + ViewMatrixOffset);
}

bool WorldToScreen(Vector3 pos, Vec2 screen)
{

	static float windowWidth2 = 1920;
	static float windowHeight2 = 1080;
	ViewMatrix matrix = GetViewMatrix();
	float x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	float y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];
	float z = matrix[2][0] * pos.x + matrix[2][1] * pos.y + matrix[2][2] * pos.z + matrix[2][3];
	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	if (w < 0.1f)
		return false;

	x /= w;
	y /= w;
	screen.x = (windowWidth2 / 2 * x) + (x + windowWidth2 / 2);
	screen.y = -(windowHeight2 / 2 * y) + (y + windowHeight2 / 2);
	return true;
}  

Vec2 W2S(Vector3 pos)
{
	Vec2 screen;
	static float windowWidth2 = 1920;
	static float windowHeight2 = 1080;
	ViewMatrix matrix = GetViewMatrix();
	float x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	float y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];
	float z = matrix[2][0] * pos.x + matrix[2][1] * pos.y + matrix[2][2] * pos.z + matrix[2][3];
	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	if (w < 0.1f)
		return { 1920,1080/2 };

	x /= w;
	y /= w;
	screen.x = (windowWidth2 / 2 * x) + (x + windowWidth2 / 2);
	screen.y = -(windowHeight2 / 2 * y) + (y + windowHeight2 / 2);
	return screen;
}

float GetDistance(Vector3* Enemy, Vector3* Local)
{
	Vector3 DeltaVec = Vector3(Enemy->x - Local->x, Enemy->y - Local->y, Enemy->z - Local->z);
	return sqrt(DeltaVec.x * DeltaVec.x + DeltaVec.y * DeltaVec.y + DeltaVec.z * DeltaVec.z);
}

int GetClosestEnemy()
{
	static int localteam = GetPlayerTeam(GetIndex());
	float closestDistance = 999999999;
	int closestDistanceIndex;

	for (int i = 0; i < MAXPLAYER; i++)
	{
		if (CheckEntityAimbot(i) && GetDistance(&GetPlayerOrigin(i), &GetPlayerOrigin(GetIndex())))
		{
			continue;
		}

		float currentDistance = GetDistance(&GetPlayerOrigin(i), &GetPlayerOrigin(GetIndex()));

		if (currentDistance < 0)
		{
			continue;
		}

		if (currentDistance <= closestDistance)
		{
			closestDistance = currentDistance;
			closestDistanceIndex = i;
		}

	}
	return closestDistanceIndex;
}

void AimAt(Vector3* EnemyPositionHead)
{
	int oof = GetIndex();
	DWORD pitchaddr = (DWORD)(moduleEngine + ClientStateOffset);
	DWORD pitchaddr2 = *(DWORD*)pitchaddr;
	Vector3 OriginLocal = GetPlayerOrigin(oof);
	Vector3 ViewOffsetLocal = GetPlayerView(oof);
	Vector3* PosLocal = &(OriginLocal + ViewOffsetLocal);
	Vector3 DeltaVector = { EnemyPositionHead->x - PosLocal->x, EnemyPositionHead->y - PosLocal->y, EnemyPositionHead->z - PosLocal->z };
	float DeltaVectorLength = sqrt(DeltaVector.x * DeltaVector.x + DeltaVector.y * DeltaVector.y + DeltaVector.z * DeltaVector.z);
	float pitch = -asin(DeltaVector.z / DeltaVectorLength) * (180 / PI);
	float yaw = atan2(DeltaVector.y, DeltaVector.x) * (180 / PI);
	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180)
	{
		*(float*)(pitchaddr2 + PitchOffset) = pitch;
		*(float*)(pitchaddr2 + YawOffset) = yaw;
	}
}

void glowEsp(float TRed, float TGreen, float TBlue, float TAlpha, float CTRed, float CTGreen, float CTBlue, float CTAlpha)
{
	DWORD glow_Pointer = *(DWORD*)(modulePanorama + GlowObjectManagerOffset);
	for (int i = 0; i < MAXPLAYER; i++)
	{
		if (!GetPlayerAddress(i))
		{
			continue;
		}
		int glow_currentPlayerGlowIndex = *(int*)(GetPlayerAddress(i) + GlowIndexOffset);
		int health = GetPlayerHealth(i);
		int team = GetPlayerTeam(i);
		if (health > 0 && health < 101)
		{
			switch (team)
			{
			case 2:
			{
				bool b2 = true;
				bool b3 = false;

				*(float*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x4)) = TRed;
				*(float*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x8)) = TGreen;
				*(float*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0xC)) = TBlue;
				*(float*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x10)) = TAlpha;
				*(bool*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x24)) = b2;
				*(bool*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x25)) = b3;
				break;
			}
			case 3:
				bool b2 = true;
				bool b3 = false;

				*(float*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x4)) = CTRed;
				*(float*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x8)) = CTGreen;
				*(float*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0xC)) = CTBlue;
				*(float*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x10)) = CTAlpha;
				*(bool*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x24)) = b2;
				*(bool*)(glow_Pointer + ((glow_currentPlayerGlowIndex * 0x38) + 0x25)) = b3;
				break;
			}
		}
	}
}

void aa(Vector3* EnemyPositionHead)
{
	int oof = GetIndex();
	DWORD pitchaddr = (DWORD)(moduleEngine + ClientStateOffset);
	DWORD pitchaddr2 = *(DWORD*)pitchaddr;
	Vector3 OriginLocal = GetPlayerOrigin(oof);
	Vector3 ViewOffsetLocal = GetPlayerView(oof);
	Vector3* PosLocal = &(OriginLocal + ViewOffsetLocal);
	Vector3 DeltaVector = { EnemyPositionHead->x - PosLocal->x, EnemyPositionHead->y - PosLocal->y, EnemyPositionHead->z - PosLocal->z };
	float DeltaVectorLength = sqrt(DeltaVector.x * DeltaVector.x + DeltaVector.y * DeltaVector.y + DeltaVector.z * DeltaVector.z);
	float pitch = -asin(DeltaVector.z / DeltaVectorLength) * (180 / PI);
	float yaw = atan2(DeltaVector.y, DeltaVector.x) * (180 / PI);
	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180)
	{
		yaw += 179;
		pitch = 89;
		*(float*)(pitchaddr2 + PitchOffset) = pitch;
		*(float*)(pitchaddr2 + YawOffset) = yaw;
	}
}

void Jitter(float degree_pitch,float degrees_yaw,int ms_to_sleep)
{
	DWORD pitchaddr2 = *(DWORD*)(moduleEngine + ClientStateOffset);
	*(float*)(pitchaddr2 + PitchOffset) += degree_pitch;
	*(float*)(pitchaddr2 + YawOffset) += degrees_yaw;
	Sleep(ms_to_sleep);
	*(float*)(pitchaddr2 + PitchOffset) -= degree_pitch;
	*(float*)(pitchaddr2 + YawOffset) -= degrees_yaw;
}

int GetVisible()
{
	int oof = GetIndex();
	for (int i = 0; i < MAXPLAYER; i++)
	{

		if (CheckEntityAimbot(i));
		{
			Vector3 vec1 = GetPlayerOrigin(oof);
			Vector3 vec11 = GetPlayerView(oof);
			Vector3 vec4 = GetBonePosEnemy(i, 8);
			vec3 v1 = { vec1.x + vec11.x, vec1.y + vec11.y ,vec1.z + vec11.z };
			vec3 v3 = { vec4.x,vec4.y,vec4.z };

			ray.Init(v1, v3);
			tracefilter.pSkip = (void*)GetPlayerAddress(oof);
			EngineTrace->TraceRay(ray, MASK_NPCWORLDSTATIC | CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX, &tracefilter, &trace); //mask shot conetnts grate


			if ((void*)GetPlayerAddress(i) == trace.hit_entity)
			{
				AimAt(&vec4);
				return i;
			}

		}
	}
	return 0;
}

void spin()
{
	for (int i = 0; i < MAXPLAYER; i++)
	{
		if (CheckEntityAimbot(i))
		{
			int xd = GetClosestEnemy();
			Vector3 XD = GetBonePosEnemy(i, 8);
			aa(&XD);
		}
	}
}

/*
void idk()
{
	if (GetVisible())
	{
		xd = true;
		GetVisible();
	}

	else
	{
		xd = false;
		//if (GetAsyncKeyState(VK_LMENU))
		//{
			//spin();
		//}
	}

}
*/

struct Colors
{
	D3DCOLOR health = D3DCOLOR_ARGB(255, 46, 139, 87);
	D3DCOLOR armor = D3DCOLOR_ARGB(255, 30, 144, 255);
	D3DCOLOR crosshair = D3DCOLOR_ARGB(255, 255, 255, 255);
	struct Team {
		D3DCOLOR esp = D3DCOLOR_ARGB(255, 0, 255, 0);
		D3DCOLOR snapline = D3DCOLOR_ARGB(255, 0, 255, 0);
		D3DCOLOR velocity = D3DCOLOR_ARGB(255, 0, 0, 255);
		D3DCOLOR headline = D3DCOLOR_ARGB(255, 0, 0, 255);;
	}team;
	struct Enemy {
		D3DCOLOR esp = D3DCOLOR_ARGB(255, 255, 0, 0);
		D3DCOLOR snapline = D3DCOLOR_ARGB(255, 255, 0, 0);
		D3DCOLOR velocity = D3DCOLOR_ARGB(255, 0, 255, 255);
		D3DCOLOR headline = D3DCOLOR_ARGB(255, 0, 255, 255);;
	}enemy;
}color;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT rect = { x,y,x + w,y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
{
	if (!LineL)
	{
		D3DXCreateLine(pDevice, &LineL);
	}

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	LineL->SetWidth(thickness);
	LineL->Draw(Line, 2, color);
}

void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color)
{
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawEspBox2D(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color)
{
		int height = ((top.y * (7 / 6)) - bot.y);
		Vec2 tl = { top.x - height / 2, top.y };
		Vec2 tr = { top.x + height / 2, top.y };
		Vec2 bl = { bot.x - height / 2 ,bot.y};
		Vec2 br = { bot.x + height / 2 , bot.y};

		DrawLine(tl, bl, thickness, color);
		DrawLine(tr, br, thickness, color);
		DrawLine(tl, tr, thickness, color);
		DrawLine(bl, br, thickness, color);
}

void Draw2DLoop(D3DCOLOR colorTeam, D3DCOLOR colorEnemy,int thickness)
{
		int oof = GetIndex();
		for (int i = 1; i <= MAXPLAYER; i++)
		{
			if (!GetPlayerAddress(i) || GetPlayerHealth(i) < 0 || GetPlayerHealth(i) > 100 || GetDormant(i) == true)
			{
				continue;
			}
			
			Vec2 vec1 = W2S(GetPlayerOrigin(i));
			Vec2 vec2 = W2S(GetBonePosEnemy(i, 8));
			
			if (GetPlayerTeam(i) == GetPlayerTeam(oof))
			{
				DrawEspBox2D(vec2,vec1, thickness, colorTeam);
			}
			else
			{
				DrawEspBox2D(vec2,vec1, thickness, colorEnemy);
			}
			
		}
}

void DrawEspBox3D(Vector3 top, Vector3 bot, float a, int width, int thickness, D3DCOLOR color)
{
	int height3D = ABS(top.z - bot.z);
	Vector3 b1, b2, b3, b4, t1, t2, t3, t4;
	b1.z = b2.z = b3.z = b4.z = bot.z;

	b1.x = bot.x + (cos(TORAD(a + 45)) * width);
	b1.y = bot.y + (sin(TORAD(a + 45)) * width);

	b2.x = bot.x + (cos(TORAD(a + 135)) * width);
	b2.y = bot.y + (sin(TORAD(a + 135)) * width);

	b3.x = bot.x + (cos(TORAD(a + 225)) * width);
	b3.y = bot.y + (sin(TORAD(a + 225)) * width);

	b4.x = bot.x + (cos(TORAD(a + 315)) * width);
	b4.y = bot.y + (sin(TORAD(a + 315)) * width);

	t1.x = b1.x;
	t1.y = b1.y;
	t1.z = b1.z + height3D;

	t2.x = b2.x;
	t2.y = b2.y;
	t2.z = b2.z + height3D;

	t3.x = b3.x;
	t3.y = b3.y;
	t3.z = b3.z + height3D;

	t4.x = b4.x;
	t4.y = b4.y;
	t4.z = b4.z + height3D;
	 
	Vec2 b1_2 = W2S(b1);
	Vec2 b2_2 = W2S(b2);
	Vec2 b3_2 = W2S(b3);
	Vec2 b4_2 = W2S(b4);
	Vec2 t1_2 = W2S(t1);
	Vec2 t2_2 = W2S(t2);
	Vec2 t3_2 = W2S(t3);
	Vec2 t4_2 = W2S(t4);
	//if (W2S(b1, b1_2) && W2S(b2, b2_2) && W2S(b3, b3_2) && W2S(b4, b4_2) && W2S(t1, t1_2) && W2S(t2, t2_2) && W2S(t3, t3_2) && W2S(t4, t4_2)) {

		// col
		DrawLine(t1_2, b1_2, thickness, color);
		DrawLine(t2_2, b2_2, thickness, color);
		DrawLine(t3_2, b3_2, thickness, color);
		DrawLine(t4_2, b4_2, thickness, color);

		// top base
		DrawLine(t1_2, t2_2, thickness, color);
		DrawLine(t2_2, t3_2, thickness, color);
		DrawLine(t3_2, t4_2, thickness, color);
		DrawLine(t4_2, t1_2, thickness, color);

		// bottom base
		DrawLine(b1_2, b2_2, thickness, color);
		DrawLine(b2_2, b3_2, thickness, color);
		DrawLine(b3_2, b4_2, thickness, color);
		DrawLine(b4_2, b1_2, thickness, color);
	//}
}


void DrawText(const char* text, float x, float y, D3DCOLOR color)
{
	RECT rect;

	if (FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &FontF);

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}
Vec2 v = { 0,0 };

void SnapLines()
{

	for (int i = 1; i <=10; i++)
	{
		if (!GetPlayerAddress(i))
		{
			continue;
		}
		if (GetPlayerHealth(i) < 0 | GetPlayerHealth(i) > 100)
		{
			continue;
		}
		if (GetPlayerAddress(i) == GetPlayerAddress(GetIndex())) //GetIndex()
		{
			continue;
		}
		if (GetDormant(i) == true)
		{
			continue;
		}
		 
		Vec2 vec2 = W2S(GetPlayerOrigin(i));
		//if(WorldToScreen(GetPlayerOrigin(i),vec2));
		//if (vec2.x != 0 && vec2.y != 0)
		//{
			DrawLine(vec2.x, vec2.y, 1920 / 2, 1080, 2, color3);
		//}

	}
}

void Release()
{
	FontF->Release();
	LineL->Release();
}



















void BunnyHop()
{
	static int v1 = 0x5;
	static int v2 = 0x4;
	DWORD xd = GetPlayerAddress(GetIndex()) + fFlagsOffset;
	DWORD ingr = *(DWORD*)xd;

	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (ingr & 1 == 1))
	{
		DWORD d1 = modulePanorama + ForceJumpOffset;
		*(int*)(d1) = v1;
		Sleep(50);
		*(int*)(d1) = v2;
	}
}

void shoot()
{
	DWORD d = DWORD(modulePanorama + dwForceAttack2);
	*(int*)(d) = 0x5;
	Sleep(20);
	*(int*)(d) = 0x4;
	Sleep(400);
}
