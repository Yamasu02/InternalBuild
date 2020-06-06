#pragma once
#define MAXPLAYER 64


const int LocalPlayerOffset = 0xD2FB94;
const int ClientStateOffset = 0x589DCC;
const int ViewAnglesOffset = 0x4D88;
const int EntityListOffset = 0x4D43AC4;
const int PitchOffset = 0x4d88;
const int YawOffset = 0x4d8c;
const int HealthOffset = 0x100;
const int TeamOffset = 0xF4;
const int MaxPlayerOffset = 0x388;
const int VecOriginOffset = 0x138;
const int BoneMatrixOffset = 0x26A8;
const int ViewOffset = 0x108;
const int ViewMatrixOffset = 0x4D35404;
const int DormantOffset = 0xED;
const int GlowIndexOffset = 0xA428;
const int GlowObjectManagerOffset = 0x528B8A0;
const int ShotsFiredOffset = 0xA380;
const int AimPunchAngleOffset = 0x302C;
const int fFlagsOffset = 0x104;
const int ForceJumpOffset = 0x51ED760;
const int SpottedOffset = 0x93D;
const int MapDirectoryOffset = 0x188;
const int MapOffset = 0x28C;
const int GameDirOffset = 0x6285F8;
const int dwForceAttack = 0x3175068;
const int dwForceAttack2 = 0x3175074;
const int CrosshairOffset = 0xB3D4;
const int isScoped = 0x3914;
const int ArmorOffset = 0xB368;
const int HelmetOffset = 0xB35C;
const int dwClientState_State = 0x108;
const int LocalIndexOffset = 0x180;
const int Immunity = 0x3930;
const double PI = 3.14159265358;

float GlowTerroristRed = 1.f;
float GlowTerroristGreen = 0.f;
float GlowTerroristBlue = 0.f;
float GlowTerroristAlpha = 1.f;

float GlowCounterTerroristRed = 0.f;
float GlowCounterTerroristGreen = 0.f;
float GlowCounterTerroristBlue = 1.f;
float GlowCounterTerroristAlpha = 1.f;