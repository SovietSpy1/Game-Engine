#pragma once
#include "DX3D/Graphics/ResourceManager/ResourceManager.h"
namespace dx3d {
	class TextureManager : public ResourceManager
	{
	public:
		TextureManager(const GraphicsResourceDesc& desc) : ResourceManager(desc)
		{
			// Initialization code for the texture manager
		}
		std::shared_ptr<Texture> createTextureFromFile(const wchar_t* file_path);
		std::shared_ptr<Texture> create2DSmokeTexture(int resolution, bool t);
	protected:
		Resource* createResourceFromFileConcrete(const wchar_t* file_path);
	private:
		Resource* createResourceConcrete(const wchar_t* name) 
		{
			// This method is not used in TextureManager, but must be defined due to inheritance
			return nullptr; // or throw an exception if this should not be called
		}
	};
}


