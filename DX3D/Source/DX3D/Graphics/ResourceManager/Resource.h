#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <string>
namespace dx3d {
	class Resource : public GraphicsResource
	{
	public:
		Resource(const GraphicsResourceDesc& desc) : GraphicsResource(desc) {}
		Resource(const wchar_t* full_path, const GraphicsResourceDesc& desc);
		virtual ~Resource();
	protected:
		std::wstring full_path{};
	};
}


