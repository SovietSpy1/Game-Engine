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
#include <DX3D/Object/Axis.h>
#include <DX3D/Object/Material.h>
#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Physics/Collider.h>
#include <DX3D/Object/Objects/Grid.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/Texture.h>
dx3d::GameObject::GameObject(const BaseDesc& desc) :Base(desc)
{
std::shared_ptr<Transform> transform = std::make_shared<Transform>();
	AddComponent<Transform>(transform);
	InputSystem::get()->addListener(this);
}
dx3d::GameObject::GameObject(const GameObject& object) : Base(BaseDesc{ object.m_logger })
{
}
void dx3d::GameObject::Update()
{

}
void dx3d::GameObject::AddMeshFromFile(const wchar_t* file_path)
{
	std::shared_ptr<Mesh> mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(file_path);
	AddComponent<Mesh>(mesh);
}
void dx3d::GameObject::AddMesh(const wchar_t* name)
{
	std::shared_ptr<Mesh> mesh = GraphicsEngine::get()->getMeshManager()->createMesh(name);
	AddComponent<Mesh>(mesh);
}

void dx3d::GameObject::AddMaterial()
{
	std::shared_ptr<Material> material =  GraphicsEngine::get()->getRenderSystem()->createMaterial();
	AddComponent<Material>(material);
}

void dx3d::GameObject::AddRigidBody()
{
	std::shared_ptr<RigidBody> rigidBody = PhysicsEngine::get()->createRigidBody(this);
	AddComponent<RigidBody>(rigidBody);
}

void dx3d::GameObject::AddCollider(ColliderType colliderType)
{
	std::shared_ptr<Collider> collider = PhysicsEngine::get()->createCollider(this, colliderType);
	AddComponent<Collider>(collider);
}

void dx3d::GameObject::SetPosition(float x, float y, float z)
{
	Transform* transform= GetComponent<Transform>();
	if (transform == nullptr) {
		DX3DLogWarning("No Transform on this Object.");
	}
	transform->position.SetTranslate(Vector3D(x, y, z));
}

void dx3d::GameObject::SetRotation(float x, float y, float z)
{
	Transform* transform= GetComponent<Transform>();
	if (transform == nullptr) {
		DX3DLogWarning("No Transform on this Object.");
		return;
	}
	transform->rotation.SetRotationX(x);
	transform->rotation.SetRotationY(y);
	transform->rotation.SetRotationZ(z);
}




template <typename ComponentType>
void dx3d::GameObject::AddComponent(std::shared_ptr<ComponentType> component)
{
	std::type_index type = typeid(ComponentType);
	auto new_component = std::make_shared<Component<ComponentType>>(BaseDesc{ m_logger }, component);
	components[type] = new_component;
}

template <typename ComponentType>
ComponentType* dx3d::GameObject::GetComponent()
{
	std::type_index type = typeid(ComponentType);
	auto it = components.find(type);
	if (it == components.end()) {
		//DX3DLogWarning((std::string(typeid(ComponentType).name()) + " not found").c_str());
		return nullptr;
	}
	auto component = std::static_pointer_cast<Component<ComponentType>>(it->second);
	return component->Get();
}

// ==== EXPLICIT INSTANTIATIONS ====

template void dx3d::GameObject::AddComponent<dx3d::Transform>(std::shared_ptr<dx3d::Transform>);
template void dx3d::GameObject::AddComponent<dx3d::Mesh>(std::shared_ptr<dx3d::Mesh>);
template void dx3d::GameObject::AddComponent<dx3d::Material>(std::shared_ptr<dx3d::Material>);
template void dx3d::GameObject::AddComponent<dx3d::Texture>(std::shared_ptr<dx3d::Texture>);
template void dx3d::GameObject::AddComponent<dx3d::RigidBody>(std::shared_ptr<dx3d::RigidBody>);
template void dx3d::GameObject::AddComponent<dx3d::Collider>(std::shared_ptr<dx3d::Collider>);
template void dx3d::GameObject::AddComponent<dx3d::Axis>(std::shared_ptr<dx3d::Axis>);
template void dx3d::GameObject::AddComponent<dx3d::Grid>(std::shared_ptr<dx3d::Grid>);

template dx3d::Transform* dx3d::GameObject::GetComponent<dx3d::Transform>();
template dx3d::Mesh* dx3d::GameObject::GetComponent<dx3d::Mesh>();
template dx3d::Material* dx3d::GameObject::GetComponent<dx3d::Material>();
template dx3d::Texture* dx3d::GameObject::GetComponent<dx3d::Texture>();
template dx3d::RigidBody* dx3d::GameObject::GetComponent<dx3d::RigidBody>();
template dx3d::Collider* dx3d::GameObject::GetComponent<dx3d::Collider>();
template dx3d::Axis* dx3d::GameObject::GetComponent<dx3d::Axis>();
template dx3d::Grid* dx3d::GameObject::GetComponent<dx3d::Grid>();