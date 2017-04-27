
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