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
	for (auto& f : actingForces) {
		velocity += f * Time::deltaTime;
	}
	owner->transform->position.Translate(velocity * Time::deltaTime);
	actingForces = {};
}

void dx3d::RigidBody::onCollideDynamic(Vector3D moveDir)
{
	velocity = Vector3D(0, 0, 0);
	acceleration = Vector3D(0, 0, 0);
	grounded = true;
	owner->transform->position.Translate(moveDir);
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
