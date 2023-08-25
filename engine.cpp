#include "engine.h"

constexpr int Strick = 2;
uint8_t* GameBase = (uint8_t*)GetModuleHandleA(0);
uint8_t** GName = (uint8_t**)(GameBase + 0x4A56340);
UWorld** GWorld = (UWorld**)(GameBase + 0x4BDAA00);
UObject** GObject = (UObject**)(GameBase + 0x4A92680);

fn_LineTraceSingle _LineTraceSingle = (fn_LineTraceSingle)(GameBase + 0x2C42C00);
fn_GetBoneMatrix _GetBoneMatrix = (fn_GetBoneMatrix)(GameBase + 0x2AD5300);
fn_ProjectWorldLocationToScreen _ProjectWorldLocationToScreen = (fn_ProjectWorldLocationToScreen)(GameBase + 0x2E1F550);

// 也有可能是0x14BEFA0
// fn_MoveInterpolationTarget _MoveInterpolationTarget = (fn_MoveInterpolationTarget)(GameBase + 0x14BEFD0);
// 也有可能是 14BEFA0 14BEFD0
// fn_SetInterpolatedComponent _SetInterpolatedComponent = (fn_SetInterpolatedComponent)(GameBase + 0x14973E0);

string GetName(uint32_t Id) {
	uint32_t Block = Id >> 16;
	uint32_t Offset = Id & (65536 - 1);
	// GName + (Strick + Block) * 8;
	FNameEntry* Info = (FNameEntry*)(GName[Strick + Block] + Strick * Offset);
	
	return string(Info->AnsiName, Info->Header.len);
}

UObject* GetUObject() {
	return *GObject;
}

UWorld* GetWorld() {
	return *GWorld;
}

Vector3 GetBoneMatrix(void* mesh, int32_t BoneIndex)
{
	FMatrix Matrix;
	_GetBoneMatrix(mesh, &Matrix, BoneIndex);

	Vector3 pos;
	pos.x = Matrix._41;
	pos.y = Matrix._42;
	pos.z = Matrix._43;

	return pos;
}

bool WorldToScreen(Vector3 WorldLocation, Vector2& ScreenLocation) {
	
	void* pthis = GetWorld()->OwningGameInstance->GetPlayerController();

	return _ProjectWorldLocationToScreen(pthis, WorldLocation, ScreenLocation, false);
}

bool LineTraceSingle(Vector3 Start, Vector3 End, AActor* Actor) {
	FHitResult OutHit;
	FLinearColor Color;
	Color.x = 0;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.push(GetWorld()->OwningGameInstance->GetPlayer(), Actor);
	bool ret = _LineTraceSingle(GetWorld(), Start, End, 0, true, ActorsToIgnore, 0, OutHit, true, Color, Color, 0);
	
	ActorsToIgnore.pop();

	return  ret;
}