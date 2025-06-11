#pragma once
namespace dx3d {
	class Vector2D {
	public:
		Vector2D() : x(0), y(0)
		{}
		Vector2D(float x, float y) : x(x), y(y)
		{
		}
		Vector2D(const Vector2D& vector) : x(vector.x), y(vector.y){}
		Vector2D operator+(const Vector2D& other) const {
			return Vector2D(x + other.x, y + other.y);
		}
		Vector2D operator-(const Vector2D& other) const {
			return Vector2D(x - other.x, y - other.y);
		}
		Vector2D operator*(const float& other) const {
			return Vector2D(x * other, y * other);
		}
		static Vector2D lerp(const Vector2D& start, const Vector2D& end, float u) {
			return start * (1.0f - u) + end * (u);
		}
		float x, y;
	private:

	};
}