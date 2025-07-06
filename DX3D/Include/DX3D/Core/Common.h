#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Math/Rect.h>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Vector2D.h>
#include <DX3D/Math/Matrix.h>
#include <d3d11.h>
#include <chrono>
#include <cmath>
#include <vector>
namespace dx3d {
	struct vec2_int {
		int x, y;
	};
	struct smokeConstantBufferDesc {
		int resolution;
		float dt;
		float diff;
		float visc;
		vec2_int emissionPoint;
		int emissionRadius;
		float emission;
		float max = 1.0f;
		float min = 0.0f;
		int b = 0;
	};
	enum class RasterizerStateType {
		BackFace,
		FrontFace,
		AllFace
	};
	enum class IA {
		POSITION,
		TEXCOORD,
		NORMAL,
		COLOR
	};
	enum class Tags {
		Camera,
	};
	enum class ColliderType
	{
		Quad,
		Box,
		Sphere,
		Capsule,
		Cylinder,
		TriangleMesh
	};
	struct BaseDesc {
		Logger* logger;
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
	struct InputSystemDesc {
		BaseDesc base;
		HWND windowHandle{};
	};
	struct DeviceContextDesc {
		ID3D11DeviceContext& context;
	};
	struct AxisDesc {
		float scale;
	};
	struct Time {
		inline static LARGE_INTEGER start{};
		inline static LARGE_INTEGER end{};
		inline static LARGE_INTEGER frequency{};
		inline static LARGE_INTEGER temp{};
		inline static float deltaTime{};
		inline static float elapsedTime{};
		inline static float currentTime{};
	};

	struct vec2 {
		float u, v;
	};
	struct vec4_32 {
		uint8_t r, g, b, a;
		vec4_32 operator-=(const vec4_32& other) {
			r -= other.r;
			g -= other.g;
			b -= other.b;
			a -= other.a;
			return *this;
		}
		vec4_32 operator+=(const vec4_32& other) {
			r += other.r;
			g += other.g;
			b += other.b;
			a += other.a;
			return *this;
		}
	};
	struct vec4 {
		float r, g, b, a;
		vec4 operator+(const vec4& other) const {
			return vec4(r + other.r, g + other.g, b + other.b, a + other.a);
		}
		vec4 operator-(const vec4& other) const {
			return vec4(r - other.r, g - other.g, b - other.b, a - other.a);
		}
		vec4 operator*(const float& other) const {
			return vec4(r *other, g * other, b *other, a * other);
		}
		float magnitude() const{
			return std::sqrtf(std::powf(r, 2) + std::powf(g, 2) + std::powf(b, 2) + std::powf(a, 2));
		}
	};
	struct Color {
		vec4 rgba;
		Color operator+(const Color& other) const {
			return Color(rgba + other.rgba);
		}
		Color operator-(const Color& other) const {
			return Color(rgba - other.rgba);
		}
		Color operator*(const float& n) const {
			return Color(rgba * n);
		}
		float magnitude() const{
			return rgba.magnitude();
		}
	};
	struct vertex {
		Vector3D position{};
		Vector2D uv{};
		Vector3D normal{};
		vec4_32 color{};
	};
	enum class MeshType
	{
		Cube,
		Quad,
		Custom
	};
	_declspec(align(16)) struct ConstantBufferDesc {
		Matrix4X4 m_world{};
		Matrix4X4 m_view{};
		Matrix4X4 m_proj{};
		Vector4D lightDirection{};
		Vector4D camPosition{};
		Matrix4X4 m_rotation{};
		float elapsedTime{};
    };
}
#define PI 3.141594265f 
#define DEG2RAD(deg) ((deg) * (PI /180.0f))
#define WM_CUSTOM WM_USER + 1