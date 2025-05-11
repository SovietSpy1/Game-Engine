#pragma once
#include <cmath>
#include <random>
namespace dx3d {
	struct Color {
		float red,
			green,
			blue,
			alpha;
		Color operator+(const Color& other) const{
			return Color({ red + other.red, green + other.green, blue + other.blue, alpha + other.alpha });
		}
		Color operator-(const Color& other) const {
			return Color({ red - other.red, green - other.green, blue - other.blue, alpha - other.alpha });
		}
		Color operator*(float n) const{
			return Color({ red * n, green * n,blue * n,alpha * n });
		}
		float magnitude() {
			return std::sqrt(std::pow(red, 2) + std::pow(green, 2) + std::pow(blue, 2) + std::pow(alpha, 2));
		}

	};
	class GameMath {
	public:
		static Color ColorLerp(Color startCol, Color endCol, float u) {
			return startCol*(1.0f - u) + endCol*((float)u);
		}
		static Color RandColor(bool randAlpha = false){
			return Color{ ((float)std::rand()) / RAND_MAX, ((float)std::rand()) / RAND_MAX , ((float)std::rand()) / RAND_MAX , randAlpha ? ((float)std::rand()) / RAND_MAX : 1 };
		}
	};
}