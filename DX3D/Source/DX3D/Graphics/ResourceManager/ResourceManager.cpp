#include "DX3D/Graphics/ResourceManager/ResourceManager.h"
#include <DX3D/Graphics/ResourceManager/Resource.h>
#include <filesystem>
dx3d::ResourceManager::ResourceManager(const GraphicsResourceDesc& desc) : GraphicsResource(desc)
{
}
std::shared_ptr<dx3d::Resource> dx3d::ResourceManager::createResourceFromFile(const wchar_t* file_path)
{
	std::wstring full_path = std::filesystem::absolute(file_path);
	auto it = map_resources.find(full_path);
	if (it != map_resources.end()) {
		return it->second; 
	}
	Resource* raw_res = createResourceFromFileConcrete(full_path.c_str());
	if (raw_res) {
		std::shared_ptr<Resource> res(raw_res);
		map_resources[full_path] = res;
		return res;
	}
	return nullptr;
}
