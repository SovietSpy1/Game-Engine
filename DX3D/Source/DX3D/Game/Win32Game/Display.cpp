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
#include <DX3D/GameManager/GameManager.h>
//gameObjects
#include <DX3D/Object/Objects/Camera.h>
#include <DX3D/Object/Objects/SkyBox.h>
#include <DX3D/Object/Objects/Player.h>
#include <string>
#include <DX3D/Object/Objects/Grid.h>
#include <DX3D/Object/Objects/Smoke.h>
#include <DX3D/Graphics/StructuredBuffer/StructuredBuffer.h>
dx3d::Display::Display(const DisplayDesc& desc) : Window(WindowDesc(desc.window.base, desc.window.size))
{
	S = this;
	constantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer();
	aspectRatio = static_cast<float>(m_size.width) / static_cast<float>(m_size.height);
	HWND console = GetConsoleWindow();
	ShowWindow(console, SW_MINIMIZE);
	m_swapChain = desc.rendererSystem->createSwapChain({ m_handle, m_size });
	m_device_context = desc.rendererSystem->createDeviceContext();
	m_device_context->SetViewportSize(m_size.width, m_size.height);
	m_device_context->createTransparentBlendState();
	m_device_context->setBlendState();
	currentCol = vec4_32{255,255,255,255 };
	gameManager = std::make_unique<GameManager>(BaseDesc{ m_logger });
}
void dx3d::Display::Update()
{
	m_device_context->clearRenderTargetColor(m_swapChain, currentCol);
	m_device_context->SetRTV(m_swapChain->m_rtv, m_swapChain->m_dsv);
	GameManager::get()->Update();
	m_swapChain->present(true);
}

void dx3d::Display::onFocus()
{
	InputSystem::get()->listening = true;
}

void dx3d::Display::onKillFocus()
{
	InputSystem::get()->listening = false;
}

dx3d::Display* dx3d::Display::get()
{
	return S;
}

void dx3d::Display::Draw(std::shared_ptr<GameObject> currentObject)
{
	//preparing
	BuiltInChecks(currentObject);
	//per object before draw
	currentObject->PreDraw();
	m_device_context->SetRTV(m_swapChain->m_rtv, m_swapChain->m_dsv);
	//drawing 
	BuiltInDraws(currentObject);
	//per object after draw
	currentObject->PostDraw();
}

void dx3d::Display::BuiltInChecks(std::shared_ptr<GameObject> currentObject)
{
	//preparing
	constantBuffers.clear();
	Transform* transform = currentObject->GetComponent<Transform>();
	cBuff.m_world = transform->Get();
	cBuff.m_rotation = transform->GetRotation();
	constantBuffer->load(cBuff);
	constantBuffers.insert({ 0, constantBuffer });
	Collider* collider = currentObject->GetComponent<Collider>();
	if (collider != nullptr) {
		if (collider->show) {
			constantBuffers.insert({ 5,collider->constantBuffer });
		}
	}
	m_device_context->setConstantBuffers(constantBuffers);
}

void dx3d::Display::BuiltInDraws(std::shared_ptr<GameObject> currentObject)
{
	Collider* collider = currentObject->GetComponent<Collider>();
	Material* material = currentObject->GetComponent<Material>();
	Mesh* mesh = currentObject->GetComponent<Mesh>();
	if (material != nullptr && mesh != nullptr) {
		m_device_context->setRasterizerState(material->rasterizerStateType);
		m_device_context->setVertexBuffer(mesh->vertexBuffer);
		m_device_context->setIndexBuffer(mesh->indexBuffer);
		m_device_context->loadShaders(material->vertexShader, material->pixelShader);
		if (material->textures.size() != 0) {
			m_device_context->setTexture(material->textures);
		}
		m_device_context->drawIndexedTriangleList(mesh->indexBuffer->getSizeIndexList(), 0, 0);
	}
	if (collider != nullptr) {
		if (collider->show) {
			m_device_context->setVertexBuffer(collider->vertexBuffer);
			m_device_context->setIndexBuffer(collider->indexBuffer);
			m_device_context->loadShaders(Collider::vertexShader, Collider::pixelShader);
			m_device_context->DrawIndexedLines(collider->indexBuffer->getSizeIndexList(), 0, 0);
		}
	}
	Axis* axis = currentObject->GetComponent<Axis>();
	if (axis != nullptr) {
		m_device_context->setVertexBuffer(axis->axisVertexBuffer);
		m_device_context->loadShaders(axis->vertexShader, axis->pixelShader);
		m_device_context->DrawLines(6, 0);
	}
	Grid* grid = currentObject->GetComponent<Grid>();
	if (grid != nullptr) {
		m_device_context->setVertexBuffer(grid->vertexBuffer);
		m_device_context->loadShaders(Grid::vertexShader, Grid::pixelShader);
		m_device_context->DrawLines(grid->vertexBuffer->getSizeVertexList(), 0);
	}
}


void dx3d::Display::CameraUpdate(Matrix4X4 lightRot, Camera* cam, float fov)
{
	Transform* transform = cam->GetComponent<Transform>();
	cBuff.lightDirection = lightRot.getZDirection();
	cBuff.elapsedTime = Time::elapsedTime;
	cBuff.camPosition = transform->position.getTranslation();
	cBuff.m_view = transform->Get().inverse();
	cBuff.m_proj.SetPerspectiveLH(fov, aspectRatio, 0.1f, 100.0f);
	//cBuff.m_proj.SetOrthoLH(1.1f * aspectRatio, 1.1f, 0, 10);
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


