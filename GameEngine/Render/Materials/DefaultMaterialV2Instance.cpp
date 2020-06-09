#include "DefaultMaterialV2Instance.h"

#include "RenderInterface.h"

#include <stdexcept>
#include "Log/Log.h"

#include "DefaultMaterialV2.h"

#include "VulkanObjects/Hardware/Device/Device.h"
#include "LoopStep/DefaultMaterialDrawStep.h"

#include "Resources/MeshResourceProvider.h"
#include "Resources/TextureResourceProvider.h"

namespace _GameEngine::_Render
{

	void createModelMatrixBuffer(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device);
	void freeModelMatrixBuffer(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device);

	void createDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device);
	void updateDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device);
	void freeDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device);

	void DefaultMaterialV2Instance_alloc(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, RenderInterface* p_renderInterface, DefaultMaterialV2DrawerAllocInfo* p_defaultMaterialV2InstanceAllocInfo)
	{
		{
			p_defaultMaterialV2Instance->_DefaultMaterial = p_defaultMaterialV2InstanceAllocInfo->DefaultMaterial;
		}
		
		{
			MeshResourceProviderUseResourceInfo l_meshResourceProviderUserResource{};
			l_meshResourceProviderUserResource.Meshpath = p_defaultMaterialV2InstanceAllocInfo->DefaultMaterialV2Instance_InputAssets->MeshPath;
			p_defaultMaterialV2Instance->ExternalResources.Mesh =
				MeshResourceProvider_UseResource(p_renderInterface->ResourceProvidersInterface.MeshResourceProvider, &l_meshResourceProviderUserResource);
		}

		{
			TextureUniqueKey l_textureUniqueKey{};
			l_textureUniqueKey.TexturePath = p_defaultMaterialV2InstanceAllocInfo->DefaultMaterialV2Instance_InputAssets->Texturepath;
			p_defaultMaterialV2Instance->ExternalResources.Texture =
				TextureResourceProvider_UseResource(p_renderInterface->ResourceProvidersInterface.TextureResourceProvider, &l_textureUniqueKey);
		}
		
		createModelMatrixBuffer(p_defaultMaterialV2Instance, p_renderInterface->Device);

		createDescriptorSet(p_defaultMaterialV2Instance, p_renderInterface->Device);
		updateDescriptorSet(p_defaultMaterialV2Instance, p_renderInterface->Device);
	};

	void DefaultMaterialV2Instance_free(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, RenderInterface* p_renderInterface)
	{
		freeModelMatrixBuffer(p_defaultMaterialV2Instance, p_renderInterface->Device);
		freeDescriptorSet(p_defaultMaterialV2Instance, p_renderInterface->Device);
		MeshResourceProvider_ReleaseResource(p_renderInterface->ResourceProvidersInterface.MeshResourceProvider, &p_defaultMaterialV2Instance->ExternalResources.Mesh->MeshUniqueKey);
		TextureResourceProvider_ReleaseResource(p_renderInterface->ResourceProvidersInterface.TextureResourceProvider, &p_defaultMaterialV2Instance->ExternalResources.Texture->TextureUniqueKey);
	};

	void DefaultMaterialV2Instance_setModelMatrix(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, RenderInterface* p_renderInterface, ModelProjection* p_modelProjection)
	{
		VulkanBuffer_pushToGPU(&p_defaultMaterialV2Instance->ModelMatrixBuffer, p_renderInterface->Device, p_modelProjection, sizeof(ModelProjection));
	};

	void createModelMatrixBuffer(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device)
	{
		p_defaultMaterialV2Instance->ModelMatrixBuffer = UniformBufferParameter_allocVulkanBuffer(&p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.ModelMatrix, p_device);
	};

	void freeModelMatrixBuffer(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device)
	{
		_Render::VulkanBuffer_free(&p_defaultMaterialV2Instance->ModelMatrixBuffer, p_device);
	};

	void updateDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device)
	{
		std::vector<VkWriteDescriptorSet> l_writeDescirptorSets(2);

		VkDescriptorBufferInfo l_decriptorBufferInfo{};
		l_writeDescirptorSets[0] = UniformBufferParameter_buildWriteDescriptorSet(
			&p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.ModelMatrix,
			&p_defaultMaterialV2Instance->ModelMatrixBuffer,
			&l_decriptorBufferInfo,
			p_defaultMaterialV2Instance->MaterialDescriptorSet);

		VkDescriptorImageInfo l_descriptorImageInfo{};
		l_writeDescirptorSets[1] = ImageSampleParameter_buildWriteDescriptorSet(
			&p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.BaseTexture,
			p_defaultMaterialV2Instance->ExternalResources.Texture,
			&l_descriptorImageInfo,
			p_defaultMaterialV2Instance->MaterialDescriptorSet);

		vkUpdateDescriptorSets(p_device->LogicalDevice.LogicalDevice, l_writeDescirptorSets.size(), l_writeDescirptorSets.data(), 0, nullptr);
	};

	void createDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device)
	{
		VkDescriptorSetAllocateInfo l_descriptorSetAllocateInfo{};
		l_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		l_descriptorSetAllocateInfo.descriptorPool = p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.DescriptorPool.DescriptorPool;
		l_descriptorSetAllocateInfo.descriptorSetCount = 1;
		l_descriptorSetAllocateInfo.pSetLayouts = &p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.DescriptorSetLayout.DescriptorSetLayout;
		if (vkAllocateDescriptorSets(p_device->LogicalDevice.LogicalDevice, &l_descriptorSetAllocateInfo, &p_defaultMaterialV2Instance->MaterialDescriptorSet) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create description set."));
		};
	};

	void freeDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device)
	{
		vkFreeDescriptorSets(p_device->LogicalDevice.LogicalDevice, p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.DescriptorPool.DescriptorPool, 1, &p_defaultMaterialV2Instance->MaterialDescriptorSet);
	};
};