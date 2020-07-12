#include "Shader.h"

#include <stdexcept>

#include "VulkanObjects/Hardware/Device/Device.h"

extern "C"
{
#include "Log/Log.h"
#include "Read/File/File.h"
#include "DataStructures/Specifications/Array.h"
#include "ErrorHandling/Errorhandling.h"
}

namespace _GameEngine::_Render
{

	VkPipelineShaderStageCreateInfo Shader_buildShaderStageCreate(Shader* p_shader, VkShaderModule p_shaderModule)
	{
		VkPipelineShaderStageCreateInfo l_pipelineShaderStageCreate{};
		l_pipelineShaderStageCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

		if (p_shader->ShaderType == ShaderType::VERTEX)
		{
			l_pipelineShaderStageCreate.stage = VK_SHADER_STAGE_VERTEX_BIT;
		}
		else if (p_shader->ShaderType == ShaderType::FRAGMENT)
		{
			l_pipelineShaderStageCreate.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		l_pipelineShaderStageCreate.module = p_shaderModule;
		l_pipelineShaderStageCreate.pName = "main";

		return l_pipelineShaderStageCreate;
	};

	VkShaderModule Shader_allocateShaderModule(Shader* p_shader, Device* p_device)
	{
		VkShaderModule l_shaderModule;
		Core_GenericArray l_compiledShader;
		{
			Core_GenericArray l_shaderPath = Core_array_fromCStyle((void*)p_shader->ShaderPath.c_str(), sizeof(char), strlen(p_shader->ShaderPath.c_str()));
			ERR_THROW(Core_File_readFile_byte(&l_shaderPath, &l_compiledShader));
			VkShaderModuleCreateInfo l_shaderModuleCreateInfo{};
			l_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			l_shaderModuleCreateInfo.codeSize = l_compiledShader.Size;
			l_shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(l_compiledShader.Memory);

			if (vkCreateShaderModule(p_device->LogicalDevice.LogicalDevice, &l_shaderModuleCreateInfo, nullptr, &l_shaderModule) != VK_SUCCESS)
			{
				throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create shader module!"));
			}
		}
		Core_GenericArray_free(&l_compiledShader);
		return l_shaderModule;
	};

	void Shader_freeShaderModule(VkShaderModule p_shaderModule, Device* p_device)
	{
		vkDestroyShaderModule(p_device->LogicalDevice.LogicalDevice, p_shaderModule, nullptr);
	};
}