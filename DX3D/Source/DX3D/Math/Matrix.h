#pragma once
#include <memory>
#include <DX3D/Math/Vector3D.h>
namespace dx3d {
	class Matrix4X4 {
	public:
		Matrix4X4() {}
		void SetIdentity() {
			::memset(mat, 0, sizeof(float) * 16);
			mat[0][0] = 1;
			mat[1][1] = 1;
			mat[2][2] = 1;
			mat[3][3] = 1;
		}
		void SetTranslate(const Vector3D& translation) {
			SetIdentity();
			mat[3][0] = translation.x;
			mat[3][1] = translation.y;
			mat[3][2] = translation.z;
		}
		void SetScale(const Vector3D& scale) {
			SetIdentity();
			mat[0][0] = scale.x;
			mat[1][1] = scale.y;
			mat[2][2] = scale.z;
		}
		void SetOrthoLH(float width, float height, float near_plane, float far_plane) {
			SetIdentity();
			mat[0][0] = 2.0f/width;
			mat[1][1] = 2.0f/height;
			mat[2][2] = 1.0f/(far_plane-near_plane);
			mat[3][2] = -(near_plane) / (far_plane - near_plane);
		}
		float mat[4][4] = {};
	};
}