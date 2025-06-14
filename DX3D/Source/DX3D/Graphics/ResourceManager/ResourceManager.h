#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <unordered_map>
#include <string>
namespace dx3d {
	class ResourceManager : public GraphicsResource
	{
	public:
		ResourceManager(const GraphicsResourceDesc& desc);
		virtual ~ResourceManager() {};
		std::shared_ptr<Resource> createResourceFromFile(const wchar_t* file_path);
		std::shared_ptr<Resource> createResourceLocal(const wchar_t* name);
	protected:
		virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) = 0;
		virtual Resource* createResourceConcrete(const wchar_t* name) = 0;
	private:
		std::unordered_map<std::wstring, std::shared_ptr<Resource>> map_resources{};
	};
}


