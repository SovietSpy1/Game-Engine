#pragma once
#include <vector>
namespace dx3d {
	class Vector3D {
	public:
		Vector3D() : x(0), y(0), z(0) 
		{ 
		}
		Vector3D(float x,float y,float z) : x(x), y(y), z(z)
		{
		}
		Vector3D(const Vector3D& vector) : x(vector.x), y(vector.y), z(vector.z) {}
		Vector3D operator+(const Vector3D& other) const {
			return Vector3D(x + other.x, y + other.y, z + other.z);
		}
		Vector3D operator+=(const Vector3D& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vector3D operator-(const Vector3D& other) const {
			return Vector3D(x - other.x, y - other.y, z - other.z);
		}
		Vector3D operator*(const float& other) const {
			return Vector3D(x * other, y * other, z * other);
		}
		void operator*=(const Vector3D& other){
			x *= other.x;
			y *= other.y;
			z *= other.z;
		}
		Vector3D operator*(const Vector3D& other) const {
			return Vector3D(x * other.x, y * other.y, z * other.z);
		}
		Vector3D cross(Vector3D b) const{
			Vector3D result{};
			result.x = y * b.z - z * b.y;
			result.y = -(x * b.z - z * b.x);
			result.z = x * b.y - y * b.x;
			return result;
		}
		float dot(const Vector3D& other) const {
			return x * other.x + y * other.y + z * other.z;
		}
		float mag() const {
			return sqrtf(x * x + y * y + z * z);
		}
		Vector3D normalize() {
			float magnitude = sqrtf(x * x + y * y + z * z);
			if (magnitude > 0) {
				x /= magnitude;
				y /= magnitude;
				z /= magnitude;
			}
			return *this;
		}
		static inline Vector3D up() {
			return Vector3D(0.0f, 1.0f, 0.0f);
		}
		static inline Vector3D right() {
			return Vector3D(1.0f, 0.0f, 0.0f);
		}
		static inline Vector3D forward() {
			return Vector3D(0.0f, 0.0f, 1.0f);
		}
		static float det(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) {
			return v1.x * (v2.y * v3.z - v2.z * v3.y) - v1.y * (v2.x * v3.z - v2.z * v3.x) + v1.z * (v2.x * v3.y - v2.y * v3.x);
		}
		static Vector3D lerp(const Vector3D& start, const Vector3D& end, float u) {
			return start * (1.0f - u) + end * (u);
		}
		float x, y, z;
	};
}