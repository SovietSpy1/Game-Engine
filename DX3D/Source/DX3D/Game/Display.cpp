#include "DX3D/Game/Display.h"
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Math/GameMath.h>
#include <DX3D/Object/Mesh.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Input/InputSystem.h>
dx3d::Display::Display(const DisplayDesc& desc) : Window(WindowDesc(desc.window.base, desc.window.size, this))
{
	m_worldCam.SetTranslate(Vector3D(0.0f, 0.0f, -2.0f));
	aspectRatio = static_cast<float>(m_size.width) / static_cast<float>(m_size.height);
	HWND console = GetConsoleWindow();
	ShowWindow(console, SW_MINIMIZE);
	m_swapChain = desc.rendererSystem.createSwapChain({ m_handle, m_size });
	m_device_context = desc.rendererSystem.createDeviceContext();
	m_vb = desc.rendererSystem.createVertexBuffer();
	constantBuffer = desc.rendererSystem.createConstantBuffer();
	indexBuffer = desc.rendererSystem.createIndexBuffer();
	inputSystem = std::make_shared<InputSystem>();
	inputSystem->addListener(this);
	inputSystem->showCursor(false);
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
	//index buffer
	m_device_context->setIndexBuffer(*indexBuffer);
	m_device_context->setBlendState();
	m_device_context->createBackfaceRasterizerState();
	m_device_context->setRasterState();
	currentCol = Color{ 0.01f,0.01f,0.01f,1.0f };
	//set constant buffer with values 
	initialized = true;
}
void dx3d::Display::onUpdate()
{
	Update();
	inputSystem->onUpdate();
	Window::onUpdate();
	m_device_context->clearRenderTargetColor(m_swapChain,currentCol.rgba);
	m_device_context->drawIndexedTriangleList(indexBuffer->getSizeIndexList(), 0, 0);
	m_swapChain->present(false);
}

void dx3d::Display::onFocus()
{
	inputSystem->addListener(this);
}

void dx3d::Display::onKillFocus()
{
	inputSystem->removeListener(this);
}

void dx3d::Display::Update()
{
	Matrix4X4 worldCam{};
	Matrix4X4 temp{};
	worldCam.SetIdentity();
	temp.SetIdentity();
	temp.SetRotationX(xRot);
	worldCam *= temp;

	temp.SetIdentity();
	temp.SetRotationY(yRot);
	worldCam *= temp;

	Vector3D new_pos = m_worldCam.getTranslation() + worldCam.getZDirection() * forward * 0.3f;
	new_pos = new_pos + worldCam.getXDirection() * rightward * 0.3f;
	worldCam.SetTranslate(new_pos);
	m_worldCam = worldCam;
	worldCam.inverse();

	ConstantBufferDesc cBuff{};
	cBuff.elapsedTime = Time::elapsedTime;
	cBuff.m_world.SetIdentity();	
	cBuff.m_view = worldCam;
	//cBuff.m_proj.SetOrthoLH(5.0f * aspectRatio, 5.0f, 0.0f, 10.0f);
	cBuff.m_proj.SetPerspectiveFovLH(1.57f, aspectRatio, 0.1f, 100.0f);
	constantBuffer->load(cBuff);
	m_device_context->setConstantBuffer(*constantBuffer);
}

dx3d::Display::~Display()
{
}

void dx3d::Display::onKeyDown(int key)
{
	if (key == 'W') {
		//xRot += Time::deltaTime * PI;
		forward = 1.0f;
	}
	if (key == 'S') {
		//xRot -= Time::deltaTime * PI;
		forward = -1.0f;
	}
	if (key == 'A') {
		//yRot += Time::deltaTime * PI;
		rightward = -1.0f;
	}
	if (key == 'D') {
		//yRot -= Time::deltaTime * PI;
		rightward = 1.0f;
	}
}

void dx3d::Display::onKeyUp(int key)
{
	forward = 0.0f;
	rightward = 0.0f;
}

void dx3d::Display::onMouseMove(const Point& mouse_pos)
{
	xRot -= (mouse_pos.y-(m_size.height/2.0f)) * Time::deltaTime * 0.1f;
	yRot -= (mouse_pos.x-(m_size.width/2.0f)) * Time::deltaTime * 0.1f;

	InputSystem::get()->setCursorPosition(Point(m_size.width / 2.0f, m_size.height / 2.0f));
}

void dx3d::Display::onLeftMouseDown(const Point& mouse_pos)
{
	scale = scale * 1.5f;
}

void dx3d::Display::onRightMouseDown(const Point& mouse_pos)
{
	scale = scale * (2.0f / 3.0f);
}

