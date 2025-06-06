#pragma once
#include <stdexcept>
#include <memory>
namespace dx3d {
	class Base;
	class Window;
	class Game;
	class RenderSystem;
	class GraphicsEngine;
	class Logger;
	class SwapChain;
	class Display;
	class DeviceContext;
	class VertexBuffer;
	class Mesh;
	class IndexBuffer;
	class ConstantBuffer;
	class InputSystem;
	class InputListener;
	class ResourceManager;
	class Resource;
	using i32 = int;
	using ui32 = unsigned int;
	using f32 = float;
	using d64 = double;

	using SwapChainPtr = std::shared_ptr<SwapChain>;
}