#include "DX3D/Physics/RigidBody.h"
#include "DX3D/Object/GameObject.h"
#include "DX3D/Object/Transform.h"
dx3d::RigidBody::RigidBody(const BaseDesc& desc, GameObject* owner) : Base(desc), owner(owner)
{
}

void dx3d::RigidBody::SetVelocity(const Vector3D& velocity) 
{
	this->velocity = velocity;
}

void dx3d::RigidBody::AddForce(const Vector3D& force)
{
	actingForces.push_back(force);
}

void dx3d::RigidBody::Update()
{
	// Apply forces with consistent floating-point operations
	for (auto& f : actingForces) {
		velocity.x += f.x * Time::deltaTime;
		velocity.y += f.y * Time::deltaTime;
		velocity.z += f.z * Time::deltaTime;
	}

	Transform* transform = owner->GetComponent<Transform>();
	if (transform == nullptr) {
		return;
	}
	
	// Apply velocity with consistent floating-point operations
	Vector3D movement = velocity * Time::deltaTime;
	transform->position.Translate(movement);
	
	actingForces = {};
}

void dx3d::RigidBody::onCollideDynamic(Vector3D moveDir)
{
	// Reset velocity and acceleration
	velocity = Vector3D(0, 0, 0);
	acceleration = Vector3D(0, 0, 0);
	grounded = true;
	
	Transform* transform = owner->GetComponent<Transform>();
	if (transform == nullptr) {
		return;
	}
	
	// Apply collision response with consistent floating-point operations
	Vector3D adjustedMoveDir = moveDir;
	
	// Ensure we don't move objects too far in one frame (prevents tunneling)
	const float maxMoveDistance = 1.0f;
	if (adjustedMoveDir.mag() > maxMoveDistance) {
		adjustedMoveDir = adjustedMoveDir.normalize() * maxMoveDistance;
	}
	
	transform->position.Translate(adjustedMoveDir);
	colliding = true;
	onCollide();
}

void dx3d::RigidBody::onCollideEnd()
{
	grounded = false;
	colliding = false;
}

void dx3d::RigidBody::onCollide()
{
	
}
