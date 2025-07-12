#include <DX3D/Game/Game.h>
#include <Windows.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Input/InputSystem.h>
#include <iostream>
#include <thread>
void dx3d::Game::run()
{
	QueryPerformanceFrequency(&Time::frequency);  // Get timer frequency
	QueryPerformanceCounter(&Time::start);
	double nextTime{};
	QueryPerformanceCounter(&Time::end);// End time
	MSG msg{};
	while (m_isRunning) {
		const int maxMessages = 10;
		int messageCount = 0;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				m_isRunning = false;
				break;
			}
			if (msg.message == WM_KEYDOWN) {
				if (msg.wParam == VK_ESCAPE) {
					m_isRunning = false;
				}
				if (msg.wParam == 'F') {
					m_display->toggleFullScreen();
				}
				if (msg.wParam == VK_SPACE) {
					m_paused = !m_paused;
					if (InputSystem::get() != nullptr) {
						if (m_paused) {
							// Show cursor when paused
							InputSystem::get()->showCursor(true);
						} else {
							// Restore cursor state based on game mode when unpaused
							if (Game::mode == 2) {
								InputSystem::get()->showCursor(false);
								// Re-center cursor for FPS mode
								Point centerPos = {
									(int)Display::get()->m_size.width / 2,
									(int)Display::get()->m_size.height / 2
								};
								InputSystem::get()->setCursorPosition(centerPos);
							} else {
								InputSystem::get()->showCursor(true);
							}
						}
					}
				}
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			/*if (++messageCount > maxMessages) {
				break;
			}*/
		}
		if (m_display->pause) {
			m_paused = true;
			m_display->pause = false;
			// Show cursor when display is paused
			if (InputSystem::get() != nullptr) {
				InputSystem::get()->showCursor(true);
			}
		}
		if (m_paused) {
			QueryPerformanceCounter(&Time::end);
			continue;
		}
		m_display->Update();
		QueryPerformanceCounter(&Time::temp);
		Time::deltaTime = static_cast<float>(Time::temp.QuadPart - Time::end.QuadPart) / Time::frequency.QuadPart;
		
		// Cap deltaTime to prevent large jumps in Release mode
		const float maxDeltaTime = 1.0f / 30.0f; // Cap at 30 FPS equivalent
		if (Time::deltaTime > maxDeltaTime) {
			Time::deltaTime = maxDeltaTime;
		}
		
		// Ensure minimum frame time for consistent physics
		while (Time::deltaTime < 1.0f / 60.0f) {
			QueryPerformanceCounter(&Time::temp);
			Time::deltaTime = static_cast<float>(Time::temp.QuadPart - Time::end.QuadPart) / Time::frequency.QuadPart;
		}
		Time::end = Time::temp;
		Time::currentTime = static_cast<float>(Time::end.QuadPart) / Time::frequency.QuadPart;
		Time::elapsedTime += Time::deltaTime;
	}
}