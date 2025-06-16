#pragma once
#include <DX3D/Core/Base.h>
namespace dx3d {
	enum class PhysicsType {
		Kinematic,
		Dynamic,
	};
	class RigidBody : public Base
	{
	public:
		RigidBody(const BaseDesc& desc, GameObject* owner);
		void SetVelocity(const Vector3D& velocity);
		void AddForce(const Vector3D& force);
		void Update();
		void onCollideDynamic(Vector3D moveDir);
		void onCollideEnd();
		void onCollide();
		Vector3D velocity{ 0, 0, 0 };
		Vector3D acceleration{ 0, 0, 0 };
		std::vector<Vector3D> actingForces{};
		GameObject* owner{ nullptr };
		bool gravity = true;
		bool grounded = false;
		bool colliding = false;
		PhysicsType physicsType = PhysicsType::Dynamic;
	};
}


