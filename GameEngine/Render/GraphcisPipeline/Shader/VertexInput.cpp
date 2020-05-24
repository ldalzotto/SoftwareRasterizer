#include "VertexInput.h"

#include "Render/Mesh/Mesh.h"

namespace _GameEngine::_Render::_Shader
{
	void VertexInput_buildInput(VertexInput* p_vertexInput)
	{
		p_vertexInput->VertexInputBinding = VkVertexInputBindingDescription{};
		p_vertexInput->VertexInputBinding.binding = 0;
		p_vertexInput->VertexInputBinding.stride = sizeof(_Mesh::Vertex);
		p_vertexInput->VertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		p_vertexInput->VertexInputAttributeDescriptions.resize(2);
		
		VkVertexInputAttributeDescription l_positionAttribute{};
		l_positionAttribute.binding = 0;
		l_positionAttribute.location = 0;
		l_positionAttribute.format = VK_FORMAT_R32G32_SFLOAT;
		l_positionAttribute.offset = offsetof(_Mesh::Vertex, pos);

		VkVertexInputAttributeDescription l_colorAttribute{};
		l_colorAttribute.binding = 0;
		l_colorAttribute.location = 1;
		l_colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
		l_colorAttribute.offset = offsetof(_Mesh::Vertex, color);

		p_vertexInput->VertexInputAttributeDescriptions.at(0) = l_positionAttribute;
		p_vertexInput->VertexInputAttributeDescriptions.at(1) = l_colorAttribute;
	};
};