#pragma once
#include <DX3D/Core/base.h>
#include <DX3D/Core/Common.h>
namespace dx3d {
	class Game: public Base
	{
	public:
		explicit Game(const GameDesc& desc);
		virtual ~Game() override;
		virtual void run() final;
		static inline int mode{};
	private:
		std::unique_ptr<Logger> m_loggerPtr{};
		std::unique_ptr<GraphicsEngine> m_graphicsEngine{};
		std::unique_ptr<PhysicsEngine> m_physicsEngine{};
		std::unique_ptr<InputSystem> m_inputSystem{};
		std::unique_ptr<Display> m_display{};
		bool m_isRunning{true};
		bool m_paused{ false };
	};
}

