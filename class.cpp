#include "class.h"
#include "engine.h"

void* UGameInstance::GetPlayerController() {
	DWORD64 Data = *(DWORD64*)(this + 0x38);
	DWORD64 Player = *(DWORD64*)(Data);

	return *(void**)(Player + 0x30);
}

AActor* UGameInstance::GetPlayer() {
	DWORD64 Data = *(DWORD64*)(this + 0x38);
	DWORD64 Player = *(DWORD64*)(Data);
	DWORD64 PlayerController = *(DWORD64*)(Player + 0x30);

	return *(AActor**) (PlayerController + 0x250);
}

string AActor::GetName() {
	uint32_t Id = *(uint32_t*)((uint8_t*)this + 0x18);
	return ::GetName(Id);
}