#pragma once
#include <Engine\Enum.h>
#include <DirectX\RenderTexture.h>
#include <memory>
#include <Engine\Componenets\Component.h>

namespace KEngine {
	namespace KComponent {
		enum PROBE_TYPE { REFLECTION, IRRADIANCE };
		/*
		this struct should be paired with LightRender system
		*/
		struct ProbeComponent : public Component {
		private:
		public:
			bool isNeedUpdate;
			PROBE_TYPE type;
			Vector3 position;
			std::unique_ptr<NDirectX::RenderTexture> 
				xPlus, xMinus, 
				yPlus, yMinus, 
				zPlus, zMinus;
			std::unique_ptr<float*> coefficients;

			ProbeComponent();
			void initRenderTexture(ID3D11Device *device, int size);
			void initCoefficients();
		};
		/*
		struct PointLightInfo {
		Vector3 position;
		Vector3 color;
		float intensity;
		};
		struct SpotLightInfo {
		Vector3 position;
		Quaternion rotation;
		Vector3 color;
		float intensity;
		float fov;
		};
		*/

	}
}