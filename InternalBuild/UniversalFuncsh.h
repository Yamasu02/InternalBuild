#pragma once
#include "MemoryFuncs.h"
#include "csgotrace.h"
#include "classes/cmd.h"
#include "csgoVector.h"


float GetDistance(Vector3* Enemy, Vector3* Local)
{
	Vector3 DeltaVec = Vector3(Enemy->x - Local->x, Enemy->y - Local->y, Enemy->z - Local->z);
	return sqrt(DeltaVec.x * DeltaVec.x + DeltaVec.y * DeltaVec.y + DeltaVec.z * DeltaVec.z);
}


bool CheckEnemyAimbot(int i)
{
	if (!GetPlayerAddress(i) || GetDormant(i) || GetImmunity(i) || GetLifeState(i) || GetPlayerTeam(i) == GetPlayerTeam(GetIndex())) // || GetPlayerHealth(i) > 100 || GetPlayerHealth(i) < 0  || i == GetIndex()
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool CheckEntityEsp(int i)
{
	if (!GetPlayerAddress(i) || GetDormant(i) || GetLifeState(i))
	{
		return false;
	}
	else
	{
		return true;
	}
}


int GetClosestEnemy()
{
	static int localteam = GetPlayerTeam(GetIndex());
	float closestDistance = pow(9.f,9.f);
	int closestDistanceIndex = 1;

	for (int i = 0; i <= MAXPLAYER; i++)
	{
		if (CheckEnemyAimbot(i) == false)
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


void GlowEntity(int i, DWORD glow_Pointer, float TRed, float TGreen, float TBlue, float TAlpha, float CTRed, float CTGreen, float CTBlue, float CTAlpha)
{
	if (CheckEntityEsp(i))
	{
		int glow_currentPlayerGlowIndex = *(int*)(GetPlayerAddress(i) + GlowIndexOffset);
		//int health = GetPlayerHealth(i);
		int team = GetPlayerTeam(i);
		//if (health > 0 && health < 101)
		//{
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
		//}
	}
}


void GlowEsp(float TRed, float TGreen, float TBlue, float TAlpha, float CTRed, float CTGreen, float CTBlue, float CTAlpha)
{
	DWORD glow_Pointer = *(DWORD*)(modulePanorama + GlowObjectManagerOffset);
	for (int i = 0; i < MAXPLAYER; i++)
	{
		GlowEntity(i, glow_Pointer, TRed, TGreen, TBlue, TAlpha, CTRed, CTGreen, CTBlue, CTAlpha);
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


void Jitter(float degree_pitch, float degrees_yaw, int ms_to_sleep)
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
			Vector3 vec1 = GetPlayerOrigin(oof);
			Vector3 vec11 = GetPlayerView(oof);
			Vector3 vec4 = GetBonePosEnemy(i, 8);
			vec3 v1 = { vec1.x + vec11.x, vec1.y + vec11.y ,vec1.z + vec11.z };
			vec3 v3 = { vec4.x,vec4.y,vec4.z };

			ray.Init(v1, v3);
			tracefilter.pSkip = (void*)GetPlayerAddress(oof);
			EngineTrace->TraceRay(ray, MASK_NPCWORLDSTATIC | CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX, &tracefilter, &trace); //mask shot conetnts grate


			if ((void*)GetPlayerAddress(i) == trace.hit_entity && CheckEnemyAimbot(i))
			{
				AimAt(&vec4);
				return i;
			}
	}
	return 0;
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
		return { 1920,1080 / 2 };

	x /= w;
	y /= w;
	screen.x = (windowWidth2 / 2 * x) + (x + windowWidth2 / 2);
	screen.y = -(windowHeight2 / 2 * y) + (y + windowHeight2 / 2);
	return screen;
}