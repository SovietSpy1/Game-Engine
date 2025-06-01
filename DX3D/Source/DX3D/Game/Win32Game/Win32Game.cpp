#include <DX3D/Game/Game.h>
#include <Windows.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Input/InputSystem.h>
void dx3d::Game::run()
{
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
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		if (frequency.QuadPart == 0) {
			DX3DLogErrorAndThrow("QueryPerformanceFrequency returned zero.");
		}
		else {
			if (Time::endCounter != 0) {
				Time::deltaCounter = static_cast<float>(counter.QuadPart - Time::endCounter);
				Time::deltaTime = Time::deltaCounter / static_cast<float>(frequency.QuadPart);
			}
			Time::endCounter = static_cast<float>(counter.QuadPart);
			Time::elapsedTime = (Time::endCounter - Time::startCounter) / static_cast<float>(frequency.QuadPart);
		}
		DX3DLogInfo(std::to_string((1.0f/Time::deltaTime)).c_str())
		m_display->onUpdate();
	}
}