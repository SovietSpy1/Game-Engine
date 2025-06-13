#include "DX3D/Graphics/ResourceManager/MeshManager/Mesh.h"
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Math/GameMath.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/TextureManager.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DirectXTex.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <locale>
std::string WideCharToUTF8(const wchar_t* wideStr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
	if (size_needed == 0) return std::string();

	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &result[0], size_needed, nullptr, nullptr);

	// Remove the null terminator that Windows API adds
	result.resize(size_needed - 1);

	return result;
}
dx3d::Mesh::Mesh(const GraphicsResourceDesc& gDesc) : Resource(gDesc)
{
	indexBuffer = gDesc.renderSystem->createIndexBuffer();
	vertexBuffer = gDesc.renderSystem->createVertexBuffer();
}

dx3d::Mesh::Mesh(const wchar_t* full_path, const GraphicsResourceDesc& desc) : Resource(full_path, desc)
{
	const WCHAR* vertexShaderPath = L"DX3D/Shaders/VertexMeshLayout/VertexShader.hlsl";
	m_renderSystem->compileVertexShader(vertexShaderPath, vertexLayoutBlob);
	indexBuffer = desc.renderSystem->createIndexBuffer();
	vertexBuffer = desc.renderSystem->createVertexBuffer();
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;
	std::string inputfile = WideCharToUTF8(full_path);
	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str());
	if (!err.empty()) {
		DX3DLogErrorAndThrow("Error loading mesh from file.");
	}
	if (!res) {
		DX3DLogErrorAndThrow("Error loading mesh from file.");
	}
	/*if (shapes.size() > 1) {
		DX3DLogErrorAndThrow("Error loading mesh from file.");
	}*/
	for (size_t s = 0; s < shapes.size(); s++) {
		vertices.reserve(shapes.at(s).mesh.indices.size());
		indices.reserve(shapes.at(s).mesh.indices.size());
		size_t indexOffset = 0;
		size_t numFaces = shapes.at(s).mesh.num_face_vertices.size();
		for (size_t f = 0; f < numFaces; f++) {
			int num_face_verts = shapes.at(s).mesh.num_face_vertices[f];
			for (int v = 0; v < num_face_verts; v++) {
				tinyobj::index_t index = shapes.at(s).mesh.indices.at(indexOffset + v);
				tinyobj::real_t vx = attribs.vertices.at(index.vertex_index * 3 + 0);
				tinyobj::real_t vy = attribs.vertices.at(index.vertex_index * 3 + 1);
				tinyobj::real_t vz = attribs.vertices.at(index.vertex_index * 3 + 2);
				tinyobj::real_t tx = 0.0f, ty = 0.0f;
				if (attribs.texcoords.size() != 0) {
					tx = attribs.texcoords.at(index.texcoord_index * 2 + 0);
					ty = attribs.texcoords.at(index.texcoord_index * 2 + 1);
				}
				tinyobj::real_t nx = 0.0f, ny = 0.0f, nz = 0.0f;
				if (attribs.normals.size() != 0) {
					nx = attribs.normals.at(index.normal_index * 3 + 0);
					ny = attribs.normals.at(index.normal_index * 3 + 1);
					nz = attribs.normals.at(index.normal_index * 3 + 2);
				}
				vertex vrtx = vertex{ Vector3D(vx,vy,vz), Vector2D(tx,ty), Vector3D(nx,ny,nz)};
				vertices.push_back(vrtx);
				indices.push_back(indexOffset + v);
			}
			indexOffset += num_face_verts;
		}
	}
	vertexBuffer->load(vertices.data(), sizeof(vertex), vertices.size(), vertexLayoutBlob->GetBufferPointer(), vertexLayoutBlob->GetBufferSize(), std::vector<IA>{IA::POSITION, IA::TEXCOORD, IA::NORMAL});
	indexBuffer->load(indices.data(), indices.size());
}
void dx3d::Mesh::LoadCubeMesh()
{
	const WCHAR* vertexShaderPath = L"DX3D/Shaders/Cube/VertexShader.hlsl";
	m_renderSystem->compileVertexShader(vertexShaderPath, vertexLayoutBlob);
	topology = TopologyType::TriangleList;
	//set positions
	std::vector<Vector3D>positions = {
		Vector3D(-0.5f, -0.5f, -0.5f),
		Vector3D(-0.5f, 0.5f, -0.5f),
		Vector3D(0.5f, 0.5f, -0.5f),
		Vector3D(0.5f, -0.5f, -0.5f),
		Vector3D(0.5f, -0.5f, 0.5f),
		Vector3D(0.5f, 0.5f, 0.5f),
		Vector3D(-0.5f, 0.5f, 0.5f),
		Vector3D(-0.5f, -0.5f, 0.5f)
	};
	//set indexes
	indices =
	{
		//front
		0,1,2, // first tri
		2,3,0, //second tri
		//back
		4,5,6,
		6,7,4,
		//top
		8,9,10,
		10,11,8,
		//bottom
		12,13,14,
		14,15,12,
		//right
		16,17,18,
		18,19,16,
		//left
		20,21,22,
		22,23,20
	};

	//set uvs
	std::vector<Vector2D>uvs = {
		Vector2D(0.0f, 0.0f),
		Vector2D(0.0f, 1.0f),
		Vector2D(1.0f, 0.0f),
		Vector2D(1.0f, 1.0f)
	};

	//set vertices
	vertices = {
		//front face
		{positions[0], uvs[1]},
		{positions[1], uvs[0]},
		{positions[2], uvs[2]},
		{positions[3], uvs[3]},
		//back face
		{positions[4], uvs[1]},
		{positions[5], uvs[0]},
		{positions[6], uvs[2]},
		{positions[7], uvs[3]},
		
		{positions[1], uvs[1]},
		{positions[6], uvs[0]},
		{positions[5], uvs[2]},
		{positions[2], uvs[3]},

		{positions[7], uvs[1]},
		{positions[0], uvs[0]},
		{positions[3], uvs[2]},
		{positions[4], uvs[3]},

		{positions[3], uvs[1]},
		{positions[2], uvs[0]},
		{positions[5], uvs[2]},
		{positions[4], uvs[3]},

		{positions[7], uvs[1]},
		{positions[6], uvs[0]},
		{positions[1], uvs[2]},
		{positions[0], uvs[3]},
	};
	indexBuffer->load(indices.data(), indices.size());
	vertexBuffer->load(vertices.data(), sizeof(vertex), vertices.size(), vertexLayoutBlob->GetBufferPointer(), vertexLayoutBlob->GetBufferSize());
}
void dx3d::Mesh::ClearMeshData()
{
	vertices.clear();
	indices.clear();
}




