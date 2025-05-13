#include "DX3D/Game/Display.h"
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Math/GameMath.h>
#include <DX3D/Object/Mesh.h>
dx3d::Display::Display(const DisplayDesc& desc) : Window(desc.window)
{
	m_swapChain = desc.rendererSystem.createSwapChain({ m_handle, m_size });
	m_device_context = desc.rendererSystem.createDeviceContext();
	m_vb = desc.rendererSystem.createVertexBuffer();
	//setting up mesh with the heart vertices and heart shaders
	m_mesh = desc.rendererSystem.createMesh();
	m_mesh->LoadHeartMesh();
	//loading mesh values into displays values
	list = m_mesh->vertices;
	size_list = list.size(); 
	m_vertexShader = m_mesh->vertexShader;
	m_pixelShader = m_mesh->pixelShader;
	//load the vertex buffer with the mesh's vertices and shader
	m_vb->load(list.data(), sizeof(vertex), size_list, m_mesh->vertexBlob->GetBufferPointer(), m_mesh->vertexBlob->GetBufferSize());
	m_device_context->loadShaders(m_vertexShader, m_pixelShader);
	m_device_context->createTransparentBlendState();
	m_device_context->SetViewportSize(m_size.width, m_size.height);
	m_device_context->setVertexBuffer(m_vb);
	m_device_context->setBlendState();
	m_device_context->createBackfaceRasterizerState();
	m_device_context->setRasterState();
	m_device_context->createConstantBuffer();
	currentCol = Color{ 0.01f,0.01f,0.01f,1.0f };
}
void dx3d::Display::onUpdate()
{
	Window::onUpdate();
	m_device_context->clearRenderTargetColor(m_swapChain,currentCol.rgba);
	m_device_context->setConstantBuffer(ConstantBuffer{ Time::elapsedTime });
	m_device_context->drawTriangleList(m_vb->getSizeVertexList(), 0);
	m_swapChain->present(false);
}

dx3d::Display::~Display()
{
}
