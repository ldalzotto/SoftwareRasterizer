
#pragma once

#include <string.h>
#include "vulkan/vulkan.h"

#include "Render/VulkanObjects/Descriptor/DescriptorPool.h"
#include "Render/VulkanObjects/GraphcisPipeline/GraphicsPipeline.h"
#include "Render/Shader/Shader.h"
#include "Render/Shader/VertexInput.h"
#include "Render/Shader/DescriptorSetLayout.h"
#include "Render/Shader/ShaderParameter.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct DefaultMaterialV2_ExternalResources
	{
		Shader VertexShader;
		Shader FragmentShader;
	};

	struct DefaultMaterialV2_InternalResources
	{
		Texture* DepthBufferTexture;
	};

	struct DefaultMaterialV2_LocalInputParameters
	{
		VertexInput VertexInput;
		UniformBufferParameter ModelMatrix;
		ImageSampleParameter BaseTexture;

		DescriptorSetLayout DescriptorSetLayout;
		DescriptorPool DescriptorPool;
	};

	struct DefaultMaterialV2
	{
		struct FinalDrawObjects
		{
			VkPipelineLayout PipelineLayout;
			GraphicsPipeline GraphicsPipeline;
		};

		DefaultMaterialV2_ExternalResources ExternalResources;
		DefaultMaterialV2_InternalResources InternalResources;
		DefaultMaterialV2_LocalInputParameters LocalInputParameters;
		FinalDrawObjects FinalDrawObjects;
	};

	void DefaultMaterial_alloc(DefaultMaterialV2* p_defaultMaterialV2, RenderInterface* p_renderInterface);
	void DefaultMaterial_free(DefaultMaterialV2* p_defaultMaterialV2, RenderInterface* p_renderInterface);
	void DefaultMaterial_reAllocGraphicsPipeline(DefaultMaterialV2* p_defaultMaterialV2, RenderInterface* p_renderInterface);
};