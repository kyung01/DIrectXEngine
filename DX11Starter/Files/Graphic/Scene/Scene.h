#pragma once
#include <list>
#include <memory>
#include "Camera.h"
#include <Graphic\Scene\Object.h>
#include <Graphic\Scene\OldSpotLight.h>
#include <Graphic\Scene\OldPointLight.h>
#include <Graphic\Scene\Probe.h>

#include <Graphic\Scene\SpotLight.h>
#include <Graphic\Scene\PointLight.h>

#include <Graphic\Scene\ILight.h>
#include <Graphic\Scene\IProbeReflection.h>
#include <Graphic\Scene\IProbeDiffuse.h>
namespace NGraphic {
	namespace NScene {
		class Scene {

		public:
			std::list<std::shared_ptr<OldSpotLight>> objs_lightsNotReady; //things to be rendered on the screen
			std::list<std::shared_ptr<OldSpotLight>> objs_lights; //things to be rendered on the screen
			std::list<std::shared_ptr<Probe>> m_probesNotReady; //things to be rendered on the screen
			std::list<std::shared_ptr<Probe>> m_probes; //things to be rendered on the screen


			std::list<std::shared_ptr<OldILight>> m_oldLights; //things to be rendered on the screen
			std::list<std::shared_ptr<IProbeReflection>> m_reflections; //things to be rendered on the screen
			std::list<std::shared_ptr<IProbeDiffuse>> m_diffuse; //things to be rendered on the screen


			std::list<ILight> m_lightsNotReady, m_lights; //things to be rendered on the screen

			Vector3 size;
			OldCamera m_camMain; // the main camera
			std::list<std::shared_ptr<Object>> objs_solid; //things to be rendered on the screen
			std::list<std::shared_ptr<Object>> objs_ui; //things to be rendered on the screen
			Scene();
			void addObject(std::shared_ptr<Object> obj);
			void loadExample00();

			std::shared_ptr<Object> getObjSolid();
			std::shared_ptr<Object> getObjUI();
			std::shared_ptr<OldPointLight> getPointLightOLD(Vector3 lightColor, float lightDistance);
			std::shared_ptr<OldSpotLight> getSpotLightOLD(float angle, Vector3 lightColor, float lightDistance);
			std::shared_ptr<Probe> getProbeReflection();
			std::shared_ptr<Probe> getProbeDiffuse();

			PointLight getPointLight(Vector3 lightColor, float lightDistance);
			SpotLight getSpotLight(float angle, Vector3 lightColor, float lightDistance);



		};
	}
}