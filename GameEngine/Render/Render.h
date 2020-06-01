#pragma once


#include "Render/Hardware/Window/Window.h"
#include "Render/Hardware/Window/Surface.h"


#include "vulkan/vulkan.h"
#include "ValidationLayers.h"
#include "Hardware/Device/Device.h"
#include "SwapChain/SwapChain.h"
#include "Texture/Texture.h"
#include "Descriptor/DescriptorPool.h"
#include "CommandBuffer/CommandPool.h"
#include "CommandBuffer/CommandBuffers.h"
#include "Synchronisation/Semaphore.h"
#include "Texture/TextureSamplers.h"
#include "LoopStep/PreRenderDeferedCommandBufferStep.h"
#include "LoopStep/CameraBufferSetupStep.h"
#include "LoopStep/DefaultMaterialDrawStep.h"
#include "Materials/DefaultMaterialV2.h"
#include "Resources/TextureResourceProvider.h"
#include "Resources/MeshResourceProvider.h"

namespace _GameEngine::_Render
{
	struct RenderDebug
	{
		VkDebugUtilsMessengerEXT DebugMessenger;
		PFN_vkCreateDebugUtilsMessengerEXT PfnCreateDebugUtilsMessengerEXT;
		PFN_vkDestroyDebugUtilsMessengerEXT PfnDestroyDebugUtilsMessengerEXT;
	};

	struct RenderMaterials
	{
		DefaultMaterialV2 DefaultMaterial;
	};

	struct ResourceProviders
	{
		TextureResourceProvider TextureResourceProvider;
		MeshResourceProvider MeshResourceProvider;
	};

	struct Render
	{
		Window Window;
		Surface WindowSurface;
		VkInstance Instance;
		ValidationLayers ValidationLayers;
		RenderDebug RenderDebug;
		Device Device;
		SwapChain SwapChain;
		Texture* DepthTexture;
		CommandPool CommandPool;
		CommandBuffer CommandBuffers;
		RenderSemaphore RenderSemaphore;
		TextureSamplers TextureSamplers;
		ResourceProviders ResourceProviders;

		RenderMaterials RenderMaterials;
		PreRenderDeferedCommandBufferStep PreRenderDeferedCommandBufferStep;
		CameraBufferSetupStep CameraBufferSetupStep;
		DefaultMaterialDrawStep DefaultMaterialDrawStep;
	};

	Render* Render_alloc();
	void Render_free(Render** p_render);
	void Render_recreateSwapChain(Render* p_render);

	void Render_render(Render* p_render);

} // namespace _GameEngine::_Render
