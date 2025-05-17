#pragma once
#include <DX3D/Core/Common.h>
#include <DX3D/Graphics/GraphicsResource.h>
namespace dx3d {
	class Mesh : public GraphicsResource
	{
	public:
		Mesh(const GraphicsResourceDesc& gDesc);
		void LoadHeartMesh();
		void LoadCubeMesh();
		void LoadQuadMesh();
		std::vector<vertex> vertices;
		Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> pixelBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	};
}


