#pragma once
#include <DX3D/Core/Base.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
namespace dx3d {
	class Grid {
	public:
		Grid() {
			if (vertexShader == nullptr) {
				const WCHAR* vertexShaderPath = L"DX3D/Shaders/Grid/VertexShader.hlsl";
				Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob;
				GraphicsEngine::get()->getRenderSystem()->compileVertexShader(vertexShaderPath, vertexBlob);
				GraphicsEngine::get()->getRenderSystem()->createVertexShader(vertexBlob, vertexShader);
			}
			if (pixelShader == nullptr) {
				const WCHAR* pixelShaderPath = L"DX3D/Shaders/Grid/PixelShader.hlsl";
				Microsoft::WRL::ComPtr<ID3DBlob> pixelBlob;
				GraphicsEngine::get()->getRenderSystem()->compilePixelShader(pixelShaderPath, pixelBlob);
				GraphicsEngine::get()->getRenderSystem()->createPixelShader(pixelBlob, pixelShader);
			}
		}
		void LoadGrid(int cellCount) {
			vertices.clear();
			lines.clear();
			float d = 1.0f / cellCount;
			float xOffset{};
			float yOffset{};
			for (int i = 0; i <= cellCount; i++) {
				Vector3D horLineStart = Vector3D(-0.5f, 0,-0.5f + yOffset);
				Vector3D horLineEnd = Vector3D(0.5f, 0,-0.5f + yOffset);
				Vector3D vertLineStart = Vector3D(-0.5f + xOffset,0, -0.5f);
				Vector3D vertLineEnd = Vector3D(-0.5f + xOffset, 0,0.5f);
				lines.push_back(horLineStart);
				lines.push_back(horLineEnd);
				lines.push_back(vertLineStart);
				lines.push_back(vertLineEnd);
				xOffset += d;
				yOffset += d;
			}
			std::vector<vec4_32> colors(lines.size(), vec4_32{255,0,0,255});
			vertices = VertexBuffer::FillInVertexData({ {IA::POSITION, lines}, {IA::COLOR, colors} });
			vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer();
			vertexBuffer->load(vertices.data(), sizeof(vertex), vertices.size());
		}
		void LoadBorder() {
			vertices.clear();
			lines.clear();
			Vector3D temp = Vector3D(-0.5f, 0, -0.5f);
			lines.push_back(temp);
			temp = Vector3D(0.5f, 0, -0.5f);
			lines.push_back(temp);
			temp = Vector3D(-0.5f, 0, -0.5f);
			lines.push_back(temp);
			temp = Vector3D(-0.5f, 0, 0.5f);
			lines.push_back(temp);
			temp = Vector3D(-0.5f, 0, 0.5f);
			lines.push_back(temp);
			temp = Vector3D(0.5f, 0, 0.5f);
			lines.push_back(temp);
			temp = Vector3D(0.5f, 0, 0.5f);
			lines.push_back(temp);
			temp = Vector3D(0.5f, 0, -0.5f);
			lines.push_back(temp);
			std::vector<vec4_32> colors(lines.size(), vec4_32{ 0,0,255,255 });
			vertices = VertexBuffer::FillInVertexData({ {IA::POSITION, lines}, {IA::COLOR, colors} });
			vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer();
			vertexBuffer->load(vertices.data(), sizeof(vertex), vertices.size());
		}
		static inline Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader{};
		static inline Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader{};
		std::vector<vertex> vertices;
		std::vector<Vector3D> lines;
		std::shared_ptr<VertexBuffer> vertexBuffer;
	};
}