#pragma once
#include <DX3D/Core/Common.h>
namespace dx3d {
	class Base
	{
	public:
		Base(const BaseDesc& desc);
		virtual ~Base();
		virtual Logger& getLogger() const noexcept final;
	public:
		Base(const Base&) = default;
		Base(Base&&) = default;
		Base& operator = (const Base&) = default;
		Base& operator=(Base&&) = default;
	protected:
		Logger* m_logger{ nullptr };
	};
}


