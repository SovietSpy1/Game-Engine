#include "DX3D/Object/Transform.h"

dx3d::Transform::Transform()
{
	position.SetIdentity();
	rotation.SetIdentity();
	scale.SetIdentity();
}

void dx3d::Transform::SetParent(Transform* parent)
{
	this->parent = parent;
}

dx3d::Matrix4X4 dx3d::Transform::Get()
{
	Matrix4X4 result;
	result = rotation * scale * position;
	if (parent != nullptr) {
		result *= parent->Get();
	}
	return result;
}

dx3d::Vector3D dx3d::Transform::GetPosition()
{
	Vector3D result;
	if (parent != nullptr) {
		Matrix4X4 worldMat = scale * rotation * position * parent->Get();
		result = worldMat.getTranslation();
	}
	else {
		result = position.getTranslation();
	}
	return result;
}

dx3d::Matrix4X4 dx3d::Transform::GetRotation()
{
	Matrix4X4 result;
	if (parent != nullptr) {
		result = rotation * parent->rotation;
	}
	else {
		result = rotation;
	}
	return result;
}

dx3d::Matrix4X4 dx3d::Transform::GetScale()
{
	Matrix4X4 result;
	if (parent != nullptr) {
		result = parent->scale * scale;
	}
	else {
		result = scale;
	}
	return result;
}
