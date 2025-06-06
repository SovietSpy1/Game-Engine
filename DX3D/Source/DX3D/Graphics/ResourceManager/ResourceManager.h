#pragma once
#include <DX3D/Core/Common.h>
#include <DX3D/Core/Base.h>
#include <unordered_map>
#include <string>
namespace dx3d {
	class ResourceManager : public Base
	{
	public:
		ResourceManager(const BaseDesc& desc);
		virtual ~ResourceManager() {};
		std::shared_ptr<Resource> createResourceFromFile(const wchar_t* file_path);
	protected:
		virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) = 0;
	private:
		std::unordered_map<std::wstring, std::shared_ptr<Resource>> map_resources{};
	};
}


