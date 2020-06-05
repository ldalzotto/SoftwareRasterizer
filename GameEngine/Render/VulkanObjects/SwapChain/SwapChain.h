#pragma once

#include <vector>
#include <functional>

#include "VulkanObjects/SwapChain/SwapChainSharedStructures.h"
#include "VulkanObjects/SwapChain/Image/SwapChainImage.h"
#include "Utils/Observer/Observer.h"

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;

	struct Device;
	struct Window;
	struct Surface;

	struct CommandPool;
}

namespace _GameEngine::_Render
{

	struct SwapChainDependencies
	{
		Device* Device;
		Window* Window;
		Surface* Surface;
	};

	struct SwapChain
	{
		VkSwapchainKHR VkSwapchainKHR;
		SwapChainDependencies SwapChainDependencies;
		SwapChainInfo SwapChainInfo;
		std::vector<SwapChainImage> SwapChainImages;

		bool MustBeRebuilt;
		_Utils::Subject OnWindowSizeChangeCallback;

		// The swap chain can be rebuild if the @ref MustBeRebuilt is set to true.
		// This event notify when such an event has occured.
		// Input parameter is (SwapChain*)
		_Utils::Observer OnSwapChainBuilded;
	};

	struct SwapChainSupportDetails
	{
		/**
			Number and dimension of images.
		*/
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> SurfaceFormats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	struct SwapChainBuildInfo
	{
		SwapChainDependencies SwapChainDependencies;
		CommandPool* CommandPool;
	};

	void SwapChain_build(SwapChain* p_swapChain, SwapChainBuildInfo* p_swapChainBuildInfo);
	SwapChainSupportDetails SwapChain_getSupportDetails(VkPhysicalDevice p_physicalDevice, Surface* p_surface);
	bool SwapChain_isSwapChainSupported(const SwapChainSupportDetails& p_swapCahinSupportDetails);
	void SwapChain_broadcastRebuildEvent(SwapChain* p_swapChain, RenderInterface* p_renderInterface);
	void SwapChain_free(SwapChain* p_swapChain);

}