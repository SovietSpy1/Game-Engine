#pragma once
#include <DX3D/Core/Base.h>
namespace dx3d {
	class Component : public Base
	{
		Component(const BaseDesc& desc) :Base(desc) {};
		virtual ~Component() {}
	};
}


