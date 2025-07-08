#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/VertexBuffer.h>
namespace dx3d {
	struct vertexWithColor {
		Vector3D position;
		vec4 color;
	};
	class Axis : public GraphicsResource{
	public:
		Axis(const AxisDesc & desc, const GraphicsResourceDesc& gDesc) : GraphicsResource(gDesc) {
			mag = desc.scale;
			RenderSystem* renderSystem = GraphicsEngine::get()->getRenderSystem();
			const WCHAR* vertexShaderPath = L"DX3D/Shaders/Axis/VertexShader.hlsl";
			const WCHAR* pixelShaderPath = L"DX3D/Shaders/Axis/PixelShader.hlsl";
			renderSystem->compilePixelShader(pixelShaderPath, pixelBlob);
			renderSystem->compileVertexShader(vertexShaderPath, vertexBlob);
			renderSystem->createVertexShader(vertexBlob, vertexShader);
			renderSystem->createPixelShader(pixelBlob, pixelShader);
			std::vector<vertexWithColor> vertices(6);
			vertices[0] = { Vector3D(0, 0, 0), vec4(1, 0, 0, 1) }; // X axis origin
			vertices[1] = { Vector3D(1,0,0) * mag, vec4(1, 0, 0, 1) }; // X axis end
			vertices[2] = { Vector3D(0, 0, 0), vec4(0, 1, 0, 1) }; // Y axis origin
			vertices[3] = { Vector3D(0,1,0) * mag, vec4(0, 1, 0, 1) }; // Y axis end
			vertices[4] = { Vector3D(0, 0, 0), vec4(0, 0, 1, 1) }; // Z axis origin
			vertices[5] = { Vector3D(0,0,1) * mag, vec4(0, 0, 1, 1) }; // Z axis end
			axisVertexBuffer = renderSystem->createVertexBuffer();
			axisVertexBuffer->load(vertices.data(), sizeof(vertexWithColor), vertices.size(), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), std::vector<IA>{IA::POSITION, IA::COLOR});
		}
	public:
		float mag;
		std::shared_ptr<VertexBuffer> axisVertexBuffer{};
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob{};
		Microsoft::WRL::ComPtr<ID3DBlob> pixelBlob{};
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader{};
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader{};
		friend class Display;
	};
}