#pragma once

extern "C"
{
#include "v2/_interface/SegmentC_def.h"
#include "v2/_interface/FrustumC_def.h"
#include "v2/_interface/MatrixC_def.h"
}

#include "ECS/ComponentT.hpp"

typedef struct RenderV2Interface_TYP RenderV2Interface;

namespace _GameEngine::_ECS
{
	extern ComponentType CameraType;

	struct Camera
	{
		_ECS::ComponentHeaderT<Camera> ComponentHeader;
		RenderV2Interface* RenderInterface;
		MATRIX4F ProjectionMatrix;
		MATRIX4F ViewMatrix;
		FRUSTUM CameraFrustum;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<Camera>()
	{
		return &CameraType;
	};

	void Camera_init(Camera* p_camera, RenderV2Interface* p_renderInterface);
	void Camera_buildProjectionMatrix(Camera* p_camera);

	MATRIX4F Camera_worldToClipMatrix(Camera* p_camera);

	SEGMENT_VECTOR3F Camera_buildWorldSpaceRay(Camera* p_camera, VECTOR2F_PTR p_screenPoint);
}