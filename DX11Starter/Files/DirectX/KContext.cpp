#pragma once
#include "KContext.h"
#include <iostream>





// For the DirectX Math library
using namespace NDirectX;
using namespace NGraphic;
using namespace NGraphic::NScene;

const int KContext::INIT_WINDOW_WIDTH = 1280;
const int KContext::INIT_WINDOW_HEIGHT = 720;
// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
KContext::KContext(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		INIT_WINDOW_WIDTH,			   // Width of the window's client area
		INIT_WINDOW_HEIGHT,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	
	// Initialize fields
	//vertexBuffer = 0;
	//indexBuffer = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
KContext::~KContext()
{
	/*
	Mesh classes destroy themselves
	*/
}
float scale = 0.9f;
// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void KContext::Init()
{
	std::cout << "\n";

	m_engine.init(device, context, INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
	m_ui.init(hInstance, hWnd, device, context, swapChain, backBufferRTV);
	//system("pause");
	if (!m_asset.init(device, context)) {
		std::cout << "Failed crucial steps.\n";
		system("pause");
	}
	//LoadShaders();
	
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void KContext::OnResize()
{
	DXCore::OnResize();
	m_engine.OnResize(this->width, this->height);
	//std::cout << "KContext Resize\n";
	//m_texture.init(device, swapChain, this->width, this->height);
	//m_depth.init(device, this->width, this->height);

}

int testLight = 0;
// --------------------------------------------------------
// Update your KContext here - user input, move objects, AI, etc.
// --------------------------------------------------------
void KContext::Update(float deltaTime, float totalTime)
{
	


	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	m_engine.update(deltaTime);
	float x, y, dis_camerMove(1.0*deltaTime);
	int count = 0;
	
	XMVECTOR dir;
		 
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
		std::cout << "Quit\n";
		Quit();
		/* Do something useful */
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void KContext::Draw(float deltaTime, float totalTime)
{
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	context->OMSetRenderTargets(1, &this->backBufferRTV, depthStencilView);
	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	context->OMSetRenderTargets(1,&this-> backBufferRTV, depthStencilView);
	m_engine.render(device, context, this->backBufferRTV, depthStencilView, viewport);
	m_ui.render();
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void KContext::OnMouseDown(WPARAM buttonState, int x, int y)
{

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void KContext::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}


int mouseMoveXY[2] = {-1,-1};
float mouseRotation[2] = { 0,0 };
void KContext::OnMouseMove(WPARAM buttonState, int x, int y)
{
	m_engine.OnMouseMove(buttonState, x, y);
	float power = .010;
	bool isContinue = true;
	int xDis = x - mouseMoveXY[0];
	int yDis = y - mouseMoveXY[1];
	if (mouseMoveXY[0] == -1) {
		isContinue = false;
	}
	mouseMoveXY[0] = x;
	mouseMoveXY[1] = y;
	if (!isContinue)return;
	//if (xDis*xDis + yDis*yDis > 100) return; 
	if (!(buttonState & 0x0001))
	{
		return;
	}
	mouseRotation[0] += xDis * power;
	mouseRotation[1] += yDis * power;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void KContext::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}