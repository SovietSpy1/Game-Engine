#pragma once
#include <cmath>
#include <random>
#include <DX3D/Core/Common.h>
namespace dx3d {
	Color ColorLerp(Color startCol, Color endCol, float u);
	Color RandColor(float alpha = 10.0f);
	void FillInPosAndUV(vertex& vertex);
}