#pragma once
#include "MainFuncs.h"

#define	HITGROUP_GENERIC	0
#define	HITGROUP_HEAD		1
#define	HITGROUP_CHEST		2
#define	HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10	



float GetHitgroupDamageMultiplier(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_HEAD:
	{
		return 4.0f;
	}
	case HITGROUP_CHEST:
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
	{
		return 1.0f;
	}
	case HITGROUP_STOMACH:
	{
		return 1.25f;
	}
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
	{
		return 0.75f;
	}
	}
	return 1.0f;
}


void ScaleDamage(int hitgroup, int i, float weapon_armor_ratio, float& current_damage)
{
	current_damage *= GetHitgroupDamageMultiplier(hitgroup);

	int armor = GetPlayerArmor(i);
	int helmet = HasHelmet(i);

	if (armor > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (helmet)
				current_damage *= (weapon_armor_ratio * .5f);
		}
		else
		{
			current_damage *= (weapon_armor_ratio * .5f);
		}
	}
}

bool DidHitNonWorldEntity(int i)
{
	return GetPlayerAddress(i) != NULL && GetPlayerAddress(i) == GetPlayerAddress(GetIndex());
}

