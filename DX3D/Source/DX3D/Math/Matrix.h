#pragma once
#include <memory>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Vector4D.h>
namespace dx3d {
	class Matrix3X3 {
	public:
		Matrix3X3() {};
		Matrix3X3(const Matrix3X3& other) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					mat[i][j] = other.mat[i][j];
				}
			}
		}
		float getDeterminant() {
			float det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
				mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
				mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
			return det;
		}
		float mat[3][3] = {};
	};
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
			mat[3][0] = translation.x;
			mat[3][1] = translation.y;
			mat[3][2] = translation.z;
		}
		void SetScale(const Vector3D& scale) {
			mat[0][0] = scale.x;
			mat[1][1] = scale.y;
			mat[2][2] = scale.z;
		}
		void SetRotationX(float x) {
			SetIdentity();
			mat[1][1] = cosf(x);
			mat[1][2] = -sinf(x);
			mat[2][1] = sinf(x);
			mat[2][2] = cosf(x);
		}
		void SetRotationY(float y) {
			SetIdentity();
			mat[0][0] = cosf(y);
			mat[0][2] = sinf(y);
			mat[2][0] = -sinf(y);
			mat[2][2] = cosf(y);
		}
		void SetRotationZ(float z) {
			SetIdentity();
			mat[0][0] = cosf(z);
			mat[0][1] = -sinf(z);
			mat[1][0] = sinf(z);
			mat[1][1] = cosf(z);
		}
		Matrix4X4 operator*(float other) {
			Matrix4X4 temp;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					temp.mat[i][j] = mat[i][j] * other;
				}
			}
			return temp;
		}
		Matrix4X4 operator*(const Matrix4X4& other) {
			float temp[4][4];
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					temp[i][j] = mat[i][0] * other.mat[0][j] + mat[i][1] * other.mat[1][j] + mat[i][2] * other.mat[2][j] + mat[i][3] * other.mat[3][j];
				}
			}
			Matrix4X4 result;
			result.setMatrix(temp);
			return result;
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
		Matrix3X3 getSubMatrix(int row, int col) const {
			Matrix3X3 subMatrix;
			for (int i = 0, subRow = 0; i < 4; i++) {
				if (i == row) continue; // Skip the row we want to remove
				for (int j = 0, subCol = 0; j < 4; j++) {
					if (j == col) continue; // Skip the column we want to remove
					subMatrix.mat[subRow][subCol] = mat[i][j]; // Copy the value to the submatrix
					subCol++;
				}
				subRow++;
			}
			return subMatrix;
		}
		float getDeterminant() {
			float det1 = mat[0][0] * this->getSubMatrix(0,0).getDeterminant();
			float det2 = mat[0][1] * this->getSubMatrix(0,1).getDeterminant();
			float det3 = mat[0][2] * this->getSubMatrix(0,2).getDeterminant();
			float det4 = mat[0][3] * this->getSubMatrix(0,3).getDeterminant();
			return det1 - det2 + det3 - det4;
		}
		Matrix4X4 inverse() {
			Matrix4X4 out{};
			Matrix4X4 cofactors{};
			Matrix4X4 adjugate{};
			Matrix4X4 inverse{};
			float det = getDeterminant();
			if (!det) return Matrix4X4();
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					cofactors.mat[i][j] = (((i + j) % 2 == 0) ? 1.0f : -1.0f)
					* getSubMatrix(i, j).getDeterminant();
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					adjugate.mat[j][i] = cofactors.mat[i][j]; // Transpose the cofactor matrix
				}
			}
			inverse = adjugate * (1.0f / det);
			return Matrix4X4{ inverse };
		}
		void setMatrix(float temp[4][4]) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					mat[i][j] = temp[i][j];
				}
			}
		}
		Vector3D getZDirection() const {
			return Vector3D(mat[2][0], mat[2][1], mat[2][2]);
		}
		Vector3D getXDirection() const {
			return Vector3D(mat[0][0], mat[0][1], mat[0][2]);
		}
		Vector3D getYDirection() const {
			return Vector3D(mat[1][0], mat[1][1], mat[1][2]);
		}
		Vector3D getTranslation() const {
			return Vector3D(mat[3][0], mat[3][1], mat[3][2]);
		}
		void SetOrthoLH(float width, float height, float near_plane, float far_plane) {
			SetIdentity();
			mat[0][0] = 2.0f/width;
			mat[1][1] = 2.0f/height;
			mat[2][2] = 1.0f/(far_plane-near_plane);
			mat[3][2] = -(near_plane) / (far_plane - near_plane);
		}
		void SetPerspectiveLH(float fov, float aspect, float near_plane, float far_plane) {
			SetIdentity();
			float yscale = 1.0f / tanf(fov / 2.0f);
			float xscale = yscale / aspect;
			mat[0][0] = xscale;
			mat[1][1] = yscale;
			mat[2][2] = far_plane / (far_plane - near_plane);
			mat[2][3] = 1.0f;
			mat[3][2] = (-near_plane * far_plane) / (far_plane - near_plane);
		}
		void Translate(Vector3D translation) {
			mat[3][0] += translation.x;
			mat[3][1] += translation.y;
			mat[3][2] += translation.z;
		}
		void SetZDirection(Vector3D forward) {
			Vector3D currentRight = getXDirection().normalize();
			Vector3D currentUp = getYDirection().normalize();
			Vector3D currentForward = getZDirection().normalize();
			forward.normalize();
			//Vector3D worldUp = Vector3D(0, 1, 0);
			Vector3D worldUp = currentUp;
			if (abs(forward.dot(worldUp)) > 0.999f) {
				//worldUp = Vector3D(0, 0, 1); // If forward is close to up, use a different up vector
				worldUp = currentForward;
			}
			Vector3D right{};
			right = worldUp.cross(forward);
			right.normalize();
			Vector3D up = forward.cross(right);
			up.normalize();
			if ((currentRight.dot(right) + currentUp.dot(up)) < (currentRight.dot(right * -1) + currentUp.dot(up * -1))) {
				right = right * -1;
				up = up * -1;
			}
			float temp[4][4] = { {right.x, right.y, right.z, 0}, {up.x, up.y, up.z, 0},
					{forward.x, forward.y, forward.z, 0},
					{0, 0, 0, 1} };
			setMatrix(temp);
		}
		Vector3D ReverseMul(const Vector3D& vec) {
			Vector3D temp{};
			temp.x = vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0] + mat[3][0];
			temp.y = vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1] + mat[3][1];
			temp.z = vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2] + mat[3][2];
			return temp;
		}
		float mat[4][4] = {};
	};
}