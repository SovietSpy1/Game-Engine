#include "DX3D/Object/Transform.h"

dx3d::Transform::Transform()
{
	position.SetIdentity();
	rotation.SetIdentity();
	scale.SetIdentity();
}

dx3d::Matrix4X4 dx3d::Transform::Get()
{
	Matrix4X4 result = rotation * scale * position;
	return result;
}
