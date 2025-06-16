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
#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Physics/Collider.h>
//gameObjects
#include <DX3D/Object/Objects/Camera.h>
#include <DX3D/Object/Objects/SkyBox.h>
#include <DX3D/Object/Objects/Player.h>
#include <string>
dx3d::Display::Display(const DisplayDesc& desc) : Window(WindowDesc(desc.window.base, desc.window.size))
{
	S = this;
	aspectRatio = static_cast<float>(m_size.width) / static_cast<float>(m_size.height);
	HWND console = GetConsoleWindow();
	ShowWindow(console, SW_MINIMIZE);
	m_swapChain = desc.rendererSystem.createSwapChain({ m_handle, m_size });
	m_device_context = desc.rendererSystem.createDeviceContext();
	constantBuffer = desc.rendererSystem.createConstantBuffer();
	inputSystem = std::make_shared<InputSystem>(InputSystemDesc(desc.window.base, (HWND)m_handle));
	inputSystem->addListener(this);
	inputSystem->showCursor(false);
	PhysicsEngine::get()->gravity = -3;

	/*currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Cube");
	currentObject->AddCollider(ColliderType::Box);
	currentObject->AddRigidBody();
	currentObject->AddMaterial();
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Lighting/SphereTex/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	currentObject->material->SetTexture(L"Assets/Textures/brick.png");
	currentObject->collider->show = true;
	currentObject->transform->scale.SetScale(Vector3D(1, 1, 1));
	currentObject->transform->position.SetTranslate(Vector3D(0, 10, 0));
	gameObjects.push_back(currentObject);*/

	
	currentObject = std::make_shared<Player>(BaseDesc{m_logger});
	currentObject->AddMesh(L"Cube");
	currentObject->AddCollider(ColliderType::Box);
	currentObject->AddRigidBody();
	currentObject->AddMaterial();
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Lighting/SphereTex/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	currentObject->material->SetTexture(L"Assets/Textures/brick.png");
	currentObject->transform->scale.SetScale(Vector3D(1, 1, 1));
	currentObject->transform->position.SetTranslate(Vector3D(0, 0, -4));
	gameObjects.push_back(currentObject);

	camera = std::make_shared<Camera>(BaseDesc{m_logger});
	camera->tags.push_back(Tags::Camera);
	camera->transform->SetParent(currentObject->transform.get());
	camera->transform->position.SetTranslate(Vector3D(0, 1, -1));
	//gameObjects.push_back(std::static_pointer_cast<GameObject>(camera));

	currentObject = std::make_shared<SkyBox>(BaseDesc{ m_logger }, camera.get());
	currentObject->AddMeshFromFile(L"Assets/Meshes/sphere.obj");
	currentObject->AddMaterial();
	currentObject->material->SetTexture(L"Assets/Textures/sky_water.jpeg");
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Skybox/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/Skybox/VertexShader.hlsl");
	currentObject->material->rasterizerStateType = RasterizerStateType::BackFace;
	currentObject->transform->scale.SetScale(Vector3D(1, 1, 1) * 99);
	gameObjects.push_back(std::static_pointer_cast<GameObject>(currentObject));
	//Per Game Object
	//setting up mesh with the heart vertices and heart shaders



	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Quad");
	currentObject->AddMaterial();
	currentObject->AddCollider(ColliderType::Quad);
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Lighting/SphereTex/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	currentObject->material->SetTexture(L"Assets/Textures/grass.jpg");
	currentObject->material->rasterizerStateType = RasterizerStateType::AllFace;
	currentObject->transform->scale.SetScale(Vector3D(10, 1, 10));
	currentObject->transform->position.SetTranslate(Vector3D(0, -1, 0));
	gameObjects.push_back(currentObject);

	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Cube");
	currentObject->AddCollider(ColliderType::Box);
	currentObject->AddRigidBody();
	currentObject->AddMaterial();
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Lighting/SphereTex/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	currentObject->material->SetTexture(L"Assets/Textures/brick.png");
	currentObject->transform->scale.SetScale(Vector3D(1, 1, 1));
	currentObject->transform->position.SetTranslate(Vector3D(0, 10, 0));
	gameObjects.push_back(currentObject);

	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMeshFromFile(L"Assets/Meshes/sphere.obj");
	currentObject->AddMaterial();
	currentObject->AddCollider(ColliderType::Box);
	currentObject->AddRigidBody();
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Lighting/SphereTex/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	currentObject->material->SetTexture(L"Assets/Textures/brick.png");
	currentObject->transform->scale.SetScale(Vector3D(1, 1, 1) * 1);
	currentObject->transform->position.SetTranslate(Vector3D(3, 10, 0));
	gameObjects.push_back(currentObject);

	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMeshFromFile(L"Assets/Meshes/statue.obj");
	currentObject->AddMaterial();
	currentObject->AddCollider(ColliderType::Box);
	currentObject->collider->SetTransform(Vector3D::up() * 1.2f + Vector3D::forward() * 0.2f);
	currentObject->collider->SetScale(Vector3D(0.1f, 0.5f, 0.1f));
	currentObject->AddRigidBody();
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Lighting/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	currentObject->transform->scale.SetScale(Vector3D(1, 1, 1) * 5);
	currentObject->transform->position.SetTranslate(Vector3D(-3, 10, 0));
	gameObjects.push_back(currentObject);

	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Wave");
	currentObject->mesh->Demo();
	currentObject->AddMaterial();
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Wave/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/Wave/VertexShader.hlsl");
	currentObject->material->SetTexture(L"Assets/Textures/wave.jpg");
	currentObject->material->rasterizerStateType = RasterizerStateType::AllFace;
	currentObject->transform->scale.SetScale(Vector3D(0.2f, 1, 0.2f));
	currentObject->transform->position.SetTranslate(Vector3D(-10, -2, -10));
	gameObjects.push_back(currentObject);

	m_device_context->SetViewportSize(m_size.width, m_size.height);
	m_device_context->createTransparentBlendState();
	m_device_context->setBlendState();
	currentCol = Color{ 0,0,0.7f };
	//set constant buffer with values 
	initialized = true;
}
void dx3d::Display::onUpdate()
{
	Window::onUpdate();
	m_device_context->clearRenderTargetColor(m_swapChain, currentCol.rgba);
	inputSystem->onUpdate();
	PhysicsEngine::get()->Update();
	camera->Update();
	CameraUpdate();
	for (int i = 0; i < gameObjects.size(); i++) {
		currentObject = gameObjects[i];
		currentObject->Update();
		GameObjectUpdate();
	}
	m_swapChain->present(true);
}

