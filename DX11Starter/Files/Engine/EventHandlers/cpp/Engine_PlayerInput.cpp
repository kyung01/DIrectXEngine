#include <Engine\EventHandlers\PlayerInput.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
using namespace KEngine::KHandler;
using namespace DirectX::SimpleMath;
#define print(content) std::cout<<"Engine_PlayerInput::"<<content<<"\n"

void PlayerInput::init(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight)
{
}

void PlayerInput::update(float timeElapsed)
{
	Vector3 dirForward = DirectX::XMVector3Rotate(Vector3(0, 0, 1), m_playerRotation);
	Vector3 dirRight = Vector3(0, 1, 0).Cross(dirForward);
	Vector3 moveDir;
	if (GetAsyncKeyState('W'))
	{
		moveDir += dirForward;
	}
	if (GetAsyncKeyState('S'))
	{
		moveDir -= dirForward;
	}
	if (GetAsyncKeyState('D'))
	{
		moveDir += dirRight;
	}
	if (GetAsyncKeyState('A'))
	{
		moveDir -= dirRight;
	}
	moveDir.Normalize();
	m_playerPosition += moveDir * timeElapsed;
	if (moveDir.Length() >= 0.1f)
		//	print("moveDir.x << " , " << moveDir.y << " , " << moveDir.z);
		print("Move " << m_playerPosition.x << " , " << m_playerPosition.y << " , " << m_playerPosition.z);
}

void PlayerInput::render(ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT viewport)
{
}

void PlayerInput::OnMouseMove(WPARAM buttonState, int x, int y)
{
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

	float maxAngle =( M_PI - 0.001f)/2.0f;
	mouseRotation[0] += xDis * power;
	mouseRotation[1] += yDis * power;
	//mouseRotation[0] = max(min(mouseRotation[0], maxAngle), -maxAngle);
	mouseRotation[1] = max(min(mouseRotation[1], maxAngle), -maxAngle);

	m_playerRotation  = Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), mouseRotation[1]);
	m_playerRotation *= Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), mouseRotation[0]);
	//	*
	//	Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), mouseRotation[0]);

	//print("MouseRotation " << mouseRotation[0] << " , " << mouseRotation[1]);

}

void PlayerInput::OnResize(int windowWidth, int windowHeight)
{
}



Vector3 PlayerInput::getPosition()
{
	return m_playerPosition;
}

Quaternion PlayerInput::getRotation()
{
	return m_playerRotation;
}