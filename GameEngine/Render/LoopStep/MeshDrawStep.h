#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "vulkan/vulkan.h"
#include "Render/Mesh/Mesh.h"
#include "Render/GraphcisPipeline/GraphicsPipeline.h"

namespace _GameEngine::_Render
{
	struct MeshUniformObject
	{
		glm::mat4x4 Model;
		glm::mat4x4 View;
		glm::mat4x4 Project;
	};

	struct MeshDrawCommand
	{
		Mesh* Mesh;
		VulkanBuffer MeshUniformBuffer;
		VkDescriptorSet DescriptorSet;
	};

	struct MeshDrawStep
	{
		std::vector<MeshDrawCommand*> MeshDrawCommands;
	};

	void MeshDrawStep_buildCommandBuffer(MeshDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer, GraphicsPipeline* p_bindedGraphicsPipeline);
}