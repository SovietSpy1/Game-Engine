#include "DX3D/Object/GameObject.h"
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/ResourceManager/MeshManager/MeshManager.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/TextureManager.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Object/Transform.h>
#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Input/InputSystem.h>
dx3d::GameObject::GameObject(const BaseDesc& desc) :Base(desc)
{
	transform = std::make_shared<Transform>();
	InputSystem::get()->addListener(this);
}
dx3d::GameObject::GameObject(const GameObject& object) : Base(BaseDesc{ object.m_logger })
{
	transform = std::make_shared<Transform>();
}
void dx3d::GameObject::Update()
{

}
void dx3d::GameObject::AddMeshFromFile(const wchar_t* file_path)
{
	mesh = GraphicsEngine::get()->getMeshManager().createMeshFromFile(file_path);
}
void dx3d::GameObject::AddMesh(const wchar_t* name)
{
	mesh = GraphicsEngine::get()->getMeshManager().createMesh(name);
}

void dx3d::GameObject::AddMaterial()
{
	material = GraphicsEngine::get()->getRenderSystem().createMaterial();
}

void dx3d::GameObject::AddRigidBody()
{
	rigidBody = PhysicsEngine::get()->createRigidBody(this);
}

void dx3d::GameObject::AddCollider(ColliderType colliderType)
{
	collider = PhysicsEngine::get()->createCollider(this, colliderType);
}

void dx3d::GameObject::SetPosition(float x, float y, float z)
{
	transform->position.SetTranslate(Vector3D(x, y, z));
}

void dx3d::GameObject::SetRotation(float x, float y, float z)
{
	transform->rotation.SetRotationX(x);
	transform->rotation.SetRotationY(y);
	transform->rotation.SetRotationZ(z);
}

void dx3d::GameObject::SetAxis(float size)
{
	axis = GraphicsEngine::get()->createAxis(AxisDesc{ size });
}
