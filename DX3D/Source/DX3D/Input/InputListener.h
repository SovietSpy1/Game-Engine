#pragma once
#include <DX3D/Math/Point.h>
namespace dx3d {
	class InputListener {
	public:
		InputListener() {}
		virtual void onKeyDown(int key) {};
		virtual void onKeyUp(int key) {};
		virtual void onMouseMove(const Point& mouse_pos) {};
		virtual void onLeftMouseDown(const Point& mouse_pos) {};
		virtual void onRightMouseDown(const Point& mouse_pos) {};
		virtual void onKeyDownFirst(int key) {};
		virtual void onHoldLeftMouseDown(const Point& mouse_pos) {};
	};
}