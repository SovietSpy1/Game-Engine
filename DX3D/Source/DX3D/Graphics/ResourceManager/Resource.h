#pragma once
#include <DX3D/Core/Base.h>
#include <string>
namespace dx3d {
	class Resource : public Base
	{
	public:
		Resource(const wchar_t* full_path, const BaseDesc& desc);
		virtual ~Resource();
	protected:
		std::wstring full_path{};
	};
}


