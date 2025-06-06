#pragma once
#include "DX3D/Graphics/ResourceManager/ResourceManager.h"
namespace dx3d {
	class TextureManager : public ResourceManager
	{
	public:
		TextureManager(const BaseDesc& desc) : ResourceManager(desc)
		{
			// Initialization code for the texture manager
		}
		Resource* createResourceFromFileConcrete(const wchar_t* file_path);
	};
}


