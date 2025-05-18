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
		void SetRotationX(float x) {
			SetIdentity();
			mat[1][1] = cos(x);
			mat[1][2] = -sin(x);
			mat[2][1] = sin(x);
			mat[2][2] = cos(x);
		}
		void SetRotationY(float y) {
			SetIdentity();
			mat[0][0] = cos(y);
			mat[0][2] = sin(y);
			mat[2][0] = -sin(y);
			mat[2][2] = cos(y);
		}
		void SetRotationZ(float z) {
			SetIdentity();
			mat[0][0] = cos(z);
			mat[0][1] = -sin(z);
			mat[1][0] = sin(z);
			mat[1][1] = cos(z);
		}
		void SetPerspectiveFovLH(float fov, float aspect, float znear, float zfar) {
			SetIdentity();
			float tan_half_fov = tan(fov / 2.0f);
			mat[0][0] = 1.0f / (aspect * tan_half_fov);
			mat[1][1] = 1.0f / tan_half_fov;
			mat[2][2] = zfar / (zfar - znear);
			mat[2][3] = 1.0f;
			mat[3][2] = (-znear * zfar) / (zfar - znear);
			mat[3][3] = 0.0f;
		}
		void operator*=(const Matrix4X4& other) {
			float temp[4][4];
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					temp[i][j] = mat[i][0] * other.mat[0][j]+ mat[i][1] * other.mat[1][j] + mat[i][2]*other.mat[2][j]+mat[i][3]*other.mat[3][j];
				}
			}
			setMatrix(temp);
		}
		void setMatrix(float temp[4][4]) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					mat[i][j] = temp[i][j];
				}
			}
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