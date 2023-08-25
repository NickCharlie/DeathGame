#pragma once
#include "struct.h"
template <typename T>

class TArray {
public:
	void push(T Player, T Actor) {
		Data = new T[2];
		Data[0] = Player;
		Data[1] = Actor;
		Count = 2;
		Max = 2;
	}

	void pop() {
		delete[] Data;
	}

	T* Data;
	int Count;
	int Max;
};

class UObject {
public:
	char                                          pa_00[0x28];														// 0x0000(0x0028)
};

class USceneComponent {
public:
	char pa_00[0x1C0];
	FTransform ComponentToWorld;
};

class USkeletalMesh
{
public:
	char pa_00[0x1B0];
	//BoneName* Names;
	TArray<BoneName> Names;
};

class USkeletalMeshComponent : public USceneComponent {
public:
	char pa_1F0[0x290];
	USkeletalMesh* SkeletalMesh;
	// 4B0 - 1C0
	// char pa_1F0[0x2C0];
	char pa_478[0x28];
	// 这里本来应该是 UScreneComponent* 但是因为骨骼是3x4的矩阵，FTransform也是3x4矩阵，所以可以通用
	TArray<FTransform> BoneTransForm[2]; //4B0
};

class APlayerCameraManager {
public:
	char pa_00[0x1AF0];
	Vector3 Camerage;
};

class AController {
public:
	char pa_00[0x2B8];
	APlayerCameraManager* PlayerCameraManager;                                          // 0x02B8(0x0008)
};

class AActor {
public:
	char pa_00[0x130];
	USceneComponent* RootComponent;													 // 0x130 0x0008
	char pa_138[0x120];
	AController* Controller;		                                                 // 0x0258(0x0008)
	// 0x280 class ACharacter USkeletalMeshComponent* - 0x138 = 0x148
	char pa_139[0x20];
	USkeletalMeshComponent* Mesh; // 0x0280(0x0008)
	// 原来是260
	char pa_xxx[0x260];
	float health; // 4E8
	string GetName();
};



class ULevel {
public:
	char pa_00[0x98];
	TArray<AActor*> Actors;
};

class UGameInstance {
public:
	void* GetPlayerController();
	AActor* GetPlayer();
};

class UWorld {
public:
	char pa_00[0x30];
	ULevel* PersistentLevel; // 0x30
	char pa_38[0x148];
	UGameInstance* OwningGameInstance; // 0x180
};
