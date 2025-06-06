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
				if (msg.wParam == VK_SPACE) {
					m_paused = !m_paused;
					m_display->getInputSystem()->showCursor(m_paused);
				}
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (++messageCount > maxMessages) {
				break;
			}
		}
		if (m_paused) {
			continue;
		}
		m_display->onUpdate();
		QueryPerformanceCounter(&Time::temp);
		Time::deltaTime = static_cast<float>(Time::temp.QuadPart - Time::end.QuadPart) / Time::frequency.QuadPart;
		while (Time::deltaTime < 1.0f / 60.0f) {
			QueryPerformanceCounter(&Time::temp);
			Time::deltaTime = static_cast<float>(Time::temp.QuadPart - Time::end.QuadPart) / Time::frequency.QuadPart;
		}
		Time::end = Time::temp;
		Time::currentTime = static_cast<float>(Time::end.QuadPart) / Time::frequency.QuadPart;
		/*if (currentTime > nextTime) {
			std::clog << 1.0f/Time::deltaTime << "\n";
			nextTime = currentTime + 0.5f; // Update every 0.5 seconds
		}*/
	}
}