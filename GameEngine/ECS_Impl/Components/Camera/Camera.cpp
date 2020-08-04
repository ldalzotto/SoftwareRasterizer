#include "Camera.h"

#include "v2/Math.h"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Vector/Vector.hpp"
#include "v2/Vector/VectorMath.hpp"

#include "Render/RenderInterface.h"
#include "Render/VulkanObjects/SwapChain/SwapChain.h"
#include "Render/VulkanObjects/Hardware/Window/Window.h"

#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	ComponentType CameraType = "Camera";

	void camera_onSwapChainBuild(Camera* p_camera, _Render::RenderInterface* p_renderInterface)
	{
		Camera_buildProjectionMatrix(p_camera);
	};

	void camera_onDetached(Camera* p_camera, ECS*)
	{
		_Core::ObserverT_unRegister(&p_camera->RenderInterface->SwapChain->OnSwapChainBuilded, (_Core::CallbackT<void, _Render::RenderInterface>*) & p_camera->OnSwapChainBuilded);
	};

	void Camera_init(Camera* p_camera, _Render::RenderInterface* p_renderInterface)
	{
		p_camera->RenderInterface = p_renderInterface;
		p_camera->OnSwapChainBuilded = { camera_onSwapChainBuild, p_camera };
		_Core::ObserverT_register(&p_camera->RenderInterface->SwapChain->OnSwapChainBuilded, (_Core::CallbackT<void, _Render::RenderInterface>*) & p_camera->OnSwapChainBuilded);

		p_camera->ComponentHeader.OnComponentFree = camera_onDetached;

		Camera_buildProjectionMatrix(p_camera);
	};

	_MathV2::Matrix4x4<float> Camera_worldToClipMatrix(Camera* p_camera)
	{
		_MathV2::Matrix4x4<float> l_return;
		_MathV2::MatrixM::mul(&p_camera->ProjectionMatrix, &p_camera->ViewMatrix, &l_return);
		return l_return;
	};

	void Camera_buildProjectionMatrix(Camera* p_camera)
	{
		_MathV2::MatrixM::perspective<float>(45.0f * _MathV2::DEG_TO_RAD,
			((float)p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.width / (float)p_camera->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.height), 0.1f, 50.0f, &p_camera->ProjectionMatrix);
	};

	_MathV2::SegmentV2<3, float> Camera_buildWorldSpaceRay(Camera* p_camera, _MathV2::Vector<2, float>& p_screenPoint)
	{
		_MathV2::Vector3<float> tmp_vec3_0, tmp_vec3_1; _MathV2::Vector4<float> tmp_vec4_0, tmp_vec4_1, tmp_vec4_2, tmp_vec4_3;

		tmp_vec3_0 = { p_screenPoint.x, p_screenPoint.y, 1.0f };
		_MathV2::Vector2<float> l_graphicsAPIPixelCoord = *_MathV2::VectorM::cast(
			_MathV2::MatrixM::mul(
				&p_camera->RenderInterface->Window->WindowToGraphicsAPIPixelCoordinates,
				&tmp_vec3_0,
				&tmp_vec3_1)
		);

		_MathV2::Matrix4x4<float> l_clipToWorldMatrix; _MathV2::MatrixM::inv(&Camera_worldToClipMatrix(p_camera), &l_clipToWorldMatrix);

		tmp_vec4_0 = { l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, -1.0f, 1.0f };
		tmp_vec4_1 = { l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, 1.0f, 1.0f };
		return
			_MathV2::SegmentV2<3, float>{
			/*Near plane*/ *_MathV2::VectorM::cast(_MathV2::MatrixM::clipSpaceMul(&l_clipToWorldMatrix, &tmp_vec4_0, &tmp_vec4_2)),
			/*Far plane*/* _MathV2::VectorM::cast( _MathV2::MatrixM::clipSpaceMul(&l_clipToWorldMatrix, &tmp_vec4_1, &tmp_vec4_3))
		};
	};
}