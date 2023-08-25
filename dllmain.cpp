// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "base.h"
#include "engine.h"
#include "dx11.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        AllocConsole();
        freopen("CONOUT$", "w+", stdout);
        // DX11Hook();
        // printf("GetName(0)->%s",GetName(0).c_str());
        CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DX11Hook, NULL, NULL, NULL));
    }
    return TRUE;
}