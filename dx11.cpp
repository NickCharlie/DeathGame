#include "dx11.h"
#include "engine.h"

typedef LRESULT(WINAPI* pWndProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef HRESULT(WINAPI* pPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(WINAPI* pResize)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);


ID3D11Device* g_pDevice = nullptr;//游戏的D3D指针
ID3D11DeviceContext* g_pDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_RenderTargetView = nullptr;

pPresent g_Present = nullptr;//游戏的原函数地址
pWndProc g_WndProc = nullptr;
pResize g_Resize = nullptr;


DWORD64* SwapVTable = nullptr;//交换链虚表

HWND g_hWnd = 0;

void GetDx11Ptr(IDXGISwapChain* pSwapChain);//函数声明
HRESULT WINAPI Initialize(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

void GetBoneIdx(AActor* Actor, BoneIdx* Idx)
{
    for (int i = 0; i < Actor->Mesh->SkeletalMesh->Names.Count; i++)
    {
        string BoneName = GetName(Actor->Mesh->SkeletalMesh->Names.Data[i].Name);

        if (BoneName == "Head") { Idx->head = i; }
        else if (BoneName == "hand_l") { Idx->hand_l = i; }
        else if (BoneName == "hand_r") { Idx->hand_r = i; }
        else if (BoneName == "ball_l") { Idx->ball_l = i; }
        else if (BoneName == "ball_r") { Idx->ball_r = i; }
        else if (BoneName == "foot_l") { Idx->foot_l = i; }
        else if (BoneName == "calf_l") { Idx->calf_l = i; }
        else if (BoneName == "calf_r") { Idx->calf_r = i; }
        else if (BoneName == "foot_r") { Idx->foot_r = i; }
        else if (BoneName == "pelvis") { Idx->pelvis = i; }
        else if (BoneName == "neck_01") { Idx->neck_01 = i; }
        else if (BoneName == "thigh_l") { Idx->thigh_l = i; }
        else if (BoneName == "thigh_r") { Idx->thigh_r = i; }
        else if (BoneName == "spine_03") { Idx->spine_03 = i; }
        else if (BoneName == "spine_02") { Idx->spine_02 = i; }
        else if (BoneName == "spine_01") { Idx->spine_01 = i; }
        else if (BoneName == "lowerarm_l") { Idx->lowerarm_l = i; }
        else if (BoneName == "lowerarm_r") { Idx->lowerarm_r = i; }
        else if (BoneName == "clavicle_l") { Idx->clavicle_l = i; }
        else if (BoneName == "clavicle_r") { Idx->clavicle_r = i; }
        else if (BoneName == "upperarm_l") { Idx->upperarm_l = i; }
        else if (BoneName == "upperarm_r") { Idx->upperarm_r = i; };
    }

    return;
}

void DrawPartBone(int Start, int End, AActor* Actor, BoneIdx* Idx, ImColor Color)
{
    int* BoneIdx = (int*)Idx;

    Vector2 Point{ 0 };
    Vector2 oPoint{ 0 };
    for (int i = Start; i <= End; i++)
    {
        Vector3 Pos = GetBoneMatrix(Actor->Mesh, BoneIdx[i]);

        if (WorldToScreen(Pos, Point) && oPoint.x != 0 && oPoint.y != 0)
        {
            ImGui::GetForegroundDrawList()->AddLine({ oPoint.x, oPoint.y }, { Point.x, Point.y }, Color);
        }

        oPoint = Point;
    }
}

void DrawBone(AActor* Actor, BoneIdx* Idx, ImColor Color)
{
    DrawPartBone(0, 5, Actor, Idx, Color);
    DrawPartBone(6, 13, Actor, Idx, Color);
    DrawPartBone(14, 21, Actor, Idx, Color);
}

void Draw_Actor_Bone() {
    UWorld* World = GetWorld();
    // UObject* GObject = GetUObject();
    if (World) {
        TArray<AActor*>& Actors = World->PersistentLevel->Actors;
        if (World->PersistentLevel) {
            for (size_t i = 0; i < Actors.Count; i++) {

                AActor* Actor = Actors.Data[i];

                if (!Actor || !(Actor->RootComponent)) {
                    continue;
                }

                string ActorName = Actor->GetName();
                if (ActorName.find("Zombie_BP") == string::npos) {
                    continue;
                }

                if (Actor->health <= 0.0f) {
                    continue;
                }

                /*if (!GObject) {
                    continue;
                }*/

                if (!Actor) continue;
                if (IsBadReadPtr((void*)Actor->Mesh, 0)) continue;
                if (!Actor->Mesh || !Actor->Mesh->BoneTransForm) continue;

                int BoneTransIdx = 0;

                if (Actor->Mesh->BoneTransForm[BoneTransIdx].Data == nullptr) {
                    BoneTransIdx = 1;
                }
                if (Actor->Mesh->BoneTransForm[BoneTransIdx].Data == nullptr) {
                    continue;
                }

                // BoneTransIdx = 1;
                TArray<BoneName>& BoneTrans = Actor->Mesh->SkeletalMesh->Names;
                if (BoneTrans.Count < 0 && BoneTrans.Count > BoneTrans.Max) continue;

                for (int j = 0; j < BoneTrans.Count; j++) {

                    if (IsBadReadPtr((void*)BoneTrans.Data, 8)) continue; //addr 加固
                    // if (IsBadReadPtr((void*)uplayerState, 28 + 4 + 4)) continue;
                    if (!BoneTrans.Data) continue; //addr
                    if (!&BoneTrans.Data[j]) continue;

                    // FMatrix Matrix = BoneTrans.Data[j].ToMatrixWithScale() * Actor->Mesh->ComponentToWorld.ToMatrixWithScale();
                    // string BoneName = GetName(Actor->Mesh->SkeletalMesh->Names.Data[j].Name);
                    // Vector3 Pos = GetBoneMatrix(Actor->Mesh, j);;

                    /*Pos.x = Matrix._41;
                    Pos.y = Matrix._42;
                    Pos.z = Matrix._43;*/

                    // 开始绘制骨骼
                    BoneIdx idx{ 0 };
                    GetBoneIdx(Actor, &idx);
                    // 射线检测
                    AActor* ownPlayer = World->OwningGameInstance->GetPlayer();
                    Vector3 EndPos = GetBoneMatrix(Actor->Mesh, idx.head);
                    bool ret = LineTraceSingle(ownPlayer->Controller->PlayerCameraManager->Camerage, EndPos, Actor);
                    ImColor color = ret ? ImColor(255, 255, 0) : ImColor(0, 255, 0);
                    // DrawBone(Actor, &idx, color);      

                    // point = { 0 };

//                    if (WorldToScreen(Pos, point)) {
//                        // char buf[256];
//                        // sprintf(buf, "%zd", j);
//                        // sprintf(buf, "%p", Actor);
//                        // printf("%p\n", Actor);
//                        // ImGui::GetForegroundDrawList()->AddText({ point.x, point.y }, ImColor(255, 255, 0), BoneName.c_str());
//
//                        //if (!&idx) continue;
//                        //DrawBone(Actor, &idx, ImColor(255, 255, 0));
//;                    }
                }

                FTransform& transform = Actor->RootComponent->ComponentToWorld;

                  // 输出Actors地址
                 Vector2 Screen = { 0 };
                 if (WorldToScreen(transform.Translation, Screen)) {
                    char buf[256];
                    sprintf(buf, "%p", Actor);
                    ImGui::GetForegroundDrawList()->AddText({ Screen.x, Screen.y }, ImColor(255, 255, 0), buf);
                 }

                  printf("%f %f %f\n", transform.Translation.x, transform.Translation.y, transform.Translation.z);

            }


        }

    }
}

HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin(u8"xxb-Insert显示及隐藏");
    ImGui::End();
    // 绘制僵尸骨骼
    Draw_Actor_Bone();

    ImGui::Render();
    g_pDeviceContext->OMSetRenderTargets(1, &g_RenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return g_Present(pSwapChain, SyncInterval, Flags);
}

void WriteVtb(int Idx, void* Address) {
    DWORD Protect;
    VirtualProtect(SwapVTable, 1024, PAGE_EXECUTE_READWRITE, &Protect);
    SwapVTable[Idx] = (DWORD64)Address;
    VirtualProtect(SwapVTable, 1024, Protect, &Protect);
}

HRESULT WINAPI hkResize(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
    //加锁,来防止未调用ReImGui函数之前再次执行导致的崩溃
    if (g_pDevice != nullptr)
    {
        g_pDevice->Release();
        g_pDevice = nullptr;
        g_RenderTargetView->Release();

        ImGui_ImplDX11_Shutdown();

        WriteVtb(8, Initialize);
    }

    return g_Resize(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    return CallWindowProcA(g_WndProc, hWnd, msg, wParam, lParam);
}

HRESULT WINAPI Initialize(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    static bool Is = false;

    if (!Is)
    {
        Is = true;
        g_WndProc = (WNDPROC)SetWindowLongPtrA(g_hWnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);//接管消息窗口

        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

        io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 17.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

        ImGui_ImplWin32_Init(g_hWnd);

    }


    GetDx11Ptr(pSwapChain);
    ImGui_ImplDX11_Init(g_pDevice, g_pDeviceContext);

    WriteVtb(8, hkPresent);

    return g_Present(pSwapChain, SyncInterval, Flags);
}

void GetDx11Ptr(IDXGISwapChain* pSwapChain) {
    //取得交换链指针
    g_pSwapChain = pSwapChain;
    //取得设备指针
    g_pSwapChain->GetDevice(__uuidof(g_pDevice), (void**)&g_pDevice);
    //取得上下文指针
    g_pDevice->GetImmediateContext(&g_pDeviceContext);

    ID3D11Texture2D* renderTarget = nullptr;
    g_pSwapChain->GetBuffer(0, __uuidof(renderTarget), (void**)&renderTarget);
    g_pDevice->CreateRenderTargetView(renderTarget, nullptr, &g_RenderTargetView);
    renderTarget->Release();
    //以上为获取必要四个指针
}
void DX11Hook() {
    g_hWnd = FindWindowA("UnrealWindow", NULL);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, NULL, &featureLevel, NULL) != S_OK)
        return;

    SwapVTable = (*(DWORD64**)g_pSwapChain);

    DWORD Protect;
    VirtualProtect(SwapVTable, 1024, PAGE_EXECUTE_READWRITE, &Protect);

    g_Present = (pPresent)SwapVTable[8];//取得游戏Present地址
    WriteVtb(8, Initialize);
    g_Resize = (pResize)SwapVTable[13];//取得游戏ResizeBuffers地址
    WriteVtb(13, hkResize);

    g_pSwapChain->Release();
}