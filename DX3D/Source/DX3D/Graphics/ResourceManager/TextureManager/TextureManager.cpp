#include "DX3D/Graphics/ResourceManager/TextureManager/TextureManager.h"
#include <DX3D/Graphics/ResourceManager/TextureManager/Texture.h>
std::shared_ptr<dx3d::Texture> dx3d::TextureManager::createTextureFromFile(const wchar_t* file_path)
{
	return std::static_pointer_cast<Texture>(createResourceFromFile(file_path));
}
std::shared_ptr<dx3d::Texture> dx3d::TextureManager::create2DSmokeTexture(int resolution)
{
	return std::make_shared<Texture>(resolution, GraphicsResourceDesc{ BaseDesc{m_logger}, m_renderSystem, m_device, m_factory });
}
dx3d::Resource* dx3d::TextureManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	Texture* tex = nullptr;
	try {
		tex = new Texture(file_path, GraphicsResourceDesc{ BaseDesc{m_logger}, m_renderSystem, m_device, m_factory });
	}
	catch (...) {}
	return tex;
}


