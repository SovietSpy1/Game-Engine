#pragma once
#include <DX3D/Core/Base.h>
namespace dx3d {
	class ComponentBase : public Base
	{
	public:
		ComponentBase(const BaseDesc& desc) : Base(desc) {}
		virtual ~ComponentBase() = default;
	};
	template<typename ComponentType>
	class Component : public ComponentBase {
	public:
		Component(const BaseDesc& desc, std::shared_ptr<ComponentType> component) : ComponentBase(desc), component{ component } {}
		std::shared_ptr<ComponentType> component;
		ComponentType* Get() {
			return component.get();
		}
	};
}


