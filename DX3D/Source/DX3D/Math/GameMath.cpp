#pragma once
#include <cmath>
#include <random>
#include <DX3D/Math/GameMath.h>

namespace dx3d {
	Color ColorLerp(Color startCol, Color endCol, float u) {
		return startCol * (1.0f - u) + endCol * ((float)u);
	}
	Color RandColor(float alpha) {
		return Color{ ((float)std::rand()) / RAND_MAX, ((float)std::rand()) / RAND_MAX , ((float)std::rand()) / RAND_MAX , alpha == 10.0f ? ((float)std::rand()) / RAND_MAX : alpha };
	}
	void FillInPosAndUV(vertex& vertex, Vector3D pos)
	{
		vertex.position = pos;
	}
}
