#pragma once
#include <map>
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

#include "SimpleShader.h"
#include "RenderTexture.h"
#include "DepthTexture.h"
#include "Graphic\Scene\Light.h"
#include <list>
#include <map>
#include <memory>
#include <d3d11.h>

#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Graphic\Asset\Asset.h>
#include <Graphic\Asset\Mesh.h>// TODO delete this
#include <Graphic\Enum.h>
#include <Graphic\RenderStateStack.h>
#include <Graphic\Buffer\KDynamicBuffer.h>
#include <Graphic\Buffer\LightParameter.h>
#include <Game\Context.h>

#include <Graphic\Scene\Scene.h>


namespace NGraphic {
	class TextureAtlasSlicer {
	protected:
		float m_countWidth, m_countHeight;
		float m_width, m_height;
		std::vector<bool> m_check;

		bool isAvailable(int x, int y);
		bool isAvailable(int x, int y, int size);

	public:
		TextureAtlasSlicer(float pixelWidth, float pixelHeight, float countWidth, float countHeight);
		bool getRoom(float &offsetX, float &offsetY, float &roomSize, int size);

	};
}