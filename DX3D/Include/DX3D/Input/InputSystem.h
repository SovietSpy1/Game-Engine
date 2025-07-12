#pragma once
#include <DX3D/Core/Common.h>
#include <DX3D/Math/Point.h>
#include <DX3D/Core/Base.h>
#include <map>
#include <set>
namespace dx3d {
	class InputSystem : public Base
	{
	public:
		InputSystem(const BaseDesc& desc);
		static InputSystem* get();
		void addListener(InputListener* listener);
		void removeListener(InputListener* listener);
		void setCursorPosition(const Point& pos);
		void Update();
		void showCursor(bool show) {
			::ShowCursor(show);
		}
		void onFocusGained();
		void onFocusLost();
		void onWindowResized();
		bool listening = true;
	private:
		static inline InputSystem* S = nullptr;
	private:
		std::set<InputListener*> listeners{};
		unsigned char keys_state[256] = {};
		unsigned char old_keys_state[256] = {};
		Point mouse_pos{};
		Point last_mouse_pos{};
		bool first_call = true;
	};
}

