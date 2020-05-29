#include "RenderPass.h"

#include "Render/SwapChain/SwapChain.h"
#include "Render/Hardware/Device/Device.h"

#include <stdexcept>

#include "Log/Log.h"

namespace _GameEngine::_Render
{
	void RenderPass_build(RenderPass* p_renderPass, RenderPassBuildInfo* p_renderPassBuildInfo)
	{
		p_renderPass->RenderPassDependencies = *p_renderPassBuildInfo->RenderPassDependencies;

		VkAttachmentDescription l_colorAttachment{};
		l_colorAttachment.format = p_renderPass->RenderPassDependencies.SwapChain->SwapChainInfo.SurfaceFormat.format;
		l_colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		l_colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		l_colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		l_colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		l_colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		l_colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		l_colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference l_colorAttachmentReference{};
		l_colorAttachmentReference.attachment = 0;
		l_colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription l_subpass{};
		l_subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		l_subpass.colorAttachmentCount = 1;
		l_subpass.pColorAttachments = &l_colorAttachmentReference;

		VkSubpassDependency l_subpassDependency{};
		l_subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		l_subpassDependency.dstSubpass = 0;
		l_subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		l_subpassDependency.srcAccessMask = 0;
		l_subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		l_subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo l_renderPassCreateInfo{};
		l_renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		l_renderPassCreateInfo.attachmentCount = 1;
		l_renderPassCreateInfo.pAttachments = &l_colorAttachment;
		l_renderPassCreateInfo.subpassCount = 1;
		l_renderPassCreateInfo.pSubpasses = &l_subpass;
		l_renderPassCreateInfo.dependencyCount = 1;
		l_renderPassCreateInfo.pDependencies = &l_subpassDependency;

		if (vkCreateRenderPass(p_renderPass->RenderPassDependencies.SwapChain->SwapChainDependencies.Device->LogicalDevice.LogicalDevice, &l_renderPassCreateInfo, nullptr, &p_renderPass->renderPass)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create render pass!"));
		}
	};

	void RenderPass_free(RenderPass* p_renderPass)
	{
		vkDestroyRenderPass(p_renderPass->RenderPassDependencies.SwapChain->SwapChainDependencies.Device->LogicalDevice.LogicalDevice, p_renderPass->renderPass, nullptr);
	};
};