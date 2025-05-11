#include "DX3D/Graphics/DeviceContext.h"
dx3d::DeviceContext::DeviceContext(const DeviceContextDesc& desc, const GraphicsResourceDesc& gDesc) : GraphicsResource(gDesc), m_device_context(desc.context)
{
}

bool dx3d::DeviceContext::clearRenderTargetColor(std::shared_ptr<SwapChain> swap_chain,float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red, green, blue, alpha };
	m_device_context.ClearRenderTargetView(swap_chain->m_rtv.Get(),clear_color);
	return true;
}
