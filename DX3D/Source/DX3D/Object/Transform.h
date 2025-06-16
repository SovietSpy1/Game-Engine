#pragma once
#include <DX3D/Math/Matrix.h>
namespace dx3d {
	class Transform {
	public:
		Transform();
		Vector3D GetPosition() const noexcept {
			return Vector3D(position.mat[3][0], position.mat[3][1], position.mat[3][2]);
		}
		void SetParent(Transform* parent);
		Matrix4X4 Get();
		Vector3D GetPosition();
		Matrix4X4 GetRotation();
		Matrix4X4 GetScale();
		Matrix4X4 position;
		Matrix4X4 rotation;
		Matrix4X4 scale;
		Transform* parent = nullptr;
	};
}