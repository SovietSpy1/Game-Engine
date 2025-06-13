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
#include <DX3D/Object/Transform.h>
#include <DX3D/Object/Axis.h>
#include <DX3D/Object/Material.h>
#include <string>
dx3d::Display::Display(const DisplayDesc& desc) : Window(WindowDesc(desc.window.base, desc.window.size))
{
	m_worldCam.SetTranslate(Vector3D(0.0f, 0.0f, -2));
	aspectRatio = static_cast<float>(m_size.width) / static_cast<float>(m_size.height);
	HWND console = GetConsoleWindow();
	ShowWindow(console, SW_MINIMIZE);
	m_swapChain = desc.rendererSystem.createSwapChain({ m_handle, m_size });
	m_device_context = desc.rendererSystem.createDeviceContext();
	constantBuffer = desc.rendererSystem.createConstantBuffer();

	inputSystem = std::make_shared<InputSystem>(InputSystemDesc(desc.window.base, (HWND)m_handle));
	inputSystem->addListener(this);
	inputSystem->showCursor(false);

	skyBox = GraphicsEngine::get()->createGameObject();
	skyBox->AddMesh(L"Assets/Meshes/sphere.obj");
	skyBox->AddMaterial();
	skyBox->material->SetTexture(L"Assets/Textures/sky_water.jpeg");
	skyBox->material->SetPixelShader(L"DX3D/Shaders/Skybox/PixelShader.hlsl");
	skyBox->material->SetVertexShader(L"DX3D/Shaders/Skybox/VertexShader.hlsl");
	gameObjects.push_back(skyBox);
	//Per Game Object
	//setting up mesh with the heart vertices and heart shaders
	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Assets/Meshes/sphere.obj");
	currentObject->AddMaterial();
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Lighting/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/VertexMeshLayout/VertexShader.hlsl");
	currentObject->transform->scale.SetScale(Vector3D(1, 1, 1) * 1);
	currentObject->SetAxis(0.5f);
	gameObjects.push_back(currentObject);

	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Assets/Meshes/statue.obj");
	currentObject->AddMaterial();
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Lighting/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/VertexMeshLayout/VertexShader.hlsl");
	currentObject->transform->scale.SetScale(Vector3D(1, 1, 1) * 3);
	currentObject->transform->position.SetTranslate(Vector3D(-3, 0, 0));
	currentObject->SetAxis(0.5f);
	gameObjects.push_back(currentObject);

	m_device_context->SetViewportSize(m_size.width, m_size.height);
	currentCol = Color{ 0,0,0.7f };
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
		if (currentObject == skyBox) {
			currentObject->transform->position.SetTranslate(m_worldCam.getTranslation());
			currentObject->transform->scale.SetScale(Vector3D(1, 1, 1) * 10);
			SkyBoxUpdate();
		}
		else{
			GameObjectUpdate();
		}
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
	cBuff.m_world = currentObject->transform->Get();
	cBuff.m_rotation = currentObject->transform->rotation;
	constantBuffer->load(cBuff);
	m_device_context->setConstantBuffer(*constantBuffer);
	m_device_context->setVertexBuffer(currentObject->mesh->vertexBuffer);
	m_device_context->setIndexBuffer(*currentObject->mesh->indexBuffer);
	m_device_context->loadShaders(currentObject->material->vertexShader, currentObject->material->pixelShader);
	if (currentObject->material->texture != nullptr) {
		m_device_context->setTexture(currentObject->material->texture);
	}
	m_device_context->drawIndexedTriangleList(currentObject->mesh->indexBuffer->getSizeIndexList(), 0, 0);
	if (currentObject->showAxis) {
		m_device_context->setVertexBuffer(currentObject->axis->axisVertexBuffer);
		m_device_context->loadShaders(currentObject->axis->vertexShader, currentObject->axis->pixelShader);
		m_device_context->DrawLines(6, 0);
	}
}

void dx3d::Display::CameraUpdate()
{
	Matrix4X4 lightRot;
	lightRot.SetIdentity();
	lightRotation += 0.25f * PI * Time::deltaTime;
	Matrix4X4 temp;
	temp.SetRotationX(-PI / 4.0f);
	lightRot *= temp;
	temp.SetRotationY(lightRotation);
	lightRot *= temp;
	cBuff.lightDirection = Vector4D(lightRot.getZDirection());

	Matrix4X4 worldCam{};
	worldCam.SetIdentity();
	temp.SetRotationX(xRot);
	worldCam *= temp;
	temp.SetRotationY(yRot);
	worldCam *= temp;
	Vector3D new_pos = m_worldCam.getTranslation() + worldCam.getZDirection() * forward * 0.01f;
	new_pos = new_pos + worldCam.getXDirection() * rightward * 0.01f;
	cBuff.camPosition = new_pos;
	worldCam.SetTranslate(new_pos);
	m_worldCam = worldCam;
	worldCam.inverse();
	cBuff.m_view = worldCam;
	cBuff.m_proj.SetPerspectiveLH(fov, aspectRatio, 0.000001f, 100.0f);
}

void dx3d::Display::SkyBoxUpdate()
{
	m_device_context->createBackfaceRasterizerState();
	m_device_context->setRasterState();
	cBuff.m_world = currentObject->transform->Get();
	constantBuffer->load(cBuff);
	m_device_context->setConstantBuffer(*constantBuffer);
	m_device_context->setVertexBuffer(currentObject->mesh->vertexBuffer);
	m_device_context->setIndexBuffer(*currentObject->mesh->indexBuffer);
	m_device_context->loadShaders(currentObject->material->vertexShader, currentObject->material->pixelShader);
	if (currentObject->material->texture != nullptr) {
		m_device_context->setTexture(currentObject->material->texture);
	}
	m_device_context->drawIndexedTriangleList(currentObject->mesh->indexBuffer->getSizeIndexList(), 0, 0);
	m_device_context->createFrontfaceRasterizerState();
	m_device_context->setRasterState();
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

void dx3d::Display::onKeyDownFirst(int key)
{
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
}

void dx3d::Display::onRightMouseDown(const Point& mouse_pos)
{
}

