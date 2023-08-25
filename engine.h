#pragma once
#include "base.h"
#include "struct.h"
#include "class.h"

typedef bool (WINAPI* fn_ProjectWorldLocationToScreen)(void* pthis, Vector3 WorldLocation, Vector2& ScreenLocation, bool bPlayerViewportRelative);
typedef FMatrix* (WINAPI* fn_GetBoneMatrix) (void* mesh, FMatrix* Matrix, int32_t BoneIdx);
typedef bool (WINAPI* fn_LineTraceSingle)(UWorld* WorldContextObject, Vector3 Start, Vector3 End, char TraceChannel, bool bTraceComplex, TArray<AActor*>& ActorsToIgnore,
	char DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);
// typedef void (WINAPI* fn_MoveInterpolationTarget) (Vector3 NewLocation, FRotator NewRotation);
// typedef void (WINAPI* fn_SetInterpolatedComponent)(USceneComponent* Component);

string GetName(uint32_t Id);
UWorld* GetWorld();
UObject* GetUObject();
Vector3 GetBoneMatrix(void* mesh, int32_t BoneIndex);
bool WorldToScreen(Vector3 WorldLocation, Vector2& ScreenLocation);
bool LineTraceSingle(Vector3 Start, Vector3 End, AActor* Actor);