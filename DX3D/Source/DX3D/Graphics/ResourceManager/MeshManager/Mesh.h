#pragma once
#include <DX3D/Core/Common.h>
#include <DX3D/Graphics/ResourceManager/Resource.h>
#include <variant>
#include <unordered_map>
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
		Mesh(const GraphicsResourceDesc& gDesc, MeshType meshType);
		void LoadCubeMesh();
		void Load3DCubeMesh();
		void LoadQuadMesh();
		void Demo();
		std::vector<vertex> vertices;
		std::vector<UINT> indices;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		MeshType meshType{ MeshType::Custom };
		TopologyType topology{ TopologyType::TriangleList };
	};
}


