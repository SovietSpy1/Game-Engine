#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/Input/InputSystem.h>
#include <iostream>
dx3d::Game::Game(const GameDesc& desc) : Base({ std::make_unique<Logger>(desc.logLevel).release() }), m_loggerPtr{ m_logger }
{
	m_graphicsEngine = std::make_unique<GraphicsEngine>(GraphicsEngineDesc{ m_logger });
	m_physicsEngine = std::make_unique<PhysicsEngine>(BaseDesc{ m_logger });
	m_inputSystem = std::make_unique<InputSystem>(BaseDesc{ m_logger });
	int demoChoice = 0;
	bool valid = false;
	std::cout << "Welcome to my game engine!" << std::endl;
	std::cout << "Choose which demo you would like to view: " << std::endl;
	std::cout << "1: Smoke Demo" << std::endl;
	std::cout << "2: Lighting and Physics Demo" << std::endl;
	while (!valid) {
		std::cin >> demoChoice;
		if (demoChoice != 1 && demoChoice != 2) {
			DX3DLogWarning("Invalid choice, please choose 1 or 2.");
		}
		else {
			valid = true;
			mode = demoChoice;
		}
		std::cout << std::endl;
	}
	switch (mode) {
	case 1:
		std::cout << "Smoke Demo selected." << std::endl;
		m_inputSystem->showCursor(true);
		break;
	case 2:
		std::cout << "Lighting and Physics Demo Selected." << std::endl;
		m_inputSystem->showCursor(false);
		break;
	}
	std::cout << "Basic Controls:" << std::endl;
	std::cout << "Space: Pause/Unpause" << std::endl;
	std::cout << "F: Toggle Fullscreen" << std::endl;
	std::cout << "Esc: Exit" << std::endl;
	switch (mode) {
	case 1:
		std::cout << "Left Click: Emit Smoke" << std::endl;
		break;
	case 2:
		std::cout << "WASD: Move" << std::endl;
		std::cout << "Mouse: Look around" << std::endl;
		std::cout << "Shift: Jump" << std::endl;
		break;
	}
	std::cout << "Enter any character to Play the game!" << std::endl;
	int playChoice = 0;
	std::cin >> playChoice;
	m_display = std::make_unique<Display>(DisplayDesc{ {m_logger, {1080, 720}}, m_graphicsEngine->getRenderSystem()});
	DX3DLogInfo("Game initialized");
}

dx3d::Game::~Game()
{
	DX3DLogInfo("Game deallocation started");
}
