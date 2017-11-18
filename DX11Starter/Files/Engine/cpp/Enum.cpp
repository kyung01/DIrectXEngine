#include <Engine\Enum.h>
#include <random>
using namespace KEngine;

KEnum KEngine::getRadnomModelID() {
	switch (rand()%10) {
	default:
	case 0:
		return MESH_SPONZA;
	case 1:
		return MESH_CONE;
	case 2:
		return MESH_CYLINDER;
	case 3:
		return MESH_CUBE;
	case 4:
		return MESH_HELIX;
	case 5:
		return MESH_SPHERE;
	case 6:
		return MESH_TORUS;
	case 7:
		return MESH_PLANE;
	case 8:
		return MESH_FRUSTUM;
	case 9:
		return MESH_SPOTLIGHT;
	}
}