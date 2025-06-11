#pragma once
#include "DX3D/Graphics/ResourceManager/ResourceManager.h"
#include <DX3D/Graphics/ResourceManager/MeshManager/Mesh.h>
namespace dx3d {
	class MeshManager : public ResourceManager
	{
	public:
		MeshManager(const GraphicsResourceDesc& desc) : ResourceManager(desc)
		{
			// Initialization code for the texture manager
		}
		std::shared_ptr<Mesh> createMeshFromFile(const wchar_t* file_path);
		std::shared_ptr<Mesh> createMesh();
	protected:
		Resource* createResourceFromFileConcrete(const wchar_t* file_path);
	};
}



