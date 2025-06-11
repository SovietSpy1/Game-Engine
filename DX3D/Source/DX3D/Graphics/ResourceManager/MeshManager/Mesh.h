#pragma once
#include <DX3D/Core/Common.h>
#include <DX3D/Graphics/ResourceManager/Resource.h>
namespace dx3d {
	enum TopologyType
	{
		TriangleList,
		TriangleStrip,
		LineList,
		LineStrip,
		PointList
	};
	class Mesh : public Resource
	{
	public:
		Mesh(const GraphicsResourceDesc& gDesc);
		Mesh(const wchar_t* full_path, const GraphicsResourceDesc& desc);
		void LoadHeartMesh();
		void LoadCubeMesh();
		void LoadQuadMesh();
		void LoadMeshFromFile(const wchar_t* file_path);
		void ClearMeshData();
		std::vector<vertex> vertices;
		std::vector<UINT> indices;
		std::vector<Vector2D> uvs;
		std::vector<Vector3D> positions;
		std::vector<vec4> colors;
		Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> pixelBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		bool indexed{ false };
		TopologyType topology{ TopologyType::TriangleList };
	};
}


