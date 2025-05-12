#pragma once
#include <cmath>
#include <random>
#include <DX3D/Core/Common.h>
namespace dx3d {
	struct Color {
		vec4 rgba;
		Color operator+(const Color& other) const{
			return Color(rgba + other.rgba);
		}
		Color operator-(const Color& other) const {
			return Color(rgba-other.rgba);
		}
		Color operator*(const float& n) const{
			return Color(rgba * n);
		}
		float magnitude() {
			return rgba.magnitude();
		}
	};
	Color ColorLerp(Color startCol, Color endCol, float u);
	Color RandColor(float alpha = 10.0f);
}