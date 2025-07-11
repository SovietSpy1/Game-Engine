#pragma once
#include "DX3D/Core/Base.h"
#include <DX3D/Input/InputListener.h>
#include <DX3D/Component/Component.h>
#include <wrl.h>
#include <typeindex>
#include <unordered_map>
namespace dx3d {
	class GameObject : public Base, public InputListener
	{
	public:
		GameObject(const BaseDesc& desc);
		GameObject(const GameObject& object);
		virtual ~GameObject() {};
		virtual void Update();
		void AddMeshFromFile(const wchar_t* file_path);
		void AddMesh(const wchar_t* name);
		void AddMaterial();
		void AddRigidBody();
		void AddCollider(ColliderType colliderType);
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		virtual void PreDraw() {};
		virtual void PostDraw() {};
		//variables
		std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>> components;
		template <typename ComponentType>
		void AddComponent(std::shared_ptr<ComponentType> component);
		template <typename ComponentType>
		ComponentType* GetComponent();
		//inherited via input listener
		virtual void onKeyDown(int key) override {};
		virtual void onKeyUp(int key) override {};
		virtual void onKeyDownFirst(int key) override {};
		// Inherited via InputListener
		virtual void onMouseMove(const Point& mouse_pos) override {};
		virtual void onLeftMouseDown(const Point& mouse_pos) override {};
		virtual void onRightMouseDown(const Point& mouse_pos) override {};
	};
}


