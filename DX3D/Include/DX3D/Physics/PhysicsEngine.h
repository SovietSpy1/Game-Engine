#pragma once
#include <DX3D/Core/Base.h>
namespace dx3d {
	class PhysicsEngine : Base
	{
	public:
		PhysicsEngine(const BaseDesc& desc);
		void Update();
		bool IntersectCheck(const std::shared_ptr<Collider>& collider1, const std::shared_ptr<Collider>& collider2, Vector3D* moveVec = nullptr);
		std::shared_ptr<RigidBody> createRigidBody(GameObject* owner);
		std::shared_ptr<Collider> createCollider(GameObject* owner, ColliderType colliderType);
		std::vector<std::shared_ptr<RigidBody>> rigidBodies;
		std::vector<std::shared_ptr<Collider>> colliders;
		float gravity = -9.81f; // Default gravity value in m/s^2
		static PhysicsEngine* get();
		static inline PhysicsEngine* S{};
	};
}