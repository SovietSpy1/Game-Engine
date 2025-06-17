#pragma once
#include "DX3D/Core/Base.h"
#include <DX3D/Input/InputListener.h>
#include <wrl.h>
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
		void SetAxis(float size = 1);
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Transform> transform;
		std::shared_ptr<Material> material;
		std::shared_ptr<RigidBody> rigidBody;
		std::shared_ptr<Collider> collider;
		std::vector<Tags> tags;
		//Axis buffer for visualization
		bool showAxis = false;
		std::shared_ptr<Axis> axis{};
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


