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
		void LoadCubeMesh();
		void ClearMeshData();
		
		std::vector<vertex> vertices;
		std::vector<UINT> indices;
		Microsoft::WRL::ComPtr<ID3DBlob> vertexLayoutBlob;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		TopologyType topology{ TopologyType::TriangleList };
	};
}


