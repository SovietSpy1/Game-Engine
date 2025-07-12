#include "DX3D/Input/InputSystem.h"
#include <DX3D/Input/InputListener.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Game/Game.h>
#include <algorithm>
#include <Windows.h>

dx3d::InputSystem::InputSystem(const BaseDesc& desc) : Base(desc)
{
	S = this;
}

dx3d::InputSystem* dx3d::InputSystem::get()
{
	return S;
}

void dx3d::InputSystem::addListener(InputListener* listener)
{
	listeners.insert(listener);
}

void dx3d::InputSystem::removeListener(InputListener* listener)
{
	listeners.erase(listener);
}

void dx3d::InputSystem::setCursorPosition(const Point& pos)
{
	::SetCursorPos(pos.x, pos.y);
}

void dx3d::InputSystem::onFocusGained()
{
	// Reset cursor state when application gains focus
	if (Game::mode == 2) {
		// For FPS mode, hide cursor and center it
		showCursor(false);
		// Center the cursor immediately
		Point centerPos = {
			(int)Display::get()->m_size.width / 2,
			(int)Display::get()->m_size.height / 2
		};
		setCursorPosition(centerPos);
		// Reset mouse position to center to prevent immediate movement
		mouse_pos = centerPos;
		last_mouse_pos = centerPos;
		first_call = true;
	} else {
		// For other modes, show cursor
		showCursor(true);
	}
}

void dx3d::InputSystem::onFocusLost()
{
	// Show cursor when application loses focus
	showCursor(true);
	// Reset first_call flag so cursor state is properly restored when focus is regained
	first_call = true;
}

void dx3d::InputSystem::onWindowResized()
{
	// Reset mouse position and cursor state when window is resized
	Point centerPos = {
		(int)Display::get()->m_size.width / 2,
		(int)Display::get()->m_size.height / 2
	};
	
	if (Game::mode == 2) {
		// For FPS mode, hide cursor and center it
		showCursor(false);
		setCursorPosition(centerPos);
		mouse_pos = centerPos;
		last_mouse_pos = centerPos;
	} else {
		// For other modes, show cursor but reset position
		showCursor(true);
		// Get current mouse position relative to new client area
		POINT mouse_point{};
		::GetCursorPos(&mouse_point);
		POINT clientPoint = mouse_point;
		if (ScreenToClient(static_cast<HWND>(Display::get()->m_handle), &clientPoint)) {
			mouse_pos = { clientPoint.x, Display::get()->m_size.height - clientPoint.y };
		} else {
			// Fallback: calculate client position manually
			RECT windowRect, clientRect;
			GetWindowRect(static_cast<HWND>(Display::get()->m_handle), &windowRect);
			GetClientRect(static_cast<HWND>(Display::get()->m_handle), &clientRect);
			
			// Calculate client area offset
			int clientX = windowRect.left + ((windowRect.right - windowRect.left) - clientRect.right) / 2;
			int clientY = windowRect.top + (windowRect.bottom - windowRect.top) - clientRect.bottom;
			
			mouse_pos = { mouse_point.x - clientX, Display::get()->m_size.height - (mouse_point.y - clientY) };
		}
		last_mouse_pos = mouse_pos;
	}
	
	first_call = true;
}

void dx3d::InputSystem::Update()
{
	if (!listening) {
		return;
	}
	POINT mouse_point{};
	::GetCursorPos(&mouse_point);
	
	switch (Game::mode) {
	case 1:
		// Use ScreenToClient for accurate coordinate conversion
		POINT clientPoint = mouse_point;
		if (ScreenToClient(static_cast<HWND>(Display::get()->m_handle), &clientPoint)) {
			mouse_pos = { clientPoint.x, Display::get()->m_size.height - clientPoint.y };
		} else {
			// Fallback: calculate client position manually
			RECT windowRect, clientRect;
			GetWindowRect(static_cast<HWND>(Display::get()->m_handle), &windowRect);
			GetClientRect(static_cast<HWND>(Display::get()->m_handle), &clientRect);
			
			// Calculate client area offset
			int clientX = windowRect.left + ((windowRect.right - windowRect.left) - clientRect.right) / 2;
			int clientY = windowRect.top + (windowRect.bottom - windowRect.top) - clientRect.bottom;
			
			mouse_pos = { mouse_point.x - clientX, Display::get()->m_size.height - (mouse_point.y - clientY) };
		}
		
		// Clamp mouse position to window bounds
		mouse_pos.x = std::max(0, std::min(mouse_pos.x, (int)Display::get()->m_size.width));
		mouse_pos.y = std::max(0, std::min(mouse_pos.y, (int)Display::get()->m_size.height));
		break;
	case 2:
		mouse_pos = { mouse_point.x, mouse_point.y };
		::SetCursorPos((int)Display::get()->m_size.width / 2.0f, (int)Display::get()->m_size.height / 2.0f);
		break;
	}
	
	if (first_call) {
		last_mouse_pos = mouse_pos;
		first_call = false;
	}
	if ((mouse_pos.x != last_mouse_pos.x || mouse_pos.y != last_mouse_pos.y)) {
		std::set<InputListener*>::iterator it = listeners.begin();
		while (it != listeners.end()) {
			(*it)->onMouseMove(Point(mouse_pos.x, mouse_pos.y));
			it++;
		}
	}
	last_mouse_pos = mouse_pos;
	if (::GetKeyboardState(keys_state)) {
		for (int i = 0; i < 256; i++) {
			std::set<InputListener*>::iterator it = listeners.begin();
			bool isNew = keys_state[i] != old_keys_state[i];
			//key is down
			if (keys_state[i] & 0x80) {
				while (it != listeners.end()) {
					if (i == VK_LBUTTON) {
						if (isNew) (*it)->onLeftMouseDown(mouse_pos);
						(*it)->onHoldLeftMouseDown(mouse_pos);
					}
					else if (i == VK_RBUTTON) {
						if (isNew) (*it)->onRightMouseDown(mouse_pos);
					}
					else {
						if (isNew) (*it)->onKeyDownFirst(i);
						(*it)->onKeyDown(i);
					}
					it++;
				}
			}
			//key is up
			else {
				if (old_keys_state[i] != keys_state[i]) {
					while (it != listeners.end()) {
						(*it)->onKeyUp(i);
						it++;
					}
				}
			}
		}
		::memcpy(old_keys_state, keys_state, sizeof(unsigned char) * 256);
	}
}
