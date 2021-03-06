#pragma once
namespace NGraphic {
	
	enum KEnum {
		SHADER_TYPE_VERTEX, SHADER_TYPE_FRAGMENT,
		MESH_ID_SPONZA,
		MESH_ID_CONE,
		MESH_ID_CYLINDER,
		MESH_ID_CUBE,
		MESH_ID_HELIX,
		MESH_ID_SPHERE,
		MESH_ID_TORUS,
		MESH_ID_PLANE,
		MESH_ID_FRUSTUM,
		MESH_SPOTLIGHT,

		TEXTURE_ID_SKYBOX_SUNNY,

		RENDER_TEST,
		RENDER_DEFAULT,
		RENDER_WORLD_DIFFUSE_NORMAL_PROPERTY,
		RENDER_WORLD,
		RENDER_SKYBOX,
		RENDER_DIRECT_LIGHT,
		RENDER_LIGHT_SHAFT,
		RENDER_LIGHT_ATLAS,
		RENDER_SKYBOX_REFLECTION,
		RENDER_FRUSTUM_INSIDE,
		RENDER_ONE_COLOR,
		RENDER_TRANSPARENT,
		RENDER_SPHERICAL_HARMONIC_DIFFUSE_MAP,


		TEXTURE_ID_DEFAULT,
		TEXTURE_ID_NORMAL_DEFAULT,

		TEXTURE_ID_HEIGHT_DEFAULT,
		TEXTURE_ID_HEIGHT_BUMP,
		TEXTURE_ID_HEIGHT_CLOUD,
		TEXTURE_ID_HEIGHT_CIRCLES,

		TEXTURE_ID_ICN_LIGHT,
		TEXTURE_ID_WHITE,
		TEXTURE_ID_RED,
		TEXTURE_ID_PROBE0,

		TEXTURE_TYPE_DEFAULT,
		TEXTURE_TYPE_DIFFUSE,
		TEXTURE_TYPE_NORMAL,
		TEXTURE_TYPE_PROPERTY,
		TEXTURE_TYPE_DISPLACEMENT,
		TEXTURE_TYPE_SPECULAR,

		TARGET_WORLD,
		TARGET_NORMAL,
		TARGET_DIFFUSE,
		TARGET_PROPERTY,
		TARGET_LIGHT_ATLAS,
		TARGET_LIGHTSHAFT_FRONT,
		TARGET_LIGHTSHAFT_BACK,
		TARGET_FINAL,
		TARGET_TEST,

		DEPTH_WORLD,
		DEPTH_FINAL,
		DEPTH_LIGHT_ATLAS,
		DEPTH_LIGHTSHAFT_FRONT,
		DEPTH_LIGHTSHAFT_BACK,
		DEPTH_TEST,

		SAMPLER_ID_WRAP,
		SAMPLER_ID_POINT,
		SAMPLER_ID_CLAMP,
		SAMPLER_ID_BORDER_ONE,
		SAMPLER_ID_BORDER_ZERO,
		SAMPLER_ID_LINEAR
		
	};
}