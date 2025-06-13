#pragma once
#include <DX3D/Math/Vector3D.h>
namespace dx3d {
	class Vector4D {
	public:
		Vector4D() : x(0), y(0), z(0), w(0)
		{
		}
		Vector4D(const Vector3D& vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(1.0f)
		{
		}
		Vector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
		{
		}
		Vector4D(const Vector4D& vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {};
		float x, y, z, w;
	private:

	};
}