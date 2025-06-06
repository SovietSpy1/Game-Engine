#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Math/Rect.h>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Matrix.h>
#include <d3d11.h>
#include <chrono>
#include <cmath>
#include <vector>
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
	struct InputSystemDesc {
		BaseDesc base;
		HWND windowHandle{};
	};
	struct DeviceContextDesc {
		ID3D11DeviceContext& context;
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
			return std::sqrt(std::pow(r, 2) + std::pow(g, 2) + std::pow(b, 2) + std::pow(a, 2));
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
		float magnitude() {
			return rgba.magnitude();
		}
	};
	struct vertex {
		Vector3D position;
		vec4 color;
		vec2 uvs;
	};
	struct MeshDesc {
		std::vector<vertex> vertices;
		WCHAR* vectorShaderPath;
		WCHAR* pixelShaderPath;
	};
	_declspec(align(16)) struct ConstantBufferDesc {
		Matrix4X4 m_world{};
		Matrix4X4 m_view{};
		Matrix4X4 m_proj{};
        float elapsedTime = 0.0f;  
    };
}
#define PI 3.141594265f
#define DEG2RAD(deg) ((deg) * (PI /180.0f))
#define WM_CUSTOM WM_USER + 1