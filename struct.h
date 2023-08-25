#pragma once
#include "base.h"

constexpr int NAME_SIZE = 1024;

struct Vector2 {
	float x, y;
};

struct Vector3 : Vector2 {
	float z;
};

struct Vector4 : Vector3 {
	float w;
};

struct BoneName
{
	uint32_t Name;
	int A;
	int B;
};

struct FNameEntryHeader {
	uint16_t bIsWide : 1;
	// static constexpr uint32_t ProbeHashBits = 5;
	uint16_t LowercaseProbeHash : 5;
	uint16_t len : 10;
};

struct FNameEntry {
	FNameEntryHeader Header;
	union {
		char AnsiName[NAME_SIZE];
		wchar_t WideName[NAME_SIZE];
	};
};

struct FMatrix {
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
	FMatrix operator* (const FMatrix& other);
};


struct FTransform {
	Vector4                                       Rotation;                                                     // 0x0000(0x0010)
	Vector4                                       Translation;                                                  // 0x0010(0x000C)
	Vector4                                       Scale3D;                                                      // 0x0020(0x000C)
	FMatrix ToMatrixWithScale();
};

struct BoneIdx {
	int head, neck_01, spine_03, spine_02, spine_01, pelvis;
	int hand_l, lowerarm_l, upperarm_l, clavicle_l, clavicle_r, upperarm_r, lowerarm_r, hand_r;
	int ball_l, foot_l, calf_l, thigh_l, thigh_r, calf_r, foot_r, ball_r;
};

// ScriptStruct  Engine.HitResult
// Struct Size::0x0088
struct FHitResult
{
public:
	char pa_00[0x0088];
};

// ScriptStruct  CoreUObject.LinearColor
// Struct Size::0x0010
struct FLinearColor : public Vector4{

};

struct FRotator
{
	float                                         Pitch;                                                        // 0x0000(0x0004)
	float                                         Yaw;                                                          // 0x0004(0x0004)
	float                                         Roll;                                                         // 0x0008(0x0004)
};