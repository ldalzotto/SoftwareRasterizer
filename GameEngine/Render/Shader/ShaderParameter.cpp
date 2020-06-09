#include "ShaderParameter.h"

#include "VulkanObjects/Hardware/Device/Device.h"
#include "Texture/Texture.h"

namespace _GameEngine::_Render
{
	VkDescriptorSetLayoutBinding UniformBufferParameter_buildLayoutBinding(UniformBufferParameter* p_uniformBufferParameter)
	{
		VkDescriptorSetLayoutBinding l_modelLayoutBinding{};
		l_modelLayoutBinding.binding = p_uniformBufferParameter->Binding;
		l_modelLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_modelLayoutBinding.descriptorCount = 1;
		l_modelLayoutBinding.stageFlags = p_uniformBufferParameter->StageFlag;
		l_modelLayoutBinding.pImmutableSamplers = nullptr;
		return std::move(l_modelLayoutBinding);
	};

	VulkanBuffer UniformBufferParameter_allocVulkanBuffer(UniformBufferParameter* p_uniformBufferParameter, Device* p_device)
	{
		VulkanBuffer l_buffer{};
		_Render::BufferAllocInfo l_bufferAllocInfo{};
		l_bufferAllocInfo.Size = p_uniformBufferParameter->BufferSize;
		l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		_Render::VulkanBuffer_alloc(&l_buffer, &l_bufferAllocInfo, p_device);
		return std::move(l_buffer);
	};

	VkWriteDescriptorSet UniformBufferParameter_buildWriteDescriptorSet(UniformBufferParameter* p_uniformBufferParameter, VulkanBuffer* p_buffer,
		VkDescriptorBufferInfo* p_descriptorBufferInfo, VkDescriptorSet p_descriptorSet)
	{
		p_descriptorBufferInfo->buffer = p_buffer->Buffer;
		p_descriptorBufferInfo->offset = 0;
		p_descriptorBufferInfo->range = p_buffer->BufferAllocInfo.Size;

		VkWriteDescriptorSet l_descriptorUniforBufferWrite{};
		l_descriptorUniforBufferWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		l_descriptorUniforBufferWrite.dstSet = p_descriptorSet;
		l_descriptorUniforBufferWrite.dstBinding = p_uniformBufferParameter->Binding;
		l_descriptorUniforBufferWrite.dstArrayElement = 0;
		l_descriptorUniforBufferWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_descriptorUniforBufferWrite.descriptorCount = 1;
		l_descriptorUniforBufferWrite.pBufferInfo = p_descriptorBufferInfo;
		l_descriptorUniforBufferWrite.pImageInfo = nullptr;
		l_descriptorUniforBufferWrite.pTexelBufferView = nullptr;
		return std::move(l_descriptorUniforBufferWrite);
	};


	VkDescriptorSetLayoutBinding ImageSampleParameter_buildLayoutBinding(ImageSampleParameter* p_imageSampleParameter)
	{
		VkDescriptorSetLayoutBinding l_textureSamplerBinding{};
		l_textureSamplerBinding.binding = p_imageSampleParameter->Binding;
		l_textureSamplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		l_textureSamplerBinding.descriptorCount = 1;
		l_textureSamplerBinding.stageFlags = p_imageSampleParameter->StageFlag;
		l_textureSamplerBinding.pImmutableSamplers = nullptr;
		return std::move(l_textureSamplerBinding);
	};

	VkWriteDescriptorSet ImageSampleParameter_buildWriteDescriptorSet(ImageSampleParameter* p_imageSampleParameter, Texture* p_texture, VkDescriptorImageInfo* p_descriptorImageInfo, VkDescriptorSet p_descriptorSet)
	{
		p_descriptorImageInfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		p_descriptorImageInfo->imageView = p_texture->ImageView.ImageView;
		p_descriptorImageInfo->sampler = p_imageSampleParameter->TextureSampler;

		VkWriteDescriptorSet l_imageDescriptorSet{};
		l_imageDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		l_imageDescriptorSet.dstSet = p_descriptorSet;
		l_imageDescriptorSet.dstBinding = p_imageSampleParameter->Binding;
		l_imageDescriptorSet.dstArrayElement = 0;
		l_imageDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		l_imageDescriptorSet.descriptorCount = 1;
		l_imageDescriptorSet.pImageInfo = p_descriptorImageInfo;
		return std::move(l_imageDescriptorSet);
	};
};