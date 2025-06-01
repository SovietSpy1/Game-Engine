#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Game/Display.h>
float dx3d::Time::deltaTime{};
float dx3d::Time::deltaCounter{};
float dx3d::Time::elapsedTime{};
float dx3d::Time::endCounter{};
float dx3d::Time::startCounter{};
float dx3d::Time::startTime{};
dx3d::Game::Game(const GameDesc& desc) : Base({ *std::make_unique<Logger>(desc.logLevel).release() }), m_loggerPtr{ &m_logger }
{
	m_graphicsEngine = std::make_unique<GraphicsEngine>(GraphicsEngineDesc{m_logger});
	m_display = std::make_unique<Display>(DisplayDesc{ {m_logger, {1080, 720}}, m_graphicsEngine->getRenderSystem()});


	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	Time::startCounter = static_cast<float>(counter.QuadPart);
	DX3DLogInfo("Game initialized");
}

dx3d::Game::~Game()
{
	DX3DLogInfo("Game deallocation started");
}
