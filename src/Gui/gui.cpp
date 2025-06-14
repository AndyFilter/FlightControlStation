#include "../External/ImGui/imgui.h"
#include "../External/ImGui/imgui_impl_win32.h"
#include "../External/ImGui/imgui_impl_dx11.h"
#include <tchar.h>
#include <iostream>
#include "Fonts.h"

#include "gui.h"

using namespace GUI;

#pragma comment(lib, "d3d11.lib") 

// Data
//static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11Device* g_pd3dDevice = NULL;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

float defaultFontSize = 13.0f;

static WNDCLASSEX wc;
static HWND hwnd;

static int (*mainGuiFunc)();

// Setup code, takes a function to run when doing GUI
HWND GUI::Setup(int (*OnGuiFunc)())
{
	if (OnGuiFunc != NULL)
		mainGuiFunc = OnGuiFunc;
	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();
	wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("Flight Control Sim"), NULL };
	::RegisterClassEx(&wc);
	hwnd = ::CreateWindow(wc.lpszClassName, _T("Flight Control Sim"), (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX), 50, 50, windowX, windowY, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return NULL;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 6.0f;
	style.ChildRounding = 6.0f;
	style.FrameRounding = 4.0f;
	style.PopupRounding = 3.0f;
	style.GrabRounding = 4.0f;
	style.FrameBorderSize = 0.5f;

	ImVec4* colors = style.Colors;
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2f, 0.15f, 0.5, 0.92f);//ImGui::ColorConvertU32ToFloat4((BLUE400 & 0x00FFFFFF) | 0x33000000);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.69f, 1, 1, 1); //ImGui::ColorConvertU32ToFloat4((GRAY900 & 0x00FFFFFF) | 0x0A000000);


	// Handle switching to fullscreen on my own
	IDXGIDevice* pDXGIDevice = nullptr;
	auto hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);

	IDXGIAdapter* pDXGIAdapter = nullptr;
	hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);

	IDXGIFactory* pIDXGIFactory = nullptr;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);

	pIDXGIFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);


	ImFontConfig config;
	config.OversampleH = 2;
	config.OversampleV = 2;

	static const ImWchar icons_ranges[] = { 0x01, 0x17D, 0 };
	//config.PixelSnapH = true;
	//auto font = io.Fonts->AddFontFromMemoryCompressedTTF(ConsoleFont_compressed_data, 13, 19, &config, icons_ranges);
	//auto font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(ConsoleFontM_compressed_data_base85, 16, &config, icons_ranges);
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\CASCADIAMONO.ttf", 15.0f, &config, icons_ranges);
	//io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\CONSOLA.ttf", 16.0f, &config, icons_ranges);

	io.Fonts->Build();


	return hwnd;
}

//void GUI::LoadFonts(float fontSizeMultiplier)
//{
//	ImGuiIO& io = ImGui::GetIO();
//
//	ImFontConfig config;
//	config.OversampleH = 2 * fontSizeMultiplier;
//	config.OversampleV = 2 * fontSizeMultiplier;
//
//	defaultFontSize += fontSizeMultiplier;
//
//	//io.FontDefault = io.Fonts->AddFontFromFileTTF("../Fonts\\CourierPrime-Regular.ttf", defaultFontSize, &config);
//	//io.Fonts->AddFontFromFileTTF("../Fonts\\CourierPrime-Bold.ttf", defaultFontSize, &config);
//	io.Fonts->AddFontFromMemoryCompressedTTF(CourierPrimeRegular_compressed_data, 13, defaultFontSize, &config);
//	io.Fonts->AddFontFromMemoryCompressedTTF(CourierPrimeBold_compressed_data, 13, defaultFontSize, &config);
//
//	printf("Loaded Courier Prime\n");
//
//	config.GlyphOffset.y = -1;
//	//io.Fonts->AddFontFromFileTTF("../Fonts\\SourceSansPro-SemiBold.ttf", defaultFontSize, &config);
//	//io.Fonts->AddFontFromFileTTF("../Fonts\\SourceSansPro-Black.ttf", defaultFontSize, &config);
//	io.Fonts->AddFontFromMemoryCompressedTTF(SourceSansProSemiBold_compressed_data, 13, defaultFontSize, &config);
//	io.Fonts->AddFontFromMemoryCompressedTTF(SourceSansProBlack_compressed_data, 13, defaultFontSize, &config);
//	config.GlyphOffset.y = 0;
//
//	//io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Gothicb.ttf", 14.f);
//	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Framd.ttf", defaultFontSize, &config);
//
//	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Lucon.ttf", defaultFontSize, &config);
//
//	io.Fonts->Build();
//
//	//for (int i = 0; i < io.Fonts->Fonts.Size; i++)
//	//{
//	//    io.Fonts->Fonts[i]->Scale = 2.f;
//	//}
//
//}

int GUI::DrawGui() noexcept
{
	static bool showMainWindow = true;
	static ImVec4 clear_color = ImVec4(0.55f, 0.45f, 0.60f, 1.00f);
	static bool done = false;

	MSG msg;
	while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		//if (msg.message == WM_QUIT)
		//    done = true;
	}
	if (done)
		return 1;

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto size = ImGui::GetIO().DisplaySize;
	//size = ImGui::GetWindowSize();
	ImGui::SetNextWindowSize({ (float)size.x, (float)size.y });
	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::Begin("Window", &showMainWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PopStyleVar();

	// Call the GUI function in main file
	if (int mainGui = mainGuiFunc())
		return mainGui;

	ImGui::End();

#ifdef _DEBUG 
	//ImGui::ShowDemoWindow();
#endif // DEBUG

	// Rendering
	ImGui::Render();
	//const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	//g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	auto presentRes = g_pSwapChain->Present(SYNC_FRAMES, 0);
	if (presentRes == DXGI_STATUS_OCCLUDED)
		Sleep(1);
	return 0;
}

void GUI::Destroy() noexcept
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0; // 60
	sd.BufferDesc.RefreshRate.Denominator = 0; // 1
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	g_pSwapChain->SetFullscreenState(FALSE, NULL);
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (!pBackBuffer)
		ExitProcess(1);
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			//UINT x = (UINT)LOWORD(lParam), y = (UINT)HIWORD(lParam);
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = windowX;
		lpMMI->ptMinTrackSize.y = windowY;
		break;
	}
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			if (onExitFunc)
				if (onExitFunc())
					break;
				else
					return 0;
		}
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		if (onExitFunc)
			if (onExitFunc())
			{
				::PostQuitMessage(0);
				return 0;
			}
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
