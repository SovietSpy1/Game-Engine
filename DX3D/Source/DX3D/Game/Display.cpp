#include "DX3D/Game/Display.h"
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Math/GameMath.h>
dx3d::Display::Display(const DisplayDesc& desc) : Window(desc.window)
{
	m_swapChain = desc.rendererSystem.createSwapChain({ m_handle, m_size });
	m_device_context = desc.rendererSystem.createDeviceContext();
	m_vb = desc.rendererSystem.createVertexBuffer();
	startColV1 = RandColor(1);
	startColV2 = RandColor(1);
	startColV3 = RandColor(1);
	list[0] = { {0.0f, 0.5f, 0.0f}, startColV1.rgba, {0.5f,0.0f} };
	list[1] = { {0.5f, -0.5f, 0.0f}, startColV2.rgba, {1.0f,1.0f} };
	list[2] = { {-0.5f,-0.5f,0.0f}, startColV3.rgba, {0.0f,1.0f} };
	size_list = ARRAYSIZE(list);
	const WCHAR* vertexShaderPath = L"DX3D/Source/DX3D/Graphics/VertexShader.hlsl";
	shader_blob = m_device_context->loadShaderBlob(vertexShaderPath);
	m_vb->load(list, sizeof(vertex), size_list, shader_blob->GetBufferPointer(), shader_blob->GetBufferSize());
	m_device_context->createVertexShader(shader_blob);
	const WCHAR* pixelShaderPath = L"DX3D/Source/DX3D/Graphics/PixelShader.hlsl";
	pixelshader_blob = m_device_context->loadPixelShaderBlob(pixelShaderPath);
	m_device_context->createPixelShader(pixelshader_blob);
	m_device_context->createTransparentBlendState();
	currentCol = Color{ 0.01f,0.01f,0.01f,1.0f };
	endColV1 = RandColor(1);
	endColV2 = RandColor(1);
	endColV3 = RandColor(1);
}
void dx3d::Display::onUpdate()
{
	Window::onUpdate();
	u += Time::deltaTime * 0.33f;
	if (u >= 1) {
		u = 0;
		startColV1 = endColV1;
		startColV2 = endColV2;
		startColV3 = endColV3;
		endColV1 = RandColor(1);
		endColV2 = RandColor(1);
		endColV3 = RandColor(1);
	}
	list[0] = { {0.0f, 0.5f, 0.0f}, ColorLerp(startColV1, endColV1, u).rgba, {0.5f,0.0f} };
	list[1] = { {0.5f, -0.5f, 0.0f}, ColorLerp(startColV2, endColV2, u).rgba, {1.0f,1.0f} };
	list[2] = { {-0.5f,-0.5f,0.0f}, ColorLerp(startColV3, endColV3, u).rgba, {0.0f,1.0f} };
	m_vb->load(list, sizeof(vertex), size_list, shader_blob->GetBufferPointer(), shader_blob->GetBufferSize());
	m_device_context->clearRenderTargetColor(m_swapChain,currentCol.rgba);
	m_device_context->loadShaders();
	m_device_context->SetViewportSize(m_size.width, m_size.height);
	m_device_context->setVertexBuffer(m_vb);
	m_device_context->setBlendState();
	m_device_context->drawTriangleList(m_vb->getSizeVertexList(), 0);
	m_swapChain->present(false);
}
