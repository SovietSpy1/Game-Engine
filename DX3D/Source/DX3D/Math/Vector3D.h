#pragma once
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
		Vector3D operator-(const Vector3D& other) const {
			return Vector3D(x - other.x, y - other.y, z - other.z);
		}
		float x, y, z;
	private:
		
	};
}