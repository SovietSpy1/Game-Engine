#include "DX3D/Input/InputSystem.h"
#include <DX3D/Input/InputListener.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Game/Game.h>

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
}

void dx3d::InputSystem::Update()
{
	if (!listening) {
		return;
	}
	POINT mouse_point{};
	::GetCursorPos(&mouse_point);
	Point clientPos = Display::get()->GetClientPosition();
	switch (Game::mode) {
	case 1:
		mouse_pos = { mouse_point.x - clientPos.x, Display::get()->m_size.height - (mouse_point.y - clientPos.y) };
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
