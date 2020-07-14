#include "RenderInterface.h"

#include "Render.h"

namespace _GameEngine::_Render
{
	void RenderInterface_initialize(Render* p_render, ::_Core::Log* p_myLog)
	{
		RenderInterface* l_renderInterface = &p_render->RenderInterface;
		l_renderInterface->MyLog = p_myLog;
		l_renderInterface->RenderHookCallbacksInterface.RenderHookCallbacks = &p_render->RenderHookCallbacks;

		l_renderInterface->Window = &p_render->Window;
		l_renderInterface->WindowSurface = &p_render->WindowSurface;
		l_renderInterface->Instance = &p_render->Instance;
		l_renderInterface->ValidationLayers = &p_render->ValidationLayers;
		l_renderInterface->RenderDebug = &p_render->RenderDebug;
		l_renderInterface->Device = &p_render->Device;
		l_renderInterface->SwapChain = &p_render->SwapChain;
		l_renderInterface->DepthTexture = &p_render->DepthTexture;
		l_renderInterface->CommandPool = &p_render->CommandPool;
		l_renderInterface->RenderSemaphore = &p_render->RenderSemaphore;
		l_renderInterface->TextureSamplers = &p_render->TextureSamplers;
		l_renderInterface->TextureSwapChainSizeSynchronizer = &p_render->TextureSwapChainSizeSynchronizer;
		l_renderInterface->MaterialInstanceContainer = &p_render->MaterialInstanceContainer;

		l_renderInterface->ResourceProvidersInterface.MeshResourceProvider = &p_render->ResourceProviders.MeshResourceProvider;
		l_renderInterface->ResourceProvidersInterface.TextureResourceProvider = &p_render->ResourceProviders.TextureResourceProvider;
		l_renderInterface->ResourceProvidersInterface.MaterialResourceProvider = &p_render->ResourceProviders.MaterialResourceProvider;

		l_renderInterface->PreRenderDeferedCommandBufferStep = &p_render->PreRenderDeferedCommandBufferStep;
		l_renderInterface->PushCameraBuffer = &p_render->PushCameraBuffer;
		l_renderInterface->Gizmo = &p_render->Gizmo;
	};
}