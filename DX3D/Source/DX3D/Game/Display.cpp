#include "DX3D/Game/Display.h"
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Math/GameMath.h>
#include <DX3D/Object/Mesh.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
dx3d::Display::Display(const DisplayDesc& desc) : Window(desc.window)
{
	m_swapChain = desc.rendererSystem.createSwapChain({ m_handle, m_size });
	m_device_context = desc.rendererSystem.createDeviceContext();
	m_vb = desc.rendererSystem.createVertexBuffer();
	constantBuffer = desc.rendererSystem.createConstantBuffer();
	indexBuffer = desc.rendererSystem.createIndexBuffer();
	//setting up mesh with the heart vertices and heart shaders
	m_mesh = desc.rendererSystem.createMesh();
	m_mesh->LoadCubeMesh();
	//loading mesh values into displays values
	list = m_mesh->vertices;
	size_list = list.size();
	unsigned int index_list[] =
	{
		//front
		0,1,2, // first tri
		2,3,0, //second tri
		//back
		4,5,6,
		6,7,4,
		//top
		1,6,5,
		5,2,1,
		//bottom
		7,0,3,
		3,4,7,
		//right
		3,2,5,
		5,4,3,
		//left
		7,6,1,
		1,0,7
	};
	indexBuffer->load(index_list, ARRAYSIZE(index_list));
	m_vertexShader = m_mesh->vertexShader;
	m_pixelShader = m_mesh->pixelShader;

	//load the vertex buffer with the mesh's vertices and shader
	m_vb->load(list.data(), sizeof(vertex), size_list, m_mesh->vertexBlob->GetBufferPointer(), m_mesh->vertexBlob->GetBufferSize());
	m_device_context->loadShaders(m_vertexShader, m_pixelShader);
	m_device_context->createTransparentBlendState();
	m_device_context->SetViewportSize(m_size.width, m_size.height);
	m_device_context->setVertexBuffer(m_vb);
	m_device_context->setIndexBuffer(*indexBuffer);
	m_device_context->setBlendState();
	m_device_context->createBackfaceRasterizerState();
	m_device_context->setRasterState();
	currentCol = Color{ 0.01f,0.01f,0.01f,1.0f };
	//set constant buffer with values 
}
void dx3d::Display::onUpdate()
{
	UpdateQuadPosition();
	Window::onUpdate();
	m_device_context->clearRenderTargetColor(m_swapChain,currentCol.rgba);
	m_device_context->drawIndexedTriangleList(indexBuffer->getSizeIndexList(), 0, 0);
	m_swapChain->present(false);
}

void dx3d::Display::UpdateQuadPosition()
{
	u += Time::deltaTime / 10.0f;
	if (u >= 1) {
		u = 0;
	}
	Matrix4X4 temp{};
	ConstantBufferDesc cBuff{};
	cBuff.elapsedTime = Time::elapsedTime;
	cBuff.m_world.SetScale(Vector3D(1,1,1));
	temp.SetRotationY(Time::elapsedTime);
	cBuff.m_world *= temp;
	temp.SetTranslate(Vector3D(0, 0, 3));
	cBuff.m_world *= temp;
	cBuff.m_view.SetIdentity();
	float orthoSize = 3;
	cBuff.m_proj.SetOrthoLH(orthoSize * (float)m_size.width/m_size.height, orthoSize, 0, 10);
	constantBuffer->load(cBuff);
	m_device_context->setConstantBuffer(*constantBuffer);
}

dx3d::Display::~Display()
{
}
