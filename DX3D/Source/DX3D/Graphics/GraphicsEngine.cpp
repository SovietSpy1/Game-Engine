#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Object/GameObject.h>
dx3d::GraphicsEngine* dx3d::GraphicsEngine::S = nullptr;
using namespace dx3d;
dx3d::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
	if (S == nullptr) {
		S = this;
	}
	else {
		DX3DLogError("GraphicsEngine already initialized, cannot initialize again.");
	}
	m_renderSystem = std::make_shared<RenderSystem>(RenderSystemDesc{m_logger});
	m_textureManager = m_renderSystem->createTextureManager();
	m_meshManager = m_renderSystem->createMeshManager();
}

dx3d::GraphicsEngine::~GraphicsEngine()
{
}

GraphicsEngine* dx3d::GraphicsEngine::get()
{
	return S;
}

RenderSystem& dx3d::GraphicsEngine::getRenderSystem() const noexcept
{
	return *m_renderSystem;
}

TextureManager& dx3d::GraphicsEngine::getTextureManager() const noexcept
{
	return *m_textureManager;
}

MeshManager& dx3d::GraphicsEngine::getMeshManager() const noexcept
{
	return *m_meshManager;
}

std::shared_ptr<GameObject> dx3d::GraphicsEngine::createGameObject() const
{
	return std::make_shared<GameObject>(BaseDesc{m_logger});
}

std::shared_ptr<Mesh> dx3d::GraphicsEngine::createMesh() const
{
	return m_renderSystem->createMesh();
}
