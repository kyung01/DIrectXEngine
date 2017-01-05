#include "..\RenderStateStack.h"

void NGraphic::RenderStateStack::save(ID3D11DeviceContext *context)
{

	UINT viewportNum = 1;
	context->RSGetViewports(&viewportNum, &viewport);
}

void NGraphic::RenderStateStack::load(ID3D11DeviceContext *context)
{
	context->RSSetViewports(1, &viewport);
}
