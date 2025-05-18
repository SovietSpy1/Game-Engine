#pragma once
#include <DX3D/Math/Point.h>
namespace dx3d {
	class InputListener {
	public:
		InputListener() {}
		virtual void onKeyDown(int key) = 0;
		virtual void onKeyUp(int key) = 0;
		virtual void onMouseMove(const Point& delta_mouse_pos) = 0;
	};
}