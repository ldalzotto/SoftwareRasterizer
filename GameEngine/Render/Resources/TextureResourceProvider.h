#pragma once

#include <unordered_map>
#include "Utils/Resource/UsageCounter.h"
#include "Functional/RAII/SharedRAIIT.hpp"
#include "Texture/Texture.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct TextureResourceProvider
	{
		RenderInterface* RenderInterface;
		std::unordered_map<size_t, _Core::SharedRAIIT<Texture, TextureResourceProvider>> TextureResources;
	};

	void TextureResourceProvider_Clear(TextureResourceProvider* p_textureResourceProvider);
	Texture* TextureResourceProvider_UseResource(TextureResourceProvider* p_textureResourceProvider, TextureUniqueKey* p_key);
	void TextureResourceProvider_ReleaseResource(TextureResourceProvider* p_textureResourceProvider, TextureUniqueKey* p_key);
}