#include "DX3D/Game/Display.h"
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Math/GameMath.h>
#include <DX3D/Graphics/ResourceManager/MeshManager/Mesh.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/TextureManager.h>
#include <DX3D/Object/GameObject.h>
dx3d::Display::Display(const DisplayDesc& desc) : Window(WindowDesc(desc.window.base, desc.window.size))
{
	m_worldCam.SetTranslate(Vector3D(0.0f, 0.0f, -2.0f));
	aspectRatio = static_cast<float>(m_size.width) / static_cast<float>(m_size.height);
	HWND console = GetConsoleWindow();
	ShowWindow(console, SW_MINIMIZE);
	m_swapChain = desc.rendererSystem.createSwapChain({ m_handle, m_size });
	m_device_context = desc.rendererSystem.createDeviceContext();
	constantBuffer = desc.rendererSystem.createConstantBuffer();

	inputSystem = std::make_shared<InputSystem>(InputSystemDesc(desc.window.base, (HWND)m_handle));
	inputSystem->addListener(this);
	inputSystem->showCursor(false);
	//Per Game Object
	//setting up mesh with the heart vertices and heart shaders
	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Assets/Meshes/sphere.obj");
	currentObject->SetTexture(L"Assets/Textures/Solarsystemscope_texture_8k_sun.jpg");
	currentObject->transform.SetTranslate(Vector3D(0.0f, 0.0f, 0.0f));

	//currentObject->transform.SetRotationX(PI / 2.0f);
	gameObjects.push_back(currentObject);
	//load the vertex buffer with the mesh's vertices and shader
	m_device_context->createBackfaceRasterizerState();
	m_device_context->setRasterState();
	m_device_context->SetViewportSize(m_size.width, m_size.height);
	currentCol = Color{ 0.01f,0.01f,0.01f,1.0f };
	//set constant buffer with values 
	initialized = true;
}
void dx3d::Display::onUpdate()
{
	m_device_context->clearRenderTargetColor(m_swapChain, currentCol.rgba);
	inputSystem->onUpdate();
	Window::onUpdate();
	CameraUpdate();
	for (int i = 0; i < gameObjects.size(); i++) {
		currentObject = gameObjects[i];
		currentObject->zRot += Time::deltaTime * 0.5f / PI;
		currentObject->transform.SetRotationY(currentObject->zRot);
		GameObjectUpdate();
	}
	m_swapChain->present(true);

}

void dx3d::Display::onFocus()
{
	inputSystem->addListener(this);
}

void dx3d::Display::onKillFocus()
{
	inputSystem->removeListener(this);
}

void dx3d::Display::GameObjectUpdate()
{
	cBuff.m_world = currentObject->transform;
	constantBuffer->load(cBuff);
	m_device_context->setConstantBuffer(*constantBuffer);
	m_device_context->setVertexBuffer(currentObject->mesh->vertexBuffer);
	m_device_context->setIndexBuffer(*currentObject->mesh->indexBuffer);
	m_device_context->loadShaders(currentObject->mesh->vertexShader, currentObject->mesh->pixelShader);
	m_device_context->setTexture(currentObject->texture);
	m_device_context->drawIndexedTriangleList(currentObject->mesh->indexBuffer->getSizeIndexList(), 0, 0);
	if (currentObject->showAxis) {
		currentObject->SetAxisBuffer();
		m_device_context->setVertexBuffer(currentObject->axisVertexBuffer);
		m_device_context->loadShaders(currentObject->vertexShader, currentObject->pixelShader);
		m_device_context->DrawLines(currentObject->axisVertexBuffer->getSizeVertexList(), 0);
	}
}

void dx3d::Display::CameraUpdate()
{
	Matrix4X4 temp{};
	Matrix4X4 worldCam{};
	worldCam.SetIdentity();
	temp.SetRotationX(xRot);
	worldCam *= temp;
	temp.SetRotationY(yRot);
	worldCam *= temp;
	Vector3D new_pos = m_worldCam.getTranslation() + worldCam.getZDirection() * forward * 0.3f;
	new_pos = new_pos + worldCam.getXDirection() * rightward * 0.3f;
	worldCam.SetTranslate(new_pos);
	m_worldCam = worldCam;
	worldCam.inverse();
	cBuff.elapsedTime = Time::elapsedTime;
	cBuff.m_view = worldCam;
	cBuff.m_proj.SetPerspectiveLH(1.57f, aspectRatio, 0.1f, 100.0f);
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
	InputSystem::get()->setCursorPosition(Point(m_size.width / 2.0f, m_size.height / 2.0f));
	xRot -= (mouse_pos.y-(m_size.height/2.0f)) * Time::deltaTime * 0.1f;
	yRot -= (mouse_pos.x-(m_size.width/2.0f)) * Time::deltaTime * 0.1f;
	if (xRot > PI / 2.0f) xRot = PI / 2.0f;
	if (xRot < -PI / 2.0f) xRot = -PI / 2.0f;
}

void dx3d::Display::onLeftMouseDown(const Point& mouse_pos)
{
	scale = scale * 1.5f;
}

void dx3d::Display::onRightMouseDown(const Point& mouse_pos)
{
	scale = scale * (2.0f / 3.0f);
}

