#pragma once
#include <memory>
#include <map> 
#include <vector> 
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

namespace Engine {
	enum ComponentId {

	};
	enum ComponentType {

	};
	struct Component {
	private:
	protected:
	public:
	};
	struct ComponentIndex {
		int at; //where am I?
		ComponentType type; //what's my type?
		int size; // how mnay bytes do I occupy here?
	};
}
/*
#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete
#include <d3d11.h>
#include <SimpleMath.h>
#include <Graphic\Enum.h>
#include <Graphic\Scene\SceneEnums.h>
//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;

namespace NGraphic {
namespace NScene {
class Object {
private:
static int OBJECT_UNIQUE_ID;
bool m_isAlive, m_isDirty;
protected:
public:
int m_id;
Vector3 m_scale;
Vector3 m_pos;
Vector3 m_dirLook;
Quaternion m_rotation;
Matrix m_matModel;
OBJ_TYPE m_ObjectType;
KEnum				m_renderType;
KEnum					m_meshId;
std::map<KEnum, KEnum> m_textures;

Object();
virtual Object setScale(Vector3 scale);
virtual Object setPos(Vector3 pos);
virtual Object setRotation(Quaternion quaternion);
Matrix getModelMatrix();
};
}
}

*/