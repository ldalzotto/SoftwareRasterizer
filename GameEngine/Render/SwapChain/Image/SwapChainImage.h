#pragma once

#include <vector>
#include "Render/CommandBuffer/CommandBuffers.h"
#include "ImageViews.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct CommandPool;
	struct SwapChainInfo;
}

namespace _GameEngine::_Render
{
	struct SwapChainImage
	{
		VkImage SwapChainImage;
		ImageView ImageView;
		CommandBuffer CommandBuffer;
	};

	struct SwapChainImageInitializationInfo
	{
		VkImage CreatedImage;
		SwapChainInfo* SwapChainInfo;
		Device* Device;
		CommandPool* CommandPool;
	};

	void SwapChainImage_init(SwapChainImage* p_swapChainImage, SwapChainImageInitializationInfo* p_swapChainImageInitializationInfo);
	void SwapChainImage_free(SwapChainImage* p_swapChainImage);
};