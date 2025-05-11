#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <DX3D/Core/Common.h>
#include <d3d11.h>
#include "DX3D/Graphics/SwapChain.h"
namespace dx3d {
	class DeviceContext : public GraphicsResource {
	public:
		explicit DeviceContext(const DeviceContextDesc& desc, const GraphicsResourceDesc& gDesc);
		bool clearRenderTargetColor(std::shared_ptr<SwapChain> swap_chain,float red, float green, float blue, float alpha);
	private:
		ID3D11DeviceContext& m_device_context;
	};
}