void dx3d::Display::onFocus()
{
	inputSystem->listening = true;
	inputSystem->showCursor(false);
}

void dx3d::Display::onKillFocus()
{
	inputSystem->listening = false;
	inputSystem->showCursor(true);
}

dx3d::Display* dx3d::Display::Get()
{
	return S;
}

void dx3d::Display::GameObjectUpdate()
{
	//prepating
	constantBuffers.clear();
	cBuff.m_world = currentObject->transform->Get();
	cBuff.m_rotation = currentObject->transform->GetRotation();
	constantBuffer->load(cBuff);
	constantBuffers.insert({ 0, constantBuffer });
	if (currentObject->collider != nullptr) {
		if (currentObject->collider->show) {
			constantBuffers.insert({ 5,currentObject->collider->constantBuffer });
		}
	}
	m_device_context->setConstantBuffers(constantBuffers);
	
	//drawing 
	if (currentObject->material != nullptr && currentObject->mesh != nullptr) {
		m_device_context->setRasterizerState(currentObject->material->rasterizerStateType);
		m_device_context->setVertexBuffer(currentObject->mesh->vertexBuffer);
		m_device_context->setIndexBuffer(currentObject->mesh->indexBuffer);
		m_device_context->loadShaders(currentObject->material->vertexShader, currentObject->material->pixelShader);
		if (currentObject->material->textures.size() != 0) {
			m_device_context->setTexture(currentObject->material->textures);
		}
		m_device_context->drawIndexedTriangleList(currentObject->mesh->indexBuffer->getSizeIndexList(), 0, 0);
	}
	if(currentObject->collider != nullptr) {
		if (currentObject->collider->show) {
			m_device_context->setVertexBuffer(currentObject->collider->vertexBuffer);
			m_device_context->setIndexBuffer(currentObject->collider->indexBuffer);
			m_device_context->loadShaders(Collider::vertexShader, Collider::pixelShader);
			m_device_context->DrawIndexedLines(currentObject->collider->indexBuffer->getSizeIndexList(), 0, 0);
		}
	}
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
	
	cBuff.lightDirection = lightRot.getZDirection();
	cBuff.elapsedTime = Time::elapsedTime;
	cBuff.camPosition = camera->transform->position.getTranslation();
	cBuff.m_view = camera->transform->Get().inverse();
	cBuff.m_proj.SetPerspectiveLH(fov, aspectRatio, 0.01f, 100.0f);
}

dx3d::Display::~Display()
{
}

void dx3d::Display::onResize(const Rect& new_size)
{
	m_swapChain->resize(new_size.width, new_size.height);
	m_device_context->SetViewportSize(new_size.width, new_size.height);
	aspectRatio = static_cast<float>(new_size.width) / static_cast<float>(new_size.height);
}

void dx3d::Display::onKeyDown(int key)
{
}

void dx3d::Display::onKeyUp(int key)
{
}

void dx3d::Display::onKeyDownFirst(int key)
{
}

void dx3d::Display::onMouseMove(const Point& mouse_pos)
{
	
}

void dx3d::Display::onLeftMouseDown(const Point& mouse_pos)
{
}

void dx3d::Display::onRightMouseDown(const Point& mouse_pos)
{
}

