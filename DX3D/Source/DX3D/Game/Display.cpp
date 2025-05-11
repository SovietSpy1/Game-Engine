#include "DX3D/Game/Display.h"
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
dx3d::Display::Display(const DisplayDesc& desc): Window(desc.window)
{
	m_swapChain = desc.rendererSystem.createSwapChain({m_handle, m_size});
	m_device_context = desc.rendererSystem.createDeviceContext();
	startCol = GameMath::RandColor();
	endCol = GameMath::RandColor();
	currentCol = startCol;
}
void dx3d::Display::onUpdate()
{
	u += Time::deltaTime * 0.5f;
	if (u >= 1) {
		u = 0;
		startCol = endCol;
		endCol = GameMath::RandColor();
	}
	currentCol = GameMath::ColorLerp(startCol, endCol, u);
	Window::onUpdate();
	m_device_context->clearRenderTargetColor(m_swapChain,currentCol.red, currentCol.green, currentCol.blue, currentCol.alpha);
	m_swapChain->present(false);
}
