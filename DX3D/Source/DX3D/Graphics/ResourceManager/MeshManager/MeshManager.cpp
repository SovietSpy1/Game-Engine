#include "DX3D/Graphics/ResourceManager/MeshManager/MeshManager.h"
#include <DX3D/Graphics/ResourceManager/MeshManager/Mesh.h>
#include <DX3D/Graphics/RenderSystem.h>
dx3d::MeshManager::MeshManager(const GraphicsResourceDesc& desc) : ResourceManager(desc)
{
}
std::shared_ptr<dx3d::Mesh> dx3d::MeshManager::createMeshFromFile(const wchar_t* file_path)
{
	return std::static_pointer_cast<Mesh>(createResourceFromFile(file_path));
}
std::shared_ptr<dx3d::Mesh> dx3d::MeshManager::createMesh(const wchar_t* name)
{
	return std::static_pointer_cast<Mesh>(createResourceLocal(name));
}
dx3d::Resource* dx3d::MeshManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	Mesh* mesh = nullptr;
	try {
		mesh = new Mesh(file_path, GraphicsResourceDesc{ BaseDesc{m_logger}, m_renderSystem, m_device, m_factory });
	}
	catch (...) {}
	return mesh;
}

dx3d::Resource* dx3d::MeshManager::createResourceConcrete(const wchar_t* name)
{
	MeshType meshType = MeshType::Custom;
	if (name == L"Quad") {
		meshType = MeshType::Quad;
	}
	else if (name == L"Cube") {
		meshType = MeshType::Cube;
	}
	Mesh* mesh = nullptr;
	try {
		mesh = new Mesh(GraphicsResourceDesc{ BaseDesc{m_logger}, m_renderSystem, m_device, m_factory }, meshType);
	}
	catch (...) {}
	return mesh;
}
