#include <DX3D/Game/Game.h>
#include <Windows.h>
#include <DX3D/Game/Display.h>
void dx3d::Game::run()
{
	std::chrono::steady_clock::time_point currentTime;
	MSG msg{};
	while (m_isRunning) {
		currentTime = std::chrono::steady_clock::now();
		Time::deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - Time::lastTime).count() / 1000.0f;
		Time::lastTime = currentTime;
		DX3DLogInfo(std::to_string((1.0f/Time::deltaTime)).c_str())
		Time::elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Time::lastTime - Time::startTime).count() / 1000.0f;
		m_display->onUpdate();
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				m_isRunning = false;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}
}