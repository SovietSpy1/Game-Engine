#pragma once
#include <DX3D/Math/Matrix.h>
namespace dx3d {
	class Transform {
	public:
		Transform();
		Vector3D GetPosition() const noexcept {
			return Vector3D(position.mat[3][0], position.mat[3][1], position.mat[3][2]);
		}
		Matrix4X4 Get();
		Matrix4X4 position;
		Matrix4X4 rotation;
		Matrix4X4 scale;
	};
}