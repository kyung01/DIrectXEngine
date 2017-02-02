
#include <Windows.h>
#include "DirectX\KContext.h"

// ImGui - standalone example application for DirectX 11
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui\imgui.h"
#include "imgui\DirectX\imgui_impl_dx11.h"
#include "imgui\DirectX\KContext.h"
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;



// --------------------------------------------------------
// Entry point for a graphical (non-console) Windows application
// --------------------------------------------------------
int WINAPI WinMain(
	HINSTANCE hInstance,		// The handle to this app's instance
	HINSTANCE hPrevInstance,	// A handle to the previous instance of the app (always NULL)
	LPSTR lpCmdLine,			// Command line params
	int nCmdShow)				// How the window should be shown (we ignore this)
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable memory leak detection as a quick and dirty
	// way of determining if we forgot to clean something up
	//  - You may want to use something more advanced, like Visual Leak Detector
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create the Game object using
	// the app handle we got from WinMain
	NDirectX::KContext dxContext(hInstance);

	// Result variable for function calls below
	HRESULT hr = S_OK;

	// Attempt to create the window for our program, and
	// exit early if something failed
	hr = dxContext.InitWindow();
	if(FAILED(hr)) return hr;

	// Attempt to initialize DirectX, and exit
	// early if something failed
	hr = dxContext.InitDirectX();
	if(FAILED(hr)) return hr;
	




	dxContext.Init();
	//NImGui::KContext guiContext;
	//guiContext.init(hInstance, dxContext.hWnd, dxContext.device, dxContext.context, dxContext.swapChain, dxContext.backBufferRTV);
	//guiContext.m_uiMain.init(&dxContext.m_renderContexts.begin()->main);//TODO delete this line
	//dxContext.m_ui = &guiContext;
	
	// Begin the message and game loop, and then return
	// whatever we get back once the game loop is over
	return dxContext.Run();
}
/*

void CreateRenderTarget()
{
DXGI_SWAP_CHAIN_DESC sd;
g_pSwapChain->GetDesc(&sd);

// Create the render target
ID3D11Texture2D* pBackBuffer;
D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
render_target_view_desc.Format = sd.BufferDesc.Format;
render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &g_mainRenderTargetView);
g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
pBackBuffer->Release();



}

void CleanupRenderTarget()
{
if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}


extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
if (ImGui_ImplDX11_WndProcHandler(hWnd, msg, wParam, lParam))
return true;

switch (msg)
{
case WM_SIZE:
if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
{
ImGui_ImplDX11_InvalidateDeviceObjects();
CleanupRenderTarget();
g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
CreateRenderTarget();
ImGui_ImplDX11_CreateDeviceObjects();
}
return 0;
case WM_SYSCOMMAND:
if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
return 0;
break;
case WM_DESTROY:
PostQuitMessage(0);
return 0;
}
return DefWindowProc(hWnd, msg, wParam, lParam);
}


int main_example(HINSTANCE wc, HWND hwnd)
{
ImGui_ImplDX11_Init(hwnd, g_pd3dDevice, g_pd3dDeviceContext);

ImVec4 clear_col = ImColor(114, 144, 154);

// Main loop
MSG msg;
ZeroMemory(&msg, sizeof(msg));
while (msg.message != WM_QUIT)
{
if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
continue;
}
ImGui_ImplDX11_NewFrame();
ImGui::Text("Hello, world!");

// Rendering
g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_col);
ImGui::Render();
g_pSwapChain->Present(0, 0);
}

ImGui_ImplDX11_Shutdown();
CleanupDeviceD3D();
UnregisterClass(_T("ImGui Example"), wc);

return 0;
}
*/