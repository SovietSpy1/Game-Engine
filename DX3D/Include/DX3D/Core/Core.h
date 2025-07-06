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
	class TextureManager;
	class Texture;
	class GameObject;
	class MeshManager;
	class Axis;
	class Transform;
	class Material;
	class Collider;
	class PhysicsEngine;
	class RigidBody;
	class Camera;
	class SkyBox;
	class GameManager;
	class ComponentBase;
	class Smoke;
	class Grid;
	class StructuredBuffer;
	struct BufferPair;
	using i32 = int;
	using ui32 = unsigned int;
	using f32 = float;
	using d64 = double;

	using SwapChainPtr = std::shared_ptr<SwapChain>;
}