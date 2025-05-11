#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Math/Rect.h>
#include <d3d11.h>
#include <chrono>
#include <cmath>
namespace dx3d {
	struct BaseDesc {
		Logger& logger;
	};
	struct WindowDesc {
		BaseDesc base;
		Rect size{};
	};
	struct DisplayDesc {
		WindowDesc window;
		RenderSystem& rendererSystem;
	};
	struct GraphicsEngineDesc {
		BaseDesc base;
	};
	struct RenderSystemDesc {
		BaseDesc base;
	};
	struct GameDesc {
		Logger::LogLevel logLevel = Logger::LogLevel::Error;
	};
	struct SwapChainDesc{
		void* winHandle{};
		Rect winSize{};
	};
	struct DeviceContextDesc {
		ID3D11DeviceContext& context;
	};
	struct Time {
		static std::chrono::steady_clock::time_point startTime;
		static std::chrono::steady_clock::time_point lastTime;
		static float deltaTime;
		static float elapsedTime;
	};

}