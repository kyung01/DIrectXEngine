#pragma once
#include <list>
#include <memory>
#include "Camera.h"
#include <Graphic\Scene\Object.h>
#include <Graphic\Scene\Light.h>>
#include <Graphic\Scene\PointLight.h>
#include <Graphic\Scene\Probe.h>
namespace NGraphic {
	namespace NScene {
		class Scene {

		public:
			std::list<std::shared_ptr<Light>> objs_lightsNotReady; //things to be rendered on the screen
			std::list<std::shared_ptr<Light>> objs_lights; //things to be rendered on the screen

			std::list<std::shared_ptr<Probe>> m_probesNotReady; //things to be rendered on the screen
			std::list<std::shared_ptr<Probe>> m_probes; //things to be rendered on the screen

			Vector3 size;
			Camera m_camMain; // the main camera
			std::list<std::shared_ptr<Object>> objs_solid; //things to be rendered on the screen
			std::list<std::shared_ptr<Object>> objs_ui; //things to be rendered on the screen
			Scene();
			void addObject(std::shared_ptr<Object> obj);
			void loadExample00();

			std::shared_ptr<Object> getObjSolid();
			std::shared_ptr<Object> getObjUI();
			std::shared_ptr<PointLight> getPointLight(Vector3 lightColor, float lightDistance);
			std::shared_ptr<Light> getSpotLight(float angle, Vector3 lightColor, float lightDistance);
			std::shared_ptr<Probe> getProbe();
		};
	}
}