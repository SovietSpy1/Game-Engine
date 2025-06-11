#include "DX3D/Graphics/ResourceManager/MeshManager/MeshManager.h"
#include <DX3D/Graphics/ResourceManager/MeshManager/Mesh.h>
std::shared_ptr<dx3d::Mesh> dx3d::MeshManager::createMeshFromFile(const wchar_t* file_path)
{
	return std::static_pointer_cast<Mesh>(createResourceFromFile(file_path));
}
std::shared_ptr<dx3d::Mesh> dx3d::MeshManager::createMesh()
{
	Mesh* mesh = nullptr;
	try {
		mesh = new Mesh(GraphicsResourceDesc{ BaseDesc{m_logger}, m_renderSystem, m_device, m_factory });
	}
	catch (...) {}
	return std::shared_ptr<Mesh>(mesh);
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
